#include "window.hpp"
using namespace sf;

int main() {
	Window_Main win;
	while (true) {
		if (win.get_event()) break;
		win.update();
		win.draw_all();
		win.display();
	}
	return 0;
}
