#include "MainMenuState.h"
#include "Button.h"

#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "ResourceFactory.h"
#include "ButtonCallback.h"
#include "Gamepad.h"
#include "MenuHandler.h"
#include "PlayerSetupMenuState.h"
#include "StateHandler.h"
#include "OptionMenuState.h"
#include "CharSelectMenuState.h"
#include "SoundHandler.h"
#include "MosSound.h"
#include "InstructionsMenuState.h"
#include "CreditsMenuState.h"
#include "MosThread.h"

//cecilia Z

using namespace sf;
using namespace std;



namespace
{
	class playCallback:public ButtonCallback
	{
	public:
		playCallback():mhandler(MenuHandler::getInstance())
		{
			
		}

		void callBack()
		{
			
			mhandler.pushState(new CharSelectMenuState()); 
		}
	private:
		MenuHandler &mhandler;
	};

	class optionCallback:public ButtonCallback
	{
	
	public:
		optionCallback():mhandler(MenuHandler::getInstance())
		{}
		void callBack()
		{
			mhandler.pushState(new OptionMenuState());
		}

	private:
		MenuHandler &mhandler;

	};


	class instructionsCallback:public ButtonCallback
	{
	public:
		instructionsCallback() : mhandler(MenuHandler::getInstance())
		{
			
		}

		void callBack()
		{
			
			mhandler.pushState(new InstructionsMenuState());

		}

	private:
		MenuHandler &mhandler;
	};

	class creditsCallback:public ButtonCallback
	{
	public:

		creditsCallback():mhandler(MenuHandler::getInstance())
		{
			
		}

		void callBack()
		{
			mhandler.pushState(new CreditsMenuState());
		}

	private:
		MenuHandler &mhandler;

	};



	class exitCallback:public ButtonCallback
		{
		public:
			exitCallback(){}
			void callBack()
			{
				StateHandler::getInstance().setState(NULL);
			}
		};


}
MainMenuState::MainMenuState(): mSprite(new Sprite(*ResourceFactory::getInstance()->loadImage(string("Images/Menu/background.jpg"))))
	,mSpriteMenu(new Sprite(*ResourceFactory::getInstance()->loadImage(string("Images/Menu/mainmenu-clean.png"))))
	,mAmountOfButtons(5),mSelectedButton(0)
	,mSelectSound(new SoundHandler())
	,mScrollSound(new SoundHandler())
	,mCancelSound(new SoundHandler())
	,mMosThread(new MosThread())
{
	
	
	//Create buttons 
	    mButtons = new Button*[mAmountOfButtons];
		mButtons[0]=new Button("Images/Menu/play1-2.png",new playCallback()); //play	
		mButtons[1]=new Button("Images/Menu/options1-2.png", new optionCallback()); // volumn option 
		mButtons[2]=new Button("Images/Menu/instructions1-2.png", new instructionsCallback()); //instructions
		mButtons[3]=new Button("Images/Menu/credits1-2.png", new creditsCallback()); //credits
		mButtons[4]=new Button("Images/Menu/exit1-2.png", new exitCallback()); //exit

		// load sounds

		mScrollSound->putSoundInPlaylist(new MosSound(std::string("Sounds/Menu/menu_scroll_1.wav")));
		mScrollSound->putSoundInPlaylist(new MosSound(std::string("Sounds/Menu/menu_scroll_2.wav")));
		mSelectSound->putSoundInPlaylist(new MosSound(std::string("Sounds/Menu/menu_press_1.wav")));
		mCancelSound->putSoundInPlaylist(new MosSound(std::string("Sounds/Menu/menu_cancel.wav")));

}

MainMenuState::~MainMenuState()
{
	delete[] mButtons;
	
	delete mSelectSound;

	delete mCancelSound;

	delete mScrollSound;

	delete mMosThread;
}


void MainMenuState::update(const sf::RenderTarget& window, MenuHandler& handler)
{
	//check which button is pressed 
	for(int i=0; i<mAmountOfButtons; i++)
	{
		// only one button can return true;
		mButtons[i]->setSelected(!(i==mSelectedButton));
	
	}

}


void MainMenuState::render(sf::RenderTarget& window) const
{
	
	//render background/sprite 
	window.Draw(*mSprite);
	// position menu background
	mSpriteMenu->SetPosition(0.74*window.GetWidth()-0.5*mSpriteMenu->GetSize().x, 0.5*window.GetHeight()-0.5*mSpriteMenu->GetSize().y);
	window.Draw(*mSpriteMenu);
	
	// put the positions and render buttons 
	for( int i=0; i< mAmountOfButtons; i++)
	{
		
		mButtons[i]->setPosition(0.75*window.GetWidth()-0.25*mButtons[i]->getSizeWidth()+58, 0.5*window.GetHeight()-0.25*mAmountOfButtons*mButtons[i]->getSizeHeight()+i*mButtons[i]->getSizeHeight()-90+i*11);
		if(i==4)
		{
			mButtons[4]->moveButton(sf::Vector2f(-12.0f, 67.5f));
		}
		mButtons[i]->render(window);	

	}

	
}


void MainMenuState::statePushed()
{	
	mMosThread->runMusic(0);
}

void MainMenuState::statePopped()
{	
	mMosThread->stopMusic();
}

void MainMenuState::buttonPressed(MenuHandler& handler, Gamepad* gamepad,int mask)
{
	//one button is pressed 

	switch(mask)
	{
		case Gamepad::DPAD_DOWN:
			mSelectedButton=(mSelectedButton+1)%mAmountOfButtons;
			mScrollSound->playSound();
			break;
		case Gamepad::DPAD_UP:
			mSelectedButton--;
			if(mSelectedButton < 0)
			mSelectedButton += mAmountOfButtons;
			mScrollSound->playSound();
			break;
		case Gamepad::A:
			mButtons[mSelectedButton]->pressButton(); 
			mSelectSound->playSound();
			break;
		case Gamepad::B:
		case Gamepad::BACK:
			
			mCancelSound->playSound();
			StateHandler::getInstance().setState(NULL);
			break;
	}  

}

void MainMenuState::buttonReleased(MenuHandler& handler, Gamepad* gamepad,int mask)
{

}