#pragma once
#include "Vector3.h"
#include "Random.h"

class Shake {
  private:
	Vector3 shake;			// �����l
	bool isShake = false;	// �h��Ă��邩
	float swingValue = 0;	// �h���� (����)
	float dekey = 0;		// ������
	Random* rand = nullptr;
  public:
	void Initialize();
	void Update();
	void Shaking(const float swing, const float dekey);
	Vector3 GetShakeValue() { return shake; }
	bool IsShake() { return isShake; }
};
