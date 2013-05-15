#include "MapState.h"
#include "Map.h"
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"

using namespace sf;

MapState* MapState::mMapState=0;

MapState* MapState::getInstance()
{
	if(mMapState==0)
	{
		mMapState= new MapState();
	}

	return mMapState;
}

MapState::MapState()
{
	mMap= new Map(std::string("batman"));
}

MapState::~MapState()
{
	delete mMap;
}


void MapState::update(const sf::RenderWindow& window)
{


}

void MapState::render( sf::RenderWindow& window) const
{
	window.Clear();
	mMap->renderBackgroundPic(window);
	mMap->renderHillPic(window);
	window.Display();
	
}