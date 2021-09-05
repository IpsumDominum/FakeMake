#include "parser.h"
#include <thread>
#include <string>
#include <sstream>
#include <fstream>
#include <cstring>
#include <stdexcept>
#include <algorithm>

extern bool DEBUG;
DependencyTree constructDependencyTree(std::string filename){
  DependencyTree dependencies{};
  std::string line;  
  std::ifstream file(filename);
  enum State { PENDING,PARSING};
  State state = PENDING;
  std::vector<Rule> rules;  
  char n;
  int line_num = 0;
  std::vector<std::string> ruleName_list;
  Rule currentRule;
  if(!fileExists(filename)){
    throw std::runtime_error("fakemake: *** No rule to make target '"+filename+"'.  Stop.");
  }
	while (getline(file, line)) {    
    line_num ++;
    if(checkLineEmpty(line)==true){
      //EMPTY LINE, continue.
      continue;
    }
    /*
    State machine:
    PENDING -> RULE -> COMMAND -> PENDING -> RULE -> COMMAND...
    */
    ///////////////Parsing and collect all rules
      n = line[0];          
      //Check first character
      if(n=='\t'){
          //If it is tab, we are expecting a command
          //Check that command follows rule              
          if(state==PARSING){
            state = PENDING;
            currentRule.command = line.substr(1,line.length());
            rules.push_back(currentRule);
            dependencies.addNode(currentRule.ruleName,currentRule.command,line_num);
          }else{
            throw std::runtime_error("Syntax Error:: line:"+std::to_string(line_num)+"|  Command should follow rule. Stop.\n '"+line+"'");
          }
      }else{
        if(state!=PENDING){
          throw std::runtime_error("Syntax Error:: line:"+std::to_string(line_num)+"|  Command should follow rule. Stop.\n '"+line+"'");
        }
          //If it is a rule we are expecting a rule
          state = PARSING;          
          //Check that rule syntax is obeyed
          std::vector<std::string> splitted = split_string(line ,":");
          if(splitted.size()!=2){
              throw std::runtime_error("Syntax Error:: line:"+std::to_string(line_num)+"|  Rule not understood, invalid syntax. Stop.\n '"+line+"'");
          }
          //Get Rule name
          std::string ruleName = splitted[0];
          //Trim white space
          ruleName.erase(remove_if(ruleName.begin(), ruleName.end(), isspace),ruleName.end());
          ruleName_list.push_back(ruleName);
          //Get dependencies list
          std::vector<std::string> dependencies_list;
          std::stringstream ss = std::stringstream(splitted[1]);
          std::string word;
          while(ss>> word){
            dependencies_list.push_back(word);
          }
          //Append rule to all rules           
          currentRule.ruleName = ruleName;
          currentRule.dependencies_list = dependencies_list;          
      }      
  }
  //DEBUG
  if(DEBUG==true){
  for(int i=0;i<rules.size();i++){
    std::cout<<"RULENAME : "<<rules[i].ruleName<<"\n";
    std::cout<<"COMMAND : "<<rules[i].command<<"\n";
    std::cout<<"DEPENDENCIES: \n";
    for(int j=0;j<rules[i].dependencies_list.size();j++){
        std::cout<<rules[i].dependencies_list[j]<<" ";
    }
    std::cout<<"\n========================\n";
    }
  }
    
  
  ////BUILD DEPENDENCY TREE
  if(rules.size()>0){
    //FOR ALL RULES
    for(int i=0;i<rules.size();i++){
      //For each depedency, if the dependency is found in rules,
      //Append a dependency from node to rule
      //Otherwise check file exists
      for(int j=0;j<rules[i].dependencies_list.size();j++){
        //Check if dependency is a rule or a file.   
          if(std::count(ruleName_list.begin(), ruleName_list.end(), 
                rules[i].dependencies_list[j])){
                /*Dependency is a rule*/
                //Append node dependency
                dependencies.addDependency(rules[i].ruleName,rules[i].dependencies_list[j]);
          }else{
            /*Dependency is a file*/
           //Check if file exists
           if(fileExists(rules[i].dependencies_list[j])){
             /*good*/
           }else{
             throw std::runtime_error("fakemake: *** No rule to make target '"+rules[i].dependencies_list[j]
                                      +"', needed by '" + rules[i].ruleName+"'.  Stop.");
           }
          }     
      }
      
    }
    //FINISHED
  }else{
    //No rules
    throw std::runtime_error("fakemake: *** No targets.  Stop.");
  }
  
  //Should return a structure which allows for finding which rule should be built first, a 
  //Dependency tree, where we may traverse from the bottom to the top.
  return dependencies;
}

