#include "FPSHandler.h"
const int FPSHandler::timeLength_ = 1000;



FPSHandler::FPSHandler()
{
	while (measurements_.size() != 0)
		measurements_.pop();
	sum_ = 0;
	enabled_ = false;
	lastTime_ = 0;
}

void FPSHandler::clear()
{
	while (measurements_.size() != 0)
		measurements_.pop();
	sum_ = 0;
	lastTime_ = 0;
}

void FPSHandler::updateFPS()
{
	if (enabled_)
	{
		int currentTime = glutGet(GLUT_ELAPSED_TIME);
		measurements_.push(currentTime - lastTime_);
		sum_ += currentTime - lastTime_;
		while (sum_ >= timeLength_)
		{
			sum_ -= measurements_.front();
			measurements_.pop();
		}
		int result;
		if (measurements_.empty() || sum_ == 0)
			result = 1001;
		else
			result = sum_ / measurements_.size();
		std::cout << '\n'
			<< "FPS: " << 1000 / (result) << "      ";
		lastTime_ = currentTime;
	}
}

bool FPSHandler::flip()
{
	return enabled_ = !enabled_;
}

FPSHandler::operator bool()
{
	return enabled_;
}
