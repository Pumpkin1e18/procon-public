# pragma once
# include <Siv3D.hpp>	// OpenSiv3D v0.4.3

# include "game/board.hpp"

# include "visualizer/settings.hpp"



// ボタンクラス
class Buttons {
private:
	// アイコンボタンの作成（undo, redo, sync）
	const Texture iconUndo = Texture(Icon(0xf060, 28));
	const Texture iconRedo = Texture(Icon(0xf061, 28));
	const Texture iconSync = Texture(Icon(0xf021, 28));
	static constexpr int icons_x = 40;
	static constexpr int icons_y = 0;

	// クリックボタンの作成
	String update_button_text = U"試合状態を取得";
	String send_button_text = U"行動情報を送信";
	String swap_button_text = U"自分と相手の色を交換する";
	Point buttons_pos = Point(750, 700);


public:
	// アイコンの位置やサイズに関する変数
	static constexpr Point icons_pos = Point(icons_x, icons_y);
	static constexpr int icon_size = 50;
	static constexpr int button_size = 50;

	// メンバ関数
	Visualizer::Clicked_Button draw_buttons();		// ボタンの描画
	int set_buttons_pos(Point buttons_pos_);		// クリックボタンの表示位置を変更する
};