#pragma once

#include <vector>

namespace generator {
	std::vector<float> temp_gen(std::size_t width, std::size_t length);
	inline void set_vertex(std::vector<float>& vertices, size_t& idx, float x, float y, float z);
}
