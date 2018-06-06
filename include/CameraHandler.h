#pragma once

#include <iostream>

#include "GL/Camera.h"
#include "TimeFunction.h"

enum CH_Mode
{
	CH_NONE,
	CH_CONST,
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
	void toggleSmoothStop(GLuint=TIME_VALUE_DEFAULT, CH_Mode=CH_LINEAR);
	void toggleSmoothMove(GLuint=TIME_VALUE_DEFAULT,
		GLfloat=SPEED_FORWARD_LIMIT, CH_Mode=CH_LINEAR);
	void step(GLuint);
private:
	static const GLfloat SPEED_FORWARD_LIMIT;
	static const GLfloat SPEED_BACKWARD_LIMIT;
	static const GLfloat SPEED_VALUE_DEFAULT;
	static const GLuint TIME_VALUE_DEFAULT;
	static const GLfloat SECOND_LENGTH_DEFAULT;
	static const unsigned char ERROR_CONFIG[];
	GLfloat speed_;
	GLfloat desiredSpeed_;
	GLfloat timeSum_;

	GLfloat timeLimit_;

	TimeFunction* function;

	void refreshFunction(GLuint, GLfloat, GLfloat, CH_Mode);
};
