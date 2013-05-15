
#include "Map.h"
#include <SFML/Graphics.hpp>
#include "ResourceFactory.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include <exception>


//Cecilia Z
using namespace sf;
namespace
{
	const std::string MAPS_LOCATION = "Images/Maps/Ingame/";

	class MapNotFoundException : public std::runtime_error
	{
		public:
			MapNotFoundException(std::string& message):
			  std::runtime_error(message)
			{  
			}
	};

}

Map::Map(std::string& file)
	: mapName(file)
	, mBackground(new Sprite(*ResourceFactory::getInstance()->loadImage(MAPS_LOCATION + file + std::string("/background.png"))))
	, mForeground(new Sprite(*ResourceFactory::getInstance()->loadImage(MAPS_LOCATION + file + std::string("/foreground.png"))))
{		
	loadHillMap(MAPS_LOCATION + file);
	loadCollisionMap(MAPS_LOCATION + file);
	
	mBackground->SetPosition(0,0);
}


Map::~Map()
{
	delete mBackground;
	delete mForeground;
	for(int i=0; i<mCollision.size();i++)
	{	
		delete mCollision[i];
	}
	for(int i=0; i<mHills.size();i++)
	{
		delete mHills[i];
	}
}

void Map::renderBackground(sf::RenderWindow& window) const
{
	window.Draw(*mBackground);
}

void Map::renderForeground(sf::RenderWindow& window) const
{
	window.Draw(*mForeground);
}

void Map::loadCollisionMap(std::string& file) 
{

	std::string temp0=file+ std::string("/collision");
	std::ifstream str(temp0);
	if(!str.is_open())
	{
		throw MapNotFoundException(std::string("Collision file not found")+file);
	}
	int x;
	int y;
	str>>x;
	str>>y;

	for(int i=0; i < y; i++)
	{
		int* collision_X = new int[x];
		for(int j=0; j < x; j++)
		{
				
			if(str.eof())
			{
				throw MapNotFoundException(std::string("Error in Collision map")+file);	
			}

			str>>collision_X[j];
		}
		mCollision.push_back(collision_X);
	}

	str.close();
}

void Map::loadHillMap(std::string& file) 
{
	std::string temp = file+ std::string("/hills");
	std::ifstream str1(temp.c_str());
	while(!str1.eof())
	{
		Hill *h=new Hill();
		str1>>h->mX; // läser från filen 
		str1>>h->mY;
		str1>>h->mR;
		mHills.push_back(h);
	}
	str1.close();

}



	

