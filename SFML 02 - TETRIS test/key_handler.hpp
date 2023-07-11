#pragma once
#include "all_in_one.hpp"
using namespace sf;

class Key_Handler {
public:
	Key_Handler() : state(Keyboard::KeyCount, -1) {
	}
	void press(int id) {
		state[id] = 0;
	}
	void release(int id) {
		state[id] = -1;
	}
	void update() {
		for (auto& i : state) {
			if (i == -1) continue;
			i++;
		}
	}
	bool pulsed(int id) {
		return (state[id] == 1);
	}
	int hold_frames(int id) {
		return state[id];
	}
private:
	std::vector<int> state;
};

