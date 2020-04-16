#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNormal_modelsapce;

// Output data ; will be interpolated for each fragment
out vec3 FragPos;
out vec3 Normal;
out vec2 UV;
out vec3 eyeVector;

// Values that stay constant for the whole mesh.
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

uniform mat4 ViewRot;

uniform vec3 LightPosition_worldspace;
uniform vec3 ViewPos;
uniform vec3 WorldModelPos;

void main(){

	FragPos = vec3(M * vec4(vertexPosition_modelspace, 1.0));
	Normal = normalize(mat3(transpose(inverse(M))) * vertexNormal_modelsapce);  
	UV = vertexUV;

	eyeVector = ViewPos - WorldModelPos;
//	eyeVector = ViewPos - FragPos;
//	eyeVector = vec3(ViewRot * vec4(ViewPos - FragPos, 1.0));
//	eyeVector = ViewPos;

	gl_Position = P * V * M * vec4(vertexPosition_modelspace, 1);
//	gl_Position = P * V * M * vec4(FragPos, 1);
//	gl_Position = MVP * vec4(FragPos, 1);

}

