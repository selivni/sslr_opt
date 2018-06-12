#version 330 core

in vec3 uv;
in vec3 norm;
in vec3 pointPosition;

uniform sampler2DArray textures;

const uint DEFAULT = 0x00000001u;
const uint DEFAULT_LIGHTS = 0x00000002u;
const uint MRT = 0x00000003u;
const uint MRT_LIGHTS = 0x00000004u;
uniform uint mode;

//MOON
uniform sampler2D moonLightDepth;
uniform mat4 moonCam;
uniform vec3 moonDir;
//MOON

void getLights()
{
	vec3 texCoord = vec3(uv.x, 1 - uv.y, uv.z);
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
	gl_FragData[0] = moonLight * moonLightColor * texture(textures, texCoord);
}

void getMRT()
{
	vec3 texCoord = vec3(uv.x, 1 - uv.y, uv.z);
	gl_FragData[0] = texture(textures, texCoord);
	gl_FragData[1] = vec4(norm, 0.0);
	if (uv.z == 10.0)
		gl_FragData[2] = vec4(1.0, 0.0, 0.0, 1.0);
	else
		gl_FragData[2] = vec4(0.0, 0.0, 0.0, 1.0);
}

void getMRTLights()
{
	vec3 texCoord = vec3(uv.x, 1 - uv.y, uv.z);
	vec4 moonPerspective = moonCam * vec4(pointPosition, 1);
	moonPerspective.xy = vec2(0.5, 0.5) + vec2(0.5, 0.5) *
		moonPerspective.xy / moonPerspective.w;
	moonPerspective.z /= moonPerspective.w;
	float moonLight;
	if (dot(moonDir, norm) >= 0 ||
		moonPerspective.z >= texture(moonLightDepth, moonPerspective.xy).r * 1.10)
		moonLight = 0.1;
	else
		moonLight = 1.0;
	vec4 moonLightColor = vec4(0.3, 0.3, 1.0, 1.0);
	gl_FragData[0] = moonLight * moonLightColor * texture(textures, texCoord);
	gl_FragData[1] = vec4(norm, 0.0);
	if (uv.z == 10.0)
		gl_FragData[2] = vec4(1.0, 0.0, 0.0, 1.0);
	else
		gl_FragData[2] = vec4(0.0, 0.0, 0.0, 1.0);
}

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
	} else if (uv.z > 130.0)
	{
		gl_FragData[0] = vec4(60.0 / 256.0, 20.0 / 256.0, 0, 1);
		return;
	}
	if (mode == DEFAULT)
		gl_FragData[0] = texture(textures, texCoord);
	else if (mode == DEFAULT_LIGHTS)
		getLights();
	else if (mode == MRT)
		getMRT();
	else if (mode == MRT_LIGHTS)
		getMRTLights();
//	outColor = texture(textures, texCoord);

}
