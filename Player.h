#pragma once
#include "PlayerBullet.h"
#include "Collider.h"
#include "Input.h"
#include "DebugText.h"
#include <memory>
#include <list>

class Player : public Collider{
  private:
	WorldTransform wt;
	std::list<std::unique_ptr<PlayerBullet>> bullets;
	Vector3 moveVal;

	Model* model = nullptr;
	uint32_t textureHandle = 0;
	Input* input = nullptr;
	DebugText* debugText = nullptr;
  public:
	void Initialize(Model* model, uint32_t textureHandle);
	void Update(const Vector3& direction);
	void Draw(const ViewProjection& viewProjection);
	void DebugText(const Vector2& leftTop);
	Vector3 GetWorldPos() override;
	void OnCollision() override;
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets; }

	Vector3 GetMove() { return moveVal; }
	Vector3* GetWorldPosPointer() { return &wt.translation_; }
  private:
	void Move();
	void Move2(const Vector3& direction);
	void Rotate();
	void Attack();
	void AdjustAngle(const Vector3& direction);
};
