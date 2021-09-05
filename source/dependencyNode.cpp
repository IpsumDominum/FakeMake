#include "dependencyNode.h"
#include <string>
#include <iostream>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include <cstdlib>

uint64_t Node::count = 0;
Node::Node(std::string n,std::string c,int l){
    ruleName = n;
    command = c;
    lineNum = l;
    id = count++;
    printDebug("DEBUG:: Node Created id:"+std::to_string(id)+" RuleName:"+ruleName+"\n");
}
int Node::addDependentNode(Node* n){
    printDebug("DEBUG:: Node "+ruleName+" Now Depend On "+n->ruleName+"\n");
    dependentNodes.push_back(n);
    return 0;
}
int Node::addChildrenNode(Node* n){
    printDebug("DEBUG:: Node "+ruleName+" Now has child Node "+n->ruleName+"\n");
    childrenNodes.push_back(n);
    return 0;
}

std::string Node::getRuleName(){
    return ruleName;
}
std::string Node::getCommand(){
    return command;
}
void Node::run(){
    printDebug("DEBUG:: RUNNING RULE "+ruleName+"\n");
    try{
        runCommand(command,lineNum,ruleName);
        setAsComplete();
    }catch(const std::exception& e){
        std::cout<<e.what()<<"\n";
    }
    //Check if children should also be run.//Recursive execution
    printDebug("DEBUG:: Checking child now for "+ruleName+"\n");
    for(int i=0;i<childrenNodes.size();i++){
        //If the children node's dependencies are satisfied
        //And that the children nodes are not completed.
        printDebug("DEBUG:: Checking "+childrenNodes[i]->ruleName+"\n");
        if(childrenNodes[i]->isRoot() && !childrenNodes[i]->isComplete()
        && !childrenNodes[i]->isPending()){
            childrenNodes[i]->setAsPending();
            //Run the children node;
            printDebug("DEBUG:: Running child now...\n");
            childrenNodes[i]->run();
        }
    }
}
