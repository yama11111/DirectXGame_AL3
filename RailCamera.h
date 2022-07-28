#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Vector2.h"
#include "DebugText.h"

class RailCamera {
  private:
	WorldTransform wt;
	ViewProjection vp;
	DebugText* debugText = nullptr;
  public:
	void Initialize(const Vector3& pos);
	void Update();
	void DebugText(const Vector2& leftTop);
	ViewProjection GetViewProjection() { return vp; }
	Matrix4* GetMatWorld() { return &wt.matWorld_; }
  private:
	void Move();
	void Rotate();
	void vpUpdate();
};
