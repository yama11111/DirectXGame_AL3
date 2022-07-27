#pragma once
#include "Model.h"

class Skydome {
  private:
	WorldTransform wt;
	Model* model;
  public:
	void Initialize(Model* model);
	void Update();
	void Draw(const ViewProjection& viewProjection);
};
