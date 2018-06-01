#include "CameraInfo.h"

VM::mat4 CameraInfo::ortho(float left, float right, float bottom, float top,
	float zNear, float zFar)
{
	VM::mat4 result(1);
	result[0][0] = 2.0 / (right - left);
	result[1][1] = 2.0 / (top - bottom);
	result[2][2] = -1 / (zFar - zNear);
	result[3][0] = -(right + left) / (right - left);
	result[3][1] = -(top + bottom) / (top - bottom);
	result[3][2] = -zNear / (zFar - zNear);
	return result;
}

VM::mat4 CameraInfo::lookAt(const VM::vec3& eye, const VM::vec3& center,
	const VM::vec3& up)
{
	VM::vec3 f = VM::normalize(center - eye);
	VM::vec3 s;
	try{
		s = VM::normalize(VM::cross(f, up));
	}
	catch (const char*)
	{
		s = VM::vec3(0, 0, 0);
	}
	VM::vec3 u = VM::cross(s, f);

	VM::mat4 result(1);
	
	result[0][0] = s.x;
	result[1][0] = s.y;
	result[2][0] = s.z;
	result[0][1] = u.x;
	result[1][1] = u.y;
	result[2][1] = u.z;
	result[0][2] =-f.x;
	result[1][2] =-f.y;
	result[2][2] =-f.z;
	result[3][0] =-dot(s, eye);
	result[3][1] =-dot(u, eye);
	result[3][2] = dot(f, eye);
	return result;
}

float CameraInfo::dot(const VM::vec3& x, const VM::vec3& y)
{
	VM::vec3 temp = x * y;
	return temp.x + temp.y + temp.z;
}

VM::mat4 CameraInfo::getMatrix()
{
	return projection * view * model;
}
