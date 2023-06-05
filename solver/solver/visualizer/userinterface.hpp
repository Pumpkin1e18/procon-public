# pragma once
# include <Siv3D.hpp>	// OpenSiv3D v0.4.3

# include "game/board.hpp"
# include "game/httpcontroller.hpp"

# include "visualizer/settings.hpp"
# include "visualizer/field.hpp"
# include "visualizer/panel.hpp"
# include "visualizer/buttons.hpp"


// UI
class UserInterface {
private:
	// マージン
	static constexpr int margin_x = 40;		// オブジェクト間の横の間隔
	static constexpr int margin_y = 20;		// オブジェクト間の縦の間隔

	// 管理するグラフィック関連のオブジェクト
	GameField game_field;
	TeamPanel my_team_panel, op_team_panel;
	TurnPanel turn_panel;
	Buttons buttons;

	// サーバと通信するためのオブジェクト
	HttpController* http_controller;


public:
	// メンバ関数
	UserInterface();																// コンストラクタ
	void set_http_controller(HttpController& http_controller_);						// http_controllerオブジェクトをセットする
	void set_window_size(Size size);												// ウィンドウサイズを変更する
	void draw(Board& board);														// フィールドやボタン等を描画する
	void do_button_action(Board& board, Visualizer::Clicked_Button clicked_button);	// 押されたボタンに対応した処理をする
	void send_actions(Board& board);												// 行動情報をサーバに送信する
	void update_state(Board& board);												// 「盤面状態」と「GUIの表示位置」を更新する
};