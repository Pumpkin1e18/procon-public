#pragma once
#include <Siv3D.hpp>
#include <fstream>
#include <stdlib.h>		// system�֐��̂���
#include <regex>		// ���K�\�����g������
#include <random>		// �������g������
#include "nlohmann/json.hpp"

namespace nl = nlohmann;

class HttpController {
private:
	// http�ʐM�̃w�b�_��
	std::string ip_address = "";		// "localhost:8000"
	std::string token = "";				// "procon30_example_token"
	std::string head_token = "";		// R"( -H "Authorization: procon30_matchID-7_teamRED_FudaiKosen")"
	std::string head_json = "";			// R"( -H "Content-Type: application/json")"
	std::string url = "";				// "http://localhost:8000/matches/"

	// settings.json�t�@�C���̃p�X
	std::string settings_file = "settings/settings.json";

	// �󂯎����json�t�@�C���̕ۑ��ꏊ
	std::string json_directory = "json/";
	std::string info_json_name = "info.json";
	std::string state_json_name = "state.json";
	std::string result_json_name = "post_result.json";

	// �t�H�[�}�b�g�𐮂���json�t�@�C���� json/formatted/ �ɕۑ�����
	void format_json(std::string directory_name, std::string file_name);
	// �_�u���N�H�[�e�[�V�����i"�j�̑O�Ƀo�b�N�X���b�V���i/�j��t������
	std::string add_backslash(std::string &data);
	// num �� 1�`100�̂ǂꂩ�̃����_���Ȑ��� �ɕϊ�����
	std::string convert_num(std::string& data);


public:
	int match_id = 6;								// ����id

	void read_settings();							// �ݒ�t�@�C����ǂݍ���
	HttpController();								// �R���X�g���N�^
	std::string get_info();							// �������O���擾
	std::string get_state();						// ������Ԏ擾
	void post_actions(std::string &file_path);		// �s����񑗐M
};