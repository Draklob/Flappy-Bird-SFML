#pragma once

#include <SFML/Audio.hpp>

#include "Game.hpp"
#include "Pipe.h"
#include "Land.h"
#include "Bird.h"
#include "Collision.h"
#include "Flash.h"
#include "HUD.h"

class GameState : public State
{
public:
	GameState( GameDataRef data);
	~GameState();

	void Init() override;
	void HandleInput() override;
	void Update(float dt) override;
	void Draw(float dt) override;

private:
	GameDataRef _data;

	sf::Sprite _background;

	Pipe *pipe;
	Land *land;
	Bird *bird;
	Collision collision;
	Flash *flash;
	HUD *hud;

	sf::Clock _clock;

	int _gameState;

	sf::RectangleShape _gameOverFlash;
	bool _flashOn;

	int _score;

	sf::SoundBuffer _hitSoundBuffer;
	sf::SoundBuffer _wingSoundBuffer;
	sf::SoundBuffer _pointSoundBuffer;

	sf::Sound _hitSound;
	sf::Sound _wingSound;
	sf::Sound _pointSound;
};

