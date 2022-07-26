#include "Enemy.h"
#include "MyCalc.h"
#include <cassert>

void Enemy::Initialize(Model* model, uint32_t textureHandle, const Vector3& pos) {
	assert(model);
	this->model = model;
	this->textureHandle = textureHandle;
	debugText = DebugText::GetInstance();
	wt.translation_ = pos;
	wt.Initialize();
	Fire();
}

void Enemy::Update() {
	bullets.remove_if([](std::unique_ptr<EnemyBullet>& bullet) { 
		return bullet->IsDead(); 
	});

	(this->*spUpdateTable[static_cast<size_t>(phase)])();
	Affine(wt);

	for (std::unique_ptr<EnemyBullet>& bullet : bullets) {
		bullet->Update();
	}

}

void Enemy::Draw(const ViewProjection& viewProjection) {

	model->Draw(wt, viewProjection, textureHandle);
	for (std::unique_ptr<EnemyBullet>& bullet : bullets) {
		bullet->Draw(viewProjection);
	}
}

void Enemy::DebugText(const Vector2& leftTop) {
	debugText->SetPos(leftTop.x, leftTop.y);
	debugText->Printf("Enemy");
	debugText->SetPos(leftTop.x + 20, leftTop.y + 20);
	debugText->Printf(
	  "translation : (%f, %f, %f)", wt.translation_.x, wt.translation_.y, wt.translation_.z);
	debugText->SetPos(leftTop.x + 20, leftTop.y + 40);
	debugText->Printf("rotation : (%f, %f, %f)", wt.rotation_.x, wt.rotation_.y, wt.rotation_.z);
}

void Enemy::Approach() {
	const float SPEED = -0.1f;
	Vector3 velocity = {0, 0, SPEED};
	wt.translation_ += velocity;
	if (wt.translation_.z < -2.0f) {
		phase = Phase::Leave;
	}
}

void Enemy::Leave() {
	const float SPEED = 1.0f;
	Vector3 velocity = {0, 0, SPEED};
	wt.translation_ += velocity;
}

void Enemy::Fire() {
	const float SPEED = -1.0f;
	Vector3 velocity(0, 0, SPEED);

	velocity = MultVec3Mat4(velocity, wt.matWorld_);

	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model, wt.translation_, velocity);
	bullets.push_back(std::move(newBullet));
}

void (Enemy::*Enemy::spUpdateTable[])() = {
  &Enemy::Approach,
  &Enemy::Leave,
};