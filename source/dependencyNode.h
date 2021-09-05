#pragma once
#include <string>
#include <vector>
#include "helper.h"
#include <mutex>
class Node{
    public:
      Node(std::string ruleName,std::string command,int lineNum);
      int addDependentNode(Node* n);
      int addChildrenNode(Node* n);
      void setAsComplete() {
        printDebug("DEBUG:: COMPLETED "+ruleName+"\n");
        completed = true;
      }
      bool isComplete(){
        return completed;
      }
      bool isRoot(){
        for(int j=0;j<dependentNodes.size();j++){          
            if(!dependentNodes[j]->isComplete()){
                //If any is not satisfied, continue
                return false;
            }
        }
        return true;
      }
      int getLineNum(){
        return lineNum;
      }
      bool isPending(){
        return pending;
      }
      void setAsPending(){
        pending = true;
      }
      bool isDependentOf(Node* n){
        bool dependent = false;
        for(int j=0;j<n->childrenNodes.size();j++){
           if(n->childrenNodes[j]->id==id){
             dependent = true;
             break;
           }
        }
        return dependent;
      }
      std::string getRuleName();
      std::string getCommand();
      std::vector<Node*> childrenNodes;
      std::vector<Node*> dependentNodes;
      void run();
    private:
      bool completed = false;
      bool root = false;
      bool pending = false;      
      int lineNum;
      std::string ruleName;
      std::string command;
      int id = 0;
      static uint64_t count;
};