# ########## BEAUTEQ MAKEFILE ##########

CXX     = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
TARGET  = beauteq
SRCS    = main.cpp inventory.cpp orders.cpp filehandling.cpp
OBJS    = $(SRCS:.cpp=.o)

# default target: build the executable
all: $(TARGET)

# link all object files into the final executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# compile each .cpp into a .o object file
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# remove compiled files
clean:
	del /Q $(TARGET).exe *.o 2>nul || rm -f $(TARGET) *.o

# rebuild from scratch
rebuild: clean all

# run the program
run: all
	./$(TARGET)

.PHONY: all clean rebuild run