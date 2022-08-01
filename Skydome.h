#pragma once
#include "Model.h"

class Skydome {
  private:
	WorldTransform wt;
	Model* model = nullptr;
  public:
	void Initialize(Model* model);
	void Update();
	void Draw(const ViewProjection& viewProjection);
};
