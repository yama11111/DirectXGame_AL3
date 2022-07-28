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
	wt.translation_ = {0, 0, 50};
	Affine(wt);
	wt.TransferMatrix();
	SetRad(0.5f);
	SetAttribute(COLL_ATTRIBUTE_PLAYER);
	SetMask(~COLL_ATTRIBUTE_PLAYER);
}

void Player::Update() { 
	bullets.remove_if([](std::unique_ptr<PlayerBullet>& bullet) { 
		return bullet->IsDead(); 
	});

	Move();
	Rotate();
	Affine(wt);
	wt.matWorld_ *= *camera;
	wt.TransferMatrix();
	Attack();
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

	const float LIMIT_X = 35;
	const float LIMIT_Y = 19;

	wt.translation_.x = max(wt.translation_.x, -LIMIT_X);
	wt.translation_.x = min(wt.translation_.x, LIMIT_X);
	wt.translation_.y = max(wt.translation_.y, -LIMIT_Y);
	wt.translation_.y = min(wt.translation_.y, LIMIT_Y);

	
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

		Vector3 pos = MultVec3Mat4(wt.translation_, wt.matWorld_);

		std::unique_ptr<PlayerBullet> newBullet = 
			std::make_unique<PlayerBullet>();
		newBullet->Initialize(model, pos, velocity);
		bullets.push_back(std::move(newBullet));
	}

}

void Player::OnCollision() {}
