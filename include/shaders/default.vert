#version 430

layout (location = 0) in vec3 Pos;
layout (location = 1) in vec3 Norm;

out vec4 tNorms;


void main()
{
	gl_Position = vec4(Pos, 1.0);
	tNorms = vec4(Norm, 1.0);
}