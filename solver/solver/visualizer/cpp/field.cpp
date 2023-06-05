# include "visualizer/field.hpp"


// �^�C��
// ----------------------------------------------------------------------------------------------------------
// �R���X�g���N�^
Tile::Tile() { set_table(); }


// �^�C���̏�Ԃ���F�ɕϊ�����ϊ��e�[�u�����쐬����
void Tile::set_table() {
	// �����̃`�[�����ԐF�̂Ƃ��̕ϊ��e�[�u���̍쐬
	red_table[Visualizer::Empty] = empty_color;
	red_table[Visualizer::Mytile] = red_tile_color;
	red_table[Visualizer::Myplayer] = red_player_color;
	red_table[Visualizer::Optile] = blue_tile_color;
	red_table[Visualizer::Opplayer] = blue_player_color;
	// �����̃`�[�����F�̂Ƃ��̕ϊ��e�[�u���̍쐬
	blue_table[Visualizer::Empty] = empty_color;
	blue_table[Visualizer::Mytile] = blue_tile_color;
	blue_table[Visualizer::Myplayer] = blue_player_color;
	blue_table[Visualizer::Optile] = red_tile_color;
	blue_table[Visualizer::Opplayer] = red_player_color;
}


// �^�C���̐F���X�V����
void Tile::update_color(Board& board) {
	if (red_table.size() == 0)set_table();
	color = (board.my_team->color == Procon::Red) ? red_table[state] : blue_table[state];
}


// ���ɃG�[�W�F���g�������������v�Z����
void Tile::set_move_vec(int dx, int dy) {
	will_move_radian = std::atan2(dx, -dy);		// atan2(float y, float x)
	will_stay = (dx == 0 and dy == 0) ? true : false;
}
// ----------------------------------------------------------------------------------------------------------









// �Q�[���t�B�[���h
// ----------------------------------------------------------------------------------------------------------
// �t�B�[���h��`�悷��
bool GameField::draw_field(Board& board) {
	bool select_moved = false;
	for (auto p : step(grid_size)) {
		// �^�C���̍쐬
		const Rect tile(field_pos + p * tile_size, tile_size);

		// �^�C���̃N���b�N����
		// �v���C���[��I��������ԂŃ^�C�����N���b�N�����Ƃ�
		if (is_selected and (tile.leftClicked() or tile.rightClicked())) {
			is_selected = false;	// �v���C���[�̑I������������
			// ����������O�ɑO�v�Z����
			Point now_pos = p;
			Point past_pos = selected_pos;
			int dx = now_pos.x - past_pos.x;
			int dy = now_pos.y - past_pos.y;
			// �I�������v���C���[�̋ߖT9�}�X���N���b�N�����Ƃ�
			if (abs(dx) <= 1 and abs(dy) <= 1) {
				bool will_flip = tile.rightClicked();
				selected_agent->set_action(dx, dy, will_flip);
				tiles[past_pos].set_move_vec(dx, dy);
				select_moved = true;
			}
		}
		// �v���C���[��I�����Ă��Ȃ���ԂŎ����̃`�[���̃v���C���[������^�C�������N���b�N�����Ƃ�
		else if (!is_selected and tile.leftClicked() and tiles[p].state == Visualizer::Myplayer) {
			is_selected = true;
			selected_pos = p;
			selected_agent = tiles[p].agent;
		}

		// �`��ݒ�
		// �}�E�X�I�[�o�[���ꂽ���̃^�C���̃t���[���F
		ColorF frame_color = Palette::Black;
		if (tile.mouseOver())frame_color = ColorF(Palette::Orange);
		// �L�̉摜�̑傫���w��
		double cat_stay_size = 0.0015 * tile_size;		// 0.05
		double cat_move_size = 0.003 * tile_size;		// 0.1
		double cat_ratio = 1.0;		// �v���C���[��I�������Ƃ��̔L�摜�̑傫���̔{��
		if (is_selected == true and selected_pos == p) cat_ratio = 1.3;
		cat_stay_size *= cat_ratio;
		cat_move_size *= cat_ratio;

		// �`��
		// �^�C���̕`��
		tile.draw(tiles[p].color).drawFrame(1, 0, frame_color);		// �^�C��
		//font(Format(tiles[p].point)).draw(Arg::center(tile.center()), Palette::Black);	// �^�C���|�C���g
		// �L�̉摜�`��
		if (tiles[p].state == Visualizer::Myplayer) {
			if (tiles[p].will_stay) cat_stay.scaled(cat_stay_size).drawAt(tile.center());
			else cat_move.scaled(cat_move_size).rotated(tiles[p].will_move_radian).drawAt(tile.center());
		}
		// �^�C���|�C���g�̕`��
		font(Format(tiles[p].point)).draw(Arg::center(tile.center()), Palette::Black);	// �^�C���|�C���g
	}
	return select_moved;
}



// �Q�[���t�B�[���h�̏�Ԃ��X�V����
void GameField::update_state(Board& board, Team& my_team, Team& op_team) {
	// �Q�[���t�B�[���h�̃����o�ϐ��X�V
	height = board.height;
	width = board.width;
	grid_size = Size(width, height);
	tile_size = Min(700 / height, 700 / width);
	is_selected = false;
	// �^�C���|�C���g�̕ύX
	for (auto& p : step(grid_size)) tiles[p].point = board.points[p];
	font = Font(int(0.4 * tile_size));		// �^�C���|�C���g�̃t�H���g�̑傫���X�V

	// �^�C���̏�Ԃ��X�V
	// �^�C�����Ԃł��ł��Ȃ��Ƃ�
	for (auto& p : step(grid_size)) tiles[p].state = Visualizer::Empty;
	// �ǂ��炩�̃`�[���̃^�C���̂Ƃ�
	for (auto& p : step(grid_size)) {
		if (board.tiled[p] == my_team.team_id) tiles[p].state = Visualizer::Mytile;
		if (board.tiled[p] == op_team.team_id) tiles[p].state = Visualizer::Optile;
	}
	// �ǂ��炩�̃`�[���̃v���C���[������^�C���̂Ƃ�
	for (auto& agent : my_team.agents) tiles[agent.y][agent.x].state = Visualizer::Myplayer;
	for (auto& agent : op_team.agents) tiles[agent.y][agent.x].state = Visualizer::Opplayer;
	// �v���C���[������^�C���I�u�W�F�N�g�Ƀv���C���[����o�^����
	for (auto& team : board.teams) for (auto& agent : team->agents) {
		Point pos(agent.x, agent.y);
		tiles[pos].agent = &agent;
		tiles[pos].set_move_vec(agent.dx, agent.dy);
	}
	// ���ۂɃ^�C���I�u�W�F�N�g�̐F��K�p����
	for (auto p : step(grid_size)) tiles[p].update_color(board);
}



// �t�B�[���h�̕\���ʒu��ύX����
void GameField::set_pos(Point field_pos_) {
	field_pos = field_pos_;
}
// ----------------------------------------------------------------------------------------------------------