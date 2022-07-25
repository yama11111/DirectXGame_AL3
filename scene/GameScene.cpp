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
			for (int k = 0; k < 9; k++) {
				wt[i][j][k].Initialize();
				wt[i][j][k].translation_.z += 5.0f * k;
				wt[i][j][k].translation_.x = -20.0f;
				wt[i][j][k].translation_.x += 5.0f * j;
				wt[i][j][k].translation_.y = -20.0f;
				wt[i][j][k].translation_.y += 5.0f * i;
				Affine(wt[i][j][k]);
			}
		}
	}

	viewProjection_.Initialize();

	viewProjection_.eye.z = -75.0f;
}

void GameScene::Update() { 
	

	viewProjection_.UpdateMatrix();
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
	/// 
	for (int k = 0; k < 9; k++) {
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				model_->Draw(wt[i][j][k], viewProjection_, textureHandle_);
			}
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