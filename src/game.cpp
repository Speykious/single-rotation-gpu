#include <iostream>
#include <string>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <cudasks.cuh>

using namespace sf;
using namespace std;

namespace game {
	Vector2u win_size;

	Font reg_font;
	Font bol_font;
	Text title_text;
	Text frame_text;
	Text device_text;

	Color state0 = Color(30, 30, 30, 255);
	Color state1 = Color(200, 200, 100, 255);
	vector<Vertex> cellVertices;

	void initVertices() {
		cellVertices = vector<Vertex>(4*LEN*LEN, Vertex(Vector2f(0, 0), state0, Vector2f(0, 0)));
		int x = 5, y = 55;
		for (int j = 0; j<LEN; j++) {
			for (int i = 0; i<LEN; i++) {
				int idx4 = 4*LEN*j+4*i;
				cellVertices[idx4  ].position.x = x+i*CSIZE;
				cellVertices[idx4  ].position.y = y+j*CSIZE;

				cellVertices[idx4+1].position.x = x+i*CSIZE;
				cellVertices[idx4+1].position.y = y+(j+1)*CSIZE;

				cellVertices[idx4+2].position.x = x+(i+1)*CSIZE;
				cellVertices[idx4+2].position.y = y+(j+1)*CSIZE;

				cellVertices[idx4+3].position.x = x+(i+1)*CSIZE;
				cellVertices[idx4+3].position.y = y+j*CSIZE;
			}
		}
	}

	void setVertices(int* ar) {
		for (int j = 0; j<LEN; j++) {
			for (int i = 0; i<LEN; i++) {
				int idx = LEN*j+i, idx4 = 4*LEN*j+4*i;
				auto state = (ar[idx]>0)? state1 : state0;
				cellVertices[idx4].color   = state;
				cellVertices[idx4+1].color = state;
				cellVertices[idx4+2].color = state;
				cellVertices[idx4+3].color = state;
			}
		}
	}

	void load(RenderWindow *window) {
		win_size = window->getSize();
		
		if (!reg_font.loadFromFile("ubuntumono-r.ttf"))
			cout << "The font 'reg_font' hasn't been initialized correctly." << endl;
		else cout << "The font 'reg_font' has been initialized succesfully!" << endl;
		if (!bol_font.loadFromFile("ubuntumono-b.ttf"))
			cout << "The font 'bol_font' hasn't been initialized correctly." << endl;
		else cout << "The font 'bol_font' has been initialized succesfully!" << endl;
		
		initVertices();
		initArrays();

		setVertices(get_h_in());

		title_text.setFont(bol_font);
		title_text.setCharacterSize(18);
		title_text.setFillColor(Color::White);
		title_text.setPosition(230, 15);
		title_text.setString("Program made by Speykious");

		frame_text.setFont(reg_font);
		frame_text.setCharacterSize(14);
		frame_text.setFillColor(Color::White);
		frame_text.setPosition(5, 5);

		device_text.setFont(reg_font);
		device_text.setCharacterSize(14);
		device_text.setFillColor(state1);
		device_text.setPosition(530, 35);
		device_text.setString("On the GPU");
	}

	double fps;
	long frame = 0;
	double rtime = 0, time = 0, refresh = 0.5;
	void update(double dt) {
		frame++;
		rtime += dt;
		time += dt;
		if (rtime>refresh) {
			fps = 1.0/dt;
			rtime -= refresh;
		}

		calculateLife(1);
		setVertices(get_h_in());

		frame_text.setString("Framerate: "+to_string(fps)+" Hz"+"\n"
							+"Time:      "+to_string(time)+" s"+"\n"
							+"Frame:     "+to_string(frame));
	}

	void draw(RenderWindow *window) {
		window->draw(title_text);
		window->draw(frame_text);
		window->draw(device_text);
		window->draw(&*cellVertices.begin(), cellVertices.size(), Quads);
	}

	void exit() {
		freeArrays();
	}
}