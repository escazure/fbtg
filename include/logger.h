#pragma once

#include <print>

#define MESSAGE_INFO 0
#define MESSAGE_SUCCESS 1
#define MESSAGE_ERROR 2

inline void log(int status, const std::string& message){
	std::string color;
	switch(status){
		case MESSAGE_ERROR:
			color = "\033[22;31m";
			break;
		case MESSAGE_INFO:
			color = "\033[22;37m";
			break;
		case MESSAGE_SUCCESS:
			color = "\033[22;32m";
			break;
	}
	std::println("{}{}\033[0m", color, message);	
}
