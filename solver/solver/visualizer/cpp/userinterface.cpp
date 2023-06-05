# include "visualizer/userinterface.hpp"



// コンストラクタ
UserInterface::UserInterface() {
	set_window_size(Size(1200, 900));		// 初期のウィンドウサイズ設定
	Scene::SetBackground(Palette::Green);		// 背景を緑にする
}



// http_controllerオブジェクトをセットする
void UserInterface::set_http_controller(HttpController& http_controller_) {
	http_controller = &http_controller_;
}



// ウィンドウサイズを変更する
void UserInterface::set_window_size(Size size) {
	Scene::Resize(size);		// シーンサイズ変更
	Window::Resize(size);		// ウィンドウサイズ変更
}



// フィールドやボタン等を描画する
void UserInterface::draw(Board& board) {
	// ボタンを描画する
	Visualizer::Clicked_Button clicked_button;
	clicked_button = buttons.draw_buttons();
	do_button_action(board, clicked_button);	// 押されたボタンに対応した処理をする
	// ゲームフィールドを描画する
	bool select_moved = game_field.draw_field(board);
	if (select_moved) send_actions(board);
	// パネルを描画する
	my_team_panel.draw_panel(*(board.my_team));
	op_team_panel.draw_panel(*(board.op_team));
	bool need_update = turn_panel.draw_panel(board);
	if (need_update) update_state(board);
}



// 押されたボタンに対応した処理をする
void UserInterface::do_button_action(Board& board, Visualizer::Clicked_Button clicked_button) {
	// 試合状態取得ボタンが押されたとき
	if (clicked_button == Visualizer::Update) {
		update_state(board);
	}
	// 行動送信ボタンが押されたとき
	if (clicked_button == Visualizer::Send) {
		send_actions(board);
	}
	// 色反転ボタンが押されたとき
	if (clicked_button == Visualizer::Swap) {
		int my_team_color = board.my_team->color;
		if (my_team_color == Procon::Red) board.set_my_team_color(Procon::Blue);
		else board.set_my_team_color(Procon::Red);
		update_state(board);
	}
}



// 行動情報をサーバに送信する
void UserInterface::send_actions(Board& board) {
	std::string file_path = board.write_actions();
	http_controller->post_actions(file_path);
}



// 「盤面状態」と「GUIの表示位置」を更新する
void UserInterface::update_state(Board& board) {
	// 盤面状態更新
	std::string file_path = http_controller->get_state();
	board.set_state(file_path);
	game_field.update_state(board, *(board.my_team), *(board.op_team));
	// パネルの位置変更
	int panels_x = buttons.icons_pos.x + game_field.tile_size * board.width + margin_x;
	int last_y = buttons.icon_size;
	last_y = my_team_panel.set_pos(Point(panels_x, last_y));
	last_y = op_team_panel.set_pos(Point(panels_x, last_y + margin_y));
	last_y = turn_panel.set_pos(Point(panels_x, last_y + margin_y));
	// ボタンの表示位置変更
	last_y = buttons.set_buttons_pos(Point(panels_x, last_y + margin_y + 50));
	// ウィンドウサイズを変更する
	int field_height = game_field.tile_size * board.height;
	int window_width = panels_x + 300 + margin_x;
	int window_height = Max(last_y, buttons.icon_size + field_height) + 40;
	set_window_size(Size(window_width, window_height));
}