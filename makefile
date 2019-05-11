CC = g++
flags = -Wall -O3

main: graph.o dfs.o detect.o cycle.o config.o
	$(CC) $(flags) graph.o dfs.o detect.o cycle.o config.o main.cc -o clsd
graph.o: graph.cc
	$(CC) $(flags) -c graph.cc -o graph.o 
dfs.o: dfs.cc
	$(CC) $(flags) -c dfs.cc -o dfs.o 
detect.o: detect.cc
	$(CC) $(flags) -c detect.cc -o detect.o 
cycle.o: cycle.cc
	$(CC) $(flags) -c cycle.cc -o cycle.o 
config.o: config.cc
	$(CC) $(flags) -c config.cc -o config.o 
