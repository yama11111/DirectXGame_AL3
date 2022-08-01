#pragma once
#include "EnemyBullet.h"
#include "TimedCall.h"
#include "Collider.h"
#include "DebugText.h"
#include <memory>
#include <list>

class Player;

class Enemy : public Collider {
  public:
	enum class Phase {
		Approach, // 接近する
		Leave,    // 離脱する
	};
  private:
	WorldTransform wt;
	std::list<std::unique_ptr<EnemyBullet>> bullets;
	Player* player = nullptr;
	Phase phase = Phase::Approach;
	std::list<std::unique_ptr<TimedCall>> timedCalls;
	Model* model = nullptr;
	uint32_t textureHandle = 0;
	DebugText* debugText = nullptr;
  public:
	void Initialize(Model* model, uint32_t textureHandle, const Vector3& pos);
	void Update();
	void Draw(const ViewProjection& viewProjection);
	void DebugText(const Vector2& leftTop);
	void SetPlayer(Player* player) { this->player = player; }
	Vector3 GetWorldPos() override;
	void OnCollision() override;
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets; }
	Vector3* GetWorldPosPointer() { return &wt.translation_; }
  private:
	void Approach();
	void Leave();
	static void (Enemy::*spUpdateTable[])();
	void Fire();
	void FireAndReset();
};
