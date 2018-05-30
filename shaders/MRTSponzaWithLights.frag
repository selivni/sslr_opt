#version 330 core

in vec3 uv;
in vec3 norm;
in vec3 pointPosition;

uniform uint material;

uniform sampler2DArray textures;

uniform sampler2D moonLightDepth;
uniform mat4 moonCam;
uniform vec3 moonDir;

void main()
{
	float materialIndex = material;
	vec3 texCoord = vec3(uv.x, 1 - uv.y, materialIndex);
	if (material == uint(1))
	{
		vec3 opacCoord = vec3(uv.x, 1 - uv.y, 27.0);
		if (texture(textures, opacCoord).y < 0.3)
			discard;
	} else if (material == uint(3))
	{
		vec3 opacCoord = vec3(uv.x, 1 - uv.y, 28.0);
		if (texture(textures, opacCoord).y < 0.3)
			discard;
	} else if (material == uint(22))
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
		moonPerspective.z >= texture(moonLightDepth, moonPerspective.xy).r * 1.10)
		moonLight = 0.1;
	else
		moonLight = 1.0;
	vec4 moonLightColor = vec4(0.3, 0.3, 1.0, 1.0);
	gl_FragData[0] = moonLight * moonLightColor * texture(textures, texCoord);
	gl_FragData[1] = vec4(norm, 0.0);
	if (material == uint(10))
		gl_FragData[2] = vec4(1.0, 0.0, 0.0, 1.0);
	else
		gl_FragData[2] = vec4(0.0, 0.0, 0.0, 1.0);
}
