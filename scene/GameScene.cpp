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
	
	for (WorldTransform& worldTransform : worldTransforms_) {
		worldTransform.Initialize();

		worldTransform.scale_ = {1.0f, 1.0f, 1.0f};
		worldTransform.rotation_ = {dist(engine), dist(engine), dist(engine)};
		worldTransform.translation_ = {
			dist2(engine),
			dist2(engine), 
			dist2(engine)
		};
		Affine(worldTransform);
	}

	viewProjection_.Initialize();

}

void GameScene::Update() { 
	
	//UpdateEye();
	//UpdateTarget();
	//UpdateUp();

	UpdateFovY();
	UpdateNearZ();

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

	for (WorldTransform& worldTransform : worldTransforms_) {

		model_->Draw(worldTransform, viewProjection_, textureHandle_);
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

	VPDebugText();
	PPDebugText();

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

void GameScene::UpdateEye() {
	Vector3 move = {0, 0, 0};
	const float kSpeed = 0.2f;

	if (input_->PushKey(DIK_W)) {
		move.z += kSpeed;
	}
	if (input_->PushKey(DIK_S)) {
		move.z -= kSpeed;
	}

	viewProjection_.eye += move;
}
void GameScene::UpdateTarget() {
	Vector3 move = {0, 0, 0};
	const float kSpeed = 0.2f;

	if (input_->PushKey(DIK_LEFT)) {
		move.x += kSpeed;
	}
	if (input_->PushKey(DIK_RIGHT)) {
		move.x -= kSpeed;
	}

	viewProjection_.target += move;
}
void GameScene::UpdateUp() {
	const float kSpeed = 0.05f;

	if (input_->PushKey(DIK_SPACE)) {
		angle += kSpeed;
		angle = fmodf(angle, PI * 2.0f);
	}

	viewProjection_.up = {cosf(angle), sinf(angle), 0.0f};
}

void GameScene::UpdateFovY() {
	if (input_->PushKey(DIK_W)) {
		if (viewProjection_.fovAngleY <= PI) {
			viewProjection_.fovAngleY += 0.05f;
		}
		if (viewProjection_.fovAngleY > PI) {
			viewProjection_.fovAngleY = PI;
		}
	}
	if (input_->PushKey(DIK_S)) {
		if (viewProjection_.fovAngleY >= 0.01f) {
			viewProjection_.fovAngleY -= 0.05f;
		}
		if (viewProjection_.fovAngleY < 0.01f) {
			viewProjection_.fovAngleY = 0.01f;
		}
	}
}
void GameScene::UpdateNearZ() {
	if (input_->PushKey(DIK_UP)) {
		viewProjection_.nearZ += 0.01f;
	}
	if (input_->PushKey(DIK_DOWN)) {
		viewProjection_.nearZ -= 0.01f;
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