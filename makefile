#!/bin/bash
CXX = g++
CXXFLAGS = -std=c++17 -Iinclude  
LDFLAGS = -lglfw -lGL -ldl -lpthread -lwayland-client

SRC = core/fbtg_core.cpp core/fbtg_callbacks.cpp core/input.cpp rendering/fbtg_render.cpp etc/plugin_system.cpp
OBJ = $(SRC:.cpp=.o)

fbtg: $(OBJ)
	$(CXX) $(OBJ) -o build/fbtg $(LDFLAGS)

core/%.o: core/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

rendering/%.o: rendering/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

etc/%.o: etc/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -f $(OBJ)
