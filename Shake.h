#pragma once
#include "Vector3.h"
#include "Random.h"

class Shake {
  private:
	Vector3 shake;			// À”’l
	bool isShake = false;	// —h‚ê‚Ä‚¢‚é‚©
	float swingValue = 0;	// —h‚ê‚é—Ê (—”)
	float dekey = 0;		// Œ¸Š—Ê
	Random* rand = nullptr;
  public:
	void Initialize();
	void Update();
	void Shaking(const float swing, const float dekey);
	Vector3 GetShakeValue() { return shake; }
	bool IsShake() { return isShake; }
};
