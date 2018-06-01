#version 330 core

in vec3 uv;
in vec3 norm;
in vec3 pointPosition;

uniform sampler2DArray textures;

out vec4 outColor;

void main()
{
	vec3 texCoord = vec3(uv.x, 1 - uv.y, uv.z) + norm - norm;/*DELETE NORM AFTERWARDS*/
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
		outColor = vec4(60.0 / 256.0, 20.0 / 256.0, 0, 1);
		return;
	}
	outColor = texture(textures, texCoord);
}
