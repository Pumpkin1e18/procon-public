# pragma once
# include <Siv3D.hpp>	// OpenSiv3D v0.4.3

# include "game/board.hpp"

# include "visualizer/settings.hpp"



// �{�^���N���X
class Buttons {
private:
	// �A�C�R���{�^���̍쐬�iundo, redo, sync�j
	const Texture iconUndo = Texture(Icon(0xf060, 28));
	const Texture iconRedo = Texture(Icon(0xf061, 28));
	const Texture iconSync = Texture(Icon(0xf021, 28));
	static constexpr int icons_x = 40;
	static constexpr int icons_y = 0;

	// �N���b�N�{�^���̍쐬
	String update_button_text = U"������Ԃ��擾";
	String send_button_text = U"�s�����𑗐M";
	String swap_button_text = U"�����Ƒ���̐F����������";
	Point buttons_pos = Point(750, 700);


public:
	// �A�C�R���̈ʒu��T�C�Y�Ɋւ���ϐ�
	static constexpr Point icons_pos = Point(icons_x, icons_y);
	static constexpr int icon_size = 50;
	static constexpr int button_size = 50;

	// �����o�֐�
	Visualizer::Clicked_Button draw_buttons();		// �{�^���̕`��
	int set_buttons_pos(Point buttons_pos_);		// �N���b�N�{�^���̕\���ʒu��ύX����
};