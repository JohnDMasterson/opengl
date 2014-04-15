#version 440

layout (location = 0) in vec3 Pos;
layout (location = 1) in vec3 Norm;

out vec3 tNorms;
out vec3 tPos;

uniform mat4 gWVP;

void main()
{
	tNorms = Norm;
	tPos = Pos;
	gl_Position = gWVP*vec4(Pos, 1.0);
}