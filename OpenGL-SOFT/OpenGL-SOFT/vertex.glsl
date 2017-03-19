#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNormal_modelspace;

// Output data ; will be interpolated for each fragment.
out vec2 UV;
out vec3 world_Pos;
out vec3 camera_normal;
out vec3 camera_vVector;
out vec3 camera_lVector;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;
uniform vec3 world_lPos;

void main(){

	gl_Position =  MVP * vec4(vertexPosition_modelspace,1);
	world_Pos = (M * vec4(vertexPosition_modelspace,1)).xyz;
	vec3 vertexPosition_cameraspace = ( V * M * vec4(vertexPosition_modelspace,1)).xyz;
	camera_vVector = vec3(0,0,0) - vertexPosition_cameraspace;
	vec3 LightPosition_cameraspace = (M * V * vec4(world_lPos,1)).xyz;
	camera_lVector = LightPosition_cameraspace + camera_vVector;
	camera_normal = ( V * M * vec4(vertexNormal_modelspace,0)).xyz;
	UV = vertexUV;
}

