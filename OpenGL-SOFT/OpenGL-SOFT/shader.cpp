#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include "shader.hpp"
using namespace std;

GLuint LoadShaders(const char * vertex_path ,const char * fragment_path){

	//Create the shaders
	GLuint vsID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fsID = glCreateShader(GL_FRAGMENT_SHADER);

	//Read the Vertex Shader code from the file
	std::string vsCode;
	std::ifstream vsStream(vertex_path, std::ios::in);
	if(vsStream.is_open()){
		std::string Line = "";
		while(getline(vsStream, Line))
			vsCode += "\n" + Line;
		vsStream.close();
	}else{
		printf("File %s not found.\n", vertex_path);
		getchar();
		return 0;
	}

	//Read the Fragment Shader code from the file
	std::string fsCode;
	std::ifstream fsStream(fragment_path, std::ios::in);
	if(fsStream.is_open()){
		std::string Line = "";
		while(getline(fsStream, Line))
			fsCode += "\n" + Line;
		fsStream.close();
	}else {
		printf("File %s not found.\n", fragment_path);
		getchar();
		return 0;
	}

	//Compile the vertex shader
	char const * VertexSourcePointer = vsCode.c_str();
	glShaderSource(vsID, 1, &VertexSourcePointer , NULL);
	glCompileShader(vsID);
	//Compile the fragment shader
	char const * FragmentSourcePointer = fsCode.c_str();
	glShaderSource(fsID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(fsID);
	//Link the program
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, vsID);
	glAttachShader(ProgramID, fsID);
	glLinkProgram(ProgramID);
	return ProgramID;
}


