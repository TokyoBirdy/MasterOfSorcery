#ifndef MAIN_MENU_STATE_INKLUDED
#define MAIN_MENU_STATE_INKLUDED


//cecilia Z

#include "MenuState.h"
#include <vector>

//delete later
using namespace std;

namespace sf
{ 
	class RenderTarget; 
	class Sprite;

}

class Button;
class MenuHandler;
class SoundHandler;
class MosThread;

class MainMenuState:public MenuState
{
public:
	
	MainMenuState();
	~MainMenuState();
	virtual void update(const sf::RenderTarget& window, MenuHandler& handler);
	virtual void render(sf::RenderTarget& window) const;
	virtual void statePushed();
	virtual void statePopped();
	virtual void buttonPressed(MenuHandler& handler,Gamepad* gamepad, int mask);
	virtual void buttonReleased(MenuHandler& handler,Gamepad* gamepad, int mask);	
private:

	//Sound played when selecting something
	SoundHandler* mSelectSound;
	//Cancel sound
	SoundHandler* mCancelSound;
	//scroll up and down sound 
	SoundHandler* mScrollSound;
	MosThread* mMosThread;
	Button** mButtons; 
	//Data
	sf::Sprite* mSprite;
	sf::Sprite* mSpriteMenu;
	int mSelectedButton;
	int mAmountOfButtons;
};

#endif