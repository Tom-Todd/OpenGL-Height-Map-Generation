#include <stdio.h>
#include <string>
#include <cstring>
#include <glm/glm.hpp>
#include "objload.h"
#include <string>


bool loadOBJ(const char * path, std::vector<glm::vec3> & loadVerts, std::vector<glm::vec2> & loadUVs, std::vector<glm::vec3> & loadNormals) {
	std::vector<unsigned int> vIndices, uvIndices, nIndices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	int group[50000];
	int groupVal = 0;

	FILE * file = fopen(path, "r");
	if (file == NULL) {
		printf("Couldn't open OBJ");
		return false;
	}
	int groupNum = 0;
	const char* curGroup = new char[256];
	while (true) {
		char lineHeader[1000000];
		if (fscanf(file, "%s", lineHeader) == EOF)break;
		//Check if it matches a vertex, a uv or a normal. Or if it's defining a triangle.
		//If not discard the line.
		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			uv.y = -uv.y;
			uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			//Get the indicies of each element
			int v[3], uv[3], n[3];
			if (fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &v[0], &uv[0], &n[0], &v[1], &uv[1], &n[1],
				&v[2], &uv[2], &n[2]) != 9)return false;
			for (int i = 0; i < 3; i++) {
				vIndices.push_back(v[i] - 1);
				uvIndices.push_back(uv[i] - 1);
				nIndices.push_back(n[i] - 1);
			}
		}else if (strcmp(lineHeader, "mtllib") == 0){
			const char* mtlPath = new char[256];
			fscanf(file, "%s\n", mtlPath);
			printf(mtlPath);
		}else if (strcmp(lineHeader, "g") == 0) {
			//TODO: SET CURRENT GROUP
		}else if (strcmp(lineHeader, "usemtl") == 0) {
			//TODO: SET CURRENT MATERIAL
		}
		else{
			char discard[1000];
			fgets(discard, 1000, file);
		}
	}

	int currentGroup = 1;
	std::vector<glm::vec3> *verticesOut = new std::vector<glm::vec3>;
	for (int i = 0; i < vIndices.size(); i++) {
		glm::vec3 vertex = vertices[vIndices[i]];
		glm::vec2 uv = uvs[uvIndices[i]];
		glm::vec3 normal = normals[nIndices[i]];

		loadVerts.push_back(vertex);
		loadUVs.push_back(uv);
		loadNormals.push_back(normal);
	}
	return true;
}