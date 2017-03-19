#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;
in vec3 world_Pos;
in vec3 camera_normal;
in vec3 camera_vVector;
in vec3 camera_lVector;

// Ouput data
out vec3 color;

// Values that stay constant for the whole mesh.
uniform sampler2D texture_sampler;
uniform mat4 MV;
uniform vec3 world_lPos;

void main(){
	vec3 MaterialDiffuseColor = texture( texture_sampler, UV ).rgb;	
	color = MaterialDiffuseColor;
}