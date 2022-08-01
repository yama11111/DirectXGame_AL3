#pragma once
#include "Vector3.h"
#include "Random.h"

class Shake {
  private:
	Vector3 shake;			// 実数値
	bool isShake = false;	// 揺れているか
	float swingValue = 0;	// 揺れる量 (乱数)
	float dekey = 0;		// 減衰量
	Random* rand = nullptr;
  public:
	void Initialize();
	void Update();
	void Shaking(const float swing, const float dekey);
	Vector3 GetShakeValue() { return shake; }
	bool IsShake() { return isShake; }
};
