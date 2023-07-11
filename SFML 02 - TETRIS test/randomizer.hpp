#pragma once
#include "all_in_one.hpp"
using namespace sf;

class Randomizer_Base {
public:
	std::deque<char> nxt;
	Randomizer_Base() {
		while (nxt.size()) nxt.pop_back();
	}
	void more(int id) {
		while (nxt.size() <= id) {
			nxt.push_back('T');
		}
	}
	char operator[](int id) {
		if (id >= nxt.size()) more(id);
		return nxt[id];
	}
	char get() {
		char ans = operator[](0);
		nxt.pop_front();
		return ans;
	}
};

class Seven_Bag_Randomizer : public Randomizer_Base {
public:
	Seven_Bag_Randomizer() : Randomizer_Base() {
	}
	void more(int id) {
		while (nxt.size() <= id) {
			std::vector<char> v = { 'I', 'J', 'L', 'O', 'S', 'T', 'Z' };
			std::shuffle(v.begin(), v.end(), std::default_random_engine(time(nullptr)));
			for (auto& i : v) nxt.push_back(i);
		}
	}
	char operator[](int id) {
		if (id >= nxt.size()) more(id);
		return nxt[id];
	}
	char get() {
		char ans = operator[](0);
		nxt.pop_front();
		return ans;
	}
};

