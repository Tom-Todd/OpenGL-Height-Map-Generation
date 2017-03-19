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

	vec3 LightColor = vec3(1,1,0.8);
	float LightPower = 500000.0f;

	vec3 MaterialDiffuseColor = texture( texture_sampler, UV ).rgb;
	vec3 MaterialAmbientColor = vec3(0.1,0.1,0.1) * MaterialDiffuseColor;
	vec3 MaterialSpecularColor = vec3(0.3,0.3,0.3);

	float distance = length( world_lPos - world_Pos );

	vec3 n = normalize( camera_normal );
	vec3 l = normalize( camera_lVector );
	float cosTheta = clamp( dot( n,l ), 0,1 );

	vec3 E = normalize(camera_vVector);
	vec3 R = reflect(-l,n);
	float cosAlpha = clamp( dot( E,R ), 0,1 );
	
	color = MaterialAmbientColor + MaterialDiffuseColor * LightColor * LightPower * cosTheta / (distance*distance) + MaterialSpecularColor * LightColor * LightPower * pow(cosAlpha,5) / (distance*distance);

}