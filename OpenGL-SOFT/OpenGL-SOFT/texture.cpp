#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>
#include <GLFW\glfw3.h>



GLuint loadBMP(const char * imagepath){
	unsigned char header[54];
	unsigned int imageSize;
	unsigned int width, height;
	unsigned char * img;

	// Open the file
	FILE * file = fopen(imagepath,"rb");
	if (file == NULL) {
		printf("Couldn't open file");
		return false;
	}

	//Read the header of the file
	fread(header, 1, 54, file);

	// Read the information about the image
	imageSize  = *(int*)&(header[0x22]);
	width      = *(int*)&(header[0x12]);
	height     = *(int*)&(header[0x16]);

	// Create a buffer
	img = new unsigned char [imageSize];

	// Read the actual data from the file into the buffer
	fread(img,1,imageSize,file);
	fclose (file);

	GLuint textureID;
	glGenTextures(1, &textureID);
	
	// Copy the data from the buffer in to openGL and delete the buffer
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, img);
	delete [] img;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
	glGenerateMipmap(GL_TEXTURE_2D);

	// Return the ID of the texture
	return textureID;
}