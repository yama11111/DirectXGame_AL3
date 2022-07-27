#include "EnemyBullet.h"
#include "MyCalc.h"

void EnemyBullet::Initialize(Model* model, 
	const Vector3& pos, const Vector3& velocity) {

	this->model = model;
	this->velocity = velocity;
	textureHandle = TextureManager::Load("orange.png");
	wt.Initialize();
	wt.translation_ = pos;
	wt.scale_ = {0.5f, 0.5f, 3.0f};
	AdjustAngle();
	Affine(wt);
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

void EnemyBullet::AdjustAngle() {
	wt.rotation_.y = std::atan2(velocity.x, velocity.z);
	float xz = SizeVector3({velocity.x, 0.0f, velocity.z});
	wt.rotation_.x = std::atan2(-velocity.y, xz);
}
