#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "MyCalc.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete debugCamera_;
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

	debugCamera_ = new DebugCamera(1280, 720);
	AxisIndicator::GetInstance()->SetVisible(true); // 表示を有効
	// 参照するビュープロジェクション
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection()); 
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());

	textureHandle_ = TextureManager::Load("mario.jpg");
	textureHandle2_ = TextureManager::Load("front.png");

	player_ = Model::Create();
	front_ = Model::Create();
	worldTransformF_.translation_ = {0, 0, 5};
	worldTransformF_.scale_ = {0.25f, 0.25f, 1.0f};
	for (size_t i = 0; i < 4; i++) {
		enemy_[i] = Model::Create();
	}
	worldTransformE_[0].translation_ = {10, 0, 0};
	worldTransformE_[1].translation_ = {-10, 0, 0};
	worldTransformE_[2].translation_ = {0, 10, 0};
	worldTransformE_[3].translation_ = {0, -10, 0};

	Matrix4 matScale;
	matScale.m[0][0] = 5.0;
	matScale.m[1][1] = 1.0;
	matScale.m[2][2] = 1.0;
	matScale.m[3][3] = 1.0;

	worldTransformF_.matWorld_ = {
		1, 0, 0, 0, 
		0, 1, 0, 0, 
		0, 0, 1, 0, 
		0, 0, 0, 1
	};
	worldTransformF_.matWorld_ = MultMatrix4(worldTransformF_.matWorld_, matScale);


	worldTransformP_.Initialize();
	worldTransformF_.Initialize();
	for (size_t i = 0; i < 4; i++) {
		worldTransformE_[i].Initialize();
	}
	viewProjection_.Initialize();
}

void GameScene::Update() { 
	
	debugCamera_->Update(); 
	
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
	
	player_->Draw(worldTransformP_, debugCamera_->GetViewProjection(), textureHandle_);
	front_->Draw(worldTransformF_, debugCamera_->GetViewProjection(), textureHandle2_);
	for (size_t i = 0; i < 4; i++) {
		enemy_[i]->Draw(worldTransformE_[i], debugCamera_->GetViewProjection(), textureHandle_);
	}

	//PrimitiveDrawer::GetInstance()->DrawLine3d({0.0f, 0.0f, 0.0f}, {100.0f, 100.0f, 100.0f}, {2.0f, 0.0f, 0.0f, 0.0f});

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
