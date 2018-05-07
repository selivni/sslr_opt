#version 330 core;

in vec3 uv;

uniform sampler2D colour;
uniform sampler2D normal;
uniform sampler2D reflection;
uniform sampler2D depth;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 camera;
uniform vec3 cameraPosition;

out ???

/*Obtains world-space coordinates*/
vec3 getWorldSpace(vec2 uv_, float depth_)
{
	mat4 inverseViewProjection = inverse(projection * view);
	vec4 position = vec4(uv.x * 2.0 - 1.0, depth_, 1);
	position = inverseViewProjection * position;
	position /= position.w;
	return position.xyz;
}

vec3 getUV(vec3 point)
{
	vec4 result = vec4(point, 1);
	result = projection * view * result;
	result.xy = vec2(0.5, 0.5) + vec2(0.5, 0.5) * result.xy / result.w;
}

void main()
{
	float iterationLimit = 50;
	float pointPrecision = 0.0005;

	float alpha = 50.0;
	float recommendedPrecision;

	vec2 st = vec2((uv.x + 1) / 2, (uv.y + 1) / 2);
	vec4 colour_ = texture(colour, st);
	vec4 normal_ = texture(normal, st);
	vec4 reflection_ = texture(reflection, st);
	vec4 depth_ = texture(depth, st);

	if (reflection_.r < 0.9)
	{
		
	}
}
