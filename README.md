#FakeMake

### COMPSCI711 Assignment 4
Student ID : 851640593
Name : Chenrong Lu

Make, but faster by using a thread pool

### Run instructions
Type: fakemake -h for instructions.
```
Usage: make [options] [target]...
Options:
-f FILE                  Read FILE as a makefile.
--v                      Print fakemake version.
--h                      Print help
-d FILE                  DEBUG mode
                         Use the file FILE
--j                      Give a joke.
-t                       Run with Threads
-s                       Silence output
-p                       Profile running time
```
You can make fakemake with make 
```
make -f makefile 
```
You can then make fakemake with fakemake
```
fakemake -f makefile2
```


### Brief Summary Of Implementation
1.Build dependency 'tree' from parsing makefile


2.For each node without dependency, run the command.


3.Check if the child of the node(which has just ran)
  has all its parents completed.If so, run it and repeat 3.


4.Done.


(Serial)

Calls run() function on Node recursively.


(Parallel)

Uses a thread pool with a command queue. Initial nodes are
inserted into command queue. If node is finished processing
and any of its child is free of dependency, run the child.
Main thread iterates over all nodes constantly and checks if
any nodes were missed (In the situation where a node is called as a
child and its parents are not completed yet), if so then append
into command queue. Maximum number of threads available is used by
default.

### Error Checking
Please see test files for some of the errors checked.
Checks for syntax error, dependency error, circular dependency,etc.

### Running the tests
ONLY TESTED ON WINDOWS 8.1
```Windows CMD
runTests.bat 
```
```Bash
runTests.sh
```
### Sample Output (Part of the Test File Output)
<pre>
============TEST1=================
Running 'fakemake -p -f tests/compileSelf' (Serial Compile Self)
g++  -std=c++17 -c source/dependencyTree.cpp -o bin/dependencyTree.o
g++  -std=c++17 -c source/dependencyNode.cpp -o bin/dependencyNode.o
g++ -std=c++17 -c source/main.cpp  -o bin/main.o
g++ -std=c++17 -c source/parser.cpp -o bin/parser.o
g++ -std=c++17 -c source/helper.cpp -o bin/helper.o
g++ -std=c++17 -c source/threadPool.cpp -o bin/threadpool.o
g++  -std=c++17 bin/main.o bin/dependencyTree.o bin/dependencyNode.o bin/parser.o bin/helper.o bin/threadPool.o -o fakemakeFromfakeMake
Serial Command Issueing : Time taken: 7615 Miliseconds
============DONE==================
============TEST2=================
Running 'fakemake -t -p -f tests/compileSelf' (Parallel Compile Self)
g++  -std=c++17 -c source/dependencyTree.cpp -o bin/dependencyTree.o
g++ -std=c++17 -c source/main.cpp  -o bin/main.o
g++ -std=c++17 -c source/parser.cpp -o bin/parser.o
g++  -std=c++17 -c source/dependencyNode.cpp -o bin/dependencyNode.o
g++ -std=c++17 -c source/helper.cpp -o bin/helper.o
g++ -std=c++17 -c source/threadPool.cpp -o bin/threadpool.o
g++  -std=c++17 bin/main.o bin/dependencyTree.o bin/dependencyNode.o bin/parser.o bin/helper.o bin/threadPool.o -o fakemakeFromfakeMake
Parallel Command Issueing : Time taken: 4302 Miliseconds
</pre>