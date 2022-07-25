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

	wt.Initialize();

	vp.Initialize();
	for (int i = 0; i < 3; i++) {
		vps[i].Initialize();
	}

	vps[0].eye = {-9.0f, -3.0f, -1.5f};
	vps[1].eye = {-9.0f, -3.0f, -9.0f};
	vps[2].eye = {6.0f, 0.0f, -4.0f};

	vp = vps[0];
}

void GameScene::Update() { 
	if (input_->TriggerKey(DIK_SPACE)) {
		v++;
		if (v >= 3) v = 0;
		vp = vps[v];
	}

	vp.UpdateMatrix();
	for (int i = 0; i < 3; i++) {
		vps[i].UpdateMatrix();
	}
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
	
	model_->Draw(wt, vp, textureHandle_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	for (int i = 0; i < 3; i++) {
		debugText_->SetPos(50, 50 + 100 * i);
		debugText_->Printf("Camera%i", i);
		debugText_->SetPos(50, 70 + 100 * i);
		debugText_->Printf("eye : (%f, %f, %f)", 
			vps[i].eye.x, vps[i].eye.y, vps[i].eye.z);
		debugText_->SetPos(50, 90 + 100 * i);
		debugText_->Printf("target : (%f, %f, %f)", 
			vps[i].target.x, vps[i].target.y, vps[i].target.z);
		debugText_->SetPos(50, 110 + 100 * i);
		debugText_->Printf("up : (%f, %f, %f)", 
			vps[i].up.x, vps[i].up.y, vps[i].up.z);
	}

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