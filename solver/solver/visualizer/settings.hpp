# pragma once
namespace Visualizer {
	// タイルの色に関する情報
	constexpr int Empty = 0;
	constexpr int Mytile = -1;
	constexpr int Myplayer = -2;
	constexpr int Optile = -3;
	constexpr int Opplayer = -4;

	// クリックしたボタンに関する情報
	enum Clicked_Button {
		Undo,
		Redo,
		Sync,		// ボタンの見た目は違うものの押したときの動作はUpdateと同じ
		Update,		// ボタンの見た目は違うものの押したときの動作はSyncと同じ
		Send,
		Swap,		// 自分と相手の色を交換する
		None,		// 何もボタンが押されなかったときの状態
	};
}