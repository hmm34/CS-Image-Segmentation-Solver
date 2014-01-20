bin/iseg: src/img-seg-solver.cpp
	mkdir -p bin
	g++ -I./ -c src/Graph.cpp -Wall -o bin/Graph.o
	g++ -I./ -c src/img-seg-solver.cpp -Wall -o bin/iseg.o
	g++ -o bin/iseg bin/Graph.o bin/iseg.o

.PHONY: clean
clean:
	rm -rf bin/iseg