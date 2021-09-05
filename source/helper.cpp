#include "helper.h"
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <vector>
#include <fstream>
#include <sstream>

bool DEBUG = false;
bool PROFILING = false;
void printDebug(std::string msg){
   if(DEBUG==true){
    std::cout<<msg;
  }
}
bool fileExists (const std::string& name) {
    std::ifstream f(name.c_str());
    return f.good();
}
std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;    
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    
    return result;
}


void runCommand(std::string cmd,int lineNum,std::string ruleName)
{
  std::string s1(cmd);  
  std::string s2(" 2> temp/."+std::to_string(lineNum)+".err");
  std::string s = s1 + s2;
  const char* command = s.c_str();
  std::cout<<s1+"\n";
  try{
    std::string result = exec(command);
  } catch (const std::exception& e){
    std::string error = e.what();
    throw std::runtime_error(error+"\nfakemake: *** [makefile:"+std::to_string(lineNum)+": "+ruleName+"] Error 1\n");
    return;
  }
  std::ifstream tempfile("temp/."+std::to_string(lineNum)+".err");
  std::string line = "";
  /*If there is any line... get the lines and we have ERRORS*/
  while(getline(tempfile, line)){
      std::cerr<<line<<"\n";
      throw std::runtime_error("fakemake: *** [makefile:"+std::to_string(lineNum)+": "+ruleName+"] Error 1\n");
      break;
  }
}

int checkLineEmpty(std::string line){
  char n;
  std::stringstream substream = std::stringstream(line);
  int count = 0;
  while(substream>>n){
    count +=1;
  }
  return count==0;
}

std::vector<std::string> split_string(std::string s,std::string delimiter){
  std::vector<std::string> splitted;
    size_t pos = 0;
    std::string token;

    while (true) {
        pos = s.find(delimiter);
        token = s.substr(0, pos);        
        splitted.push_back(token);
        s.erase(0, pos + delimiter.length());
        if(pos== std::string::npos){
          break;
        }
    }
  return splitted;
}

