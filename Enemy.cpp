#include "Enemy.h"
#include "MyCalc.h"
#include <cassert>

void Enemy::Initialize(Model* model, uint32_t textureHandle, 
	const Vector3& velocity) {
	assert(model);
	this->model = model;
	this->textureHandle = textureHandle;
	debugText = DebugText::GetInstance();
	this->velocity = velocity;
	wt.Initialize();
}

void Enemy::Update() { 
	
	Move();
	Affine(wt);

}

void Enemy::Draw(const ViewProjection& viewProjection) {
	
	model->Draw(wt, viewProjection, textureHandle);

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

void Enemy::Move() { 
	wt.translation_ += velocity;
}
