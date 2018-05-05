#version 330 core

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

out vec4 outColour;

/*Obtains world-space coordinates*/
vec3 getWSCoord(vec2 uv_, float depth_)
{
	mat4 inverseViewProjection = inverse(projection * view);
	vec4 position = vec4(uv_.x * 2.0 - 1.0, (uv_.y * 2.0 - 1.0), depth_, 1);
	position = inverseViewProjection * position;
	position /= position.w;
	return position.xyz;
}

vec3 unproject(vec2 coordinates, float depth_)
{
	mat4 inversedProjection = inverse(projection);
	vec4 result = vec4(coordinates.x, coordinates.y,
		depth_, 1.0);
	result = inversedProjection * result;
	result /= result.w;
	return result.xyz;
}

vec3 getUV(vec3 point)
{
	vec4 result = vec4(point, 1);
	result = projection * view * result;
//	result /= result.w;
	result.x = (result.x/result.w + 1) / 2;
	result.y = (result.y/result.w + 1) / 2;
//	result.xy = vec2(0.5, 0.5) + vec2(0.5, -0.5) * result.xy / result.w;
	return vec3(result.xy, result.z/result.w);
}

void main()
{
	float const1 = 0.5, const2 = 0.5;

	vec2 st = vec2((uv.x + 1) / 2, (uv.y + 1) / 2);
	vec4 normal_ = texture(normal, st);
	vec4 reflection_ = texture(reflection, st);
	vec4 depth_ = texture(depth, st);
	depth_ = vec4(depth_.r, depth_.r, depth_.r, 1.0);
	vec4 colour_ = texture(colour, st);

/*THE REAL PROGRAM STARTS HERE*/
	if (reflection_.r < 0.9)
	{
		outColour = colour_;
		return;
	}
	st = vec2((uv.x + 1) / 2, (uv.y + 1) / 2);
	vec3 startingPoint = getWSCoord(st, texture(depth, st).r);
	vec4 camPos = vec4(cameraPosition, 1);
	camPos /= camPos.w;
	vec3 strikeVector = normalize(startingPoint - camPos.xyz);
	vec3 direction = normalize(reflect(strikeVector, normal_.xyz));
	float alpha = 50.0;
	vec3 currentPoint = startingPoint;
//	vec2 currST;
	vec3 prCurrPoint;
	float realDepth;
	float iter = 0.0;
	do
	{
		currentPoint = startingPoint + direction * alpha;
		prCurrPoint = getUV(currentPoint);
//		currST = vec2((prCurrPoint.x + 1) / 2, (prCurrPoint.y + 1) / 2);
//		prCurrPoint /= prCurrPoint.w;
		realDepth = texture(depth, prCurrPoint.xy).r;
		alpha = distance(startingPoint, getWSCoord(prCurrPoint.xy, realDepth));
//		if (alpha * (realDepth - prCurrPoint.z) < 0.0)
//			alpha = -alpha;
		iter += 1.0;
	} while (/*abs(realDepth - prCurrPoint.z) > 0.01 && */iter < 30.0);
	if (abs(prCurrPoint.x) > 1 || abs(prCurrPoint.y) > 1)
	{
		const1 = 1;
		const2 = 0;
	} else
	{
		const2 = 5 / sqrt(alpha);
		if (const2 > 1) const2 = 1;
		const1 = 1 - const2;
	}
	outColour = const1 * texture(colour, st) + const2 * texture(colour, prCurrPoint.xy);
}
