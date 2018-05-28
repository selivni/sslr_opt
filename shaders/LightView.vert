#version 330 core

in vec3 point;
in vec3 uvCoord;
in vec3 normal;

out float depth;

uniform mat4 camera;

void main()
{
	gl_Position = camera * vec4(point, 1);
	depth = gl_Position.z / gl_Position.w;
}
