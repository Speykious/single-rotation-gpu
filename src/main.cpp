#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <SFML/Graphics.hpp>

#include <game.hpp>

using namespace std;
using namespace sf;

#define FRAMERATE 60

int main()
{
	RenderWindow window(VideoMode(610, 660), "Single Rotation - Cellular Automata with CUDA/SFML", Style::Titlebar | Style::Close);
	//window.setFramerateLimit(FRAMERATE);
	Clock mainClock;
    Color background(50, 60, 50, 255);

    game::load(&window);
	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed) {
				game::exit();
				window.close();
			}

			if (event.type == Event::KeyReleased) {
				switch (event.key.code) {
					case 36: window.close(); break;		// code 36 for Escape
				}
				// cout << event.key.code << endl;
			}
			
			// catch the resize events
			if (event.type == Event::Resized)
			{
				// update the view to the new size of the window
				FloatRect visibleArea(0, 0, event.size.width, event.size.height);
				window.setView(View(visibleArea));
			}
		}
		window.clear(background);
        game::update(mainClock.restart().asMicroseconds()*1e-6);
        game::draw(&window);
		window.display();
	}

	return 0;
}