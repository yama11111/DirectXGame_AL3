#include "EnemyBullet.h"
#include "MyCalc.h"
#include "Player.h"

void EnemyBullet::Initialize(Model* model, 
	const Vector3& pos, const Vector3& velocity) {

	this->model = model;
	this->velocity = velocity;
	textureHandle = TextureManager::Load("orange.png");
	wt.Initialize();
	wt.translation_ = pos;
	wt.scale_ = {0.5f, 0.5f, 0.5f};
	AdjustAngle();
	Affine(wt);
	wt.TransferMatrix();
	SetRad(0.5f);
	SetAttribute(COLL_ATTRIBUTE_ENEMY);
	SetMask(~COLL_ATTRIBUTE_ENEMY);
}

void EnemyBullet::Update() {

	if (--deathT <= 0) {
		isDead = true;
	}
	Homing();
	wt.translation_ += velocity;
	Affine(wt);
	wt.TransferMatrix();
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) {

	model->Draw(wt, viewProjection, textureHandle);
}

void EnemyBullet::OnCollision() { isDead = true; }

Vector3 EnemyBullet::GetWorldPos() {
	Vector3 pos = wt.translation_;
	return pos;
}

void EnemyBullet::AdjustAngle() {
	wt.rotation_.y = std::atan2(velocity.x, velocity.z);
	float xz = SizeVector3({velocity.x, 0.0f, velocity.z});
	wt.rotation_.x = std::atan2(-velocity.y, xz);
}

void EnemyBullet::Homing() { 
	Vector3 fromVel = velocity;
	fromVel = NormalizeVector3(fromVel);

	Vector3 toPlayer = SubVector3(player->GetWorldPos(), wt.translation_);
	toPlayer = NormalizeVector3(toPlayer);

	velocity = MultVector3(Slerp(fromVel, toPlayer, 0.02f), 0.5f);
	AdjustAngle();
}
