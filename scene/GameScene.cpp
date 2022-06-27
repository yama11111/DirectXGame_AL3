#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "MyMath.h"

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
	for (size_t i = 0; i < 4; i++) {
		enemy_[i] = Model::Create();
	}

	InitState();
}

void GameScene::Update() { 
	
	if (input_->TriggerKey(DIK_R)) {
		InitState();
	}

	debugCamera_->Update(); 

	if (input_->TriggerKey(DIK_Q)) {
		if (bioMove) bioMove = false;
		else bioMove = true;
	}

	if (bioMove) {
		BioMove();
	} else {
		CameraMove();
	}

	worldTransformF_.translation_.x = worldTransformP_.translation_.x + direction.x * 4;
	worldTransformF_.translation_.y = worldTransformP_.translation_.y + direction.y * 4;
	worldTransformF_.translation_.z = worldTransformP_.translation_.z + direction.z * 4;

	Affine(worldTransformP_);
	Affine(worldTransformF_);

	for (size_t i = 0; i < 2; i++) {
		viewProjection_[i].UpdateMatrix();
	}

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
	
	ViewProjection v;
	if (bioMove) {
		v = viewProjection_[0];
	} else {
		v = viewProjection_[1];
	}

	player_->Draw(worldTransformP_, v, textureHandle_);
	front_->Draw(worldTransformF_, v, textureHandle2_);
	for (size_t i = 0; i < 4; i++) {
		enemy_[i]->Draw(worldTransformE_[i], v, textureHandle_);
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

void GameScene::InitState() {
	worldTransformP_.Initialize();
	worldTransformF_.Initialize();
	for (size_t i = 0; i < 4; i++) {
		worldTransformE_[i].Initialize();
	}
	for (size_t i = 0; i < 2; i++) {
		viewProjection_[i].Initialize();
	}

	worldTransformP_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransformP_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransformP_.translation_ = {0.0f, 0.0f, 0.0f};
	worldTransformP_.matWorld_ = Scaling(worldTransformP_.matWorld_, worldTransformP_.scale_);
	worldTransformP_.matWorld_ = Rotation(worldTransformP_.matWorld_, worldTransformP_.rotation_);
	worldTransformP_.matWorld_ = Moving(worldTransformP_.matWorld_, worldTransformP_.translation_);
	worldTransformP_.TransferMatrix();

	worldTransformF_.scale_ = {0.25f, 0.25f, 1.0f};
	worldTransformF_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransformF_.translation_ = {0.0f, 0.0f, 5.0f};
	worldTransformF_.matWorld_ = Scaling(worldTransformF_.matWorld_, worldTransformF_.scale_);
	worldTransformF_.matWorld_ = Rotation(worldTransformF_.matWorld_, worldTransformF_.rotation_);
	worldTransformF_.matWorld_ = Moving(worldTransformF_.matWorld_, worldTransformF_.translation_);
	worldTransformF_.TransferMatrix();

	worldTransformE_[0].translation_ = {10, 0, 0};
	worldTransformE_[0].matWorld_ =
	  Moving(worldTransformE_[0].matWorld_, worldTransformE_[0].translation_);
	worldTransformE_[0].TransferMatrix();

	worldTransformE_[1].translation_ = {-10, 0, 0};
	worldTransformE_[1].matWorld_ =
	  Moving(worldTransformE_[1].matWorld_, worldTransformE_[1].translation_);
	worldTransformE_[1].TransferMatrix();

	worldTransformE_[2].translation_ = {0, 10, 0};
	worldTransformE_[2].matWorld_ =
	  Moving(worldTransformE_[2].matWorld_, worldTransformE_[2].translation_);
	worldTransformE_[2].TransferMatrix();

	worldTransformE_[3].translation_ = {0, -10, 0};
	worldTransformE_[3].matWorld_ =
	  Moving(worldTransformE_[3].matWorld_, worldTransformE_[3].translation_);
	worldTransformE_[3].TransferMatrix();

	bioMove = true;
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

void GameScene::BioMove() {
	if (input_->PushKey(DIK_D)) {
		worldTransformP_.rotation_.y += PI / 36;
		if (worldTransformP_.rotation_.y >= 2 * PI) {
			worldTransformP_.rotation_.y -= 2 * PI;
		}
	}
	if (input_->PushKey(DIK_A)) {
		worldTransformP_.rotation_.y -= PI / 36;
		if (worldTransformP_.rotation_.y <= -2 * PI) {
			worldTransformP_.rotation_.y += 2 * PI;
		}
	}
	worldTransformF_.rotation_ = worldTransformP_.rotation_;

	// z正面基準
	Vec3 direction1, direction2, direction3;
	// x,z
	direction1 = {
	  sin(worldTransformP_.rotation_.y + PI / 2.0f), 0.0f,
	  cos(worldTransformP_.rotation_.y + PI / 2.0f)};
	// x,y
	direction2 = {sin(worldTransformP_.rotation_.z), cos(worldTransformP_.rotation_.z), 0.0f};
	// z,y
	direction3 = {0.0f, cos(worldTransformP_.rotation_.x), sin(worldTransformP_.rotation_.x)};

	direction = direction1.Closs(direction3);
	direction.Nomalize();

	if (input_->PushKey(DIK_W)) {
		worldTransformP_.translation_.x += direction.x * POWER;
		worldTransformP_.translation_.y += direction.y * POWER;
		worldTransformP_.translation_.z += direction.z * POWER;
	}
	if (input_->PushKey(DIK_S)) {
		worldTransformP_.translation_.x -= direction.x * POWER;
		worldTransformP_.translation_.y -= direction.y * POWER;
		worldTransformP_.translation_.z -= direction.z * POWER;
	}

	viewProjection_[0].eye.x = worldTransformP_.translation_.x - direction.x * 10;
	viewProjection_[0].eye.y = worldTransformP_.translation_.y - direction.y * 10 + 5;
	viewProjection_[0].eye.z = worldTransformP_.translation_.z - direction.z * 10;

	viewProjection_[0].target.x = worldTransformP_.translation_.x + direction.x * 10;
	viewProjection_[0].target.y = worldTransformP_.translation_.y + direction.y * 10;
	viewProjection_[0].target.z = worldTransformP_.translation_.z + direction.z * 10;
}

void GameScene::CameraMove() {
	//if (input_->PushKey(DIK_UP)) {
	//	viewProjection_[1].target.y += POWER;
	//}
	//if (input_->PushKey(DIK_DOWN)) {
	//	viewProjection_[1].target.y -= POWER;
	//}
	//if (input_->PushKey(DIK_RIGHT)) {
	//	viewProjection_[1].target.x += POWER;
	//}
	//if (input_->PushKey(DIK_LEFT)) {
	//	viewProjection_[1].target.x -= POWER;
	//}

	if (input_->PushKey(DIK_UP)) {
		viewProjection_[1].eye.y += POWER;
	}
	if (input_->PushKey(DIK_DOWN)) {
		viewProjection_[1].eye.y -= POWER;
	}
	if (input_->PushKey(DIK_RIGHT)) {
		viewProjection_[1].eye.x += POWER;
	}
	if (input_->PushKey(DIK_LEFT)) {
		viewProjection_[1].eye.x -= POWER;
	}

	Vec3 t = {
	  viewProjection_[1].target.x, viewProjection_[1].target.y, viewProjection_[1].target.z};
	Vec3 e = {viewProjection_[1].eye.x, viewProjection_[1].eye.y, viewProjection_[1].eye.z};
	Vec3 directionF;
	directionF = t - e;
	directionF.Nomalize();

	Vec3 y = {0, 1, 0};
	Vec3 directionR = y.Closs(directionF);

	if (input_->PushKey(DIK_W)) {
		worldTransformP_.translation_.x += directionF.x * POWER;
		worldTransformP_.translation_.y += directionF.y * POWER;
		worldTransformP_.translation_.z += directionF.z * POWER;
	}
	if (input_->PushKey(DIK_S)) {
		worldTransformP_.translation_.x -= directionF.x * POWER;
		worldTransformP_.translation_.y -= directionF.y * POWER;
		worldTransformP_.translation_.z -= directionF.z * POWER;
	}

	if (input_->PushKey(DIK_D)) {
		worldTransformP_.translation_.x += directionR.x * POWER;
		worldTransformP_.translation_.y += directionR.y * POWER;
		worldTransformP_.translation_.z += directionR.z * POWER;
	}
	if (input_->PushKey(DIK_A)) {
		worldTransformP_.translation_.x -= directionR.x * POWER;
		worldTransformP_.translation_.y -= directionR.y * POWER;
		worldTransformP_.translation_.z -= directionR.z * POWER;
	}
}

void GameScene::DebugState() {
	debugText_->SetPos(50, 50);
	debugText_->Printf(
	  "player : worldTransform_.translation_ : (% f, % f, % f)", worldTransformP_.translation_.x,
	  worldTransformP_.translation_.y, worldTransformP_.translation_.z);
	debugText_->SetPos(50, 70);
	debugText_->Printf(
	  "player : worldTransform_.rotation_ : (% f, % f, % f)", worldTransformP_.rotation_.x,
	  worldTransformP_.rotation_.y, worldTransformP_.rotation_.z);

	float x1, y1, z1, x2, y2, z2;
	if (bioMove) {
		x1 = viewProjection_[0].eye.x;
		y1 = viewProjection_[0].eye.y;
		z1 = viewProjection_[0].eye.z;

		x2 = viewProjection_[0].target.x;
		y2 = viewProjection_[0].target.y;
		z2 = viewProjection_[0].target.z;
	} else {
		x1 = viewProjection_[1].eye.x;
		y1 = viewProjection_[1].eye.y;
		z1 = viewProjection_[1].eye.z;

		x2 = viewProjection_[1].target.x;
		y2 = viewProjection_[1].target.y;
		z2 = viewProjection_[1].target.z;
	}

	debugText_->SetPos(50, 90);
	debugText_->Printf("viewProjection_.eye : (% f, % f, % f)", x1, y1, z1);
	debugText_->SetPos(50, 110);
	debugText_->Printf("viewProjection_.target : (% f, % f, % f)", x2, y2, z2);

	// debugText_->SetPos(50, 140);
	// debugText_->Printf("direction1 : (% f, % f, % f)", direction1.x, direction1.y, direction1.z);
	// debugText_->SetPos(50, 160);
	// debugText_->Printf("direction2 : (% f, % f, % f)", direction2.x, direction2.y, direction2.z);
	// debugText_->SetPos(50, 180);
	// debugText_->Printf("direction3 : (% f, % f, % f)", direction3.x, direction3.y, direction3.z);
	// debugText_->SetPos(50, 200);
	// debugText_->Printf("direction  : (% f, % f, % f)", direction.x, direction.y, direction.z);
}
