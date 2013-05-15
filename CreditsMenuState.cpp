#include "CreditsMenuState.h"
#include "SFML/Graphics.hpp"
#include "MenuHandler.h"
#include "SFML/Window.hpp"
#include "SFML/Audio.hpp"
#include "ResourceFactory.h"
#include "Button.h"
#include "ButtonCallback.h"
#include "MainMenuState.h"
#include "MosSound.h"
#include "SoundHandler.h"
#include "MusicHandler.h"
#include"Gamepad.h"
#include "SFML\System\Clock.hpp"

//Cecilia Z

using namespace sf;
using namespace std;

const unsigned int SPEED=50;


CreditsMenuState::CreditsMenuState():
 mSpriteBackground(new Sprite(*ResourceFactory::getInstance()->loadImage(string("Images/Menu/background.jpg"))))
	 ,mSpriteCreditsBackground(new Sprite(*ResourceFactory::getInstance()->loadImage(string("Images/Menu/creditssekvens.jpg"))))
	 ,mClock(new sf::Clock())
{
}

CreditsMenuState:: ~CreditsMenuState()
{
	delete mSpriteBackground;
	delete mSpriteCreditsBackground;

}

void CreditsMenuState::update(const sf::RenderTarget& window, MenuHandler& handler)
{
	
}

void CreditsMenuState::render(sf::RenderTarget& window) const 
{
	window.Draw(*mSpriteBackground);
	float time = mClock->GetElapsedTime();		
	int dist= time*SPEED*5;	
	int height = mSpriteCreditsBackground->GetSize().y*.5;
	int repetitions = (int)(dist/height);
	dist-=repetitions * height;
	mSpriteCreditsBackground->SetPosition(0.5*window.GetWidth()-0.5*mSpriteCreditsBackground->GetSize().x,-dist);
	window.Draw(*mSpriteCreditsBackground);
	mSpriteCreditsBackground->SetPosition(0.5*window.GetWidth()-0.5*mSpriteCreditsBackground->GetSize().x,-dist+height);
	window.Draw(*mSpriteCreditsBackground);
	
}


void CreditsMenuState::statePushed()
{}

void CreditsMenuState::statePopped()
{}

void CreditsMenuState::buttonPressed(MenuHandler& handler, Gamepad* gamepad, int mask)
{
	switch(mask)
	{	
	case Gamepad::B:
	case Gamepad::BACK:
		handler.popState();
		break;
	}

}

void CreditsMenuState::buttonReleased(MenuHandler& handler, Gamepad* gamepad, int mask)
{
	
}

