#version 450

//shader input
layout (location = 0) in vec4 inColor;
layout (location = 1) in vec2 inUV;


layout(set = 0, binding = 0) uniform sampler2D displayTexture;

//output write
layout (location = 0) out vec4 outFragColor;

void main() 
{
	//return red
	outFragColor = texture(displayTexture, inUV) * outFragColor;
}

