#pragma once
#include "WorldTransform.h"
#include "Model.h"
class EnemyBullet {
  private:
	WorldTransform wt;
	Model* model = nullptr;
	uint32_t textureHandle = 0;
	Vector3 velocity;
	static const int32_t LIFE_T = 60 * 5;
	int32_t deathT = LIFE_T;
	bool isDead = false;
  public:
	void Initialize(Model* model, const Vector3& pos, const Vector3& velocity);
	void Update();
	void Draw(const ViewProjection& viewProjection);
	bool IsDead() const { return isDead; }
};
