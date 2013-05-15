#ifndef MAP_STATE_INCLUDED
#define MAP_STATE_INCLUDED

#include "State.h"
//cecilia Z
namespace sf{ class RenderWindow; }



class Map;
//struct MapDesc;

class MapState: public State
{
public:
	static MapState* getInstance();
	MapState();
	/*MapState(MapState& m){}*/
	~MapState();
	virtual void update(const sf::RenderWindow& window);
	virtual void render(  sf::RenderWindow& window) const ;
	
private:
	Map* mMap;
	static MapState* mMapState;
};



#endif