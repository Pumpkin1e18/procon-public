# pragma once
# include <Siv3D.hpp>	// OpenSiv3D v0.4.3
# include <cmath>		// abs, atan2（二次元のアークタンジェント）

# include "proconsettings.hpp"
# include "settings.hpp"

# include "game/httpcontroller.hpp"
# include "game/board.hpp"



// タイル
// ----------------------------------------------------------------------------------------------------------
class Tile {
private:
	// タイルの実際に表示される色をRGBで指定する
	ColorF empty_color = Palette::White;		// タイルがどっちのチームのものでもないときの色
	ColorF red_tile_color = Palette::Pink;		// タイルが赤チームのもののときの色
	ColorF red_player_color = ColorF(1, 0.3, 0.3);		// タイルに赤チームのプレイヤーがいるときの色
	ColorF blue_tile_color = Palette::Skyblue;	// タイルが青チームのもののときの色
	ColorF blue_player_color = ColorF(0.3, 0.3, 1);	// タイルに青チームのプレイヤーがいるときの色

	// 変数「state」から実際に表示される色（RGB）に変換する「変換テーブル」の宣言
	HashTable<int, ColorF> red_table;		// 自分のチームが赤色のときの変換テーブル
	HashTable<int, ColorF> blue_table;		// 自分のチームが青色のときの変換テーブル


public:
	// タイルに関する変数
	int point = -16;		// タイルポイント
	int state = Visualizer::Empty;			// タイルの状態（どちらかのチームのタイルか、プレイヤーがいるか等）
	ColorF color = Palette::White;			// タイルの色（RGBで指定）

	// このタイルにエージェントがいる場合の変数
	Agent* agent;							// エージェントオブジェクトのポインタ
	bool will_stay = true;					// 次のステップでstayするかどうか
	double will_move_radian = 0;			// エージェントが次のステップで動く方向（degreeで指定）

	// メンバ関数
	Tile();									// コンストラクタ
	void set_table();						// タイルの状態から色に変換する変換テーブルを作成する
	void update_color(Board& board);		// タイルの色を更新する
	void set_move_vec(int dx, int dy);		// 次にエージェントが動く方向を計算する
};
// ----------------------------------------------------------------------------------------------------------





// ゲームフィールド
// ----------------------------------------------------------------------------------------------------------
class GameField {
private:
	// 「フォント」と「テクスチャ（画像）」の変数
	Font font = Font(int(0.4 * tile_size));
	const Texture cat_move = Texture(U"image/cat_move.png");
	const Texture cat_stay = Texture(U"image/cat_stay.png");

	// 「フィールドの大きさ」と「タイル」の変数
	int height = 20;
	int width = 20;
	Size grid_size = Size(width, height);
	Grid<Tile> tiles = Grid<Tile>(grid_size, Tile());

	// プレイヤー選択に関する変数
	bool is_selected = false;
	Point selected_pos;
	Agent* selected_agent;


public:
	// フィールドの位置や大きさに関する変数
	Point field_pos = Point(40, 50);	// フィールドの左上の位置
	int tile_size = 35;					// タイルの大きさ

	// メンバ関数
	bool draw_field(Board& board);									// フィールドを描画する
	void update_state(Board& board, Team& my_team, Team& op_team);	// ゲームフィールドの状態を更新する
	void set_pos(Point field_pos_);									// フィールドの表示位置を変更する
};
// ----------------------------------------------------------------------------------------------------------