//Irene Andersson+ Cecilia Z
#include "Slide.h"
#include <cassert>
#include "ResourceFactory.h"
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"

using namespace sf;


namespace
{
	const int NUMBER_OF_SPRITE_MOVEMENT = 8;

	class SlideShowUpError:public std::runtime_error
	{
	public:
		SlideShowUpError(std::string& message):std::runtime_error(message)
		{
		};
	};

}

Slide::Slide(const char* texturepath):mSlide(new Sprite(*ResourceFactory::getInstance()->loadImage(std::string(texturepath))))
,maximumPos(new sf::Vector2f (0.0f,0.0f)),	minimumPos(new sf::Vector2f (0.0f,0.0f))
{
	Value = 50.0f;
	mSlide->SetPosition(sf::Vector2f (0.0f,0.0f));
	 // if no pictue are loaded
	if(!mSlide)
	{	
		throw SlideShowUpError(std::string("Slide Image can not show up"));
	}
}

Slide::~Slide()
{
	delete mSlide;
}
	
void Slide::setMinimum(sf::Vector2f& minimum)
{
	*minimumPos = minimum;
}

void Slide::setMaximum(sf::Vector2f& maximum)
{
	*maximumPos = maximum;
}

sf::Vector2f Slide::getMinimum()
{
	return *minimumPos;
}

	//get the maximum value
sf::Vector2f Slide::getMaximum()
{
	return *maximumPos;
}

sf::Vector2f Slide::getPos()
{
	return mSlide->GetPosition();
}

void Slide::setPos(sf::Vector2f& position)
{
	mSlide->SetPosition(position);
}

void Slide::move(sf::Vector2f& position)
{
	// update sprite 
	mSlide->Move(position);

	if(mSlide->GetPosition().x < getMinimum().x)
		mSlide->SetPosition(sf::Vector2f(getMinimum().x,mSlide->GetPosition().y));

	if(mSlide->GetPosition().x > getMaximum().x)
		mSlide->SetPosition(sf::Vector2f(getMaximum().x,mSlide->GetPosition().y));
}

void Slide::Move(float amount)
{
	Value += amount;
	Update();
}

void Slide::SetValue(float val)
{
	Value = val;
	Update();
}

float Slide::GetValue()
{ return Value; }

void Slide::Update()
{
	if(Value < 0)
		Value = 0;
	else if(Value > 100)
		Value = 100;

	mSlide->SetPosition( *minimumPos + (*maximumPos - *minimumPos) * (Value/100.0f) );
}

float Slide::GetWidth()
{
	return mSlide->GetSize().x;
}

float Slide::GetHeight()
{
	return mSlide->GetSize().y;
}

void Slide::render(sf::RenderTarget& rWnd)
{

	rWnd.Draw(*mSlide);
}