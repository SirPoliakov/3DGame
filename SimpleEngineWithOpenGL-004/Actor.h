#pragma once

#include <vector>
#include "Vector2.h"
#include <SDL_stdinc.h>

using std::vector;

class Game;
class Component;

class Actor
{
public:

	enum class ActorState
	{
		Active, Paused, Dead
	};

	Actor();
	virtual ~Actor();
	Actor(const Actor&) = delete;
	Actor& operator=(const Actor&) = delete;

	Vector2 getForward() const;

	Game& getGame() const { return game; }
	const ActorState getState() const { return state; }
	const Vector2 getPosition() const { return position; }
	const float getScale() const { return scale; }
	const float getRotation() const { return rotation; }

	void setPosition(Vector2 positionP);
	void setScale(float scaleP);
	void setRotation(float rotationP);
	void setState(ActorState stateP);

	void update(float dt);
	void updateComponents(float dt);
	virtual void updateActor(float dt);
	void addComponent(Component* component);
	void removeComponent(Component* component);

	void processInput(const Uint8* keyState);
	virtual void actorInput(const Uint8* keyState);

private:
	Game& game;
	ActorState state;
	Vector2 position;
	float scale;
	float rotation;  //Rotations in radians

	vector<Component*> components;
}; 