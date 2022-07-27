#pragma once
#include "Vector3.h"
class Collider {
  private:
	float radius = 0.0f;
  public:
	float GetRad() { return radius; }
	void SetRad(const float radius) { this->radius = radius; }
	virtual Vector3 GetWorldPos() = 0;
	virtual void OnCollision() {}
};
