#include "GameScene.h"
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

	// 大元
	worldTransform_[PartId::Root].Initialize();
	// 脊髄
	worldTransform_[PartId::Spine].translation_ = {0, 0, 0};
	worldTransform_[PartId::Spine].parent_ = &worldTransform_[PartId::Root];
	worldTransform_[PartId::Spine].Initialize();
	// 上半身
	worldTransform_[PartId::Chest].translation_ = {0, 0, 0};
	worldTransform_[PartId::Chest].parent_ = &worldTransform_[PartId::Spine];
	worldTransform_[PartId::Chest].Initialize();
	worldTransform_[PartId::Head].translation_ = {0, 4.5f, 0};
	worldTransform_[PartId::Head].parent_ = &worldTransform_[PartId::Chest];
	worldTransform_[PartId::Head].Initialize();
	worldTransform_[PartId::ArmL].translation_ = {4.5f, 0, 0};
	worldTransform_[PartId::ArmL].parent_ = &worldTransform_[PartId::Chest];
	worldTransform_[PartId::ArmL].Initialize();
	worldTransform_[PartId::ArmR].translation_ = {-4.5f, 0, 0};
	worldTransform_[PartId::ArmR].parent_ = &worldTransform_[PartId::Chest];
	worldTransform_[PartId::ArmR].Initialize();
	// 下半身
	worldTransform_[PartId::Hip].translation_ = {0, -4.5f, 0};
	worldTransform_[PartId::Hip].parent_ = &worldTransform_[PartId::Spine];
	worldTransform_[PartId::Hip].Initialize();
	worldTransform_[PartId::LegL].translation_ = {4.5f, -4.5f, 0};
	worldTransform_[PartId::LegL].parent_ = &worldTransform_[PartId::Hip];
	worldTransform_[PartId::LegL].Initialize();
	worldTransform_[PartId::LegR].translation_ = {-4.5f, -4.5f, 0};
	worldTransform_[PartId::LegR].parent_ = &worldTransform_[PartId::Hip];
	worldTransform_[PartId::LegR].Initialize();

	// 視野角設定
	//viewProjection_.fovAngleY = XMConvertToRadians(10.0f);
	// アスペクト比設定
	//viewProjection_.aspectRatio = 1.0f;
	// ニアクリップ、ファークリップ設定
	//viewProjection_.nearZ = 52.0f;
	//viewProjection_.farZ = 53.0f;

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

	// 上半身回転
	const float kChestRotSpeed = 0.05f; // 速度

	if (input_->PushKey(DIK_U)) {
		worldTransform_[PartId::Chest].rotation_.y -= kChestRotSpeed;
	} else if (input_->PushKey(DIK_I)) {
		worldTransform_[PartId::Chest].rotation_.y += kChestRotSpeed;
	}

	// 下半身回転
	const float kHipRotSpeed = 0.05f; // 速度

	if (input_->PushKey(DIK_J)) {
		worldTransform_[PartId::Hip].rotation_.y -= kHipRotSpeed;
	} else if (input_->PushKey(DIK_K)) {
		worldTransform_[PartId::Hip].rotation_.y += kHipRotSpeed;
	}

	worldTransform_[PartId::Root].UpdateMatrix();
	worldTransform_[PartId::Spine].UpdateMatrix();
	worldTransform_[PartId::Chest].UpdateMatrix();
	worldTransform_[PartId::Head].UpdateMatrix();
	worldTransform_[PartId::ArmL].UpdateMatrix();
	worldTransform_[PartId::ArmR].UpdateMatrix();
	worldTransform_[PartId::Hip].UpdateMatrix();
	worldTransform_[PartId::LegL].UpdateMatrix();
	worldTransform_[PartId::LegR].UpdateMatrix();

	// 注視点移動
	worldTransform_[PartId::Root].translation_.x += move.x;
	worldTransform_[PartId::Root].translation_.y += move.y;
	worldTransform_[PartId::Root].translation_.z += move.z;

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
	
	debugText_->SetPos(50, 150);
	debugText_->Printf(
	  "Root : (% f, % f, %f)", 
		worldTransform_[PartId::Root].translation_.x,
		worldTransform_[PartId::Root].translation_.y,
		worldTransform_[PartId::Root].translation_.z);
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
	//model_->Draw(worldTransform_[PartId::Root], viewProjection_, textureHandle_);
	//model_->Draw(worldTransform_[PartId::Spine], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[PartId::Chest], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[PartId::Head], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[PartId::ArmL], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[PartId::ArmR], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[PartId::Hip], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[PartId::LegL], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[PartId::LegR], viewProjection_, textureHandle_);

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
