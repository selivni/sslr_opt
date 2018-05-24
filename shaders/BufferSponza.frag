#version 330 core

in vec3 uv;

uniform sampler2D colour;
uniform sampler2D normal;
uniform sampler2D reflection;
uniform sampler2D depth;
uniform sampler2D recommendations;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 cameraPosition;
uniform vec2 windowSize;

out vec4 outColour;

mat4 inverseModel = inverse(model);
mat4 inverseProjectionView = inverse(projection * view);

/*Obtains world-space coordinates*/
vec3 getWSCoord(vec2 uv_, float depth_)
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
	return vec3(result.xy, result.z/result.w);
}

float sqr(float x)
{
	return x * x;
}

#define M_PI 3.1415926535897932384626433832795
float gaussValue(float x, float y, float sigma)
{
	if (sigma <= 0.01)
	{
		if (abs(x) < 0.1 && abs(y) < 0.1)
			return 1.0;
		else
			return 0.0;
	}
	return exp(-(sqr(x) + sqr(y))/(2 * sqr(sigma)))/(2 * M_PI * sqr(sigma));
}

vec3 filterBlur(vec2 st, float strength)
{
//	strength = sqrt(strength * strength * strength);
	float radiusDivider = 100.0;
	float strengthDivider = 10.0;
/*
	mat3 matrix1 = mat3 
	(
		vec3(1.0/16, 1.0/8, 1.0/16),
		vec3(1.0/8, 1.0/4, 1.0/8),
		vec3(1.0/16, 1.0/8, 1.0/16)
	);
	strength /= strengthDivider;
	mat3 newFil = matrix1;
	newFil[0][0] = newFil[0][2] = newFil[2][0] = newFil[2][2] *= strength / 4;
	newFil[0][1] = newFil[1][0] = newFil[1][2] = newFil[2][1] *= strength / 2;
*/
	strength = pow(pow(strength, 3.0) / pow(200, 3.0), 1);
	int i, j;
	mat3 newFil;
	float normSum = 0.0;
	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
		{
			newFil[i][j] = gaussValue(i - 1, j - 1, strength);
			normSum += newFil[i][j];
		}
	vec3 sum = newFil[0] + newFil[1] + newFil[2];
	newFil /= sum.x + sum.y + sum.z;
	float horDeviation = strength / windowSize.x;
	float vertDeviation = strength / windowSize.y;
	if (horDeviation > 4 / windowSize.x)
		horDeviation = 4 / windowSize.x;
	if (vertDeviation > 4 / windowSize.y)
		vertDeviation = 4 / windowSize.y;
	vec2 step = vec2(horDeviation, vertDeviation);
	mat3 topRow = mat3 
	(
		texture(colour, st + vec2(-step.x, step.y)).xyz * newFil[0][0],
		texture(colour, st + vec2(0, step.y)).xyz * newFil[0][1],
		texture(colour, st + vec2(step.x, step.y)).xyz * newFil[0][2]
	);
	mat3 midRow = mat3 
	(
		texture(colour, st + vec2(-step.x, 0)).xyz * newFil[1][0],
		texture(colour, st + vec2(0, 0)).xyz * newFil[1][1],
		texture(colour, st + vec2(step.x, 0)).xyz * newFil[1][2]
	);
	mat3 lowRow = mat3 
	(
		texture(colour, st + vec2(-step.x, -step.y)).xyz * newFil[2][0],
		texture(colour, st + vec2(0, -step.y)).xyz * newFil[2][1],
		texture(colour, st + vec2(step.x, -step.y)).xyz * newFil[2][2]
	);
/*
	topRow[0] *= newFil[0][0];
	topRow[1] *= newFil[0][1];
	topRow[2] *= newFil[0][2];
	midRow[0] *= newFil[1][0];
	midRow[1] *= newFil[1][1];
	midRow[2] *= newFil[1][2];
	lowRow[0] *= newFil[2][0];
	lowRow[1] *= newFil[2][1];
	lowRow[2] *= newFil[2][2];
*/

	topRow = topRow + midRow + lowRow;
	return topRow[0] + topRow[1] + topRow[2];
}

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
	const float alphaMultiplier = 50.0;
	const float precisionDivider = 500.0;
	const float highestPrecision = 0.0001;

	float const1 = 0.5, const2 = 0.5;
	float iterationLimit = 10.0;
	float pointPrecision;

	vec2 st = vec2((uv.x + 1) / 2, (uv.y + 1) / 2);
	vec4 normal_ = texture(normal, st);
	vec4 reflection_ = texture(reflection, st);
	vec4 depth_ = texture(depth, st);
	depth_ = vec4(depth_.r, depth_.r, depth_.r, 1.0);
	vec4 colour_ = texture(colour, st);
	vec2 recommendations_ = texture(recommendations, st).rg;

//	outColour = vec4(0, recommendations_.g , 0, 1);
//	outColour = vec4(recommendations_, 0, 1);
//	return;

/*THE REAL PROGRAM STARTS HERE*/
	if (reflection_.r < 0.9)
	{
		outColour = colour_;
		return;
	}
	vec3 startingPoint = getWSCoord(st, depth_.r);
	vec4 camPos = model * vec4(cameraPosition, 1);
	vec3 strikeVector = (startingPoint - camPos.xyz);
	vec3 direction = normalize(reflect(strikeVector, normal_.xyz));

	float alpha = exp(recommendations_.r * alphaMultiplier);
	pointPrecision = recommendations_.g / precisionDivider;
	if (pointPrecision < highestPrecision)
		pointPrecision = highestPrecision;

	vec3 currentPoint = startingPoint;
	vec3 prCurrPoint;
	float realDepth;
	float iter = 0.0;
	do
	{
		currentPoint = currentPoint + direction * alpha;
		if (iter > 1.5)
		{
			vec4 viewVector = getViewVector(currentPoint, startingPoint);
			if (blendValue(viewVector) > 1)
				break;
		}
		prCurrPoint = getUV(currentPoint);

		if (abs(prCurrPoint.x) > 1.1 || abs(prCurrPoint.y) > 1.1)
			break;

		realDepth = texture(depth, prCurrPoint.xy).r;
		alpha = distance(currentPoint, getWSCoord(prCurrPoint.xy, realDepth));
		if (alpha > 0.0 && realDepth - prCurrPoint.z < 0.0)
			alpha = -alpha;
		if (alpha < 0.0 && realDepth - prCurrPoint.z > 0.0)
		{
			prCurrPoint.x = 1.1;
			break;
		}
//		if (alpha * (realDepth - prCurrPoint.z) < 0.0)
//			alpha = -alpha;
		iter += 1.0;
	} while ((abs(realDepth - prCurrPoint.z) > pointPrecision ||
			  iter < 2) && iter < iterationLimit);
	vec4 viewVector = getViewVector(currentPoint, startingPoint);
	if (abs(prCurrPoint.x) > 1 || abs(prCurrPoint.y) > 1)
	{
		const1 = 1;
		const2 = 0;
	} else
	{
		const2 = blendValue(viewVector);
		if (const2 > 1) const2 = 1;
		const1 = 1 - const2;
	}
//	outColour = vec4(filterBlur(prCurrPoint.xy, length(viewVector)), 1);
	outColour = const1 * texture(colour, st) +
		const2 * vec4(filterBlur(prCurrPoint.xy, length(viewVector)), 1);
}
