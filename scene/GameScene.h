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

	uint32_t textureHandle_ = 0;
	uint32_t textureHandle2_ = 0;
	Model* player_ = nullptr;
	WorldTransform worldTransformP_;
	Model* front_ = nullptr;
	WorldTransform worldTransformF_;
	Model* enemy_[4] = {nullptr, nullptr, nullptr, nullptr};
	WorldTransform worldTransformE_[4];
	ViewProjection viewProjection_[2];

	bool bioMove = false;
	const float POWER = 0.5f;
	Vec3 direction;

  private:
	void InitState();
	void Affine(WorldTransform& wt);
	void BioMove();
	void CameraMove();
	void DebugState();
};
