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
	delete sprite_;
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
	textureHandle2_ = TextureManager::Load("reticle.png");
	textureHandle3_ = TextureManager::Load("scope.png");
	model_ = Model::Create();
	sprite_ = Sprite::Create(textureHandle2_, {1280 / 2 - 64, 720 / 2 - 64});
	sprite2_ = Sprite::Create(textureHandle3_, {0, 0});

	std::random_device seed_gen;
	std::mt19937_64 engine(seed_gen());
	
	std::uniform_real_distribution<float> dist(0, 2 * PI);
	std::uniform_real_distribution<float> dist2(-10, 10);

	for (int i = 0; i < 100; i++) {
		wt[i].Initialize();
		wt[i].rotation_ = {dist(engine), dist(engine), dist(engine)};
		wt[i].translation_ = {dist2(engine), dist2(engine), dist2(engine)};
		Affine(wt[i]);
	}

	vp.Initialize();
	vp.eye.z = -75.0;
}

void GameScene::Update() {

	if (input_->TriggerKey(DIK_SPACE)) {
		s = !s;
	}

	if (s) {
		if (input_->TriggerKey(DIK_W)) z = !z;
		if (input_->TriggerKey(DIK_S)) z = !z;
		if (z) {
			t += 0.1f;
			if (t >= 1.0f) t = 1.0f;
		} else {
			t -= 0.1f;
			if (t <= 0.0f) t = 0.0f;
		}
		vp.fovAngleY = lerp(PI / 180 * 30.0f, PI / 180 * 15.0f, t);
	} else {
		vp.fovAngleY = PI / 180 * 90.0f;
	}

	if (input_->PushKey(DIK_UP)) vp.target.y += 0.1f;
	if (input_->PushKey(DIK_DOWN)) vp.target.y -= 0.1f;
	if (input_->PushKey(DIK_RIGHT)) vp.target.x += 0.1f;
	if (input_->PushKey(DIK_LEFT)) vp.target.x -= 0.1f;

	vp.UpdateMatrix();
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
	for (int i = 0; i < 100; i++) {
		model_->Draw(wt[i], vp, textureHandle_);
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

	if (s) {
		sprite_->Draw();
		sprite2_->Draw();
	}

	if (s) {
		debugText_->SetPos(1000, 70);
		if (z) debugText_->Printf("x8");
		else debugText_->Printf("x4");
	}

	debugText_->SetPos(50, 50);
	debugText_->Printf("eye : (%f, %f, %f)", 
		vp.eye.x, vp.eye.y, vp.eye.z);
	debugText_->SetPos(50, 70);
	debugText_->Printf("target : (%f, %f, %f)", 
		vp.target.x, vp.target.y, vp.target.z);
	debugText_->SetPos(50, 90);
	debugText_->Printf("up : (%f, %f, %f)", 
		vp.up.x, vp.up.y, vp.up.z);
	debugText_->SetPos(50, 110);
	debugText_->Printf("fovAngleY(Degree) : %f", 
		vp.fovAngleY * 180 / PI);
	debugText_->SetPos(50, 130);
	debugText_->Printf("nearZ : %f", 
		vp.nearZ);

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

double GameScene::lerp(const double a, const double b, const double t) { return a + t * (b - a); }