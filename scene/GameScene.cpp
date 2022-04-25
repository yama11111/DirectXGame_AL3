#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

using namespace DirectX;

GameScene::GameScene() {}

GameScene::~GameScene() { 
	delete player_;
	delete front_;
	for (size_t i = 0; i < 4; i++) {
		delete enemy_[i];
	}
}

void GameScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	textureHandle_ = TextureManager::Load("mario.jpg");
	textureHandle2_ = TextureManager::Load("front.png");

	player_ = Model::Create();
	worldTransformP_.Initialize();
	front_ = Model::Create();
	worldTransformF_.translation_ = {0, 0, 5};
	worldTransformF_.scale_ = {0.25f, 0.25f, 1.0f};
	worldTransformF_.Initialize();

	for (size_t i = 0; i < 4; i++) {
		enemy_[i] = Model::Create();
	}
	worldTransformE_[0].translation_ = {10, 0, 0};
	worldTransformE_[1].translation_ = {-10, 0, 0};
	worldTransformE_[2].translation_ = {0, 10, 0};
	worldTransformE_[3].translation_ = {0, -10, 0};

	for (size_t i = 0; i < 4; i++) {
		worldTransformE_[i].Initialize();
	}
	viewProjection_.Initialize();
}

void GameScene::Update() { 

	if (input_->PushKey(DIK_RIGHT)) {
		worldTransformP_.rotation_.y += XM_PI / 36;
		if (worldTransformP_.rotation_.y >= 2 * XM_PI) {
			worldTransformP_.rotation_.y = 0;
		}
	} else if (input_->PushKey(DIK_LEFT)) {
		worldTransformP_.rotation_.y -= XM_PI / 36;
		if (worldTransformP_.rotation_.y <= -2 * XM_PI) {
			worldTransformP_.rotation_.y = 0;
		}
	}
	worldTransformF_.rotation_ = worldTransformP_.rotation_;

	const float POWER = 0.5f;
	// x,z逆
	XMFLOAT3 target = {sin(worldTransformP_.rotation_.y), 0.0f, cos(worldTransformP_.rotation_.y)};

	if (input_->PushKey(DIK_UP)) {
		worldTransformP_.translation_.x += target.x * POWER;
		worldTransformP_.translation_.y += target.y * POWER;
		worldTransformP_.translation_.z += target.z * POWER;

	} else if (input_->PushKey(DIK_DOWN)) {
		worldTransformP_.translation_.x -= target.x * POWER;
		worldTransformP_.translation_.y -= target.y * POWER;
		worldTransformP_.translation_.z -= target.z * POWER;
	}

	worldTransformF_.translation_.x = worldTransformP_.translation_.x + target.x * 4;
	worldTransformF_.translation_.y = worldTransformP_.translation_.y + target.y * 4;
	worldTransformF_.translation_.z = worldTransformP_.translation_.z + target.z * 4;

	viewProjection_.eye.x = worldTransformP_.translation_.x - target.x * 10;
	viewProjection_.eye.y = worldTransformP_.translation_.y - target.y * 10 + 5;
	viewProjection_.eye.z = worldTransformP_.translation_.z - target.z * 10;

	viewProjection_.target.x = worldTransformP_.translation_.x + target.x * 10;
	viewProjection_.target.y = worldTransformP_.translation_.y + target.y * 10;
	viewProjection_.target.z = worldTransformP_.translation_.z + target.z * 10;

	worldTransformF_.rotation_ = worldTransformP_.rotation_;

	worldTransformP_.UpdateMatrix(); 
	worldTransformF_.UpdateMatrix(); 
	for (size_t i = 0; i < 4; i++) {
		worldTransformE_[i].UpdateMatrix(); 
	}
	viewProjection_.UpdateMatrix();

	debugText_->SetPos(50, 50);
	debugText_->Printf(
		"player : worldTransform_.translation_ : (% f, % f, % f)", 
		worldTransformP_.translation_.x, worldTransformP_.translation_.y, worldTransformP_.translation_.z);
	debugText_->SetPos(50, 70);
	debugText_->Printf(
		"player : worldTransform_.rotation_ : (% f, % f, % f)", 
		worldTransformP_.rotation_.x, worldTransformP_.rotation_.y, worldTransformP_.rotation_.z);
	debugText_->SetPos(50, 90);
	debugText_->Printf(
		"viewProjection_.eye : (% f, % f, % f)", 
		viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	player_->Draw(worldTransformP_, viewProjection_, textureHandle_);
	front_->Draw(worldTransformF_, viewProjection_, textureHandle2_);
	for (size_t i = 0; i < 4; i++) {
		enemy_[i]->Draw(worldTransformE_[i], viewProjection_, textureHandle_);
	}
	

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
