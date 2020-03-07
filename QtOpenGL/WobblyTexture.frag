#version 330 core

in vec2 UV;

out vec3 color;

uniform sampler2D renderedTexture;
uniform sampler2D noiseTexture;
uniform float time;

void main(){
//	color = texture( renderedTexture, vec2( sin(time) * 10 + UV.x, UV.y ) ).xyz ;

	vec3 noiseColor = texture( noiseTexture, vec2(UV.x + (time * 0.002), UV.y)).xyz;
//	color = noiseColor;
	color = texture( renderedTexture, vec2(UV.x + (noiseColor.x * 0.08), UV.y)).xyz;
//	color = vec3(color.x, color.x, color.x);
}