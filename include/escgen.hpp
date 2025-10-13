#include <dlfcn.h>
#include <fstream>

#pragma once

namespace escgen{
	struct Vertex{
		float x, y, z;
		float r, g, b;

		Vertex(float _x,float _y,float _z,float _r,float _g,float _b){
			x = _x; y = _y; z = _z;
			r = _r; g = _g; b = _b;
		};
	};

	bool write_fun(const std::string& _fun, const std::string& _name){
		std::ofstream out("functions/" + _name + ".cpp");
		if(!out.good()){
			return false;
		}
		std::string fun_wrapper = "extern \"C\" float " + _name + "(float x, float y = 0){return " + _fun + ";}";
		out << fun_wrapper;
		return true;
	}

	bool compile_fun(const std::string& _name){
		std::string cmd = "g++ -shared -fPIC -O2 ./functions/" + _name + ".cpp -o ./functions/" + _name;
		int result = std::system(cmd.c_str());
		if(result != 0){
			return false;
		}
		return true;
	}

	bool get_fun_pointer(const std::string& _name, float (**_fun)(float,float), void** _handle){
		void* handle = dlopen(("./functions/"+_name).c_str(), RTLD_LAZY);
		if(!handle){
			return false;
		}

		*_fun = (float (*)(float,float))dlsym(handle, _name.c_str());
		const char* error = dlerror();
		if(error){
			dlclose(handle);
			return false;
		}
		*_handle = handle;
		return true;
	}
	
	void save_function(std::string& _fun, const std::string& _name){}
	void load_function(const std::string& _name){}
}
