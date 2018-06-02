#pragma once

struct TexturesInfo
{
	int ambientIndex;
	int opacityIndex;//Can be -1
	bool fake;//Only an ambient texture can be fake
	TexturesInfo(int, int, bool);
};

