#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "MyCalc.h"
#include <random>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete debugCamera_;
	delete model_;
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
	model_ = Model::Create();

	std::random_device seed_gen;
	std::mt19937_64 engine(seed_gen());
	
	std::uniform_real_distribution<float> dist(0, 2 * PI);
	std::uniform_real_distribution<float> dist2(-10, 10);

	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			wt[i][j].Initialize();
			wt[i][j].translation_ = {-20.0f, 20.0f, 0.0f};
			wt[i][j].translation_.x += 5.0f * j;
			wt[i][j].translation_.y -= 5.0f * i;
			Affine(wt[i][j]);
		}
	}

	vp.Initialize();
	vp.eye.z = -75.0;
}

void GameScene::Update() {

	if (input_->PushKey(DIK_UP)) {
		vp.fovAngleY -= 0.01f;
		if (vp.fovAngleY <= 0.01) vp.fovAngleY = 0.01f;
	}
	if (input_->PushKey(DIK_DOWN)) {
		vp.fovAngleY += 0.01f;
		if (vp.fovAngleY >= PI) vp.fovAngleY = PI;
	}

	if (input_->PushKey(DIK_W)) vp.target.y += 0.1f;
	if (input_->PushKey(DIK_S)) vp.target.y -= 0.1f;
	if (input_->PushKey(DIK_D)) vp.target.x += 0.1f;
	if (input_->PushKey(DIK_A)) vp.target.x -= 0.1f;

	vp.UpdateMatrix();
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
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			model_->Draw(wt[i][j], vp, textureHandle_);
		}
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

	debugText_->SetPos(50, 50);
	debugText_->Printf("eye : (%f, %f, %f)", 
		vp.eye.x, vp.eye.y, vp.eye.z);
	debugText_->SetPos(50, 70);
	debugText_->Printf("target : (%f, %f, %f)", 
		vp.target.x, vp.target.y, vp.target.z);
	debugText_->SetPos(50, 90);
	debugText_->Printf("up : (%f, %f, %f)", 
		vp.up.x, vp.up.y, vp.up.z);
	debugText_->SetPos(50, 110);
	debugText_->Printf("fovAngleY(Degree) : %f", 
		vp.fovAngleY * 180 / PI);
	debugText_->SetPos(50, 130);
	debugText_->Printf("nearZ : %f", 
		vp.nearZ);

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::Affine(WorldTransform& wt) {
	wt.matWorld_ = {
	  1, 0, 0, 0, 
	  0, 1, 0, 0, 
	  0, 0, 1, 0, 
	  0, 0, 0, 1,
	};
	wt.matWorld_ = Scaling(wt.matWorld_, wt.scale_);
	wt.matWorld_ = Rotation(wt.matWorld_, wt.rotation_);
	wt.matWorld_ = Moving(wt.matWorld_, wt.translation_);
	wt.TransferMatrix();
}