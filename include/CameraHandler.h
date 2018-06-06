#pragma once

#include "GL/Camera.h"
#include "TimeFunction.h"

enum CH_Mode
{
	CH_NONE,
	CH_LINEAR,
	CH_HYPERBOLIC,
	CH_PARABOLIC,
	CH_SQRT
};

class CameraHandler: public GL::Camera
{
public:
	CameraHandler();
	GLfloat addSpeed(GLfloat=SPEED_VALUE_DEFAULT);
	GLfloat subSpeed(GLfloat=SPEED_VALUE_DEFAULT);
	void stop();
	bool toggleSmoothStop(GLuint=TIME_VALUE_DEFAULT, CH_Mode=CH_LINEAR);
	bool toggleSmoothMove(GLuint=TIME_VALUE_DEFAULT, GLfloat=SPEED_FORWARD_LIMIT,
		CH_Mode=CH_LINEAR);
	GLfloat step(GLuint);
private:
	static const GLfloat SPEED_FORWARD_LIMIT;
	static const GLfloat SPEED_BACKWARD_LIMIT;
	static const GLfloat SPEED_VALUE_DEFAULT;
	static const GLuint TIME_VALUE_DEFAULT;
	static const GLfloat SECOND_LENGTH_DEFAULT;
	GLfloat speed_;
	GLfloat value0_;//Starting value
	CH_Mode smoothMode_;
	TimeFunction* function;
};
