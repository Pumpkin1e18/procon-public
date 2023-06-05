# pragma once
namespace Visualizer {
	// �^�C���̐F�Ɋւ�����
	constexpr int Empty = 0;
	constexpr int Mytile = -1;
	constexpr int Myplayer = -2;
	constexpr int Optile = -3;
	constexpr int Opplayer = -4;

	// �N���b�N�����{�^���Ɋւ�����
	enum Clicked_Button {
		Undo,
		Redo,
		Sync,		// �{�^���̌����ڂ͈Ⴄ���̂̉������Ƃ��̓����Update�Ɠ���
		Update,		// �{�^���̌����ڂ͈Ⴄ���̂̉������Ƃ��̓����Sync�Ɠ���
		Send,
		Swap,		// �����Ƒ���̐F����������
		None,		// �����{�^����������Ȃ������Ƃ��̏��
	};
}