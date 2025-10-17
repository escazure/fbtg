#include "fbtg_render.h"

std::vector<plugin_system::Vertex> temp_gen(std::size_t _width, std::size_t _length, float(**_fun)(float,float)){
	std::vector<plugin_system::Vertex> t;
	t.reserve(_width * _length);
	for(std::size_t i = 1; i < _width+1; i++){
		for(std::size_t j = 1; j < _length+1; j++){
			float height = (*_fun)(j,i);
			t.emplace_back(plugin_system::Vertex(i,j,height,0,0,0));	
		}
	}
	float max = 0.0, min = 0.0;
	for(std::size_t i = 0; i < _width * _length; i++){
		if(max < t[i].z) max = t[i].z;
		if(min > t[i].z) min = t[i].z;
	}	
	for(std::size_t i = 0; i < _width * _length; i++){
		t[i].r = (t[i].z - min) / (max - min);
		t[i].g = t[i].r;
		t[i].b = t[i].r;
	}
	return t;
}

void render_triangle(plugin_system::Vertex& _a, plugin_system::Vertex& _b, plugin_system::Vertex& _c){
	glBegin(GL_TRIANGLES);
	glColor3f(_a.r, _a.g, _a.b);
	glVertex3f(_a.x, _a.y, _a.z);
	
	glColor3f(_b.r, _b.g, _b.b);
	glVertex3f(_b.x, _b.y, _b.z);

	glColor3f(_c.r, _c.g, _c.b);
	glVertex3f(_c.x, _c.y, _c.z);
	glEnd();
}

void render_buffer(std::vector<plugin_system::Vertex>& _tbuffer, std::size_t width, std::size_t length){
	if(_tbuffer.size() < 4){
		return ;
	}
	for(std::size_t x = 0; x < width - 1; x++){
		for(std::size_t y = 0; y < length-1; y++){
			int i00 = x*length+y;
			int i10 = (x+1)*length+y;
			int i01 = x*length+(y+1);
			int i11 = (x+1)*length+(y+1);

			render_triangle(_tbuffer[i00], _tbuffer[i10], _tbuffer[i01]);
			render_triangle(_tbuffer[i10], _tbuffer[i11], _tbuffer[i01]);
		}
	}
}
