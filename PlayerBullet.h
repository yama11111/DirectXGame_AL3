#pragma once
#include "WorldTransform.h"
#include "Model.h"

class PlayerBullet {
  private:
	WorldTransform wt;
	Model* model = nullptr;
	uint32_t textureHandle = 0;
  public:
	void Initialize(Model* model, const Vector3& pos);
	void Update();
	void Draw(const ViewProjection& viewProjection);
};
