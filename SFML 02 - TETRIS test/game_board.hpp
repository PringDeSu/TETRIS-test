#pragma once
#include "all_in_one.hpp"
#include "kick_table.hpp"
#include "randomizer.hpp"
using namespace sf;

const int TABLE_X = 10, TABLE_Y = 23, TABLE_OUT = 3;

const bool TEST_BOOLEAN = false;

class Piece {
public:
	std::vector<Vector2i> dp;
	Color color;
	char c;
	Piece() : dp(), color(Color::Magenta), c('T') {
		dp = { Vector2i(1, 0), Vector2i(0, 1), Vector2i(1, 1), Vector2i(2, 1) };
	}
	Piece(char _c) : dp(), color(Color::Transparent), c(_c) {
		if (c == 'I') dp = { Vector2i(0, 1), Vector2i(1, 1), Vector2i(2, 1), Vector2i(3, 1) };
		else if (c == 'J') dp = { Vector2i(0, 0), Vector2i(0, 1), Vector2i(1, 1), Vector2i(2, 1) };
		else if (c == 'L') dp = { Vector2i(2, 0), Vector2i(0, 1), Vector2i(1, 1), Vector2i(2, 1) };
		else if (c == 'O') dp = { Vector2i(1, 1), Vector2i(2, 1), Vector2i(1, 2), Vector2i(2, 2) };
		else if (c == 'S') dp = { Vector2i(1, 0), Vector2i(2, 0), Vector2i(0, 1), Vector2i(1, 1) };
		else if (c == 'T') dp = { Vector2i(1, 0), Vector2i(0, 1), Vector2i(1, 1), Vector2i(2, 1) };
		else if (c == 'Z') dp = { Vector2i(0, 0), Vector2i(1, 0), Vector2i(1, 1), Vector2i(2, 1) };
		if (c == 'I') color = Color(0, 255, 255);
		else if (c == 'J') color = Color(0, 0, 255);
		else if (c == 'L') color = Color(255, 127, 0);
		else if (c == 'O') color = Color(255, 255, 0);
		else if (c == 'S') color = Color(0, 255, 0);
		else if (c == 'T') color = Color(255, 0, 255);
		else if (c == 'Z') color = Color(255, 0, 0);
	}
};

class Piece_EX : public Piece {
public:
	Vector2i p;
	int state;
	bool is_special;
	Piece_EX() : Piece(), p(TABLE_X / 2, 1), state(0), is_special(false) {
	}
	Piece_EX(char c) : Piece(c), p(4, 1), state(0), is_special(false) {
		if (c == 'I' || c == 'O') is_special = true;
		//std::cout << c << std::endl;
	}
	void force_move(Vector2i dep) {
		p += dep;
	}
	void force_rotate(int dr) {
		state += dr;
		if (state >= 4) state -= 4;
		for (auto& i : dp) {
			if (is_special) {
				if (dr == 1) i = Vector2i(3 - i.y, i.x);
				else if (dr == 2) i = Vector2i(3 - i.x, 3 - i.y);
				else if (dr == 3) i = Vector2i(i.y, 3 - i.x);
			}
			else {
				if (dr == 1) i = Vector2i(2 - i.y, i.x);
				else if (dr == 2) i = Vector2i(2 - i.x, 2 - i.y);
				else if (dr == 3) i = Vector2i(i.y, 2 - i.x);
			}
		}
	}
};

class Table {
public:
	std::vector<std::vector<Color>> occ;
	Table() : occ(TABLE_X + 2, std::vector<Color>(TABLE_Y + 2, Color::Transparent)) {
		for (int i = 0; i < TABLE_X + 2; i++) {
			occ[i][0] = Color::Black;
			occ[i][TABLE_Y + 1] = Color::Black;
		}
		for (int i = 0; i < TABLE_Y + 2; i++) {
			occ[0][i] = Color::Black;
			occ[TABLE_X + 1][i] = Color::Black;
		}
	}
	bool clear(Vector2i p) {
		if (p.x < 0 || p.x >= TABLE_X + 2) return false;
		if (p.y < 0 || p.y >= TABLE_Y + 2) return false;
		return (occ[p.x][p.y] == Color::Transparent);
	}
	void set(Vector2i p, Color color) {
		color.a = 191;
		occ[p.x][p.y] = color;
	}
	void check() {
		//std::cout << "PRING" << std::endl;
		std::vector<int> del;
		for (int i = TABLE_Y; i > 0; i--) {
			bool flag = true;
			for (int j = 1; j <= TABLE_X; j++) {
				if (occ[j][i] == Color::Transparent) {
					flag = false;
					break;
				}
			}
			if (flag) del.push_back(i);
		}
		del.push_back(0);
		int pv = 0, pd = TABLE_Y;
		for (int i = TABLE_Y; i > 0; i--) {
			if (del[pv] == i) {
				pv++;
				continue;
			}
			for (int j = 1; j <= TABLE_X; j++) occ[j][pd] = occ[j][i];
			pd--;
		}
		while (pd > 0) {
			for (int j = 1; j <= TABLE_X; j++) occ[j][pd] = Color::Transparent;
			pd--;
		}
	}
};

