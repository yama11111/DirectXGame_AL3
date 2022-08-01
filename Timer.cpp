#include "Timer.h"

Timer::Timer() {
	timer = 0;
	maxTime = 0;
	second = 0;
	isActive = false;
	end = false;
	endMoment = false;
	isDekey = false;
}

Timer::Timer(const float maxTime, const float second,
	const bool isDekey) {
	timer = 0;
	this->maxTime = maxTime;
	this->second = second;
	isActive = false;
	end = false;
	endMoment = false;
	this->isDekey = isDekey;
}

void Timer::Reset() {
	timer = 0;
	isActive = false;
	end = false;
	endMoment = false;
}

void Timer::Init() {
	Reset();
	maxTime = 0;
	second = 0;
}

void Timer::Set(const float timer, const float maxTime, 
	const float second, const bool isDekey) {
	this->timer = timer;
	this->maxTime = maxTime;
	this->second = second;
	isActive = false;
	this->isDekey = isDekey;
}

void Timer::Start() {
	isActive = true;
	end = false;
}

void Timer::Start(const float timer, const float maxTime, 
	const float second, const bool isDekey) {
	Set(timer, maxTime, second, isDekey);
	Start();
}

void Timer::Stop() { isActive = false; }

void Timer::End() {
	timer = 0;
	isActive = false;
	end = true;
}

void Timer::Update(const bool loop) {
	endMoment = false;
	if (isActive) {
		if (isDekey) {
			timer -= second;
			if (timer <= 0) {
				isActive = loop;
				endMoment = true;
				if (!loop) end = true;
				else timer = maxTime;
			}
		}
		else {
			timer += second;
			if (timer >= maxTime) {
				isActive = loop;
				endMoment = true;
				if (!loop) end = true;
				else timer = 0;
			}
		}
	}
}