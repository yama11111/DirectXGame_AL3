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

	worldTransforms_[kRoot].Initialize();

	worldTransforms_[kSpine].Initialize();
	worldTransforms_[kSpine].translation_ = {0, 4.5f, 0};
	worldTransforms_[kSpine].parent_ = &worldTransforms_[kRoot];

	worldTransforms_[kChest].Initialize();
	worldTransforms_[kChest].translation_ = {0, 0, 0};
	worldTransforms_[kChest].parent_ = &worldTransforms_[kSpine];

	worldTransforms_[kHead].Initialize();
	worldTransforms_[kHead].translation_ = {0, 4.5f, 0};
	worldTransforms_[kHead].parent_ = &worldTransforms_[kChest];

	worldTransforms_[kArmL].Initialize();
	worldTransforms_[kArmL].translation_ = {4.5f, 0, 0};
	worldTransforms_[kArmL].parent_ = &worldTransforms_[kChest];

	worldTransforms_[kHandL].Initialize();
	worldTransforms_[kHandL].translation_ = {0, -4.5f, 0};
	worldTransforms_[kHandL].parent_ = &worldTransforms_[kArmL];

	worldTransforms_[kArmR].Initialize();
	worldTransforms_[kArmR].translation_ = {-4.5f, 0, 0};
	worldTransforms_[kArmR].parent_ = &worldTransforms_[kChest];

	worldTransforms_[kHandR].Initialize();
	worldTransforms_[kHandR].translation_ = {0, -4.5f, 0};
	worldTransforms_[kHandR].parent_ = &worldTransforms_[kArmR];

	worldTransforms_[kHip].Initialize();
	worldTransforms_[kHip].translation_ = {0, -4.5f, 0};
	worldTransforms_[kHip].parent_ = &worldTransforms_[kSpine];

	worldTransforms_[kLegL].Initialize();
	worldTransforms_[kLegL].translation_ = {3.0f, -4.5f, 0};
	worldTransforms_[kLegL].parent_ = &worldTransforms_[kHip];

	worldTransforms_[kFootL].Initialize();
	worldTransforms_[kFootL].translation_ = {0, -4.5f, 0};
	worldTransforms_[kFootL].parent_ = &worldTransforms_[kLegL];

	worldTransforms_[kLegR].Initialize();
	worldTransforms_[kLegR].translation_ = {-3.0f, -4.5f, 0};
	worldTransforms_[kLegR].parent_ = &worldTransforms_[kHip];

	worldTransforms_[kFootR].Initialize();
	worldTransforms_[kFootR].translation_ = {0, -4.5f, 0};
	worldTransforms_[kFootR].parent_ = &worldTransforms_[kLegR];

	worldTransforms_[kRoot].rotation_.y = PI / 180 * 30.0f;

	viewProjection_.Initialize();

}

void GameScene::Update() { 

	MoveChara();
	
	if (input_->PushKey(DIK_W)) {
		Walk();
	} else {
		ReturnIdle();
	}

	UpdateMatrix();
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

	for (int i = 0; i < kNumPartId; i++) {

		model_->Draw(worldTransforms_[i], viewProjection_, textureHandle_);
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

	VPDebugText();
	PPDebugText();
	CharaDebugText();

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

void GameScene::MoveChara() {
	Vector3 move = {0, 0, 0};
	const float kSpeed = 0.2f;

	if (input_->PushKey(DIK_RIGHT)) {
		move.x += kSpeed;
	}
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kSpeed;
	}

	worldTransforms_[kRoot].translation_ += move;

	float angle2 = 0.0f;
	const float kSpeed2 = 0.05f;

	if (input_->PushKey(DIK_A)) {
		angle2 += kSpeed2;
	}
	if (input_->PushKey(DIK_D)) {
		angle2 -= kSpeed2;
	}

	worldTransforms_[kRoot].rotation_.y -= angle2;
}

void GameScene::Walk() {
	PartWalk(kArmL, true);
	PartWalk(kArmR, false);
	PartWalk(kLegL, false);
	PartWalk(kLegR, true);
}

void GameScene::PartWalk(const int part, bool b) {
	float angle = PI / 180 * 3.0f;

	if (b) {
		if (a) {
			if (worldTransforms_[part].rotation_.x >= -PI / 180 * 45.0f) {
				worldTransforms_[part].rotation_.x -= angle;
			} else {
				a = false;
			}
		} else {
			if (worldTransforms_[part].rotation_.x <= PI / 180 * 45.0f) {
				worldTransforms_[part].rotation_.x += angle;
			} else {
				a = true;
			}
		}
	} else {
		if (a) {
			if (worldTransforms_[part].rotation_.x <= PI / 180 * 45.0f) {
				worldTransforms_[part].rotation_.x += angle;
			} else {
				a = false;
			}
		} else {
			if (worldTransforms_[part].rotation_.x >= -PI / 180 * 45.0f) {
				worldTransforms_[part].rotation_.x -= angle;
			} else {
				a = true;
			}
		}
	}
}

void GameScene::ReturnIdle() {
	PartIdle(kArmL);
	PartIdle(kArmR);
	PartIdle(kLegL);
	PartIdle(kLegR);
}

void GameScene::PartIdle(const int part) {
	float angle = PI / 180 * 3.0f;

	if ((worldTransforms_[part].rotation_.x > 0)) {
		worldTransforms_[part].rotation_.x -= angle;
		if (worldTransforms_[part].rotation_.x <= 0) {
			worldTransforms_[part].rotation_.x = 0;
		}
	}
	if ((worldTransforms_[part].rotation_.x < 0)) {
		worldTransforms_[part].rotation_.x += angle;
		if (worldTransforms_[part].rotation_.x >= 0) {
			worldTransforms_[part].rotation_.x = 0;
		}
	}
}


void GameScene::UpdateMatrix() {
	for (int i = 0; i < kNumPartId; i++) {
		Affine(worldTransforms_[i]);
		if (worldTransforms_[i].parent_ != nullptr) {
			worldTransforms_[i].matWorld_ *= worldTransforms_[i].parent_->matWorld_;
			worldTransforms_[i].TransferMatrix();
		}
	}
}

void GameScene::VPDebugText() {
	debugText_->SetPos(50, 50);
	debugText_->Printf(
	  "eye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);

	debugText_->SetPos(50, 70);
	debugText_->Printf(
	  "target:(%f,%f,%f)", viewProjection_.target.x, viewProjection_.target.y, viewProjection_.target.z);

	debugText_->SetPos(50, 90);
	debugText_->Printf(
	  "target:(%f,%f,%f)", viewProjection_.up.x, viewProjection_.up.y,
	  viewProjection_.up.z);
}
void GameScene::PPDebugText() {
	debugText_->SetPos(50, 110);
	debugText_->Printf(
	  "fovAngleY(Degree):%f", viewProjection_.fovAngleY * 180 / PI);

	debugText_->SetPos(50, 130);
	debugText_->Printf(
	  "nearZ:%f", viewProjection_.nearZ);
}
void GameScene::CharaDebugText() {
	debugText_->SetPos(50, 150);
	debugText_->Printf(
		"Root:(%f,%f,%f)", 
		worldTransforms_[kRoot].translation_.x, 
		worldTransforms_[kRoot].translation_.y, 
		worldTransforms_[kRoot].translation_.z);
}