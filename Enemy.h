#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "DebugText.h"

class Enemy {
  public:
	enum class Phase {
		Approach, // ê⁄ãﬂÇ∑ÇÈ
		Leave,    // ó£íEÇ∑ÇÈ
	};
  private:
	WorldTransform wt;
	Model* model = nullptr;
	uint32_t textureHandle = 0;
	Phase phase = Phase::Approach;
	DebugText* debugText = nullptr;
  public:
	void Initialize(Model* model, uint32_t textureHandle, const Vector3& pos);
	void Update();
	void Draw(const ViewProjection& viewProjection);
	void DebugText(const Vector2& leftTop);
  private:
	void ApproachUpdate();
	void LeaveUpdate();
};
