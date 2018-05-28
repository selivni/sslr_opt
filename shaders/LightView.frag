#version 330 core

in float depth;

void main()
{
	gl_FragDepth = depth;
}
