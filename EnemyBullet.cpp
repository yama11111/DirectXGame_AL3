#include "EnemyBullet.h"
#include "MyCalc.h"

void EnemyBullet::Initialize(Model* model, 
	const Vector3& pos, const Vector3& velocity) {

	this->model = model;
	textureHandle = TextureManager::Load("orange.png");
	wt.Initialize();
	wt.translation_ = pos;
	Affine(wt);
	this->velocity = velocity;
}

void EnemyBullet::Update() {

	if (--deathT <= 0) {
		isDead = true;
	}
	wt.translation_ += velocity;
	Affine(wt);
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) {

	model->Draw(wt, viewProjection, textureHandle);
}