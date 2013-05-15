#ifndef MENUSTATE_INKLUDED
#define MENUSTATE_INKLUDED


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
}
class SoundHandler;
class MosThread;
class MosSound;
class Button;
class Slide;
class MenuHandler;

class OptionMenuState:public MenuState
{
public:
	OptionMenuState();
	~OptionMenuState();
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
	sf::Sprite* mSpriteOptionMenuBackground;
	
	//volumn 
	Slide** mSlide; //array of slide
	int mSelectedSlide;
	int mAmountOfSlides;
	int mVolume;
	Button** mButton; // array of buttons
	int mSeletedButton;
	int mAmountofButtons;
	//Sound played when selecting something
	SoundHandler* mSelectSound;
	//scroll up and down sound 
	SoundHandler* mScrollSound;
	MosThread* mMosThread;
	MosSound* mMosSound;

};



#endif