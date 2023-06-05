# pragma once
# include <Siv3D.hpp>	// OpenSiv3D v0.4.3

# include "game/board.hpp"



// チームパネル
class TeamPanel {
private:
	Font font = Font(20);					// パネルに表示するフォントを作成
	Rect rect = Rect(750, 20, 300, 150);	// パネル作成

public:
	void draw_panel(Team& team);			// チームパネルを表示する
	int set_pos(Point pos);					// チームパネルの表示位置と大きさ変更
};



// ターンパネル
class TurnPanel {
private:
	Font font = Font(20);					// 大きさ15のフォントを作成
	Rect rect = Rect(750, 20, 300, 200);	// 初期位置

public:
	bool draw_panel(Board& board);			// ターンパネルを表示する
	int set_pos(Point pos);					// ターンパネルの表示位置変更
};