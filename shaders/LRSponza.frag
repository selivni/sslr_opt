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

/*Obtains world-space coordinates*/
vec3 getWorldSpace(vec2 uv_, float depth_)
{
	mat4 inverseViewProjection = inverse(projection * view);
	vec4 position = vec4(uv_.x * 2.0 - 1.0, uv_.y * 2.0 - 1.0, depth_, 1);
	position = inverseViewProjection * position;
	position /= position.w;
	return position.xyz;
}

vec3 getUV(vec3 point)
{
	vec4 result = vec4(point, 1);
	result = projection * view * result;
	result.xy = vec2(0.5, 0.5) + vec2(0.5, 0.5) * result.xy / result.w;
	return vec3(result.xy, result.z / result.w);
}

out vec4 result;

void main()
{
	const float iterationLimit = 50;
	const float pointPrecision = 0.0005;

	const float lowestPrecision = 0.001;
	const float highestPrecision = 0.0001;

	const float a2 = 0.02299423076;
	const float a1 = -0.32199423076;
	const float a0 = 0.3;

	float alpha = 50.0;
	const float maximalAlpha = 50.0;
	float recommendedPrecision = 0.0005;

	vec2 st = vec2((uv.x + 1) / 2, (uv.y + 1) / 2);
	vec4 colour_ = texture(colour, st);
	vec4 normal_ = texture(normal, st);
	vec4 reflection_ = texture(reflection, st);
	vec4 depth_ = texture(depth, st);

	if (reflection_.r < 0.9)
	{
		result = vec4(0.0, 0.0, 0.0, 0.0);
		return;
	}
	vec3 startingPoint = getWorldSpace(st, depth_.r);
	vec4 camPos = model * vec4(cameraPosition, 1);
	camPos /= camPos.w;
	vec3 strikeVector = startingPoint - camPos.xyz;
	vec3 direction = normalize(reflect(strikeVector, normal_.xyz));

	vec3 currentPoint = startingPoint;
	vec3 prCurrPoint;
	float realDepth;
	float iter = 0.0;
	do
	{
		currentPoint = currentPoint + direction * alpha;
		prCurrPoint = getUV(currentPoint);

		if (abs(prCurrPoint.x) > 1.1 || abs(prCurrPoint.y) > 1.1)
			break;

		realDepth = texture(depth, prCurrPoint.xy).r;
		alpha = 0.5 * distance(currentPoint, getWorldSpace(prCurrPoint.xy, realDepth));
//		if (alpha * (realDepth - prCurrPoint.z) < 0.0)
//			alpha = -alpha;
		iter += 1.0;
	} while ((abs(realDepth - prCurrPoint.z) > pointPrecision ||
		iter < 5) && iter < iterationLimit);

	if (abs(prCurrPoint.x) > 1.01 || abs(prCurrPoint.y) > 1.01)
	{
		result = vec4(0.5 * log(length(currentPoint - startingPoint)) /
			maximalAlpha, lowestPrecision, 0, 0);
	} else
	{
		float viewDistance = length(camPos.xyz - currentPoint) / 100;
		if (viewDistance >= 13)
			recommendedPrecision = highestPrecision;
		else if (viewDistance <= 1)
			recommendedPrecision = lowestPrecision;
		else
			recommendedPrecision = 
				a2 * viewDistance * viewDistance + a1 * viewDistance + a0;
		result = vec4(log(length(currentPoint - startingPoint)) / 
			maximalAlpha, recommendedPrecision, 0, 0);
	}
}
