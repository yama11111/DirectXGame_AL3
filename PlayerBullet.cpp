#include "PlayerBullet.h"
#include "MyCalc.h"

void PlayerBullet::Initialize(Model* model, 
	const Vector3& pos, const Vector3& velocity) {
	
	this->model = model;
	textureHandle = TextureManager::Load("blue.png");
	wt.Initialize();
	wt.translation_ = pos;
	Affine(wt);
	wt.TransferMatrix();
	this->velocity = velocity;
	SetRad(1.0f);
	SetAttribute(COLL_ATTRIBUTE_PLAYER);
	SetMask(~COLL_ATTRIBUTE_PLAYER);
}

void PlayerBullet::Update() {

	if (--deathT <= 0) {
		isDead = true;
	}
	wt.translation_ += velocity;
	Affine(wt);
	wt.TransferMatrix();
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {

	model->Draw(wt, viewProjection, textureHandle);
}

void PlayerBullet::OnCollision() { isDead = true; }

Vector3 PlayerBullet::GetWorldPos() {
	Vector3 pos = wt.translation_;
	return pos;
}
