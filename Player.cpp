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
	SetRad(0.5f);
	SetAttribute(COLL_ATTRIBUTE_PLAYER);
	SetMask(~COLL_ATTRIBUTE_PLAYER);
}

void Player::Update(const Vector3& direction) { 
	bullets.remove_if([](std::unique_ptr<PlayerBullet>& bullet) { 
		return bullet->IsDead(); 
	});

	//Move();
	Move2(direction);
	Rotate();
	Affine(wt);
	//Attack();
	for (std::unique_ptr<PlayerBullet>& bullet : bullets) {
		bullet->Update();
	}
}

void Player::Draw(const ViewProjection& viewProjection) {
	
	model->Draw(wt, viewProjection, textureHandle);
	for (std::unique_ptr<PlayerBullet>& bullet : bullets) {
		bullet->Draw(viewProjection);
	}
}

void Player::DebugText(const Vector2& leftTop) {
	
	debugText->SetPos(leftTop.x, leftTop.y);
	debugText->Printf("Player" );
	debugText->SetPos(leftTop.x + 20, leftTop.y + 20);
	debugText->Printf("translation : (%f, %f, %f)", 
		wt.translation_.x, wt.translation_.y, wt.translation_.z );
	debugText->SetPos(leftTop.x + 20, leftTop.y + 40);
	debugText->Printf("rotation : (%f, %f, %f)", 
		wt.rotation_.x, wt.rotation_.y, wt.rotation_.z);
}

void Player::Move() { 
	
	Vector3 move = {0, 0, 0};
	const float power = 0.5f;

	if (input->PushKey(DIK_W)) move.y += power;
	if (input->PushKey(DIK_S)) move.y -= power;
	if (input->PushKey(DIK_D)) move.x += power;
	if (input->PushKey(DIK_A)) move.x -= power;

	wt.translation_ += move;

	//const float LIMIT_X = 35;
	//const float LIMIT_Y = 19;

	//wt.translation_.x = max(wt.translation_.x, -LIMIT_X);
	//wt.translation_.x = min(wt.translation_.x, LIMIT_X);
	//wt.translation_.y = max(wt.translation_.y, -LIMIT_Y);
	//wt.translation_.y = min(wt.translation_.y, LIMIT_Y);

	moveVal = move;
}

void Player::Rotate() {
	Vector3 rota = {0, 0, 0};
	const float power = 0.05f;
	if (input->PushKey(DIK_E)) rota.y += power;
	if (input->PushKey(DIK_Q)) rota.y -= power;

	wt.rotation_ += rota;
}

Vector3 Player::GetWorldPos() { 
	Vector3 pos = wt.translation_;
	return pos; 
}

void Player::Attack() {
	if (input->TriggerKey(DIK_SPACE)) {
		const float SPEED = 1.0f;
		Vector3 velocity(0, 0, SPEED);

		velocity = MultVec3Mat4(velocity, wt.matWorld_);

		std::unique_ptr<PlayerBullet> newBullet = 
			std::make_unique<PlayerBullet>();
		newBullet->Initialize(model, wt.translation_, velocity);
		bullets.push_back(std::move(newBullet));
	}

}

void Player::Move2(const Vector3& direction) {
	Vector3 nd = NormalizeVector3(direction);

	Vector3 y = {0, 1, 0};
	Vector3 dR = CrossVector3(y, nd);
	Vector3 ndR = NormalizeVector3(dR);

	const float POWER = 0.5f;
	Vector3 front = MultVector3(nd, POWER); // 前ベクトル
	Vector3 right = MultVector3(dR, POWER); // 右ベクトル

	if (input->PushKey(DIK_W)) wt.translation_ += front;
	if (input->PushKey(DIK_S)) wt.translation_ -= front;
	if (input->PushKey(DIK_D)) wt.translation_ += right;
	if (input->PushKey(DIK_A)) wt.translation_ -= right;

	AdjustAngle(direction); // 角度修正
}

void Player::OnCollision() {}

void Player::AdjustAngle(const Vector3& direction) {
	wt.rotation_.y = std::atan2(direction.x, direction.z);
	float xz = SizeVector3({direction.x, 0.0f, direction.z});
	wt.rotation_.x = std::atan2(-direction.y, xz);
}