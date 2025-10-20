#include "fbtg_gen.h"

inline void generator::set_vertex(std::vector<float>& vertices, size_t& idx, float x, float y, float z){
	vertices[idx++] = x;
	vertices[idx++] = y;
	vertices[idx++] = z;
}

std::vector<float> generator::temp_gen(std::size_t width, std::size_t length){
	std::vector<float> vertices;
	vertices.resize((width-1)*(length-1)*18); // 6 vertices per quad * 3 values per vertex
	std::size_t idx = 0;
	for(std::size_t i = 0; i < width-1; i++){
		for(std::size_t j = 0; j < length-1; j++){
			set_vertex(vertices, idx, i,j,0);
			set_vertex(vertices, idx, i+1,j,0);
			set_vertex(vertices, idx, i,j+1,0);

			set_vertex(vertices, idx, i,j+1,0);
			set_vertex(vertices, idx, i+1,j,0);
			set_vertex(vertices, idx, i+1,j+1,0);
		}
	}
	return vertices;
}
