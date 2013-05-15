#ifndef INSTRUCTIONSMENUSTATE_INKLUDED
#define INSTRUCTIONSMENUSTATE_INKLUDED


//cecilia Z
#include "MenuState.h"

namespace sf {
	template <typename T>
	class Vector2;
	typedef Vector2<float> Vector2f;
	class Sprite;
	class Sound;
	class SoundBuffer;
	class Music;
	class RenderTarget; 
	class Clock;
}

class SoundHandler;
class MosSound;
class MenuHandler;

class InstructionsMenuState:public MenuState
{
public:
	InstructionsMenuState();
	~InstructionsMenuState();
	virtual void update(const sf::RenderTarget& window, MenuHandler& handler);
	virtual void render(sf::RenderTarget& window) const ;
	//This function is called after the state has been pushed.
	virtual void statePushed();
	//This function is called after the state has been popped and right before it's deleted.
	virtual void statePopped();
	virtual void buttonPressed(MenuHandler& handler, Gamepad* gamepad, int mask);
	virtual void buttonReleased(MenuHandler& handler, Gamepad* gamepad, int mask);
	

private:
	sf::Sprite* mSpriteBackground;
	sf::Sprite* mSpriteInstructionsBackground;
	sf::Clock *mClock;
	SoundHandler* mChainSound;
	MosSound* mMosSound;

};



#endif