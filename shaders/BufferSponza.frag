#version 330 core

in vec3 uv;

uniform sampler2D colour;
uniform sampler2D normal;
uniform sampler2D reflection;
uniform sampler2D depth;

out vec4 outColour;

void main()
{
	vec2 st = vec2((uv.x + 1) / 2, (uv.y + 1) / 2);
	vec4 normal_ = texture(normal, st);
	vec4 reflection_ = texture(reflection, st);
	vec4 depth_ = texture(depth, st);
	depth_ = vec4(depth_.r, depth_.r, depth_.r, 1.0);
	vec4 colour_ = texture(colour, st);
	outColour = depth_;
}
