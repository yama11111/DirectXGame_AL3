#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

using namespace DirectX;

GameScene::GameScene() {}

GameScene::~GameScene() { delete model_; }

void GameScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	textureHandle_ = TextureManager::Load("mario.jpg");
	model_ = Model::Create();
	worldTransform_.Initialize();
	viewProjection_.Initialize();
}

void GameScene::Update() { 

	float rotaY = worldTransform_.rotation_.y;
	if (input_->PushKey(DIK_RIGHT)) {
		rotaY += XM_PI / 36;
		if (rotaY >= 2 * XM_PI) {
			rotaY = 0;
		}
	} else if (input_->PushKey(DIK_LEFT)) {
		rotaY -= XM_PI / 36;
		if (rotaY <= -2 * XM_PI) {
			rotaY = 0;
		}
	}
	worldTransform_.rotation_.y = rotaY;

	const float POWER = 0.5f;
	XMFLOAT3 target = {sin(rotaY), 0.0f, cos(rotaY)};
	float norm = sqrt(target.x * target.x + target.y * target.y + target.z * target.z);

	if (input_->PushKey(DIK_UP)) {
		worldTransform_.translation_.x += target.x / norm * POWER;
		worldTransform_.translation_.y += target.y / norm * POWER;
		worldTransform_.translation_.z += target.z / norm * POWER;

		//viewProjection_.eye.x += target.x / norm * POWER;
		//viewProjection_.eye.y += target.y / norm * POWER;
		//viewProjection_.eye.z += target.z / norm * POWER;

	} else if (input_->PushKey(DIK_DOWN)) {
		worldTransform_.translation_.x -= target.x / norm * POWER;
		worldTransform_.translation_.y -= target.y / norm * POWER;
		worldTransform_.translation_.z -= target.z / norm * POWER;

		//viewProjection_.eye.x -= target.x / norm * POWER;
		//viewProjection_.eye.y -= target.y / norm * POWER;
		//viewProjection_.eye.z -= target.z / norm * POWER;
	}

	worldTransform_.UpdateMatrix(); 
	viewProjection_.UpdateMatrix();

	debugText_->SetPos(50, 50);
	debugText_->Printf(
		"worldTransform_.translation : (% f, % f, % f)", 
		worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);
	debugText_->SetPos(50, 70);
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
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);

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
