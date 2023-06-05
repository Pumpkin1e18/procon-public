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
	// �}�[�W��
	static constexpr int margin_x = 40;		// �I�u�W�F�N�g�Ԃ̉��̊Ԋu
	static constexpr int margin_y = 20;		// �I�u�W�F�N�g�Ԃ̏c�̊Ԋu

	// �Ǘ�����O���t�B�b�N�֘A�̃I�u�W�F�N�g
	GameField game_field;
	TeamPanel my_team_panel, op_team_panel;
	TurnPanel turn_panel;
	Buttons buttons;

	// �T�[�o�ƒʐM���邽�߂̃I�u�W�F�N�g
	HttpController* http_controller;


public:
	// �����o�֐�
	UserInterface();																// �R���X�g���N�^
	void set_http_controller(HttpController& http_controller_);						// http_controller�I�u�W�F�N�g���Z�b�g����
	void set_window_size(Size size);												// �E�B���h�E�T�C�Y��ύX����
	void draw(Board& board);														// �t�B�[���h��{�^������`�悷��
	void do_button_action(Board& board, Visualizer::Clicked_Button clicked_button);	// �����ꂽ�{�^���ɑΉ���������������
	void send_actions(Board& board);												// �s�������T�[�o�ɑ��M����
	void update_state(Board& board);												// �u�Ֆʏ�ԁv�ƁuGUI�̕\���ʒu�v���X�V����
};