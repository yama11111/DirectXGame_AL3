#include "Player.h"
#include <cassert>
#include "MyCalc.h"

void Player::Initialize(Model* model, uint32_t textureHandle) { 
	
	assert(model);
	this->model = model;
	this->textureHandle = textureHandle;
	input = Input::GetInstance();
	debugText = DebugText::GetInstance();
	wt.Initialize();

}

void Player::Update() { 
	Move();
	Affine(wt);
}

void Player::Draw(ViewProjection& viewProjection) {
	
	model->Draw(wt, viewProjection, textureHandle);

}

void Player::DebugText(const Vector2& leftTop) {
	
	debugText->SetPos(leftTop.x, leftTop.y);
	debugText->Printf("Player" );
	debugText->SetPos(leftTop.x + 20, leftTop.y + 20);
	debugText->Printf("translation : (%f, %f, %f)", wt.translation_.x, wt.translation_.y, wt.translation_.z );

}

void Player::Move() { 
	
	Vector3 move = {0, 0, 0};
	const float power = 0.5f;
	if (input->PushKey(DIK_W)) move.y += power;
	if (input->PushKey(DIK_S)) move.y -= power;
	if (input->PushKey(DIK_D)) move.x += power;
	if (input->PushKey(DIK_A)) move.x -= power;

	wt.translation_ += move;

	const float LIMIT_X = 35;
	const float LIMIT_Y = 19;

	wt.translation_.x = max(wt.translation_.x, -LIMIT_X);
	wt.translation_.x = min(wt.translation_.x, LIMIT_X);
	wt.translation_.y = max(wt.translation_.y, -LIMIT_Y);
	wt.translation_.y = min(wt.translation_.y, LIMIT_Y);

	
}
