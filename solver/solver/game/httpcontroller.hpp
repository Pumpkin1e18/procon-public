#pragma once
#include <Siv3D.hpp>
#include <fstream>
#include <stdlib.h>		// system関数のため
#include <regex>		// 正規表現を使うため
#include <random>		// 乱数を使うため
#include "nlohmann/json.hpp"

namespace nl = nlohmann;

class HttpController {
private:
	// http通信のヘッダ部
	std::string ip_address = "";		// "localhost:8000"
	std::string token = "";				// "procon30_example_token"
	std::string head_token = "";		// R"( -H "Authorization: procon30_matchID-7_teamRED_FudaiKosen")"
	std::string head_json = "";			// R"( -H "Content-Type: application/json")"
	std::string url = "";				// "http://localhost:8000/matches/"

	// settings.jsonファイルのパス
	std::string settings_file = "settings/settings.json";

	// 受け取ったjsonファイルの保存場所
	std::string json_directory = "json/";
	std::string info_json_name = "info.json";
	std::string state_json_name = "state.json";
	std::string result_json_name = "post_result.json";

	// フォーマットを整えたjsonファイルを json/formatted/ に保存する
	void format_json(std::string directory_name, std::string file_name);
	// ダブルクォーテーション（"）の前にバックスラッシュ（/）を付け足す
	std::string add_backslash(std::string &data);
	// num を 1～100のどれかのランダムな数字 に変換する
	std::string convert_num(std::string& data);


public:
	int match_id = 6;								// 試合id

	void read_settings();							// 設定ファイルを読み込む
	HttpController();								// コンストラクタ
	std::string get_info();							// 試合事前情報取得
	std::string get_state();						// 試合状態取得
	void post_actions(std::string &file_path);		// 行動情報送信
};