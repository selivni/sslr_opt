#pragma once
#include <GL/gl.h>
#include <cmath>

class TimeFunction
{
public:
	virtual bool configureFunction(GLfloat v0, GLfloat v1, GLfloat T) = 0;
	virtual GLfloat getDistance(GLfloat t0, GLfloat t) = 0;
	virtual GLfloat getSpeedDelta(GLfloat t0, GLfloat t) = 0;
private:
	static const GLfloat defaultT;
};

class TimeFunctionConst: public TimeFunction
{
public:
	bool configureFunction(GLfloat v0_, GLfloat v1_, GLfloat T_);
	GLfloat getDistance(GLfloat t0, GLfloat t);
	GLfloat getSpeedDelta(GLfloat t0, GLfloat t);
	TimeFunctionConst();
private:
	GLfloat v0;
	bool configured;
};

class TimeFunctionLinear: public TimeFunction
{
public:
	bool configureFunction(GLfloat v0_, GLfloat v1_, GLfloat T_);
	GLfloat getDistance(GLfloat t0, GLfloat t);
	GLfloat getSpeedDelta(GLfloat t0, GLfloat t);
	TimeFunctionLinear();
private:
	GLfloat v0;
	GLfloat a;
	bool configured;
};

class TimeFunctionParabolic: public TimeFunction
{
public:
	bool configureFunction(GLfloat v0_, GLfloat v1_, GLfloat T_);
	GLfloat getDistance(GLfloat t0, GLfloat t);
	GLfloat getSpeedDelta(GLfloat t0, GLfloat t);
	TimeFunctionParabolic();
private:
	GLfloat v0;
	GLfloat a;
	bool configured;
};

class TimeFunctionSqrt: public TimeFunction
{
public:
	bool configureFunction(GLfloat v0_, GLfloat v1_, GLfloat T_);
	GLfloat getDistance(GLfloat t0, GLfloat t);
	GLfloat getSpeedDelta(GLfloat t0, GLfloat t);
	TimeFunctionSqrt();
private:
	GLfloat v0;
	GLfloat a;
	bool configured;
};

class TimeFunctionHyperbolic: public TimeFunction
{
public:
	bool configureFunction(GLfloat v0_, GLfloat v1_, GLfloat T_);
	GLfloat getDistance(GLfloat t0, GLfloat t);
	GLfloat getSpeedDelta(GLfloat t0, GLfloat t);
	TimeFunctionHyperbolic();
private:
	GLfloat v0;
	GLfloat a;
	GLfloat direction;
	bool configured;
};
