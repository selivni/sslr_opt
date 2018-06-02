#pragma once
#include "Utility.h"

struct CameraInfo
{
public:
	VM::mat4 model;
	VM::mat4 view;
	VM::mat4 projection;
	static VM::mat4 ortho(float, float, float, float, float, float);
	static VM::mat4 lookAt(const VM::vec3&, const VM::vec3&, const VM::vec3&);
	VM::mat4 getMatrix();
private:
	static float dot(const VM::vec3&, const VM::vec3&);
};

