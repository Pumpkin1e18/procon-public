# pragma once
# include <Siv3D.hpp>	// OpenSiv3D v0.4.3

# include "game/board.hpp"



// �`�[���p�l��
class TeamPanel {
private:
	Font font = Font(20);					// �p�l���ɕ\������t�H���g���쐬
	Rect rect = Rect(750, 20, 300, 150);	// �p�l���쐬

public:
	void draw_panel(Team& team);			// �`�[���p�l����\������
	int set_pos(Point pos);					// �`�[���p�l���̕\���ʒu�Ƒ傫���ύX
};



// �^�[���p�l��
class TurnPanel {
private:
	Font font = Font(20);					// �傫��15�̃t�H���g���쐬
	Rect rect = Rect(750, 20, 300, 200);	// �����ʒu

public:
	bool draw_panel(Board& board);			// �^�[���p�l����\������
	int set_pos(Point pos);					// �^�[���p�l���̕\���ʒu�ύX
};