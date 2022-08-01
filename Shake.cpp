#include "Shake.h"

void Shake::Initialize() {
	isShake = false;
	swingValue = 0;
	dekey = 0;
	rand = Random::GetInstance();
}

void Shake::Shaking(const float swing, const float dekey) {
	shake.x = 0;
	shake.y = 0;
	shake.z = 0;
	isShake = true;
	this->dekey = dekey;
	swingValue = swing;
}

void Shake::Update() {
	// �͈͂����߂�
	if (swingValue > 0) {
		swingValue -= dekey;
	}

	// �͈͓��̗�������
	if (isShake) {
		shake.x = rand->GetRandF(-swingValue, swingValue);
		shake.y = rand->GetRandF(-swingValue, swingValue);
		shake.z = rand->GetRandF(-swingValue, swingValue);
	}

	if (swingValue <= 0) {
		shake = {0.0f, 0.0f, 0.0f};
		swingValue = 0;
		isShake = false;
	}
}