class Game_Board {
public:
	Table table;
	Piece_EX piece;
	Seven_Bag_Randomizer rnd;
	char hold;
	Game_Board() : table(), ARR(0), ARR_state(-1), ARR_count(0), kct("SRS_plus.kct"), holded(false), hold('P'), gravity_count(60) {
		rnd = Seven_Bag_Randomizer();
		//std::cout << rnd[0] << rnd[1] << rnd[2] << rnd[3] << rnd[4] << std::endl;
		piece = Piece_EX(rnd.get());
	}
	void set_ARR(int _ARR) {
		ARR = _ARR;
	}
	bool all_clear(Vector2i dep) {
		for (int i = 0; i < 4; i++) {
			if (!table.clear(piece.p + piece.dp[i] + dep)) return false;
		}
		return true;
	}
	void move(Vector2i dep, bool sim_stop = false) {
		if (sim_stop) return;
		//for (int i = 0; i < 4; i++) {
		//	if (!table.clear(piece.p + piece.dp[i] + dep)) return;
		//}
		if (!all_clear(dep)) return;
		piece.force_move(dep);
	}
	void move_to_end(Vector2i dep, bool sim_stop) {
		if (sim_stop) return;
		while (all_clear(dep)) {
			piece.force_move(dep);
		}
	}
	void ARR_countdown(int state, bool sim_stop) {
		if (state == ARR_state) ARR_count++;
		else {
			ARR_state = state;
			ARR_count = 0;
		}
		if (ARR_state == 2) return;
		if (ARR == 0) {
			move_to_end(Vector2i((ARR_state == 1 ? 1 : -1), 0), sim_stop);
		}
		else {
			if (ARR_count == ARR) ARR_count = 0;
			if (ARR_count == 0) move(Vector2i((ARR_state == 1 ? 1 : -1), 0), sim_stop);
		}
	}
	void set() {
		while (all_clear(Vector2i(0, 1))) piece.force_move(Vector2i(0, 1));
		for (int i = 0; i < 4; i++) table.set(piece.p + piece.dp[i], piece.color);
		piece = Piece_EX(rnd.get());
		holded = false;
		table.check();
	}
	void rotate(int dr) {
		if (dr == 0) return;
		int pre = piece.state;
		piece.force_rotate(dr);
		int cur = piece.state;
		if (all_clear(Vector2i(0, 0))) return;
		vV& dp = (piece.is_special ? kct.special : kct.normal)[pre][cur];
		for (auto& i : dp) {
			if (all_clear(i)) {
				piece.force_move(i);
				return;
			}
		}
		piece.force_rotate(4 - dr);
	}
	void switch_hold() {
		if (holded) return;
		holded = true;
		if (hold == 'P') {
			hold = piece.c;
			piece = Piece_EX(rnd.get());
			return;
		}
		char tmp = hold;
		hold = piece.c;
		piece = Piece_EX(tmp);
	}
	void drop() {
		if (all_clear(Vector2i(0, 1))) {
			piece.force_move(Vector2i(0, 1));
		}
		else {
			set();
		}
		gravity_count = 60;
	}
	void gravity_drop(int cnt) {
		if (!all_clear(Vector2i(0, 1))) {
			gravity_count--;
			if (gravity_count == 0) {
				drop();
			}
			return;
		}
		if (cnt == 0) {
			while (all_clear(Vector2i(0, 1))) {
				piece.force_move(Vector2i(0, 1));
			}
			gravity_count = 60;
		}
		else {
			gravity_count -= cnt;
			if (gravity_count <= 0) {
				drop();
			}
		}
	}
	void ghost(Piece_EX& rule) {
		rule = piece;
		int x = 0;
		while (all_clear(Vector2i(0, x))) x++;
		rule.force_move(Vector2i(0, --x));
	}
private:
	int ARR, ARR_state, ARR_count;
	Kick_Table kct;
	bool holded;
	int gravity_count;
};

