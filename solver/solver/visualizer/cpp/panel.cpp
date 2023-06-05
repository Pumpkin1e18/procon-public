# pragma once
# include "visualizer/panel.hpp"


// チームパネル
// ----------------------------------------------------------------------------------------------------------
// チームパネルを表示する
void TeamPanel::draw_panel(Team& team) {
	// 表示内容作成
	String text = U"";
	text += (team.is_my_team ? U"自分チーム" : U"相手チーム");
	text += U"\nタイルポイント：" + Format(team.tile_point);
	text += U"\n領域ポイント：" + Format(team.area_point);
	text += U"\n探索方法：";
	text += (team.is_my_team ? U"人間" : U"貪欲法");
	// パネルの色指定
	ColorF color_background = (team.color == Procon::Red) ? Palette::Pink : Palette::Skyblue;
	ColorF color_frame = (team.color == Procon::Red) ? Palette::Red : Palette::Blue;
	// パネルを描画
	rect.draw(color_background);
	rect.drawFrame(5, 0, color_frame);
	font(text).draw(rect.stretched(-25, -15), Palette::Black);		// 左右 20px 空け、上下 15px 空ける
}


// チームパネルの表示位置と大きさ変更
int TeamPanel::set_pos(Point pos) {
	Size size = Size(300, 150);		// Size(x, y)
	rect = Rect(pos, size);
	return pos.y + size.y;
}
// ----------------------------------------------------------------------------------------------------------






// ターンパネル
// ----------------------------------------------------------------------------------------------------------
// ターンパネルを表示する
bool TurnPanel::draw_panel(Board& board) {
	// 時間に関数変数作成
	time_t now_time = std::time(nullptr) * 1000;
	time_t next_turn_time = board.started_unix_time;
	next_turn_time += (board.turn + 1) * (board.interval_millis + board.turn_millis);
	double time_to_next_turn = (next_turn_time - now_time) / 1000.0;
	// 表示する文字列作成
	String text = U"";
	text += U"現在のターン：" + Format(board.turn);
	text += U"\n次のターンまで：" + Format(int(time_to_next_turn)) + U"秒";
	// 描画
	rect.draw(Palette::White);
	rect.drawFrame(5, 0, Palette::Black);
	font(text).draw(rect.stretched(-25, -15), Palette::Black);
	return time_to_next_turn < 0;
}


// ターンパネルの表示位置変更
int TurnPanel::set_pos(Point pos) {
	Size size = Size(300, 90);
	rect = Rect(pos, size);
	return pos.y + size.y;
}
// ----------------------------------------------------------------------------------------------------------