bin/iseg: src/img-seg-solver.cpp
	mkdir -p bin
	g++ -I./ -c src/graph.cpp -Wall -o bin/graph.o
	g++ -I./ -c src/img-seg-solver.cpp -Wall -o bin/iseg.o
	g++ -o bin/iseg bin/graph.o bin/iseg.o

.PHONY: clean
	ifdef SystemRoot
		RM = del /Q /S
	else
		RM = rm -f
	endif

clean:
	$(RM) bin/iseg
	$(RM) bin/iseg.o
	$(RM) bin/graph.o