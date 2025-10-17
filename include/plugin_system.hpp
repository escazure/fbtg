#pragma once

#include <dlfcn.h>
#include <fstream>

namespace plugin_system{
	struct Vertex{
		float x, y, z;
		float r, g, b;

		Vertex(float _x,float _y,float _z,float _r,float _g,float _b);
	};

	bool write_fun(const std::string& _fun, const std::string& _name);
	bool compile_fun(const std::string& _name);
	bool get_fun_pointer(const std::string& _name, float (**_fun)(float, float), void** _handle);
}
