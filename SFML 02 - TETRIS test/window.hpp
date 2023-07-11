#pragma once
#include "all_in_one.hpp"
#include "game_board.hpp"
#include "key_handler.hpp"
#include "setting.hpp"
using namespace sf;

//const Color ACTIVATE_COLOR = Color::Magenta, GARBAGE_COLOR = Color(39, 39, 39);
const int MARGIN_LEFT = 50, MARGIN_RIGHT = 50, MARGIN_UP = 30, MARGIN_DOWN = 50;
const int BORDER_WIDTH = 5;
const int WIDTH = 30;

class Rectangle_EX : public RectangleShape {
public:
	Rectangle_EX(int x, int y, int dx, int dy, Color color) : RectangleShape(Vector2f(dx, dy)) {
		setPosition(x, y);
		setFillColor(color);
	}
};

class Window_Base : public RenderWindow {
public:
	Game_Board game;
	Window_Base() : RenderWindow(VideoMode((TABLE_X + 10) * WIDTH + MARGIN_LEFT + MARGIN_RIGHT + BORDER_WIDTH * 2, TABLE_Y * WIDTH + MARGIN_UP + MARGIN_DOWN + BORDER_WIDTH), "TETRIS"), game() {
		setFramerateLimit(60);
		setKeyRepeatEnabled(false);
	}
	void set_left_up(Vector2i p) {
		View v = getDefaultView();
		v.move(Vector2f(-p.x, -p.y));
		setView(v);
	}
	void draw_piece(Piece p) {
		int dy = 0;
		if (p.c == 'O') dy--;
		for (int i = 0; i < 4; i++) {
			Rectangle_EX rect(WIDTH * p.dp[i].x, WIDTH * (p.dp[i].y + dy), WIDTH, WIDTH, p.color);
			draw(rect);
		}
	}
	void draw_piece_EX(Piece_EX& p) {
		for (int i = 0; i < 4; i++) {
			Rectangle_EX rect(WIDTH * (p.p.x + p.dp[i].x - 1), WIDTH * (p.p.y + p.dp[i].y - 1), WIDTH, WIDTH, p.color);
			draw(rect);
		}
	}
	void draw_game_board() {
		set_left_up(Vector2i(MARGIN_LEFT + WIDTH * 5, MARGIN_UP + WIDTH * TABLE_OUT));
		Rectangle_EX rect(0, 0, BORDER_WIDTH, (TABLE_Y - TABLE_OUT) * WIDTH, Color::White);
		Rectangle_EX rect2(BORDER_WIDTH + TABLE_X * WIDTH, 0, BORDER_WIDTH, (TABLE_Y - TABLE_OUT) * WIDTH, Color::White);
		Rectangle_EX rect3(0, WIDTH * (TABLE_Y - TABLE_OUT), TABLE_X * WIDTH + BORDER_WIDTH * 2, BORDER_WIDTH, Color::White);
		draw(rect);
		draw(rect2);
		draw(rect3);
		set_left_up(Vector2i(MARGIN_LEFT + WIDTH * 5 + BORDER_WIDTH, MARGIN_UP));
		for (int i = 1; i <= TABLE_X; i++) {
			for (int j = 1; j <= TABLE_Y; j++) {
				//if (game.table.clear(Vector2i(i, j))) continue;
				Rectangle_EX rect(WIDTH * (i - 1), WIDTH * (j - 1), WIDTH, WIDTH, game.table.occ[i][j]);
				draw(rect);
			}
		}
		//Rectangle_EX rect(WIDTH * (game.piece.x - 1), WIDTH * (game.piece.y - 1), WIDTH, WIDTH, ACTIVATE_COLOR);
		//draw(rect);
		draw_piece_EX(game.piece);
		Piece_EX rule;
		game.ghost(rule);
		rule.color.a = 63;
		draw_piece_EX(rule);
		set_left_up(Vector2i(MARGIN_LEFT, MARGIN_UP + WIDTH * TABLE_OUT));
		if (game.hold != 'P') draw_piece(Piece(game.hold));
		for (int i = 0; i < 5; i++) {
			set_left_up(Vector2i(MARGIN_LEFT + WIDTH * 16 + BORDER_WIDTH * 2, MARGIN_UP + (TABLE_OUT + 3 * i) * WIDTH));
			draw_piece(Piece(game.rnd[i]));
		}
	}
	void draw_all() {
		clear();
		draw_game_board();
	}
};

class Window_Main : public Window_Base {
public:
	Key_Handler keyboard;
	Setting setting;
	bool close_next_frame;
	Window_Main() : Window_Base(), keyboard(), setting(), close_next_frame(false) {
		game.set_ARR(setting.ARR);
	}
	bool get_event() {
		if (close_next_frame) {
			close();
			return true;
		}
		Event e;
		while (pollEvent(e)) {
			if (e.type == Event::EventType::Closed) {
				close();
				return true;
			}
			if (e.type == Event::EventType::KeyPressed) {
				keyboard.press(e.key.code);
			}
			else if (e.type == Event::EventType::KeyReleased) {
				keyboard.release(e.key.code);
			}
		}
		return 0;
	}
	void update() {
		keyboard.update();
		bool ml = keyboard.pulsed(setting.key_left), mr = keyboard.pulsed(setting.key_right);
		bool mu = keyboard.pulsed(setting.key_up), md = keyboard.pulsed(setting.key_down);
		bool set = keyboard.pulsed(setting.key_set), cls = keyboard.pulsed(setting.key_close);
		if (keyboard.hold_frames(setting.key_reset) == 20) game = Game_Board();
		if (ml) game.move(Vector2i(-1, 0));
		if (mr) game.move(Vector2i(1, 0));
		if (mu) game.move(Vector2i(0, -1));
		if (md) game.move(Vector2i(0, 1));
		if (set) game.set();
		if (cls) close_next_frame = true;
		int dr = (keyboard.pulsed(setting.key_cw) * 1 + keyboard.pulsed(setting.key_half) * 2 + keyboard.pulsed(setting.key_ccw) * 3) % 4;
		game.rotate(dr);
		int hl = keyboard.hold_frames(setting.key_left), hr = keyboard.hold_frames(setting.key_right);
		bool sim_stop = false;
		if (hl >= setting.DAS && hr >= 0 && hr < setting.DAS) sim_stop = true;
		if (hr >= setting.DAS && hl >= 0 && hl < setting.DAS) sim_stop = true;
		if (hl >= setting.DAS) {
			if (hr >= setting.DAS) {
				if (hl >= hr) game.ARR_countdown(1, sim_stop);
				else game.ARR_countdown(0, sim_stop);
			}
			else {
				game.ARR_countdown(0, sim_stop);
			}
		}
		else {
			if (hr >= setting.DAS) {
				game.ARR_countdown(1, sim_stop);
			}
			else {
				game.ARR_countdown(2, sim_stop);
			}
		}
		if (keyboard.hold_frames(setting.key_down) != -1) game.gravity_drop(setting.SDF);
		else game.gravity_drop(1);
		if (keyboard.pulsed(setting.key_hold)) game.switch_hold();
	}
};

