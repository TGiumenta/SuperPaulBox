#version 440 core

uniform mat4 WorldViewProjection;

layout (location = 0) in vec4 Position;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

void main()
{	
	gl_Position = WorldViewProjection * Position;
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}