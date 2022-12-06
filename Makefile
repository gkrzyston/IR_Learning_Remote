CXX = gcc
CXXFLAGS = -std=c11 -Wconversion -Wall -Wextra -pedantic -D _DEFAULT_SOURCE
SOURCES = $(wildcard *.c)
EXECUTABLE = irlm

release: CXXFLAGS += -O3 -DNDEBUG
release:
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(EXECUTABLE)

debug: CXXFLAGS += -g3 -DDEBUG
debug:
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(EXECUTABLE)_debug

.PHONY: release debug clean

clean:
	rm -rf $(EXECUTABLE)* *.out
