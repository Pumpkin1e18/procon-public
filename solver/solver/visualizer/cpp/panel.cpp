# pragma once
# include "visualizer/panel.hpp"


// �`�[���p�l��
// ----------------------------------------------------------------------------------------------------------
// �`�[���p�l����\������
void TeamPanel::draw_panel(Team& team) {
	// �\�����e�쐬
	String text = U"";
	text += (team.is_my_team ? U"�����`�[��" : U"����`�[��");
	text += U"\n�^�C���|�C���g�F" + Format(team.tile_point);
	text += U"\n�̈�|�C���g�F" + Format(team.area_point);
	text += U"\n�T�����@�F";
	text += (team.is_my_team ? U"�l��" : U"�×~�@");
	// �p�l���̐F�w��
	ColorF color_background = (team.color == Procon::Red) ? Palette::Pink : Palette::Skyblue;
	ColorF color_frame = (team.color == Procon::Red) ? Palette::Red : Palette::Blue;
	// �p�l����`��
	rect.draw(color_background);
	rect.drawFrame(5, 0, color_frame);
	font(text).draw(rect.stretched(-25, -15), Palette::Black);		// ���E 20px �󂯁A�㉺ 15px �󂯂�
}


// �`�[���p�l���̕\���ʒu�Ƒ傫���ύX
int TeamPanel::set_pos(Point pos) {
	Size size = Size(300, 150);		// Size(x, y)
	rect = Rect(pos, size);
	return pos.y + size.y;
}
// ----------------------------------------------------------------------------------------------------------






// �^�[���p�l��
// ----------------------------------------------------------------------------------------------------------
// �^�[���p�l����\������
bool TurnPanel::draw_panel(Board& board) {
	// ���ԂɊ֐��ϐ��쐬
	time_t now_time = std::time(nullptr) * 1000;
	time_t next_turn_time = board.started_unix_time;
	next_turn_time += (board.turn + 1) * (board.interval_millis + board.turn_millis);
	double time_to_next_turn = (next_turn_time - now_time) / 1000.0;
	// �\�����镶����쐬
	String text = U"";
	text += U"���݂̃^�[���F" + Format(board.turn);
	text += U"\n���̃^�[���܂ŁF" + Format(int(time_to_next_turn)) + U"�b";
	// �`��
	rect.draw(Palette::White);
	rect.drawFrame(5, 0, Palette::Black);
	font(text).draw(rect.stretched(-25, -15), Palette::Black);
	return time_to_next_turn < 0;
}


// �^�[���p�l���̕\���ʒu�ύX
int TurnPanel::set_pos(Point pos) {
	Size size = Size(300, 90);
	rect = Rect(pos, size);
	return pos.y + size.y;
}
// ----------------------------------------------------------------------------------------------------------