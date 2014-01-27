bin/iseg: src/img-seg-solver.cpp
	mkdir -p bin
	g++ -I./ -c src/graph.cpp -Wall -o bin/graph.o
	g++ -I./ -c src/img-seg-solver.cpp -Wall -o bin/iseg.o
	g++ -o bin/iseg bin/graph.o bin/iseg.o
	g++ -I./ -c test/test-suite.cpp -Wall -o bin/test-suite.o
	g++ -o bin/test-suite bin/test-suite.o bin/graph.o

.PHONY: clean

clean: 
	rm -rf bin/