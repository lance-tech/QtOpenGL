#version 330 core
in vec3 fragmentColor;
in vec2 UV;

out vec4 color;

uniform sampler2D TextureSampler0;

uniform float Time;

void main()
{
	vec3 textureColor = texture(TextureSampler0, UV).rgb;
	color = vec4(fragmentColor * textureColor, 1.0f);
}