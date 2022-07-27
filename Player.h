#pragma once
#include "PlayerBullet.h"
#include "Input.h"
#include "DebugText.h"
#include <memory>
#include <list>

class Player {
  private:
	WorldTransform wt;
	std::list<std::unique_ptr<PlayerBullet>> bullets;

	Model* model = nullptr;
	uint32_t textureHandle = 0;
	Input* input = nullptr;
	DebugText* debugText = nullptr;
  public:
	void Initialize(Model* model, uint32_t textureHandle);
	void Update();
	void Draw(const ViewProjection& viewProjection);
	void DebugText(const Vector2& leftTop);
	Vector3 GetWorldPos();
  private:
	void Move();
	void Rotate();
	void Attack();
};
