#include "dependencyTree.h"
#include <string>
#include <utility>
#include <thread>
#include <set>
#include <mutex>
#include <chrono>

DependencyTree::DependencyTree(){
    printDebug("DEBUG:: Dependency Tree Constructed\n");
}
void DependencyTree::printNodes(){
   std::cout << "MAP: \n";
    for (int i=0;i<nodeMap.size();i++){
        std::cout << nodeMap[i].getRuleName() <<" | "<<nodeMap[i].getCommand() <<" | "<<"\n";
    }
}
void DependencyTree::addNode(std::string ruleName,std::string command,int lineNum){
    //Create a Node    
    Node n(ruleName,command,lineNum);
    nodeMap.push_back(n);
    printDebug("DEBUG:: Node Added To Tree "+ruleName+"\n");
}

std::pair<Node*,int> DependencyTree::findNode(std::string ruleName){
    for(int i=0;i<nodeMap.size();i++){
        if(nodeMap[i].getRuleName()==ruleName){
            return {&nodeMap[i],1};
        }
    }
    return {&nodeMap[0],0};
}

int DependencyTree::addDependency(std::string ruleName1,std::string ruleName2){
    //If both ruleNames are found.
    std::pair<Node*,int> result1 = findNode(ruleName1);
    std::pair<Node*,int> result2 = findNode(ruleName2);
    if( result1.second!=0 && result2.second!=0){
        //Connect them
        //1 is dependent on 2
        //2's children is 1
        Node* n1 = result1.first;
        Node* n2 = result2.first;
        if(n2->isDependentOf(n1)){
           /*Circular dependency!*/
           std::cout<<"fakemake: Circular "<<n1->getRuleName()<<" <- "<<n2->getRuleName()<<" dependency dropped."<<"\n";
        }else{
            n1->addDependentNode(n2);
            n2->addChildrenNode(n1);
        }
    }
    return 0;
}
void DependencyTree::runCommands(int FAKEMAKE_USE_THREAD){
    extern bool PROFILING;
    auto start = std::chrono::high_resolution_clock::now();
    if(FAKEMAKE_USE_THREAD==1){
        /*Threads*/
        printDebug("DEBUG:: Using threads...\n");
        //In parallel, run the root nodes first, then create new threads upon discovering any
        //of the children becoming a root node.
        ThreadPool threadpool = ThreadPool();
        //Master thread keeps checking that all nodes have completed.
        //Probably much more efficient to have a node count.
        while(true){
            bool allCompleted = true;
            for(int i=0;i<nodeMap.size();i++){
                if(nodeMap[i].isRoot() && nodeMap[i].isComplete()==false &&
                 nodeMap[i].isPending()==false){
                    nodeMap[i].setAsPending();
                    try{
                    threadpool.AddJob(std::bind(&Node::run,&nodeMap[i]));
                    }catch(const std::exception& e){
                        std::cout<<e.what()<<"ERRR\n";
                    }
                }
                //If any is not completed...
                if(!nodeMap[i].isComplete()){
                    allCompleted = false;
                }                
            }
            if(allCompleted==true){
                break;
            }else{
                if(threadpool.isEmpty()){
                    //Nothing in queue and not all deps complete.                    
                    //Circular dependency therefore..
                }                
            }
        }
        auto stop = std::chrono::high_resolution_clock::now();
        if(PROFILING==true){    
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop-start);
            auto durationCount = duration.count();
            std::cout<<"Parallel Command Issueing : Time taken: "<<durationCount<<" Miliseconds\n";
        }    
        //Need to cleanup the pool after use.
        threadpool.terminate();

    }else{
        /*Serial*/
        printDebug("DEBUG:: Not using threads...\n");
        //For all the initial root nodes.
        int currentIdx = 0;
        printDebug("DEBUG:: Node Map Size "+std::to_string(nodeMap.size())+"\n");
        for(int i=0;i<nodeMap.size();i++){
        //Run the command, then check if the nodes below it are now root nodes
            if(nodeMap[i].isRoot() && !nodeMap[i].isComplete()){                
                try{
                    nodeMap[i].run();
                }catch(const std::exception& e){
                    std::string error = e.what();
                    throw std::runtime_error(error);
                    return;
                }
            }
        }
        auto stop = std::chrono::high_resolution_clock::now();
        if(PROFILING==true){    
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop-start);
            std::cout<<"Serial Command Issueing : Time taken: "<<duration.count()<<" Miliseconds\n";
        }    
    }
    
}




