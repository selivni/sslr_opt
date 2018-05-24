#version 330 core

in vec3 uv;

uniform sampler2D colour;
uniform sampler2D normal;
uniform sampler2D reflection;
uniform sampler2D depth;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 cameraPosition;

mat4 inverseModel = inverse(model);
mat4 inverseProjectionView = inverse(projection * view);

/*Obtains world-space coordinates*/
vec3 getWorldSpace(vec2 uv_, float depth_)
{
	vec4 position = vec4(uv_.x * 2.0 - 1.0, uv_.y * 2.0 - 1.0, depth_, 1);
	position = inverseProjectionView * position;
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

vec4 getViewVector(vec3 currentPoint, vec3 startingPoint)
{
	vec4 returnValue = inverseModel * vec4(currentPoint - startingPoint, 1.0);
	returnValue /= returnValue.w;
	return returnValue;
}

float blendValue(vec4 viewVector)
{
	return 4.0 / pow(length(viewVector.xyz), 0.54);
}

void main()
{
	const float iterationLimit = 10;
	const float pointPrecision = 0.0005;

	const float lowestPrecision = 0.001;
	const float highestPrecision = 0.0001;

//	const float lowestPrecision = 0.001;
//	const float highestPrecision = 0.0001;

	const float a3 = -4.87013E-7;
	const float a2 = 1.85714E-5;
	const float a1 = -2.45877E-4;
	const float a0 = 1.22779E-3;

	float alpha = 50.0;
	const float maximalAlpha = 50.0;
	float recommendedPrecision = 0.0005;
	float precisionMultiplier = 500.0;

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
		if (iter > 1.5)
		{
			vec4 viewVector = getViewVector(currentPoint, startingPoint);
			if (blendValue(viewVector) > 1)
				break;
		}
		if (abs(prCurrPoint.x) > 1.1 || abs(prCurrPoint.y) > 1.1)
			break;

		realDepth = texture(depth, prCurrPoint.xy).r;
		alpha = distance(currentPoint, getWorldSpace(prCurrPoint.xy, realDepth));
		if (alpha > 0.0 && realDepth - prCurrPoint.z < 0.0)
			alpha = -alpha;
		if (alpha < 0.0 && realDepth - prCurrPoint.z > 0.0)
		{
			currentPoint = startingPoint + direction * 50.0;
			prCurrPoint.x = 1.1;
			break;
		}
		iter += 1.0;
	} while ((abs(realDepth - prCurrPoint.z) > pointPrecision ||
		iter < 5) && iter < iterationLimit);

	if (prCurrPoint.x > 1.0 || prCurrPoint.y > 1.0 ||
		prCurrPoint.x < -0.0 || prCurrPoint.y < -0.0)
	{
		result = vec4(0.5 * log(length(currentPoint - startingPoint)) /
			maximalAlpha, lowestPrecision * precisionMultiplier, 0, 0);
	} else
	{
		vec4 viewVector = getViewVector(currentPoint, startingPoint);
		float viewDistance = length(viewVector.xyz) / 100;
		float viewDistance2 = viewDistance * viewDistance;
		float viewDistance3 = viewDistance2 * viewDistance;
		if (viewDistance >= 13)
			recommendedPrecision = highestPrecision;
		else if (viewDistance <= 1)
			recommendedPrecision = lowestPrecision;
		else
			recommendedPrecision = a3 * viewDistance3 + a2 * viewDistance2
				+ a1 * viewDistance + a0;
		result = vec4(log(length(currentPoint - startingPoint)) / 
			maximalAlpha, recommendedPrecision * precisionMultiplier, 0, 0);
	}
}
