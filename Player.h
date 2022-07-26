#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include "DebugText.h"

class Player {
  public:
	WorldTransform wt;

	Model* model = nullptr;
	uint32_t textureHandle = 0;
	Input* input = nullptr;
	DebugText* debugText = nullptr;
  public:
	void Initialize(Model* model, uint32_t textureHandle);
	void Update();
	void Draw(ViewProjection& viewProjection);
	void DebugText(const Vector2& leftTop);
  private:
	void Move();
};
