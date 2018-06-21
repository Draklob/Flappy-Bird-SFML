#pragma once

#include "Game.hpp"
#include "DEFINITIONS.h"

class Bird
{
public:
	Bird( GameDataRef data);
	~Bird();

	void Draw();
	void Animate( float dt);

	void Update( float dt );

	void Tap();

	const sf::Sprite &GetSprite() const;

private:
	GameDataRef _data;
	sf::Sprite _birdSprite;

	std::vector<sf::Texture> _animationFrames;

	unsigned int _animationIterator;

	sf::Clock _clock;

	sf::Clock _movementClock;

	int _birdState;
	float _rotation;
};
