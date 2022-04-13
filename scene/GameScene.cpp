﻿#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <random>

using namespace DirectX;

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	// 乱数
	std::random_device seed_gen;
	std::mt19937_64 engine(seed_gen());
	std::uniform_real_distribution<float> rotDist(0.0f, XM_2PI); // 角度
	std::uniform_real_distribution<float> posDist(-10.0f, 10.0f); // 座標

	// リソース読み込み
	textureHandle_ = TextureManager::Load("mario.jpg");
	// モデル
	model_ = Model::Create();

	//for (size_t i = 0; i < _countof(worldTransform_); i++) {
	//	worldTransform_[i].scale_ = {1.0f, 1.0f, 1.0f};
	//	worldTransform_[i].rotation_ = {rotDist(engine), rotDist(engine), rotDist(engine)};
	//	worldTransform_[i].translation_ = {posDist(engine), posDist(engine), posDist(engine)};
	//	worldTransform_[i].Initialize();
	//}

	worldTransform_[0].Initialize();

	worldTransform_[1].translation_ = {0, 4.5f, 0};
	worldTransform_[1].parent_ = &worldTransform_[0];
	worldTransform_[1].Initialize();

	// 視野角設定
	//viewProjection_.fovAngleY = XMConvertToRadians(10.0f);
	// アスペクト比設定
	//viewProjection_.aspectRatio = 1.0f;
	// ニアクリップ、ファークリップ設定
	viewProjection_.nearZ = 52.0f;
	viewProjection_.farZ = 53.0f;

	viewProjection_.Initialize();
}

void GameScene::Update() {
	//// 視点移動
	//XMFLOAT3 move = {0, 0, 0}; // 移動先
	//const float kEyeSpeed = 0.2f; // 速度

	//if (input_->PushKey(DIK_W)) {
	//	move = {0, 0, kEyeSpeed};
	//} else if (input_->PushKey(DIK_S)) {
	//	move = {0, 0, -kEyeSpeed};
	//}

	//viewProjection_.eye.x += move.x;
	//viewProjection_.eye.y += move.y;
	//viewProjection_.eye.z += move.z;

	//// 注視点移動
	//XMFLOAT3 move2 = {0, 0, 0}; // 移動先
	//const float kTargetSpeed = 0.2f; // 速度

	//if (input_->PushKey(DIK_LEFT)) {
	//	move2 = {-kTargetSpeed, 0, 0};
	//} else if (input_->PushKey(DIK_RIGHT)) {
	//	move2 = {kTargetSpeed, 0, 0};
	//}

	//viewProjection_.target.x += move2.x;
	//viewProjection_.target.y += move2.y;
	//viewProjection_.target.z += move2.z;

	//// 上方向ベクトル移動
	//const float kUpRotSpeed = 0.05f; // 速度

	//if (input_->PushKey(DIK_SPACE)) {
	//	viewAngle += kUpRotSpeed;
	//	viewAngle = fmodf(viewAngle, XM_2PI);
	//}

	//viewProjection_.up = {cosf(viewAngle), sinf(viewAngle), 0.0f};

	//// 視野角変更
	//if (input_->PushKey(DIK_W)) {
	//	viewProjection_.fovAngleY += 0.01f;
	//	viewProjection_.fovAngleY = min(viewProjection_.fovAngleY, XM_PI);
	//} else if (input_->PushKey(DIK_S)) {
	//	viewProjection_.fovAngleY -= 0.01f;
	//	viewProjection_.fovAngleY = max(viewProjection_.fovAngleY, 0.01f);
	//}
	//// ニアクリップ変更
	//if (input_->PushKey(DIK_UP)) {
	//	viewProjection_.nearZ += 0.1f;
	//} else if (input_->PushKey(DIK_DOWN)) {
	//	viewProjection_.nearZ -= 0.1f;
	//}

	//viewProjection_.UpdateMatrix();

	// キャラ移動
	XMFLOAT3 move = {0, 0, 0}; // 移動先
	const float kCharacterSpeed = 0.2f; // 速度

	if (input_->PushKey(DIK_LEFT)) {
		move = {-kCharacterSpeed, 0, 0};
	} else if (input_->PushKey(DIK_RIGHT)) {
		move = {kCharacterSpeed, 0, 0};
	}

	// デバッグ表示
	debugText_->SetPos(50, 50);
	debugText_->Printf(
	  "eye : (% f, % f, % f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);

	debugText_->SetPos(50, 70);
	debugText_->Printf(
	  "target : (% f, % f, % f)", viewProjection_.target.x, viewProjection_.target.y, viewProjection_.target.z);

	debugText_->SetPos(50, 90);
	debugText_->Printf(
	  "up : (% f, % f, % f)", viewProjection_.up.x, viewProjection_.up.y, viewProjection_.up.z);

	debugText_->SetPos(50, 110);
	debugText_->Printf(
	  "fovAngleY(Degree) : % f", XMConvertToDegrees(viewProjection_.fovAngleY));

	debugText_->SetPos(50, 130);
	debugText_->Printf(
	  "nearZ : % f", viewProjection_.nearZ);
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
	for (size_t i = 0; i < _countof(worldTransform_); i++) {
		model_->Draw(worldTransform_[i], viewProjection_, textureHandle_);
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
