#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "DebugText.h"

class Enemy {
  private:
	WorldTransform wt;
	Model* model = nullptr;
	uint32_t textureHandle = 0;
	Vector3 velocity;
	DebugText* debugText = nullptr;
  public:
	void Initialize(Model* model, uint32_t textureHandle, 
		const Vector3& velocity);
	void Update();
	void Draw(const ViewProjection& viewProjection);
	void DebugText(const Vector2& leftTop);
  private:
	void Move();
};
