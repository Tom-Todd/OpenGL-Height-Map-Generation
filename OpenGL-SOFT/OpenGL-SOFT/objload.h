#ifndef OBJLOAD_H
#define OBJLOAD_H
#include "glm/gtc/matrix_transform.hpp"
#include <vector>

bool loadOBJ(const char * path, std::vector<glm::vec3> & loadVerts, std::vector<glm::vec2> & loadUVs, std::vector<glm::vec3> & loadNormals);

#endif