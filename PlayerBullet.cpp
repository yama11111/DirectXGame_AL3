#include "PlayerBullet.h"
#include "MyCalc.h"

void PlayerBullet::Initialize(Model* model, const Vector3& pos) {
	
	this->model = model;
	textureHandle = TextureManager::Load("black.png");
	wt.Initialize();
	wt.translation_ = pos;
	Affine(wt);
}

void PlayerBullet::Update() {

	Affine(wt);
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {

	model->Draw(wt, viewProjection, textureHandle);
}
