#pragma once
#include "Game.hpp"


class MainMenuState : public State
{
public:
	MainMenuState( GameDataRef data);
	~MainMenuState();

	void Init() override;
	void HandleInput() override;
	void Update(float dt) override;
	void Draw(float dt) override;

private:
	GameDataRef _data;

	sf::Clock _clock;

	sf::Sprite _background;
	sf::Sprite _title;
	sf::Sprite _playButton;

};

