#pragma once
#include "CircleCollisionComponent.h"
#include "Actor.h"

class Astroid : public Actor
{
public:
	Astroid();
	~Astroid();

	CircleCollisionComponent& getCollision() { return *collision; }

private:
	CircleCollisionComponent* collision;
};