#include "RailCamera.h"
#include "MyCalc.h"

void RailCamera::Initialize(const Vector3& pos) {
	wt.translation_ = pos;
	wt.Initialize();
	Affine(wt);
	wt.TransferMatrix();
	vp.farZ = 750.0f;
	vp.Initialize();
	debugText = DebugText::GetInstance();
}

void RailCamera::Update() { 
	Move();
	Rotate();
	Affine(wt);
	wt.TransferMatrix();
	vpUpdate();
}

void RailCamera::DebugText(const Vector2& leftTop) {
	debugText->SetPos(leftTop.x, leftTop.y);
	debugText->Printf("Camera");
	debugText->SetPos(leftTop.x + 20, leftTop.y + 20);
	debugText->Printf("eye : (%f, %f, %f)", vp.eye.x, vp.eye.y, vp.eye.z);
}

void RailCamera::Move() {
	Vector3 move = {0, 0, 0};
	const float power = 0.05f;

	move.z += power;

	wt.translation_ += move;
}

void RailCamera::Rotate() {
	Vector3 rota = {0, 0, 0};
	const float power = 0.01f;

	rota.z += power;

	wt.rotation_ += rota;
}

void RailCamera::vpUpdate() {
	vp.eye = wt.translation_;
	Vector3 forward(0, 0, 1);
	forward = MultVec3Mat4(forward, wt.matWorld_);
	vp.target = AddVector3(vp.eye, forward);
	Vector3 up(0, 1, 0);
	vp.up = MultVec3Mat4(up, wt.matWorld_);
	vp.UpdateMatrix();
	vp.TransferMatrix();
}
