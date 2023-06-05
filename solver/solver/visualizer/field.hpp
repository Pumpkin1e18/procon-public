# pragma once
# include <Siv3D.hpp>	// OpenSiv3D v0.4.3
# include <cmath>		// abs, atan2�i�񎟌��̃A�[�N�^���W�F���g�j

# include "proconsettings.hpp"
# include "settings.hpp"

# include "game/httpcontroller.hpp"
# include "game/board.hpp"



// �^�C��
// ----------------------------------------------------------------------------------------------------------
class Tile {
private:
	// �^�C���̎��ۂɕ\�������F��RGB�Ŏw�肷��
	ColorF empty_color = Palette::White;		// �^�C�����ǂ����̃`�[���̂��̂ł��Ȃ��Ƃ��̐F
	ColorF red_tile_color = Palette::Pink;		// �^�C�����ԃ`�[���̂��̂̂Ƃ��̐F
	ColorF red_player_color = ColorF(1, 0.3, 0.3);		// �^�C���ɐԃ`�[���̃v���C���[������Ƃ��̐F
	ColorF blue_tile_color = Palette::Skyblue;	// �^�C�����`�[���̂��̂̂Ƃ��̐F
	ColorF blue_player_color = ColorF(0.3, 0.3, 1);	// �^�C���ɐ`�[���̃v���C���[������Ƃ��̐F

	// �ϐ��ustate�v������ۂɕ\�������F�iRGB�j�ɕϊ�����u�ϊ��e�[�u���v�̐錾
	HashTable<int, ColorF> red_table;		// �����̃`�[�����ԐF�̂Ƃ��̕ϊ��e�[�u��
	HashTable<int, ColorF> blue_table;		// �����̃`�[�����F�̂Ƃ��̕ϊ��e�[�u��


public:
	// �^�C���Ɋւ���ϐ�
	int point = -16;		// �^�C���|�C���g
	int state = Visualizer::Empty;			// �^�C���̏�ԁi�ǂ��炩�̃`�[���̃^�C�����A�v���C���[�����邩���j
	ColorF color = Palette::White;			// �^�C���̐F�iRGB�Ŏw��j

	// ���̃^�C���ɃG�[�W�F���g������ꍇ�̕ϐ�
	Agent* agent;							// �G�[�W�F���g�I�u�W�F�N�g�̃|�C���^
	bool will_stay = true;					// ���̃X�e�b�v��stay���邩�ǂ���
	double will_move_radian = 0;			// �G�[�W�F���g�����̃X�e�b�v�œ��������idegree�Ŏw��j

	// �����o�֐�
	Tile();									// �R���X�g���N�^
	void set_table();						// �^�C���̏�Ԃ���F�ɕϊ�����ϊ��e�[�u�����쐬����
	void update_color(Board& board);		// �^�C���̐F���X�V����
	void set_move_vec(int dx, int dy);		// ���ɃG�[�W�F���g�������������v�Z����
};
// ----------------------------------------------------------------------------------------------------------





// �Q�[���t�B�[���h
// ----------------------------------------------------------------------------------------------------------
class GameField {
private:
	// �u�t�H���g�v�Ɓu�e�N�X�`���i�摜�j�v�̕ϐ�
	Font font = Font(int(0.4 * tile_size));
	const Texture cat_move = Texture(U"image/cat_move.png");
	const Texture cat_stay = Texture(U"image/cat_stay.png");

	// �u�t�B�[���h�̑傫���v�Ɓu�^�C���v�̕ϐ�
	int height = 20;
	int width = 20;
	Size grid_size = Size(width, height);
	Grid<Tile> tiles = Grid<Tile>(grid_size, Tile());

	// �v���C���[�I���Ɋւ���ϐ�
	bool is_selected = false;
	Point selected_pos;
	Agent* selected_agent;


public:
	// �t�B�[���h�̈ʒu��傫���Ɋւ���ϐ�
	Point field_pos = Point(40, 50);	// �t�B�[���h�̍���̈ʒu
	int tile_size = 35;					// �^�C���̑傫��

	// �����o�֐�
	bool draw_field(Board& board);									// �t�B�[���h��`�悷��
	void update_state(Board& board, Team& my_team, Team& op_team);	// �Q�[���t�B�[���h�̏�Ԃ��X�V����
	void set_pos(Point field_pos_);									// �t�B�[���h�̕\���ʒu��ύX����
};
// ----------------------------------------------------------------------------------------------------------