#include "Enemy.h"
#include "Player.h"
#include "MyCalc.h"
#include <cassert>

void Enemy::Initialize(Model* model, uint32_t textureHandle, const Vector3& pos) {
	assert(model);
	this->model = model;
	this->textureHandle = textureHandle;
	debugText = DebugText::GetInstance();
	wt.translation_ = pos;
	wt.scale_ = {3.0f, 3.0f, 3.0f};
	wt.Initialize();
	//FireAndReset();
	SetRad(1.0f);
	SetAttribute(COLL_ATTRIBUTE_ENEMY);
	SetMask(~COLL_ATTRIBUTE_ENEMY);
}

void Enemy::Update() {
	//timedCalls.remove_if([](std::unique_ptr<TimedCall>& timedCall) {
	//	return timedCall->IsFinished(); 
	//});

	//for (std::unique_ptr<TimedCall>& timedCall : timedCalls) {
	//	timedCall->Update();
	//}

	//bullets.remove_if([](std::unique_ptr<EnemyBullet>& bullet) { 
	//	return bullet->IsDead(); 
	//});

	//(this->*spUpdateTable[static_cast<size_t>(phase)])();
	Affine(wt);

	//for (std::unique_ptr<EnemyBullet>& bullet : bullets) {
	//	bullet->Update();
	//}


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

Vector3 Enemy::GetWorldPos() {
	Vector3 pos = wt.translation_;
	return pos;
}

void Enemy::Approach() {
	const float SPEED = -0.05f;
	Vector3 velocity = {0, 0, SPEED};
	wt.translation_ += velocity;
	if (wt.translation_.z < 1.0f) {
		phase = Phase::Leave;
		timedCalls.clear();
	}
}

void Enemy::OnCollision() {}

void Enemy::Leave() {
	const float SPEED = 1.0f;
	Vector3 velocity = {0, 0, SPEED};
	wt.translation_ += velocity;
}

void Enemy::Fire() {
	assert(player);

	const float SPEED = -0.5f;

	Vector3 velocity = GetWorldPos();
	velocity -= player->GetWorldPos();
	velocity = NormalizeVector3(velocity);
	velocity *= SPEED;

	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model, wt.translation_, velocity);
	newBullet->SetPlayer(player);
	bullets.push_back(std::move(newBullet));
}

void Enemy::FireAndReset() {
	Fire();
	timedCalls.push_back(std::make_unique<TimedCall>(
		std::bind(&Enemy::FireAndReset, this), 30));

}

void (Enemy::*Enemy::spUpdateTable[])() = {
  &Enemy::Approach,
  &Enemy::Leave,
};