#include "Skydome.h"
#include "MyCalc.h"
#include <cassert>

void Skydome::Initialize(Model* model) {
	assert(model);
	this->model = model;
	wt.Initialize();
	wt.scale_ = {100.0f, 100.0f, 100.0f};
	Affine(wt);
}

void Skydome::Update() { Affine(wt); }

void Skydome::Draw(const ViewProjection& viewProjection) { 
	model->Draw(wt, viewProjection); 
}
