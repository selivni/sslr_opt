#include "TimeFunction.h"

const GLfloat TimeFunction::SECOND = 1000.0f;

TimeFunction::~TimeFunction()
	{}

GLfloat sqr(GLfloat x)
	{return x * x;}

GLfloat abs(GLfloat x)
{
	if (x < 0)
		return -x;
	return x;
}

//================================CONSTANT======================================

bool TimeFunctionConst::configure(GLfloat v0_, GLfloat v1_, GLfloat T_)
{
	if (T_ <= 0.0f)
		return false;
	v0 = v0_;
	configured = true;
	return true;
}

GLfloat TimeFunctionConst::getDistance(GLfloat t0, GLfloat t)
{
	if (!configured || t <= t0)
		return 0.0f;
	return v0 * (t - t0);
}

GLfloat TimeFunctionConst::getSpeedDelta(GLfloat t0, GLfloat t)
{
	return 0.0f;
}

TimeFunctionConst::TimeFunctionConst()
{
	configured = false;
}

//==================================LINEAR======================================

bool TimeFunctionLinear::configure(GLfloat v0_, GLfloat v1_, GLfloat T_)
{
	v0 = v0_;
	a = (v1_ - v0_) / T_;
	configured = true;
	return true;
}

GLfloat TimeFunctionLinear::getDistance(GLfloat t0, GLfloat t)
{
	if (!configured || t <= t0)
		return 0.0f;
	return v0 * (t - t0) + a * (sqr(t) - sqr(t0)) / 2.0f;
}

GLfloat TimeFunctionLinear::getSpeedDelta(GLfloat t0, GLfloat t)
{
	if (!configured || t <= t0)
		return 0.0f;
	return a * (t - t0);
}

TimeFunctionLinear::TimeFunctionLinear()
{
	configured = false;
}

//================================PARABOLIC=====================================

bool TimeFunctionParabolic::configure(
	GLfloat v0_, GLfloat v1_, GLfloat T_)
{
	v0 = v0_;
	a = (v1_ - v0_) / sqr(T_);
	configured = true;
	return true;
}

GLfloat TimeFunctionParabolic::getDistance(GLfloat t0, GLfloat t)
{
	if (!configured || t <= t0)
		return 0.0f;
	return v0 * (t - t0) + a * (t * t * t - t0 * t0 * t0) / 3.0f;
}

GLfloat TimeFunctionParabolic::getSpeedDelta(GLfloat t0, GLfloat t)
{
	if (!configured || t <= t0)
		return 0.0f;
	return a * (sqr(t) - sqr(t0));
}

TimeFunctionParabolic::TimeFunctionParabolic()
{
	configured = false;
}

//================================SQRT==========================================

bool TimeFunctionSqrt::configure(
	GLfloat v0_, GLfloat v1_, GLfloat T_)
{
	if (T_ <= 0)
		return false;
	v0 = v0_;
	a = (v1_ - v0_) / sqrt(T_);
	configured = true;
	return true;
}

GLfloat TimeFunctionSqrt::getDistance(GLfloat t0, GLfloat t)
{
	if (!configured || t <= t0 || t < 0.0f || t0 < 0.0f)
		return 0.0f;
	return v0 * (t - t0) + 2.0f * a * (t * sqrt(t) - t0 * sqrt(t0)) / 3.0f;
}

GLfloat TimeFunctionSqrt::getSpeedDelta(GLfloat t0, GLfloat t)
{
	if (!configured || t <= t0 || t < 0.0f || t0 < 0.0f)
		return 0.0f;
	return a * (sqrt(t) - sqrt(t0));
}

TimeFunctionSqrt::TimeFunctionSqrt()
{
	configured = false;
}

//================================HYPERBOLIC====================================

bool TimeFunctionHyperbolic::configure(
	GLfloat v0_, GLfloat v1_, GLfloat T_)
{
	actualV0 = v0;
	if (T_ <= 0 || v1_ == v0_)
		return false;
	if (v1_ > v0_)
		direction = 1.0f;
	else
		direction = -1.0f;
	GLfloat discriminant = sqr(T_) + direction * 4 * T_ / (v1_ - v0_);
	if (discriminant < 0.0f)
		return false;
	a = (direction * T_ - direction * sqrt(discriminant)) / 2.0f;
	if (a == 0.0f || a == T_)
		return false;
	v0 = v0_ - 1.0f / a;
	configured = true;
	return true;
}

GLfloat TimeFunctionHyperbolic::getDistance(GLfloat t0, GLfloat t)
{
	if (!configured || t <= t0)
		return 0.0f;
	if (a == direction * t0 || a == direction * t)
		return 0.0f;
	return v0 * (t - t0) - direction *
		(log(abs(a - direction * t)) - log(abs(a - direction * t0)));
}

GLfloat TimeFunctionHyperbolic::getSpeedDelta(GLfloat t0, GLfloat t)
{
	if (!configured || t <= t0)
		return 0.0f;
	if (abs(-direction * t + a) < 0.0000001f ||
		abs(-direction * t0 + a) < 0.0000001f)
		return 0.0f;
	return 1.0f / (-direction * t + a) - 1.0f / (-direction * t0 + a);
}

TimeFunctionHyperbolic::TimeFunctionHyperbolic()
{
	configured = false;
}
