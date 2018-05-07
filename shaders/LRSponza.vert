#version 330 core

in vec3 point;

out vec3 uv;

void main()
{
	gl_Position = vec4(point, 1);
	uv = point;
}
