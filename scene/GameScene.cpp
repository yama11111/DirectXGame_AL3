#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "MyCalc.h"
#include <random>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	debugCamera_ = new DebugCamera(WinApp::kWindowHeight, WinApp::kWindowWidth);
	AxisIndicator::GetInstance()->SetVisible(true); // 表示を有効
	// 参照するビュープロジェクション
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection()); 
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());

	textureHandle_ = TextureManager::Load("player.png");
	textureHandle2_ = TextureManager::Load("enemy.png");
	model_ = Model::Create();

	std::random_device seed_gen;
	std::mt19937_64 engine(seed_gen());
	
	std::uniform_real_distribution<float> dist(0, 2 * PI);
	std::uniform_real_distribution<float> dist2(-10, 10);

	Player* newPlayer = new Player();
	newPlayer->Initialize(model_, textureHandle_);
	player.reset(newPlayer);

	Enemy* newEnemy = new Enemy();
	newEnemy->SetPlayer(player.get());
	newEnemy->Initialize(model_, textureHandle2_, {0, 10, 20});
	enemy.reset(newEnemy);

	viewProjection_.Initialize();
}

void GameScene::Update() { 

	player->Update();
	if (enemy) enemy->Update();

#ifdef DEBUG
	if (input_->TriggerKey(DIK_0)) {
		isDebug = !isDebug;
	}
#endif // DEBUG

	if (isDebug) {
		debugCamera_->Update();
		vp = debugCamera_->GetViewProjection();
	} else {
		viewProjection_.UpdateMatrix();
		vp = viewProjection_;
	}
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
	player->Draw(vp);
	if(enemy) enemy->Draw(vp);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	player->DebugText({50, 50});
	if (enemy) enemy->DebugText({50, 110});


	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
