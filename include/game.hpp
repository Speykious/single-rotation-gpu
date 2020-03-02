#include <iostream>
#include <string>
#include <sstream>
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

namespace game {
	void load(RenderWindow *);
	void update(double);
	void draw(RenderWindow *);
	void exit();
}