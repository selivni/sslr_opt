#include "CameraHandler.h"

const GLfloat CameraHandler::SPEED_FORWARD_LIMIT = 1024.0;
const GLfloat CameraHandler::SPEED_BACKWARD_LIMIT = -1024.0;
const GLfloat CameraHandler::SPEED_VALUE_DEFAULT = 128.0;
const GLuint CameraHandler::TIME_VALUE_DEFAULT = 1000;
const GLfloat CameraHandler::SECOND_LENGTH_DEFAULT = 1000.0;
const unsigned char CameraHandler::ERROR_CONFIG[] = "Couldn't configure the "
	"time function!";

CameraHandler::CameraHandler():
	speed_(0.0),
	desiredSpeed_(0.0),
	timeSum_(0.0),
	timeLimit_(0.0),
	function(nullptr)
	{}

GLfloat CameraHandler::addSpeed(GLfloat value)
{
	GLfloat returnValue = SPEED_FORWARD_LIMIT - speed_;
	if (speed_ + value > SPEED_FORWARD_LIMIT)
		speed_ = SPEED_FORWARD_LIMIT;
	else
	{
		speed_ += value;
		returnValue = value;
	}
	speed_ /= SECOND_LENGTH_DEFAULT;
	desiredSpeed_ = speed_;
	refreshFunction(SECOND_LENGTH_DEFAULT, speed_, speed_, CH_CONST);
	return returnValue;
}

GLfloat CameraHandler::subSpeed(GLfloat value)
{
	GLfloat returnValue = SPEED_BACKWARD_LIMIT + speed_;
	if (speed_ - value < SPEED_BACKWARD_LIMIT)
		speed_ = SPEED_BACKWARD_LIMIT;
	else
	{
		speed_ -= value;
		returnValue = value;
	}
	speed_ /= SECOND_LENGTH_DEFAULT;
	desiredSpeed_ = speed_;
	refreshFunction(SECOND_LENGTH_DEFAULT, speed_, speed_, CH_CONST);
	return returnValue;
}

void CameraHandler::stop()
{
	speed_ = 0.0f;
	timeLimit_ = 0.0f;
	delete function;
	function = nullptr;
}

void CameraHandler::toggleSmoothStop(GLuint T, CH_Mode mode)
{
	timeSum_ = 0.0f;
	refreshFunction(T, speed_, 0.0f, mode);
	timeLimit_ = T;
	desiredSpeed_ = 0.0f;
}

void CameraHandler::toggleSmoothMove(GLuint T, GLfloat v1, CH_Mode mode)
{
	timeSum_ = 0.0f;
	if (v1 > SPEED_FORWARD_LIMIT)
		v1 = SPEED_FORWARD_LIMIT;
	else if (v1 < SPEED_BACKWARD_LIMIT)
		v1 = SPEED_BACKWARD_LIMIT;
	v1 /= SECOND_LENGTH_DEFAULT;
	refreshFunction(T, speed_, v1, mode);
	timeLimit_ = T;
	desiredSpeed_ = v1;
}

void CameraHandler::step(GLuint tickTime)
{
	if (function == nullptr)
		return;
	speed_ += function->getSpeedDelta(timeSum_, timeSum_ + tickTime);
	GLfloat distance = function->getDistance(timeSum_, timeSum_ + tickTime);
	goForward(distance);
	timeSum_ += tickTime;
	if (timeSum_ >= timeLimit_ && timeLimit_ != -1.0f)
	{
		speed_ = desiredSpeed_;
		timeLimit_ = -1.0f;
		refreshFunction(TIME_VALUE_DEFAULT, desiredSpeed_, desiredSpeed_,
			CH_CONST);
	}
/*
	std::cout << "speed: " << speed_ << " distance: " << distance
		<< std::endl;
*/
}

void CameraHandler::refreshFunction(GLuint T, GLfloat v0, GLfloat v1, CH_Mode mode)
{
	if (function != nullptr)
		delete function;
	if (mode == CH_CONST)
		function = new TimeFunctionConst;
	else if (mode == CH_LINEAR)
		function = new TimeFunctionLinear;
	else if (mode == CH_HYPERBOLIC)
		function = new TimeFunctionHyperbolic;
	else if (mode == CH_PARABOLIC)
		function = new TimeFunctionParabolic;
	else if (mode == CH_SQRT)
		function = new TimeFunctionSqrt;
	if (!function->configure(v0, v1, T))
	{
		std::cout << ERROR_CONFIG << std::endl;
		delete function;
		function = new TimeFunctionConst;
		function->configure(v0, v0, SECOND_LENGTH_DEFAULT);
	}
}
