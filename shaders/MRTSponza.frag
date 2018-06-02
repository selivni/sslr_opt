#version 330 core

in vec3 uv;
in vec3 norm;
in vec3 pointPosition;

uniform sampler2DArray textures;

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

	gl_FragData[0] = texture(textures, texCoord);
	gl_FragData[1] = vec4(norm, 0.0);
	if (uv.z == 10.0)
		gl_FragData[2] = vec4(1.0, 0.0, 0.0, 1.0);
	else
		gl_FragData[2] = vec4(0.0, 0.0, 0.0, 1.0);
}
