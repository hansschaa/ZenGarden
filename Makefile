CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g 
SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o)
EXECUTABLE = zen

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXECUTABLE)
