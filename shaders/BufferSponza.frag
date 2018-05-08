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

uniform mat4 camera;
uniform vec3 cameraPosition;

out vec4 outColour;

/*Obtains world-space coordinates*/
vec3 getWSCoord(vec2 uv_, float depth_)
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
	return vec3(result.xy, result.z/result.w);
}

void main()
{
	const float alphaMultiplier = 50.0;
	const float highestPrecision = 0.0001;

	float const1 = 0.5, const2 = 0.5;
	float iterationLimit = 5.0;
	float pointPrecision;

	vec2 st = vec2((uv.x + 1) / 2, (uv.y + 1) / 2);
	vec4 normal_ = texture(normal, st);
	vec4 reflection_ = texture(reflection, st);
	vec4 depth_ = texture(depth, st);
	depth_ = vec4(depth_.r, depth_.r, depth_.r, 1.0);
	vec4 colour_ = texture(colour, st);
	vec2 recommendations_ = texture(recommendations, st).rg;

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
	pointPrecision = recommendations_.g;
	if (pointPrecision < highestPrecision)
		pointPrecision = highestPrecision;

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
		alpha = distance(currentPoint, getWSCoord(prCurrPoint.xy, realDepth));
		if (alpha * (realDepth - prCurrPoint.z) < 0.0)
			alpha = -alpha;
		iter += 1.0;
	} while ((abs(realDepth - prCurrPoint.z) > pointPrecision ||
			  iter < 2) && iter < iterationLimit);
	if (abs(prCurrPoint.x) > 1 || abs(prCurrPoint.y) > 1)
	{
		const1 = 1;
		const2 = 0;
	} else
	{
		const2 = 5 / sqrt(length(currentPoint - startingPoint));
		if (const2 > 1) const2 = 1;
		const1 = 1 - const2;
	}
	outColour = const1 * texture(colour, st) + const2 * texture(colour, prCurrPoint.xy);
}
