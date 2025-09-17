#!/bin/bash
CXX = g++
CXXFLAGS = -std=c++17 -Iinclude 
LDFLAGS = -lglfw -lGL -ldl -lpthread -lwayland-client

SRC = core/*.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = fbtg

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -f $(OBJ)
