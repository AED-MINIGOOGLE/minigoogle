CXX=g++
INPUT=main.cpp invertedindex.cpp cli.cpp coreengine.cpp utility.cpp parser.cpp
CFLAGS=-O3 -fthreadsafe-statics -std=c++11 -fexceptions
OUTPUT=engine
all:
	$(CXX) -o $(OUTPUT) $(INPUT) $(CFLAGS)

clean:
	rm -r engine
