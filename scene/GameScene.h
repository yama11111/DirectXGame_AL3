#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"

#include "Player.h"
#include "Enemy.h"
#include "Skydome.h"
#include "CollisionManager.h"
#include "Camera.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

 public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

 private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	DebugCamera* debugCamera_ = nullptr;
	uint32_t textureHandle_ = 0;
	uint32_t textureHandle2_ = 0;
	uint32_t textureHandle3_ = 0;
	Model* model_ = nullptr;
	Model* modelSkydome = nullptr;
	std::unique_ptr<Player> player;
	std::unique_ptr<Enemy> enemy;
	std::unique_ptr<Skydome> skydome;
	std::unique_ptr<CollisionManager> collManager;

	WorldTransform worldTransforms_[100];

	ViewProjection vp;
	Camera camera;

	float angle = 0.0f;
	bool isDebug = false;

 private:
};
