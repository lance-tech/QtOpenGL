#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;

uniform mat4 MVP;

// Output data ; will be interpolated for each fragment.
out vec2 UV;

void main(){
	vec4 outPosition = MVP * vec4(vertexPosition_modelspace, 1);
	gl_Position = inverse(MVP)* outPosition;
//	inverse(MVP)
//	gl_Position =  vec4(vertexPosition_modelspace,1);
	UV = (vertexPosition_modelspace.xy+vec2(1,1))/2.0;
}

