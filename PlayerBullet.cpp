#include "PlayerBullet.h"
#include "MyCalc.h"

void PlayerBullet::Initialize(Model* model, const Vector3& pos, 
	const Vector3& velocity) {
	
	this->model = model;
	textureHandle = TextureManager::Load("black.png");
	wt.Initialize();
	wt.translation_ = pos;
	Affine(wt);
	this->velocity = velocity;
}

void PlayerBullet::Update() {

	if (--deathT <= 0) {
		isDead = true;
	}
	wt.translation_ += velocity;
	Affine(wt);
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {

	model->Draw(wt, viewProjection, textureHandle);
}
