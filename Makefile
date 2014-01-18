bin/iseg: src/img-seg-solver.cpp
	mkdir -p bin
	g++ src/img-seg-solver.cpp -Wall -o bin/iseg

.PHONY: clean
clean:
	rm -rf bin/iseg
