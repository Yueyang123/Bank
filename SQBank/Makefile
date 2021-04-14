CXX = g++
INC = -I./include
CXXFLAGS = -l sqlite3 -g 
 
TARGET=./main
 
SRC=$(wildcard ./*.cpp)
 
OBJ=$(patsubst %.cpp, %.o, $(SRC))
 
$(TARGET): $(OBJ)
	$(CXX)  -o $@ $^ $(CXXFLAGS)
 
$(OBJ):%.o: %.cpp
	$(CXX)  $(INC) -o $@ -c $< $(CXXFLAGS)
 
clean:
	rm -f *.o
	rm -f $(TARGET)