//Irene Andersson +ccecilia Z
#ifndef SLIDE_INCLUDED
#define SLIDE_INCLUDED

namespace sf 
{ 
	class RenderTarget;
	class Sprite;
	class Image;
	class Sound;
	template<typename T>
	class Vector2;

	typedef Vector2<float> Vector2f;
}

class Slide
{
public:
	//minimum can´t be higher than maximum
	//initializing: the pointer points at the middle of minimum and maximum
	Slide(const char* texturepath);
	~Slide();
	
	//set the minimum value
	void setMinimum(sf::Vector2f& );

	//set the maximum value
	void setMaximum(sf::Vector2f& );
	

	//get the minimum value
	sf::Vector2f getMinimum();

	//get the maximum value
	sf::Vector2f getMaximum();
	

	sf::Vector2f getPos();
	void setPos(sf::Vector2f& position);

	void move(sf::Vector2f& position);
	// render backslide and frontslide
	
	void render(sf::RenderTarget& rWnd);

	float GetWidth();
	float GetHeight();

	void Move(float amount);
	void SetValue(float val);
	float GetValue();

	//faultmessage, used with throw command
	/*class Error {};*/
private:
	//Data 
	float Value;
	void Update();

	sf::Sprite* mSlide;
	sf::Vector2f* maximumPos;
	sf::Vector2f* minimumPos;
};

#endif