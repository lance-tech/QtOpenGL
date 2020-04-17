#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNormal_modelsapce;

layout(location = 3) in vec3 vertexTangent_modelspace;
layout(location = 4) in vec3 vertexBitangent_modelspace;

// Output data ; will be interpolated for each fragment
out vec3 FragPos;
out vec3 Normal;
out vec2 UV;
out vec3 eyeVector;

out vec3 EyeDirection_cameraspace;
out vec3 LightDirection_cameraspace;

out vec3 LightDirection_tangentspace;
out vec3 EyeDirection_tangentspace;

// Values that stay constant for the whole mesh.
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform mat3 MV3x3;

uniform vec3 LightPosition_worldspace;
uniform vec3 ViewPos;
uniform vec3 WorldModelPos;

void main(){

	FragPos = vec3(M * vec4(vertexPosition_modelspace, 1.0));
	Normal = normalize(mat3(transpose(inverse(M))) * vertexNormal_modelsapce);  
	UV = vertexUV;

//	eyeVector = ViewPos - WorldModelPos;
	eyeVector = ViewPos - FragPos;
//	eyeVector = vec3(ViewRot * vec4(ViewPos - FragPos, 1.0));
//	eyeVector = ViewPos;

	
	vec3 vertexPosition_cameraspace = ( V * M * vec4(vertexPosition_modelspace,1)).xyz;
	EyeDirection_cameraspace = ViewPos - vertexPosition_cameraspace;

	// Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
	vec3 LightPosition_cameraspace = ( V * vec4(LightPosition_worldspace,1)).xyz;
	LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;

	// model to camera = ModelView
	vec3 vertexTangent_cameraspace = MV3x3 * vertexTangent_modelspace;
	vec3 vertexBitangent_cameraspace = MV3x3 * vertexBitangent_modelspace;
	vec3 vertexNormal_cameraspace = MV3x3 * vertexNormal_modelsapce;

	mat3 TBN = transpose(mat3(
		vertexTangent_cameraspace,
		vertexBitangent_cameraspace,
		vertexNormal_cameraspace	
	)); // You can use dot products instead of building this matrix and transposing it. See References for details.

	LightDirection_tangentspace = TBN * LightDirection_cameraspace;
	EyeDirection_tangentspace =  TBN * EyeDirection_cameraspace;

	gl_Position = P * V * M * vec4(vertexPosition_modelspace, 1);

}

