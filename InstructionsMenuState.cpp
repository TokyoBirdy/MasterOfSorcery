#include "InstructionsMenuState.h"
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
#include <cmath>

//Cecilia Z

const unsigned int Speed=200;
const unsigned int ChainLength= 135;

using namespace sf;
using namespace std;


InstructionsMenuState::InstructionsMenuState():
 mSpriteBackground(new Sprite(*ResourceFactory::getInstance()->loadImage(string("Images/Menu/background.jpg"))))
	 ,mSpriteInstructionsBackground(new Sprite(*ResourceFactory::getInstance()->loadImage(string("Images/Menu/instructionsscreen.png"))))
	 ,mClock(new sf::Clock())
	 ,mChainSound(new SoundHandler())
{
		mChainSound->putSoundInPlaylist(new MosSound(std::string("Sounds/Menu/menu_plank_up.wav")));	
}

InstructionsMenuState:: ~InstructionsMenuState()
{
	
	delete mSpriteBackground;
	delete mSpriteInstructionsBackground;
	delete mChainSound;
	
}

void InstructionsMenuState::update(const sf::RenderTarget& window, MenuHandler& handler)
{
	
	
}

void InstructionsMenuState::render(sf::RenderTarget& window) const 
{
	window.Draw(*mSpriteBackground);
	float time = mClock->GetElapsedTime();
	time = min(time, 0.6f);
	float dist = time*sin(time+1.0)*Speed-ChainLength;
	mSpriteInstructionsBackground->SetPosition(0.5*window.GetWidth()-0.5*mSpriteInstructionsBackground->GetSize().x,dist);
	window.Draw(*mSpriteInstructionsBackground);
}


void InstructionsMenuState::statePushed()
{
		mChainSound->playSound();	
}

void InstructionsMenuState::statePopped()
{

}

void InstructionsMenuState::buttonPressed(MenuHandler& handler, Gamepad* gamepad, int mask)
{
	switch(mask)
	{
	
	case Gamepad::A:
	case Gamepad::B:
	case Gamepad::BACK:
		handler.popState();
		break;
	}

}

void InstructionsMenuState::buttonReleased(MenuHandler& handler, Gamepad* gamepad, int mask)
{
	
}

