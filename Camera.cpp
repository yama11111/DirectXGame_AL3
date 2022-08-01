#include "Camera.h"
#include <cmath>
#include "MyCalc.h"
#include "Ease.h"

void Camera::Initialize() { 
	debugText = DebugText::GetInstance();
	timer.Set(0, 1.0f, 0.05f);
	timer2.Set(0, 1.0f, 0.05f);
	vp.Initialize();
	vpT.Initialize();
	vp.eye = {-14, 0, -14};
	vp.target = {14, 0, 14};
	afterFovAngleY = PI / 8;
	befforFovAngleY = PI / 4;
}

void Camera::Update() {
	TimerUpdate();
	Ease();
	Ease2();
	shake.Update();
	vp.UpdateMatrix();
	vpT.UpdateMatrix();
}

void Camera::DebugText(const Vector2& leftTop) {

	debugText->SetPos(leftTop.x, leftTop.y);
	debugText->Printf("Camera");
	debugText->SetPos(leftTop.x + 20, leftTop.y + 20);
	debugText->Printf("target : (%f, %f, %f)", 
		vpT.target.x, vpT.target.y, vpT.target.z);
	debugText->SetPos(leftTop.x + 20, leftTop.y + 40);
	debugText->Printf("eye : (%f, %f, %f)", 
		vpT.eye.x, vpT.eye.y, vpT.eye.z);
	debugText->SetPos(leftTop.x + 20, leftTop.y + 60);
	debugText->Printf("fovAngleY(degree) : %f", 
		180 / PI * vpT.fovAngleY);
}

ViewProjection Camera::GetViewProjectionT() {
	vpT = vp;
	vpT.target += shake.GetShakeValue();
	vpT.eye.y = 5;
	vpT.eye += shake.GetShakeValue();
	return vpT;
}

void Camera::RotateY(const float radian) { 
	vp.target = {
		sinf(radian) + vp.eye.x, 
		vp.eye.y, 
		cosf(radian) + vp.eye.z};
}

void Camera::Zoom(const float radian) {
	vp.fovAngleY = radian;
	if (vp.fovAngleY <= 0.01f) vp.fovAngleY = 0.01f;
	if (vp.fovAngleY >= PI) vp.fovAngleY = PI;
}

void Camera::Shake() { shake.Shaking(1.5, 0.3); }

void Camera::CalcDirection() {
	// 向き計算
	if (aim && subject) {
		direction = SubVector3(*aim, *subject);
	}
}

Vector3 Camera::GetDirection() {
	CalcDirection();
	return direction;
}

void Camera::Follow() {
	CalcDirection();
	Vector3 nd = NormalizeVector3(direction);	 // 単位化
	Vector3 distance = MultVector3(nd, 20);		 // 離れる幅
	//vp.eye	  = SubVector3(*subject, distance);	 // 被写体 の 後ろ
	//vp.target = AddVector3(*subject, distance);	 // 被写体 の 前

	destEye	   = SubVector3(*subject, distance); // 被写体 の 後ろ
	destTarget = AddVector3(*subject, distance); // 被写体 の 前
}

void Camera::TimerUpdate() {

	if (!timer.IsActive()) {
		Vector3 v = SubVector3(destEye, vp.eye);
		float l = SizeVector3(v);
		if (l > 0) {
			timer.Reset();
			timer.Start();
		}
	}
	timer.Update();

	if (!timer2.IsActive()) {
		if (isZoom) {
			timer2.Start(0, 1.0f, 0.005f);
		}
	}
	timer2.Update();
	if (isZoom) {
		if (timer2.IsEnd()) {
			if (timer2.IsDekey()) {
				isZoom = false;
				isZoom2 = false;
			} else {
				isZoom2 = true;
				timer2.Start(1.0f, 1.0f, 0.05f, true);
			}
		}
	}
}

void Camera::Ease() { 
	vp.eye.x	= EaseIn(vp.eye.x, destEye.x, 
								timer.Current(), 2.0f);
	vp.eye.y	= EaseIn(vp.eye.y, destEye.y, 
								timer.Current(), 2.0f);
	vp.eye.z	= EaseIn(vp.eye.z, destEye.z, 
								timer.Current(), 2.0f);

	vp.target.x	= EaseIn(vp.target.x, destTarget.x, 
								timer.Current(), 2.0f); 
	vp.target.y	= EaseIn(vp.target.y, destTarget.y, 
								timer.Current(), 2.0f); 
	vp.target.z	= EaseIn(vp.target.z, destTarget.z, 
								timer.Current(), 2.0f); 
}

void Camera::Ease2() {
		vp.fovAngleY = EaseOut(befforFovAngleY, afterFovAngleY, 
			timer2.Current(), 2.0f);
	//debugText->SetPos(20, 330);
	//debugText->Printf(" : %f", timer2.Current());
}