
#ifndef MAP_INKLUDED
#define MAP_INKLUDED

#include<vector>
//cecilia Z

namespace sf
{
	class RenderWindow;
	class Sprite;
}


class Map
{
public:
	Map(std::string& file);
	
	~Map();

	struct Hill
	{
		int mX;
		int mY;
		int mR;
	};

	void renderBackground(sf::RenderWindow& window) const;
	void renderForeground(sf::RenderWindow& window) const;
	void loadCollisionMap(std::string &file);
	void loadHillMap(std::string& file) ;
	typedef std::vector<int*> VectorCollision;
	VectorCollision mCollision;	
private:	
	std::string& mapName;
	sf::Sprite* mBackground;
	sf::Sprite* mForeground;
	typedef std::vector<Hill*>VectorHills;	
	VectorHills mHills;
};

#endif