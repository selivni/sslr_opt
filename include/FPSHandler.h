#pragma once
#include <queue>

#include "Utility.h"

class FPSHandler
{
public:
	void updateFPS();
	FPSHandler();
	void clear();
	bool flip();
	int getLast();
	operator bool();
private:
	static const int timeLength_;

	std::queue<int> measurements_;
	int sum_;
	bool enabled_;
	int lastTime_;
};

