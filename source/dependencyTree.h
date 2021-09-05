#pragma once
#include <iostream>
#include <vector>
#include "dependencyNode.h"
#include "helper.h"
#include "threadPool.h"

class DependencyTree{
  public:
    DependencyTree();
    int addDependency(std::string ruleName1,std::string ruleName2);
    void addNode(std::string ruleName,std::string command,int lineNum);
    void printNodes();
    void runCommands(int FAKEMAKE_USE_THREAD);
    std::pair<Node*,int> findNode(std::string ruleName);
  private:
    std::vector<Node> nodeMap;
};

