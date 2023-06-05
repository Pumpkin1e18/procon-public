# pragma once
# include "visualizer/buttons.hpp"



// �{�^���̕`��
Visualizer::Clicked_Button Buttons::draw_buttons() {
	// �A�C�R���{�^���̕`��
	const Rect rectUndo(icons_x + 0, 0, icon_size, icon_size);
	const Rect rectRedo(icons_x + icon_size, 0, icon_size, icon_size);
	const Rect rectSync(icons_x + 2 * icon_size, 0, icon_size, icon_size);
	for (auto& rect : { rectUndo, rectRedo, rectSync }) {
		rect.draw(ColorF(0, 0.5, 0));		// �ʏ펞�͗ΐF
		if (rect.mouseOver()) rect.draw(ColorF(0, 0.4, 0));		// �}�E�X�I�[�o�[����ƔZ���ΐF
	}
	const int icon_size_2 = int(icon_size / 2);
	Point center_of_icon = Point(icons_x + icon_size_2, icon_size_2);
	for (auto& icon : { iconUndo, iconRedo, iconSync }) {
		icon.drawAt(center_of_icon, ColorF(0.2));
		center_of_icon.x += icon_size;
	}
	// �N���b�N�{�^���̕`��
	Array<String> text = { update_button_text, send_button_text, swap_button_text };
	Array<Visualizer::Clicked_Button> clicked_button
		= { Visualizer::Update, Visualizer::Send, Visualizer::Swap };
	Point button_pos = buttons_pos;
	for (int i : step(text.size())) {
		if (SimpleGUI::Button(text[i], button_pos, 300)) return clicked_button[i];
		button_pos.y += button_size;
	}
	return Visualizer::None;
}



// �N���b�N�{�^���̕\���ʒu��ύX����
int Buttons::set_buttons_pos(Point buttons_pos_) {
	buttons_pos = buttons_pos_;
	return buttons_pos.y + 3 * button_size;
}