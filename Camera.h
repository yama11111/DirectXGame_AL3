#pragma once
#include "ViewProjection.h"
#include "Shake.h"
#include "DebugText.h"
#include "Timer.h"

class Camera {
  private:
	ViewProjection vp;
	ViewProjection vpT; // “]‘——p
	Shake shake;
	DebugText* debugText = nullptr;

	Vector3* subject;	// ”íŽÊ‘Ì
	Vector3* aim;		// Æ€
	Vector3 direction;	// Œü‚« (”íŽÊ‘Ì ¨ Æ€)

	Vector3 destEye;
	Vector3 destTarget;
	Timer timer;

	bool isZoom = false;
	bool isZoom2 = false;
	float afterFovAngleY;
	float befforFovAngleY;
	Timer timer2;
	float ts;
  public:
	void Initialize();
	void Update();
	void DebugText(const Vector2& leftTop);
	void SetTarget(const Vector3& target) { vp.target = target; }
	void SetEye(const Vector3& eye) { vp.eye = eye; }
	ViewProjection GetViewProjectionT();
	ViewProjection GetViewProjection() { return vp; }
	void RotateY(const float radian);
	void Zoom(const float radian);
	void Shake();

	void SetSubject(Vector3* subject) { this->subject = subject; };
	void SetAim(Vector3* aim) { this->aim = aim; };
	void CalcDirection();
	Vector3 GetDirection();
	void Follow();

	void t(const float t) { ts += t; }
	void Zooming() { isZoom = true; }
  private:
	void TimerUpdate();
	void Ease();
	void Ease2();
};
