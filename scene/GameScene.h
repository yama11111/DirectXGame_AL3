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
#include "Vec3.h"

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

	uint32_t playerTex = 0;
	uint32_t barrelTex = 0;
	uint32_t layTex = 0;

	Model* playerM = nullptr;
	WorldTransform playerWT;
	Model* barrelM = nullptr;
	WorldTransform barrelWT;
	Model* enemysM[4] = {nullptr, nullptr, nullptr, nullptr};
	WorldTransform enemysWT[4];
	Model* layM = nullptr;
	WorldTransform layWT;

	ViewProjection view;

	const float POWER = 0.5f;
	Vec3 directionF;
	bool isShot = false;
	bool isHit[4] = {false, false, false, false};

  private:
	void InitState();
	void Affine(WorldTransform& wt);
	void Move();
	void Shot();
	bool LayCollision(Vector3& position, const float radius);
	void DebugState();
};
