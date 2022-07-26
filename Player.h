#pragma once
#include "PlayerBullet.h"
#include "Input.h"
#include "DebugText.h"

class Player {
  private:
	WorldTransform wt;
	PlayerBullet* bullet = nullptr;

	Model* model = nullptr;
	uint32_t textureHandle = 0;
	Input* input = nullptr;
	DebugText* debugText = nullptr;
  public:
	void Initialize(Model* model, uint32_t textureHandle);
	void Update();
	void Draw(const ViewProjection& viewProjection);
	void DebugText(const Vector2& leftTop);
  private:
	void Move();
	void Rotate();
	void Attack();
};
