#include "OptionMenuState.h"
#include "Slide.h"
#include "SFML/Graphics.hpp"
#include "MenuHandler.h"
#include "SFML/Window.hpp"
#include "Gamepad.h"
#include "SFML/Audio.hpp"
#include "ResourceFactory.h"
#include "Button.h"
#include "ButtonCallback.h"
#include "MainMenuState.h"
#include "MosSound.h"
#include "SoundHandler.h"
#include "MosThread.h"

//Cecilia Z
const float SPACE=41.0f;
const float MINIMUMPOSITION=415;
const float MAXIMUMPOSITION=600;
const float WIDTH = 180;
extern const unsigned int SCRN_WDTH = 1024;
extern const unsigned int SCRN_HGHT = 768;
using namespace sf;
using namespace std;



OptionMenuState::OptionMenuState():
 mSpriteBackground(new Sprite(*ResourceFactory::getInstance()->loadImage(string("Images/Menu/background.jpg"))))
	 ,mSpriteOptionMenuBackground(new Sprite(*ResourceFactory::getInstance()->loadImage(string("Images/Menu/options-menu-clean.png"))))
	 ,mSelectedSlide(-1),mAmountOfSlides(2),mVolume(50)
	 ,mSeletedButton(-1),mAmountofButtons(3)
	 ,mSelectSound(new SoundHandler()),mScrollSound(new SoundHandler())
	 ,mMosThread(new MosThread())
{
	mButton= new Button*[mAmountofButtons];
	mButton[0]= new Button("Images/Menu/music1-2.png",NULL);
	mButton[1]= new Button("Images/Menu/sound1-2.png",NULL);
	mButton[2]= new Button("Images/Menu/back1-2.png",NULL);

	//load slide images
	mSlide=new Slide*[mAmountOfSlides];
	for(int i=0; i<mAmountOfSlides;i++)
	{
		mSlide[i] = new Slide("Images/Menu/gemslider.png");
		float x = (float)(0.74*SCRN_WDTH - 0.5 * mSlide[i]->GetWidth());
		float y = (float)(0.5*SCRN_HGHT - 0.5 * mAmountOfSlides * mSlide[i]->GetHeight() + i * mSlide[i]->GetHeight()) - SPACE + i * 110;
		mSlide[i]->setPos(sf::Vector2f(x, y));
		mSlide[i]->setMinimum(sf::Vector2f(x - WIDTH/2, y));
		mSlide[i]->setMaximum(sf::Vector2f(x + WIDTH/2, y));
	}

		mScrollSound->putSoundInPlaylist(new MosSound(std::string("Sounds/Menu/menu_scroll_1.wav")));
		mScrollSound->putSoundInPlaylist(new MosSound(std::string("Sounds/Menu/menu_scroll_2.wav")));
		mSelectSound->putSoundInPlaylist(new MosSound(std::string("Sounds/Menu/menu_press_1.wav")));
		mSelectSound->putSoundInPlaylist(new MosSound(std::string("Sounds/Menu/menu_press_2.wav")));
	    // 0 is music 

		mSlide[0]->SetValue(100);
		mMosThread->setVolume(mSlide[0]->GetValue());
		// 1 is sound
		mSlide[1]->SetValue(100);
		mMosSound->setVolume(mSlide[1]->GetValue());
}

OptionMenuState:: ~OptionMenuState()
{
	delete[] mButton;
	delete[] mSlide;
	delete mSpriteBackground;
	delete mSpriteOptionMenuBackground;
	delete mSelectSound;
	delete mScrollSound;
	delete mMosThread;

}

void OptionMenuState::update(const sf::RenderTarget& window, MenuHandler& handler)
{
	// if the selected button is not the back button, move the slide
	if(mSeletedButton!=2)
	{
		mSelectedSlide=mSeletedButton;
	}
	
	// change the volumn selectedslide maybe need to be changed later 

	for(int j=0; j<mAmountofButtons;j++)
	{
			mButton[j]->setSelected(!(mSeletedButton==j));
	}

	MusicHandler::SOUNDVOLUME = mSlide[0]->GetValue();
	MosSound::SOUNDVOLUME = mSlide[1]->GetValue();
	
}

void OptionMenuState::render(sf::RenderTarget& window) const 
{
	window.Draw(*mSpriteBackground);
	mSpriteOptionMenuBackground->SetPosition(0.74*window.GetWidth()-0.5*mSpriteOptionMenuBackground->GetSize().x,0.5*window.GetHeight()-0.5*mSpriteOptionMenuBackground->GetSize().y);
	window.Draw(*mSpriteOptionMenuBackground);
	
	for(int j=0; j<mAmountofButtons;j++)
	{
		mButton[j]->setPosition((float)(0.75*window.GetWidth()-0.25*mButton[j]->getSizeWidth()+38),(float)(0.365*window.GetHeight()+j*147)); 
			// move the back button
		mButton[2]->moveButton(sf::Vector2f(8.0f,8.0f));
		mButton[j]->render(window);
	}


	for(int i=0; i<mAmountOfSlides; i++)
	{
		//render  slides
		mSlide[i]->render(window); 		
	}
}


void OptionMenuState::statePushed()
{}

void OptionMenuState::statePopped()
{}

void OptionMenuState::buttonPressed(MenuHandler& handler, Gamepad* gamepad, int mask)
{
	switch(mask)
	{
	case Gamepad::DPAD_DOWN:
		mSeletedButton++;
		mSeletedButton%=mAmountofButtons;
		mSelectSound->playSound();
		break;
	case Gamepad::DPAD_UP:	
		mSeletedButton--;
		if(mSeletedButton<0)
			mSeletedButton += mAmountofButtons;
		mSelectSound->playSound();
		break;			
		
	case Gamepad::DPAD_LEFT:	
		// if no button is chosen, slide can not be moved
		if(mSelectedSlide>=0)
		{
			//mSlide[mSelectedSlide]->move( sf::Vector2f(-4.0f, 0) );	
			mSlide[mSelectedSlide]->Move(-4);
			mScrollSound->playSound();
		}
		if(mSelectedSlide==0)
		{
			// music
			MusicHandler::SOUNDVOLUME -=4.0f;	
		}
		if(mSelectedSlide==1)
		{
			// sound
			MosSound::SOUNDVOLUME-=4.0f;
		}
		break;
	case Gamepad::DPAD_RIGHT:
		// if no button is chosen, slide can not be moved
		if(mSelectedSlide>=0)
		{	
			//mSlide[mSelectedSlide]->move( sf::Vector2f(+4.0f, 0) );
			mSlide[mSelectedSlide]->Move(+4);
			mScrollSound->playSound();			
		}
		if(mSelectedSlide==0)
		{
			// music
			MusicHandler::SOUNDVOLUME +=4.0f;	
		}

		if(mSelectedSlide==1)
		{
			// sound
			MosSound::SOUNDVOLUME+=4.0f;
		}
		break;
	case Gamepad::A:
	case Gamepad::B:
	case Gamepad::BACK:
		handler.popState();
		break;
	}

}

void OptionMenuState::buttonReleased(MenuHandler& handler, Gamepad* gamepad, int mask)
{
}

