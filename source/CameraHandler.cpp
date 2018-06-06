#include "CameraHandler.h"

const GLfloat CameraHandler::SPEED_FORWARD_LIMIT = 1024.0;
const GLfloat CameraHandler::SPEED_BACKWARD_LIMIT = -1024.0;
const GLfloat CameraHandler::SPEED_VALUE_DEFAULT = 128.0;
const GLuint CameraHandler::TIME_VALUE_DEFAULT = 1000;
const GLfloat CameraHandler::SECOND_LENGTH_DEFAULT = 1000.0;

CameraHandler::CameraHandler():
	speed_(0.0),
	value0_(0.0),
	smoothMode_(CH_NONE),
	function(nullptr),
	{}

GLfloat CameraHandler::addSpeed(GLfloat value)
{
	GLfloat returnValue = SPEED_FORWARD_LIMIT - speed_;
	if (speed_ + value > SPEED_FORWARD_LIMIT)
	{
		speed_ = SPEED_FORWARD_LIMIT;
		return returnValue;
	} else
	{
		speed_ += value;
		return value;
	}
}

GLfloat CameraHandler::subSpeed(GLfloat value)
{
	GLfloat returnValue = SPEED_BACKWARD_LIMIT + speed_;
	if (speed_ - value < SPEED_BACKWARD_LIMIT)
	{
		speed_ = SPEED_BACKWARD_LIMIT;
		return returnValue;
	} else
	{
		speed_ -= value;
		return value;
	}
}

void CameraHandler::stop()
{
	speed_ = 0.0;
	smoothMode_ = CH_NONE;
}
