#include "CollisionManager.h"
#include "MyCalc.h"

CollisionManager::~CollisionManager() { Clear(); }

void CollisionManager::Initialize() {}

void CollisionManager::Update() { CheckAllCollisions(); }

void CollisionManager::PushBack(Collider* collider) { colliders.push_back(collider); }

void CollisionManager::Clear() {
	if (!colliders.empty()) colliders.clear();
}

void CollisionManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB) {
	if (
	  (colliderA->GetAttribute() & colliderB->GetMask()) == 0 ||
	  (colliderB->GetAttribute() & colliderA->GetMask()) == 0) {
		return;
	}
	Vector3 posA = colliderA->GetWorldPos();
	Vector3 posB = colliderB->GetWorldPos();
	Vector3 dist = SubVector3(posB, posA);
	if (SizeVector3(dist) <= colliderA->GetRad() + colliderB->GetRad()) {
		colliderA->OnCollision();
		colliderB->OnCollision();
	}
}

void CollisionManager::CheckAllCollisions() {
	std::list<Collider*>::iterator itrA = colliders.begin();
	for (; itrA != colliders.end(); ++itrA) {
		Collider* colA = *itrA;

		std::list<Collider*>::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders.end(); ++itrB) {
			Collider* colB = *itrB;

			CheckCollisionPair(colA, colB);
		}
	}
}
