fakemake: bin/main.o bin/dependencyTree.o bin/dependencyNode.o bin/parser.o bin/helper.o bin/threadPool.o
	g++  -std=c++17 bin/main.o bin/dependencyTree.o bin/dependencyNode.o bin/parser.o bin/helper.o bin/threadPool.o -o fakemake
	
bin/dependencyTree.o: source/dependencyTree.cpp source/helper.h source/dependencyTree.h source/threadPool.h
	g++  -std=c++17 -c source/dependencyTree.cpp -o bin/dependencyTree.o
bin/dependencyNode.o : source/dependencyNode.cpp source/dependencyNode.h
	g++  -std=c++17 -c source/dependencyNode.cpp -o bin/dependencyNode.o
bin/main.o :  source/main.cpp source/dependencyTree.h
	g++ -std=c++17 -c source/main.cpp  -o bin/main.o
bin/parser.o :  source/parser.cpp source/parser.h 
	g++ -std=c++17 -c source/parser.cpp -o bin/parser.o 
bin/helper.o :  source/helper.cpp source/helper.h
	g++ -std=c++17 -c source/helper.cpp -o bin/helper.o
bin/threadPool.o :  source/threadPool.cpp source/threadPool.h bin/dependencyNode.o
	g++ -std=c++17 -c source/threadPool.cpp -o bin/threadpool.o



