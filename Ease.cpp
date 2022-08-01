#include "Ease.h"
#include <math.h>
float lerp(const float a, const float b, const float t) { 
	return a + t * (b - a); 
}

float EaseIn(const float start, const float end, 
	const float time, const float power) {
	return lerp(start, end, pow(time, power));
}

float EaseOut(const float start, const float end, 
	const float time, const float power) {
	return lerp(start, end, 1 - pow(1 - time, power));
}