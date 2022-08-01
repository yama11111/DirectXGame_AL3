#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "MyCalc.h"
#include "Random.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete modelSkydome;
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
	textureHandle3_ = TextureManager::Load("orange.png");
	model_ = Model::Create();
	modelSkydome = Model::CreateFromOBJ("skydome", true);

	Random* rand = Random::GetInstance();
	rand->Initialize();
	
	size_t s1 = rand->SetSeedRand(0, 2 * PI);
	size_t s2 = rand->SetSeedRand(-100, 100);
	for (WorldTransform& worldTransform : worldTransforms_) {
		worldTransform.Initialize();
		worldTransform.scale_ = {1.0f, 1.0f, 1.0f};
		worldTransform.rotation_ = {
			rand->GetSeedRand(s1), 
			rand->GetSeedRand(s1), 
			rand->GetSeedRand(s1)
		};
		worldTransform.translation_ = {
			rand->GetSeedRand(s2), 
			rand->GetSeedRand(s2), 
			rand->GetSeedRand(s2)
		};
		Affine(worldTransform);
	}

	camera.Initialize();
	angle = PI / 4;

	Player* newPlayer = new Player();
	newPlayer->Initialize(model_, textureHandle_);
	player.reset(newPlayer);

	Enemy* newEnemy = new Enemy();
	newEnemy->SetPlayer(player.get());
	newEnemy->Initialize(model_, textureHandle2_, {20, 0, 20});
	enemy.reset(newEnemy);

	Skydome* newSkydome = new Skydome();
	newSkydome->Initialize(modelSkydome);
	skydome.reset(newSkydome);

	CollisionManager* newCollManager = new CollisionManager();
	newCollManager->Initialize();
	collManager.reset(newCollManager);

	camera.SetSubject(player->GetWorldPosPointer());
	camera.SetAim(enemy->GetWorldPosPointer());
}

void GameScene::Update() { 

	player->Update(camera.GetDirection());
	camera.Follow();

	//camera.SetTarget(player->GetWorldPos());
	//camera.SetEye(AddVector3(
	//	camera.GetViewProjection().eye, player->GetMove()));

	if (input_->PushKey(DIK_RIGHT))
		camera.t(0.01f);
	if (input_->PushKey(DIK_LEFT))
		camera.t(-0.01f);
	//camera.Zoom(angle);

	if (input_->TriggerKey(DIK_SPACE))camera.Zooming();

	camera.Update();
	if (input_->TriggerKey(DIK_B)) camera.Shake();

	if (enemy) enemy->Update();
	skydome->Update();

	collManager->Clear();

	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player->GetBullets();
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy->GetBullets();
	collManager->PushBack(player.get());
	collManager->PushBack(enemy.get());
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
		collManager->PushBack(bullet.get());
	}
	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) {
		collManager->PushBack(bullet.get());
	}

	collManager->Update();

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_0)) {
		isDebug = !isDebug;
	}
#endif // _DEBUG

	if (isDebug) {
		debugCamera_->Update();
		vp = debugCamera_->GetViewProjection();
	} else {
		vp = camera.GetViewProjectionT();
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

	for (WorldTransform& worldTransform : worldTransforms_) {

		model_->Draw(worldTransform, vp, textureHandle3_);
	}

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	//skydome->Draw(vp);
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
	camera.DebugText({50, 170});

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}