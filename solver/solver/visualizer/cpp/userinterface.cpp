# include "visualizer/userinterface.hpp"



// �R���X�g���N�^
UserInterface::UserInterface() {
	set_window_size(Size(1200, 900));		// �����̃E�B���h�E�T�C�Y�ݒ�
	Scene::SetBackground(Palette::Green);		// �w�i��΂ɂ���
}



// http_controller�I�u�W�F�N�g���Z�b�g����
void UserInterface::set_http_controller(HttpController& http_controller_) {
	http_controller = &http_controller_;
}



// �E�B���h�E�T�C�Y��ύX����
void UserInterface::set_window_size(Size size) {
	Scene::Resize(size);		// �V�[���T�C�Y�ύX
	Window::Resize(size);		// �E�B���h�E�T�C�Y�ύX
}



// �t�B�[���h��{�^������`�悷��
void UserInterface::draw(Board& board) {
	// �{�^����`�悷��
	Visualizer::Clicked_Button clicked_button;
	clicked_button = buttons.draw_buttons();
	do_button_action(board, clicked_button);	// �����ꂽ�{�^���ɑΉ���������������
	// �Q�[���t�B�[���h��`�悷��
	bool select_moved = game_field.draw_field(board);
	if (select_moved) send_actions(board);
	// �p�l����`�悷��
	my_team_panel.draw_panel(*(board.my_team));
	op_team_panel.draw_panel(*(board.op_team));
	bool need_update = turn_panel.draw_panel(board);
	if (need_update) update_state(board);
}



// �����ꂽ�{�^���ɑΉ���������������
void UserInterface::do_button_action(Board& board, Visualizer::Clicked_Button clicked_button) {
	// ������Ԏ擾�{�^���������ꂽ�Ƃ�
	if (clicked_button == Visualizer::Update) {
		update_state(board);
	}
	// �s�����M�{�^���������ꂽ�Ƃ�
	if (clicked_button == Visualizer::Send) {
		send_actions(board);
	}
	// �F���]�{�^���������ꂽ�Ƃ�
	if (clicked_button == Visualizer::Swap) {
		int my_team_color = board.my_team->color;
		if (my_team_color == Procon::Red) board.set_my_team_color(Procon::Blue);
		else board.set_my_team_color(Procon::Red);
		update_state(board);
	}
}



// �s�������T�[�o�ɑ��M����
void UserInterface::send_actions(Board& board) {
	std::string file_path = board.write_actions();
	http_controller->post_actions(file_path);
}



// �u�Ֆʏ�ԁv�ƁuGUI�̕\���ʒu�v���X�V����
void UserInterface::update_state(Board& board) {
	// �Ֆʏ�ԍX�V
	std::string file_path = http_controller->get_state();
	board.set_state(file_path);
	game_field.update_state(board, *(board.my_team), *(board.op_team));
	// �p�l���̈ʒu�ύX
	int panels_x = buttons.icons_pos.x + game_field.tile_size * board.width + margin_x;
	int last_y = buttons.icon_size;
	last_y = my_team_panel.set_pos(Point(panels_x, last_y));
	last_y = op_team_panel.set_pos(Point(panels_x, last_y + margin_y));
	last_y = turn_panel.set_pos(Point(panels_x, last_y + margin_y));
	// �{�^���̕\���ʒu�ύX
	last_y = buttons.set_buttons_pos(Point(panels_x, last_y + margin_y + 50));
	// �E�B���h�E�T�C�Y��ύX����
	int field_height = game_field.tile_size * board.height;
	int window_width = panels_x + 300 + margin_x;
	int window_height = Max(last_y, buttons.icon_size + field_height) + 40;
	set_window_size(Size(window_width, window_height));
}