#pragma once
#include "core.h"
#include "state.h"

void render_gui(AppState& state);
void render_quad();
void render_skybox(Shader shader, const glm::vec3& lightDir);
void render_water_plane(float size, float level, Shader shader, const glm::vec3& lightDir);
void render_terrain(Shader shader, const glm::vec3& lightDir, const glm::mat4& view, const glm::vec4& clipPlane = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

void init_skybox();

