# pragma once
# include <Siv3D.hpp>	// OpenSiv3D v0.4.3

# include "proconsettings.hpp"

# include "game/httpcontroller.hpp"
# include "game/team.hpp"
# include "game/board.hpp"

# include "visualizer/userinterface.hpp"



// Main関数
void Main() {
	// 必要なオブジェクト作成
	UserInterface ui;
	Team my_team = Team();
	Team op_team = Team();
	Board board = Board(my_team, op_team);
	HttpController http_controller;
	Console << U"created object!";

	// jsonファイルのパス
	std::string file_path;

	// 試合事前情報取得
	file_path = http_controller.get_info();
	board.set_info(file_path);
	http_controller.match_id = board.match_id;

	// 試合状態取得
	file_path = http_controller.get_state();
	board.set_state(file_path);

	// 行動情報送信
	file_path = board.write_actions();
	http_controller.post_actions(file_path);

	// 「盤面状態」と「GUIの表示位置」を更新する
	ui.set_http_controller(http_controller);
	ui.update_state(board);

	// 描画する
	while (System::Update()) {
		ui.draw(board);
	}
}

