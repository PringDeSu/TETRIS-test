#pragma once
#include "all_in_one.hpp"
using namespace sf;

typedef std::vector<Vector2i> vV;
typedef std::vector<vV> vvV;
typedef std::vector<vvV> vvvV;

// ********
// *      * ^
// *  de  * |
// *      * |
// ********

class Kick_Table {
public:
	vvvV normal, special;
	Kick_Table(std::string s) : normal(4, vvV(4, vV())), special(4, vvV(4, vV())) {
		int n;
		std::ifstream fin(s);
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (i == j) continue;
				fin >> n;
				normal[i][j].resize(n);
				for (auto& k : normal[i][j]) {
					fin >> k.x >> k.y;
					k.y *= -1;
				}
			}
		}
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (i == j) continue;
				fin >> n;
				special[i][j].resize(n);
				for (auto& k : special[i][j]) {
					fin >> k.x >> k.y;
					k.y *= -1;
				}
			}
		}
	}
};
