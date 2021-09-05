#include <iostream>
#include "dependencyTree.h"
#include "parser.h"
#include <cstring>
#include <stdlib.h> 
#include <time.h>       /* time */
#include <chrono>

extern bool DEBUG;
extern bool PROFILING;
int main(int argc, char **argv){  
  enum FAKEMAKE_USE_THREAD {NO_THREAD,USE_THREAD};
  std::string fileName;
  bool wrongUsage = false;
  int use_thread_or_not = NO_THREAD;  
  if(argc>=2){
      enum State {EXPECTING_SPECIFIER,EXPECTING_FILE_ARGUMENT};
      State state = EXPECTING_SPECIFIER;
      ////////////////////////////////
      //LOOP
      ////////////////////////////////
      for(int i=1;i<argc;i++){
        ////////////////////////////////
        if(state == EXPECTING_SPECIFIER){
          if(strcmp(argv[i],"-f")==0){
              DEBUG = false;              
              state = EXPECTING_FILE_ARGUMENT;
            }else if(strcmp(argv[i],"-d")==0){
              DEBUG = true;
              state = EXPECTING_FILE_ARGUMENT;
            }else if(strcmp(argv[i],"-t")==0){
              //Run in parallel
              use_thread_or_not = USE_THREAD;
            }else if(strcmp(argv[i],"-s")==0){
              std::cout.setstate(std::ios_base::failbit);
            }else if(strcmp(argv[i],"--h")==0 || strcmp(argv[i],"-h")==0){              
              wrongUsage = true;
            }else if(strcmp(argv[i],"--v")==0 || strcmp(argv[i],"-v")==0){
              std::cout<<"FakeMake v0.1.0\nAuthor: Chenrong Lu\nBuilt for COMPSCI711\nTested on WINDOWS10\nCopyright (C) 1988-2016 Free Software Foundation, Inc.\nLicense GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>\nThis is free software: you are free to change and redistribute it.\nThere is NO WARRANTY, to the extent permitted by law.\n";
              return 0;
            }else if(strcmp(argv[i],"-p")==0 || strcmp(argv[i],"--p")==0){              
              PROFILING = true;            
            }else if(strcmp(argv[i],"-j")==0 || strcmp(argv[i],"--j")==0){
              srand(time(NULL));
              int random = rand() %2;
              if(random==0){
                std::cout<<"What is the pirate's favourite letter? You may think it's R, but it is in fact the C.\n";
              }else if (random==1){
                std::cout<<"Why did the anarchist draw bad technical drawings? He doesn't use a ruler.\n";
              }
              return 0;
            }else{
              if(i==argc-1){
                fileName = argv[i];
              }else{
                std::cout<<"fakemake: invalid option -- "<<argv[i]<<"\n";
                wrongUsage = true;
              }
            }
          ////////////////////////////////
          }else if(state==EXPECTING_FILE_ARGUMENT){
            state = EXPECTING_SPECIFIER;
            fileName = argv[i];
            if(!fileExists(fileName)){
                std::cout<<"fakemake: "+fileName+": No such file or directory\n";
            }
          }
        }
        if(state==EXPECTING_FILE_ARGUMENT){
          std::cout<<"fakemake: option requires an argument -- f"<<"\n";
        }
      if(wrongUsage==true){
          std::cout.clear();
          std::cout<<"fakemake: \nUsage: make [options] [target]...\nOptions:\n-f FILE \t\t Read FILE as a makefile.\n--v \t\t\t Print fakemake version.\n--h \t\t\t Print help\n-d FILE \t\t DEBUG mode\n\t\t\t Use the file FILE\n--j \t\t\t Give a joke.\n-t \t\t\t Run with Threads\n-s \t\t\t Silence output\n-p \t\t\t Profile running time"<<std::endl;
          return 0;
      }
  }else{
    fileName = "makefile";
  }    
  DependencyTree dependencies;
  try{
    dependencies = constructDependencyTree(fileName);        
    dependencies.runCommands(use_thread_or_not);
  } catch (const std::exception& e){
    std::cout<<e.what()<<"\n";
    return 0;
  }

  //Now we end up with a dependency tree,
  //With root nodes as an array of nodes we should run first.
  //Dynamically create new threads as suitable (ie.= current number of root nodes)
  //Root nodes here mean nodes which have no dependencies.  
  
  return 0;
}