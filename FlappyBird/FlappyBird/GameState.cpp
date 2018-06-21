#include "GameState.h"
#include "DEFINITIONS.h"
#include <iostream>
#include "GameOverState.h"


GameState::GameState( GameDataRef data) : _data( data )
{
}


GameState::~GameState()
{
}

void GameState::Init()
{
	if( !_hitSoundBuffer.loadFromFile( HIT_SOUND_FILEPATH ))
	{
		std::cout << "Error loading Hit Sound Effect" << std::endl;
	}

	if (!_wingSoundBuffer.loadFromFile( WING_SOUND_FILEPATH))
	{
		std::cout << "Error loading Hit Sound Effect" << std::endl;
	}

	if (!_pointSoundBuffer.loadFromFile(POINT_SOUND_FILEPATH))
	{
		std::cout << "Error loading Hit Sound Effect" << std::endl;
	}

		_hitSound.setBuffer(_hitSoundBuffer);
		_wingSound.setBuffer(_wingSoundBuffer);

		_pointSound.setBuffer(_pointSoundBuffer);

	_data->assets.LoadTexture("Game Background", GAME_BACKGROUND_FILEPATH);
	_data->assets.LoadTexture("Pipe Up", PIPE_UP_FILEPATH);
	_data->assets.LoadTexture("Pipe Down", PIPE_DOWN_FILEPATH);
	_data->assets.LoadTexture("Land", LAND_FILEPATH);

	_data->assets.LoadTexture("Bird Frame 1", BIRD_FRAME_1_FILEPATH);
	_data->assets.LoadTexture("Bird Frame 2", BIRD_FRAME_2_FILEPATH);
	_data->assets.LoadTexture("Bird Frame 3", BIRD_FRAME_3_FILEPATH);
	_data->assets.LoadTexture("Bird Frame 4", BIRD_FRAME_4_FILEPATH);
	_data->assets.LoadTexture("Scoring Pipe", SCORING_FILEPATH);
	_data->assets.LoadFont( "Flappy Font", FLAPPY_FONT_FILEPATH);

	_background.setTexture(this->_data->assets.GetTexture("Game Background"));
	
	land = new Land( _data);
	pipe = new Pipe( _data );
	bird = new Bird( _data);
	flash = new Flash( _data );
	hud = new HUD( _data);

	_score = 0;
	hud->UpdateScore( _score );
	_gameState = GameStates::eReady;

}

void GameState::HandleInput()
{
	sf::Event event;
	while (_data->window.pollEvent(event))
	{
		if (sf::Event::Closed == event.type)
			_data->window.close();

		if( _data->input.IsSpriteClicked( _background, sf::Mouse::Left, _data->window ))
		{
			if( GameStates::eReady == _gameState)
				_gameState = GameStates::ePlaying;

			if( GameStates::eGameOver != _gameState )
			{
				bird->Tap();
				_wingSound.play();
			}
		}
	}
}

void GameState::Update(float dt)
{
	if (GameStates::eGameOver != _gameState)
	{
		bird->Animate(dt);
		land->MoveLand(dt);
	}
	if( GameStates::ePlaying == _gameState )
	{
		pipe->MovePipes(dt);

		if (_clock.getElapsedTime().asSeconds() > PIPE_SPAWN_FREQUENCY)
		{
			pipe->RandomisePipeOffset();

			pipe->SpawnBottomPipe();
			//pipe->SpawnInvisiblePipe();
			pipe->SpawnTopPipe();
			pipe->SpawnScoringPipe();

			_clock.restart();
		}

		bird->Update(dt);

		std::vector<sf::Sprite> landSprites = land->GetSprites();

		for( int i = 0; i < landSprites.size(); i++ )
		{
			if( collision.CheckSpriteCollision( bird->GetSprite(), landSprites.at(i) ))
			{
				_hitSound.play();

				_gameState = GameStates::eGameOver;

				_clock.restart();

			}
		}

		std::vector<sf::Sprite> pipeSprites = pipe->GetSprites();

		for (int i = 0; i < pipeSprites.size(); i++)
		{
			if (collision.CheckSpriteCollision(bird->GetSprite(), 0.7f, pipeSprites.at(i), 0.99f ) )
			{
				_hitSound.play();

				_gameState = GameStates::eGameOver;
				_clock.restart();

			}
		}

		std::vector<sf::Sprite> &scoringSprites = pipe->GetScoringSprites();

		for (int i = 0; i < scoringSprites.size(); i++)
		{
			if (collision.CheckSpriteCollision(bird->GetSprite(), 0.7f, scoringSprites.at(i), 0.99f))
			{
				_score++;
				_pointSound.play();

				hud->UpdateScore( _score );

				scoringSprites.erase( scoringSprites.begin() + i );
			}
		}
	}

	if( GameStates::eGameOver == _gameState)
	{
		flash->Show(dt);

		if ( _clock.getElapsedTime().asSeconds() > TIME_BEFORE_GAME_OVER_APPEARS )
		{
			_data->machine.AddState( StateRef( new GameOverState( _data, _score )), true);
		}

	}

		
}

void GameState::Draw(float dt)
{
	_data->window.clear();

	_data->window.draw(_background);
	pipe->DrawPipes();
	land->DrawLand();
	
	bird->Draw();
	flash->Draw();

	hud->Draw();

	_data->window.display();
}
