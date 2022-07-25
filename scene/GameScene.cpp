#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "MyMath.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete debugCamera_;
	delete playerM;
	delete barrelM;
	for (size_t i = 0; i < 4; i++) {
		delete enemysM[i];
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

	playerTex = TextureManager::Load("mario.jpg");
	barrelTex = TextureManager::Load("front.png");

	playerM = Model::Create();
	barrelM = Model::Create();
	for (size_t i = 0; i < 4; i++) {
		enemysM[i] = Model::Create();
	}
	layM = Model::Create();

	InitState();
}

void GameScene::Update() { 
	
	if (input_->TriggerKey(DIK_R)) {
		InitState();
	}

	debugCamera_->Update(); 

	Move();

	if (input_->PushKey(DIK_SPACE)) isShot = true;
	else isShot = false;

	if (isShot) {
		Shot();
		for (int i = 0; i < 4; i++) {
			isHit[i] = LayCollision(enemysWT[i].translation_, 5);
		}
	} else {
		for (int i = 0; i < 4; i++) {
			isHit[i] = false;
		}
	}

	Affine(playerWT);
	Affine(barrelWT);
	Affine(layWT);

	view.UpdateMatrix();

	DebugState();
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

	playerM->Draw(playerWT, view, playerTex);
	barrelM->Draw(barrelWT, view, barrelTex);
	for (size_t i = 0; i < 4; i++) {
		if (!isHit[i]) enemysM[i]->Draw(enemysWT[i], view, playerTex);
	}
	if (isShot) layM->Draw(layWT, view, layTex);

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

void GameScene::InitState() {
	playerWT.Initialize();
	barrelWT.Initialize();
	for (int i = 0; i < 4; i++) {
		enemysWT[i].Initialize();
	}
	layWT.Initialize();
	view.Initialize();

	playerWT.scale_ = {1.0f, 1.0f, 1.0f};
	playerWT.rotation_ = {0.0f, 0.0f, 0.0f};
	playerWT.translation_ = {0.0f, 0.0f, -10.0f};
	Affine(playerWT);

	barrelWT.scale_ = {0.25f, 0.25f, 1.0f};
	barrelWT.rotation_ = {0.0f, 0.0f, 0.0f};
	barrelWT.translation_ = {0.0f, 0.0f, 5.0f};
	Affine(barrelWT);

	enemysWT[0].translation_ = { 5, 0, 15};
	enemysWT[1].translation_ = {-5, 0, 15};
	enemysWT[2].translation_ = { 15, 0, 25};
	enemysWT[3].translation_ = {-15, 0, 25};
	for (int i = 0; i < 4; i++) {
		Affine(enemysWT[i]);
	}

	layWT.scale_ = {0.5f, 0.5f, 20.0f};
	Affine(layWT);

	view.eye = {0, 20, -30};
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

void GameScene::Move() {
	// z正面基準
	Vec3 direction1, direction2;
	// x,z
	direction1 = {
	  sin(playerWT.rotation_.y + PI / 2.0f), 0.0f, cos(playerWT.rotation_.y + PI / 2.0f)};
	// z,y
	direction2 = {0.0f, cos(playerWT.rotation_.x), sin(playerWT.rotation_.x)};

	directionF = direction1.Closs(direction2);
	directionF.Nomalize();

	Vec3 y = {0, 1, 0};
	Vec3 directionR = y.Closs(directionF);

	if (input_->PushKey(DIK_W)) {
		playerWT.translation_.x += directionF.x * POWER;
		playerWT.translation_.y += directionF.y * POWER;
		playerWT.translation_.z += directionF.z * POWER;
	}
	if (input_->PushKey(DIK_S)) {
		playerWT.translation_.x -= directionF.x * POWER;
		playerWT.translation_.y -= directionF.y * POWER;
		playerWT.translation_.z -= directionF.z * POWER;
	}

	if (input_->PushKey(DIK_D)) {
		playerWT.translation_.x += directionR.x * POWER;
		playerWT.translation_.y += directionR.y * POWER;
		playerWT.translation_.z += directionR.z * POWER;
	}
	if (input_->PushKey(DIK_A)) {
		playerWT.translation_.x -= directionR.x * POWER;
		playerWT.translation_.y -= directionR.y * POWER;
		playerWT.translation_.z -= directionR.z * POWER;
	}

	barrelWT.translation_.x = playerWT.translation_.x + directionF.x * 2;
	barrelWT.translation_.y = playerWT.translation_.y + directionF.y * 2;
	barrelWT.translation_.z = playerWT.translation_.z + directionF.z * 2;
}

void GameScene::Shot() {
	layWT.translation_.x = barrelWT.translation_.x + directionF.x * layWT.scale_.x;
	layWT.translation_.y = barrelWT.translation_.y + directionF.y * layWT.scale_.y;
	layWT.translation_.z = barrelWT.translation_.z + directionF.z * layWT.scale_.z;
}

bool GameScene::LayCollision(Vector3& position, const float radius) { 

}

void GameScene::DebugState() {
	debugText_->SetPos(50, 50);
	debugText_->Printf(
	  "player : translation : (% f, % f, % f)", playerWT.translation_.x,
	  playerWT.translation_.y, playerWT.translation_.z);
	debugText_->SetPos(50, 70);
	debugText_->Printf(
	  "player : rotation : (% f, % f, % f)", playerWT.rotation_.x,
	  playerWT.rotation_.y, playerWT.rotation_.z);

	debugText_->SetPos(50, 90);
	debugText_->Printf("view.eye : (% f, % f, % f)", view.eye.x, view.eye.y, view.eye.z);
	debugText_->SetPos(50, 110);
	debugText_->Printf(
	  "view.target : (% f, % f, % f)", view.target.x, view.target.y, view.target.z);
}