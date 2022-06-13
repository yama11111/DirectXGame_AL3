#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "MyMath.h"
#include "Vec3.h"

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
	worldTransformF_.translation_ = {0, 0, 5};
	worldTransformF_.scale_ = {0.25f, 0.25f, 1.0f};
	for (size_t i = 0; i < 4; i++) {
		enemy_[i] = Model::Create();
	}
	worldTransformE_[0].translation_ = {10, 0, 0};
	worldTransformE_[1].translation_ = {-10, 0, 0};
	worldTransformE_[2].translation_ = {0, 10, 0};
	worldTransformE_[3].translation_ = {0, -10, 0};

	Matrix4 matScale;
	matScale.m[0][0] = worldTransformF_.scale_.x;
	matScale.m[1][1] = worldTransformF_.scale_.y;
	matScale.m[2][2] = worldTransformF_.scale_.z;
	matScale.m[3][3] = 1.0;

	worldTransformF_.matWorld_ = {
		1, 0, 0, 0, 
		0, 1, 0, 0, 
		0, 0, 1, 0, 
		0, 0, 0, 1
	};
	worldTransformF_.matWorld_ = MultMatrix4(worldTransformF_.matWorld_, matScale);
	worldTransformF_.TransferMatrix();
	worldTransformF_.Initialize();

	worldTransformP_.Initialize();
	for (size_t i = 0; i < 4; i++) {
		worldTransformE_[i].Initialize();
	}
	viewProjection_.Initialize();
}

void GameScene::Update() { 
	
	debugCamera_->Update(); 
		if (input_->TriggerKey(DIK_Q)) {
		if (bioMove) {
			bioMove = false;
		} else {
			bioMove = true;
		}
	}

	if (bioMove) {
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
	}
	worldTransformF_.rotation_ = worldTransformP_.rotation_;

	const float POWER = 0.5f;
	// x,z逆

	Vec3 direction;
	direction = {sin(worldTransformP_.rotation_.y), 0.0f, cos(worldTransformP_.rotation_.y)};

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

	worldTransformF_.translation_.x = worldTransformP_.translation_.x + direction.x * 4;
	worldTransformF_.translation_.y = worldTransformP_.translation_.y + direction.y * 4;
	worldTransformF_.translation_.z = worldTransformP_.translation_.z + direction.z * 4;

	if (bioMove) {
		viewProjection_.eye.x = worldTransformP_.translation_.x - direction.x * 10;
		viewProjection_.eye.y = worldTransformP_.translation_.y - direction.y * 10 + 5;
		viewProjection_.eye.z = worldTransformP_.translation_.z - direction.z * 10;

		viewProjection_.target.x = worldTransformP_.translation_.x + direction.x * 10;
		viewProjection_.target.y = worldTransformP_.translation_.y + direction.y * 10;
		viewProjection_.target.z = worldTransformP_.translation_.z + direction.z * 10;
	} else {
		viewProjection_ = debugCamera_->GetViewProjection();
	}

	Vec3 t = {viewProjection_.target.x, viewProjection_.target.y, viewProjection_.target.z};
	Vec3 e = {viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z};
	Vec3 directionF;
	directionF = t - e;
	directionF.Nomalize();

	if (!bioMove) {
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

	worldTransformE_[2].translation_ = viewProjection_.target;
	worldTransformE_[3].translation_ = viewProjection_.eye;

	//worldTransformP_.UpdateMatrix();
	//worldTransformF_.UpdateMatrix();
	//for (size_t i = 0; i < 4; i++) {
	//	worldTransformE_[i].UpdateMatrix();
	//}
	viewProjection_.UpdateMatrix();

	debugText_->SetPos(50, 50);
	debugText_->Printf(
	  "player : worldTransform_.translation_ : (% f, % f, % f)", worldTransformP_.translation_.x,
	  worldTransformP_.translation_.y, worldTransformP_.translation_.z);
	debugText_->SetPos(50, 70);
	debugText_->Printf(
	  "player : worldTransform_.rotation_ : (% f, % f, % f)", worldTransformP_.rotation_.x,
	  worldTransformP_.rotation_.y, worldTransformP_.rotation_.z);
	debugText_->SetPos(50, 90);
	debugText_->Printf(
	  "viewProjection_.eye : (% f, % f, % f)", viewProjection_.eye.x, viewProjection_.eye.y,
	  viewProjection_.eye.z);
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
	
	player_->Draw(worldTransformP_, viewProjection_, textureHandle_);
	front_->Draw(worldTransformF_, viewProjection_, textureHandle2_);
	for (size_t i = 0; i < 4; i++) {
		enemy_[i]->Draw(worldTransformE_[i], viewProjection_, textureHandle_);
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
