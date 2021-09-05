#pragma once
#include <vector>
#include <string>

std::string exec(const char* cmd);
void runCommand(std::string cmd,int lineNum,std::string ruleName);
bool fileExists (const std::string& name);
int checkLineEmpty(std::string line);
void printDebug(std::string msg);
std::vector<std::string> split_string(std::string s,std::string delimiter);

struct Rule{
  std::string command;
  std::string ruleName;
  std::vector<std::string> dependencies_list;
};
