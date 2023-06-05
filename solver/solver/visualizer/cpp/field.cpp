# include "visualizer/field.hpp"


// タイル
// ----------------------------------------------------------------------------------------------------------
// コンストラクタ
Tile::Tile() { set_table(); }


// タイルの状態から色に変換する変換テーブルを作成する
void Tile::set_table() {
	// 自分のチームが赤色のときの変換テーブルの作成
	red_table[Visualizer::Empty] = empty_color;
	red_table[Visualizer::Mytile] = red_tile_color;
	red_table[Visualizer::Myplayer] = red_player_color;
	red_table[Visualizer::Optile] = blue_tile_color;
	red_table[Visualizer::Opplayer] = blue_player_color;
	// 自分のチームが青色のときの変換テーブルの作成
	blue_table[Visualizer::Empty] = empty_color;
	blue_table[Visualizer::Mytile] = blue_tile_color;
	blue_table[Visualizer::Myplayer] = blue_player_color;
	blue_table[Visualizer::Optile] = red_tile_color;
	blue_table[Visualizer::Opplayer] = red_player_color;
}


// タイルの色を更新する
void Tile::update_color(Board& board) {
	if (red_table.size() == 0)set_table();
	color = (board.my_team->color == Procon::Red) ? red_table[state] : blue_table[state];
}


// 次にエージェントが動く方向を計算する
void Tile::set_move_vec(int dx, int dy) {
	will_move_radian = std::atan2(dx, -dy);		// atan2(float y, float x)
	will_stay = (dx == 0 and dy == 0) ? true : false;
}
// ----------------------------------------------------------------------------------------------------------









// ゲームフィールド
// ----------------------------------------------------------------------------------------------------------
// フィールドを描画する
bool GameField::draw_field(Board& board) {
	bool select_moved = false;
	for (auto p : step(grid_size)) {
		// タイルの作成
		const Rect tile(field_pos + p * tile_size, tile_size);

		// タイルのクリック判定
		// プレイヤーを選択した状態でタイルをクリックしたとき
		if (is_selected and (tile.leftClicked() or tile.rightClicked())) {
			is_selected = false;	// プレイヤーの選択を解除する
			// 処理をする前に前計算する
			Point now_pos = p;
			Point past_pos = selected_pos;
			int dx = now_pos.x - past_pos.x;
			int dy = now_pos.y - past_pos.y;
			// 選択したプレイヤーの近傍9マスをクリックしたとき
			if (abs(dx) <= 1 and abs(dy) <= 1) {
				bool will_flip = tile.rightClicked();
				selected_agent->set_action(dx, dy, will_flip);
				tiles[past_pos].set_move_vec(dx, dy);
				select_moved = true;
			}
		}
		// プレイヤーを選択していない状態で自分のチームのプレイヤーがいるタイルを左クリックしたとき
		else if (!is_selected and tile.leftClicked() and tiles[p].state == Visualizer::Myplayer) {
			is_selected = true;
			selected_pos = p;
			selected_agent = tiles[p].agent;
		}

		// 描画設定
		// マウスオーバーされた時のタイルのフレーム色
		ColorF frame_color = Palette::Black;
		if (tile.mouseOver())frame_color = ColorF(Palette::Orange);
		// 猫の画像の大きさ指定
		double cat_stay_size = 0.0015 * tile_size;		// 0.05
		double cat_move_size = 0.003 * tile_size;		// 0.1
		double cat_ratio = 1.0;		// プレイヤーを選択したときの猫画像の大きさの倍率
		if (is_selected == true and selected_pos == p) cat_ratio = 1.3;
		cat_stay_size *= cat_ratio;
		cat_move_size *= cat_ratio;

		// 描画
		// タイルの描画
		tile.draw(tiles[p].color).drawFrame(1, 0, frame_color);		// タイル
		//font(Format(tiles[p].point)).draw(Arg::center(tile.center()), Palette::Black);	// タイルポイント
		// 猫の画像描画
		if (tiles[p].state == Visualizer::Myplayer) {
			if (tiles[p].will_stay) cat_stay.scaled(cat_stay_size).drawAt(tile.center());
			else cat_move.scaled(cat_move_size).rotated(tiles[p].will_move_radian).drawAt(tile.center());
		}
		// タイルポイントの描画
		font(Format(tiles[p].point)).draw(Arg::center(tile.center()), Palette::Black);	// タイルポイント
	}
	return select_moved;
}



// ゲームフィールドの状態を更新する
void GameField::update_state(Board& board, Team& my_team, Team& op_team) {
	// ゲームフィールドのメンバ変数更新
	height = board.height;
	width = board.width;
	grid_size = Size(width, height);
	tile_size = Min(700 / height, 700 / width);
	is_selected = false;
	// タイルポイントの変更
	for (auto& p : step(grid_size)) tiles[p].point = board.points[p];
	font = Font(int(0.4 * tile_size));		// タイルポイントのフォントの大きさ更新

	// タイルの状態を更新
	// タイルが赤でも青でもないとき
	for (auto& p : step(grid_size)) tiles[p].state = Visualizer::Empty;
	// どちらかのチームのタイルのとき
	for (auto& p : step(grid_size)) {
		if (board.tiled[p] == my_team.team_id) tiles[p].state = Visualizer::Mytile;
		if (board.tiled[p] == op_team.team_id) tiles[p].state = Visualizer::Optile;
	}
	// どちらかのチームのプレイヤーがいるタイルのとき
	for (auto& agent : my_team.agents) tiles[agent.y][agent.x].state = Visualizer::Myplayer;
	for (auto& agent : op_team.agents) tiles[agent.y][agent.x].state = Visualizer::Opplayer;
	// プレイヤーがいるタイルオブジェクトにプレイヤー情報を登録する
	for (auto& team : board.teams) for (auto& agent : team->agents) {
		Point pos(agent.x, agent.y);
		tiles[pos].agent = &agent;
		tiles[pos].set_move_vec(agent.dx, agent.dy);
	}
	// 実際にタイルオブジェクトの色を適用する
	for (auto p : step(grid_size)) tiles[p].update_color(board);
}



// フィールドの表示位置を変更する
void GameField::set_pos(Point field_pos_) {
	field_pos = field_pos_;
}
// ----------------------------------------------------------------------------------------------------------