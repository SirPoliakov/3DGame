#include "Game.h"
#include "Actor.h"
#include "Assets.h"
#include "Astroid.h"
#include "Timer.h"
#include "SpriteComponent.h"
#include "AnimSpriteComponent.h"
#include "BackgroundSpriteComponent.h"
#include "Ship.h"

bool Game::initialize()
{
	bool isWindowInit = window.initialize();
	bool isRendererInit = renderer.initialize(window);

	return isWindowInit && isRendererInit; // Return bool && bool && bool ...to detect error
}

void Game::processInput()
{
	// SDL Event
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			isRunning = false;
			break;
		}
	}
	// Keyboard state
	const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);
	// Escape: quit game
	if (keyboardState[SDL_SCANCODE_ESCAPE])
	{
		isRunning = false;
	}

	//Actor Input
	isUpdatingActors = true;
	for (auto actor : actors)
	{
		actor->processInput(keyboardState);
	}
	isUpdatingActors = false;

	// Process Mouse
	int* x = 0; int * y = 0;
	Uint32 buttons = SDL_GetMouseState(x, y);
	if (SDL_BUTTON(buttons) & SDL_BUTTON_LEFT)
	{
		grid->processClick(*x, *y);
	}
}

void Game::update(float dt)
{
	// Update Actor
	isUpdatingActors = true;
	for (auto actor : actors) 
	{
		actor->update(dt);
	}
	isUpdatingActors = false;

	//Move pending actors to actors
	for (auto pendingActor : pendingActors)
	{
		actors.emplace_back(pendingActor);
	}
	pendingActors.clear();

	// Delete dead Actors
	vector<Actor*> deadActors;
	for (auto actor : actors)
	{
		if (actor->getState() == Actor::ActorState::Dead)
		{
			deadActors.emplace_back(actor);
		}
	}
	for (auto deadActor : deadActors)
	{
		delete deadActor;
	}
}

void Game::render()
{
	renderer.beginDraw();
	renderer.draw();
	renderer.endDraw();
}

void Game::addActor(Actor* actor)
{
	if (isUpdatingActors)
	{
		pendingActors.emplace_back(actor);
	}
	else
	{
		actors.emplace_back(actor);
	}
}

void Game::removeActor(Actor* actor)
{
	// Erase actor from 2 vectors
	auto iter = std::find(begin(pendingActors), end(pendingActors), actor);
	if (iter != end(pendingActors))
	{
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, end(pendingActors) - 1);
		pendingActors.pop_back();
	}
	iter = std::find(begin(actors), end(actors), actor);
	if (iter != end(actors))
	{
		std::iter_swap(iter, end(actors) - 1);
		actors.pop_back();
	}
}
void Game::load()
{
	// Load textures
	Assets::loadTexture(renderer, "Res\\Ship01.png", "ship01"); 
	Assets::loadTexture(renderer, "Res\\Ship02.png", "ship02");
	Assets::loadTexture(renderer, "Res\\Ship03.png", "ship03");
	Assets::loadTexture(renderer, "Res\\Ship04.png", "ship04");
	Assets::loadTexture(renderer, "Res\\Farback01.png", "Farback01");
	Assets::loadTexture(renderer, "Res\\Farback02.png", "Farback02");
	Assets::loadTexture(renderer, "Res\\Stars.png", "Stars");
	Assets::loadTexture(renderer, "Res\\Astroid.png", "Astroid");
	Assets::loadTexture(renderer, "Res\\Ship.png", "Ship");
	Assets::loadTexture(renderer, "Res\\Lazer.png", "Lazer");


	Assets::loadTexture(renderer, "Res\\Airplane.png", "Airplane");
	Assets::loadTexture(renderer, "Res\\Base.png", "Base");
	Assets::loadTexture(renderer, "Res\\Missile.png", "Missile");
	Assets::loadTexture(renderer, "Res\\Projectile.png", "Projectile");
	Assets::loadTexture(renderer, "Res\\TileBrown.png", "TileBrown");
	Assets::loadTexture(renderer, "Res\\TileBrownSelected.png", "TileBrownSelected");
	Assets::loadTexture(renderer, "Res\\TileGreen.png", "TileGreen");
	Assets::loadTexture(renderer, "Res\\TileGreenSelected.png", "TileGreenSelected");
	Assets::loadTexture(renderer, "Res\\TileGrey.png", "TileGrey");
	Assets::loadTexture(renderer, "Res\\TileGreySelected.png", "TileGreySelected");
	Assets::loadTexture(renderer, "Res\\TilePan.png", "TilePan");
	Assets::loadTexture(renderer, "Res\\TilePanSelected.png", "TilePanSelected");
	Assets::loadTexture(renderer, "Res\\TileRed.png", "TileRed");
	Assets::loadTexture(renderer, "Res\\TileRedSelected.png", "TileRedSelected");
	Assets::loadTexture(renderer, "Res\\Tower.png", "Tower");


	grid = new Grid();

	const int astroidNumber = 20;
	for (int i = 0; i < astroidNumber; i++)
	{
		new Astroid();
	}

	// Single sprite
	/*
	auto actor = new Actor();
	SpriteComponent*  sprite = new SpriteComponent(actor, Assets::getTexture("ship01"));
	actor->setPosition(Vector2{100, 100});
	*/

	// Animated sprite
	vector<Texture*> animTextures
	{
		&Assets::getTexture("Ship01"),
		&Assets::getTexture("Ship02"),
		&Assets::getTexture("Ship03"),
		&Assets::getTexture("Ship04"),
	};
	Actor* shipp = new Actor();
	AnimSpriteComponent* animatedSprite = new AnimSpriteComponent(shipp, animTextures);
	shipp->setPosition(Vector2{ 100, 300 });

	//Controlled Ship
	Ship* ship = new Ship();
	ship->setPosition(Vector2{100, 300});

	// Background :
	
	// Create the "far back" background
	vector<Texture*> bgTexsFar
	{
		&Assets::getTexture("Farback01"),
		&Assets::getTexture("Farback02")
	};
	Actor* bgFar = new Actor();
	BackgroundSpriteComponent* bgSpritesFar = new BackgroundSpriteComponent(bgFar, bgTexsFar);
	bgSpritesFar->setScrollSpeed(-100.0f);

	// Create a closer background
	Actor* bgClose = new Actor();
	std::vector<Texture*> bgTexsClose
	{
		&Assets::getTexture("Stars"),
		&Assets::getTexture("Stars")
	};
	BackgroundSpriteComponent* bgSpritesClose = new BackgroundSpriteComponent(bgClose, bgTexsClose, 50);
	bgSpritesClose->setScrollSpeed(-200.0f);
}

vector<Astroid*>& Game::getAstroids()
{
	return astroids;
}

void Game::addAstroid(Astroid* astroid)
{
	astroids.emplace_back(astroid);
}

void Game::removeAstroid(Astroid* astroid)
{
	auto iter = std::find(begin(astroids), end(astroids), astroid);
	if (iter != astroids.end())
	{
		astroids.erase(iter);
	}
}

void Game::unload()
{
	// Delete actor
	// Because ~Actor calls RemoveActor, have to use a different style loop
	while (!actors.empty())
	{
		delete actors.back();
	}

	// Ressources
	Assets::clear();
}

void Game::loop()
{
	Timer timer;
	float dt = 0;
	while (isRunning)
	{
		float dt = timer.computeDeltaTime() / 1000.0f;
		processInput();
		update(dt);
		render();
		timer.delayTime();
	}
}

void Game::close()
{
	renderer.close();
	window.close();
	SDL_Quit();
}

Renderer Game::getRenderer()
{
	return renderer;
}