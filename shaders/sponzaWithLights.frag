#version 330 core

in vec3 uv;
in vec3 norm;
in vec3 pointPosition;

uniform sampler2DArray textures;

uniform sampler2D moonLightDepth;
uniform mat4 moonCam;
uniform vec3 moonDir;

out vec4 outColor;

void main()
{
	vec3 texCoord = vec3(uv.x, 1 - uv.y, uv.z);
	if (uv.z == 1.0)
	{
		vec3 opacCoord = vec3(uv.x, 1 - uv.y, 27.0);
		if (texture(textures, opacCoord).y < 0.3)
			discard;
	} else if (uv.z == 3.0)
	{
		vec3 opacCoord = vec3(uv.x, 1 - uv.y, 28.0);
		if (texture(textures, opacCoord).y < 0.3)
			discard;
	} else if (uv.z == 22.0)
	{
		vec3 opacCoord = vec3(uv.x, 1 - uv.y, 29.0);
		if (texture(textures, opacCoord).y < 0.3)
			discard;
	}
	vec4 moonPerspective = moonCam * vec4(pointPosition, 1);
	moonPerspective.xy = vec2(0.5, 0.5) + vec2(0.5, 0.5) *
		moonPerspective.xy / moonPerspective.w;
	moonPerspective.z /= moonPerspective.w;
	float moonLight;
	if (dot(moonDir, norm) >= 0 ||
		moonPerspective.z >= texture(moonLightDepth, moonPerspective.xy).r * 1.07)
		moonLight = 0.1;
	else
		moonLight = 1.0;
	vec4 moonLightColor = vec4(0.3, 0.3, 1.0, 1.0);
	outColor = moonLight * moonLightColor * texture(textures, texCoord);
//	outColor = vec4((moonPerspective.z), texture(moonLightDepth, moonPerspective.xy).r, moonPerspective.z, 1);
//	outColor = vec4(moonPerspective.z, moonPerspective.z, moonPerspective.z, 1);
}
