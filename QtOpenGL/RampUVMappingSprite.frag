#version 330 core
in vec3 fragmentColor;
in vec2 UV;

out vec4 color;

uniform sampler2D TextureSampler0;
uniform sampler2D rampSampler;

uniform float Time;

void main()
{
	vec3 textureColor = texture(TextureSampler0, UV).rgb;
	vec3 rampColor = texture(rampSampler, 1-textureColor.rg).rgb;
	color = vec4(rampColor, 1.0f);
//	color = vec4(1.0, 0.0, 0.0, 1.0f);
//	color = vec4(fragmentColor * textureColor, 1.0f);
}