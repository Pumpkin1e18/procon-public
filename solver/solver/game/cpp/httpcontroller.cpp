#include "game/HttpController.hpp"


// �t�H�[�}�b�g�𐮂���json�t�@�C���� json/formatted/ �ɕۑ�����
void HttpController::format_json(std::string directory_name, std::string file_name) {
    std::ifstream input_file(directory_name + file_name);       // �C���v�b�g�t�@�C���̎w��@json/info.json
    directory_name += "formatted/";
    std::ofstream output_file(directory_name + file_name);      // �A�E�g�v�b�g�t�@�C���̎w��@json/formatted/info.json
    nl::json json;                                              // json�f�[�^���i�[����f�[�^�\��
    input_file >> json;                                         // �t�H�[�}�b�g����Ă��Ȃ�json�t�@�C����ǂݍ���
    output_file << json.dump(4);                                // �t�H�[�}�b�g���ꂽjson�t�@�C������������
}



// �_�u���N�H�[�e�[�V�����i"�j�̑O�Ƀo�b�N�X���b�V���i/�j��t������
std::string HttpController::add_backslash(std::string& data) {
    std::regex e("\"");                                         // "
    std::string res = std::regex_replace(data, e, "\\\"");      // \"
    return res;
}



// num �� 1�`100�̂ǂꂩ�̃����_���Ȑ��� �ɕϊ�����
std::string HttpController::convert_num(std::string& data) {
    std::mt19937 mt((int)time(0));                              // �����Z���k�E�c�C�X�^��32�r�b�g��
    std::regex e("num");                                        // num
    std::string s = std::to_string(mt() % 100 + 1);             // 1�`100�̂ǂꂩ
    std::string res = std::regex_replace(data, e, s);           // num -> 1�`100�̂ǂꂩ
    Console << U"����ID: " << Unicode::Widen(s);
    Console << Unicode::Widen(res);
    return res;
}



// �ݒ�t�@�C����ǂݍ���
void HttpController::read_settings() {
    std::ifstream settings_json(settings_file);                 // �ǂݍ��ރt�@�C���̎w��
    nl::json settings;                                          // json�f�[�^���i�[����f�[�^�\��

    // �ݒ�t�@�C���̏���ϐ��ɑ��
    settings_json >> settings;                                  // �ݒ�t�@�C������ǂݍ���
    ip_address = settings["ip_address"];                        // localhost:8000
    token = settings["token"];                                  // procon30_matchID-num_teamRED_FudaiKosen
    token = convert_num(token);                                 // procon30_matchID-7_teamRED_FudaiKosen
    // ���̑��̕ϐ��ɒl����
    head_token = R"( -H "Authorization: )" + token + "\"";
    head_json = R"( -H "Content-Type: application/json")";
    url = " http://" + ip_address + "/matches/";
    // Console << Unicode::Widen(ip_address);
    // Console << Unicode::Widen(token);
}



// �R���X�g���N�^
HttpController::HttpController() {
    // �ݒ�t�@�C����ǂݍ���
    read_settings();

    // json�t�@�C�����i�[����t�H���_���쐬����
    std::string directory_path = "md json\\formatted";          // mkdir 
    system(directory_path.c_str());                             // json�t�H���_�쐬
}



// �������O���擾
std::string HttpController::get_info() {
    // �O����
    if (ip_address == "")read_settings();                       // ����������������Ă��Ȃ���΂���
    std::string file_path = json_directory + info_json_name;    // json/info.json

    // ���s����R�}���h�����쐬����
    std::string send_text = "curl";                             // curl
    send_text += head_token;                                    // -H "Authorization: procon30_example_token"
    send_text += url;                                           // localhost:8000/matches
    send_text += " > " + file_path;                             // > json/info.json

    // �R�}���h�����s���āA�t�H�[�}�b�g���ꂽjson�t�@�C�����ۑ�����
    int res = system(send_text.c_str());                        // curl�R�}���h���s
    if (res != 0)                                               // �R�}���h���s�Ɏ��s�����ꍇ
        Console << U"�������O�����擾�ł��܂���ł���";       // �G���[���b�Z�[�W���o��
    format_json(json_directory, info_json_name);                // �t�H�[�}�b�g�𐮂���json�t�@�C����ۑ�����
    return file_path;                                           // �ۑ������t�@�C���p�X��Ԃ�
}



// ������Ԏ擾
std::string HttpController::get_state() {
    // �O����
    if (ip_address == "")read_settings();                       // ����������������Ă��Ȃ���΂���
    std::string file_path = json_directory + state_json_name;   // json/state.json

    // ���s����R�}���h�����쐬����
    std::string send_text = "curl";                             // curl
    send_text += head_token;                                    // -H "Authorization: procon30_example_token"
    send_text += url;                                           // localhost:8000/matches
    send_text += std::to_string(match_id) + "/";                // 7/
    send_text += " > " + file_path;                             // > json/state.json

    // �R�}���h�����s���āA�t�H�[�}�b�g���ꂽjson�t�@�C�����ۑ�����
    int res = system(send_text.c_str());                        // curl�R�}���h���s
    if (res != 0)                                               // �R�}���h���s�Ɏ��s�����ꍇ
        Console << U"������Ԃ��擾�ł��܂���ł���";           // �G���[���b�Z�[�W���o��
    format_json(json_directory, state_json_name);               // �t�H�[�}�b�g�𐮂���json�t�@�C����ۑ�����
    return file_path;                                           // �ۑ������t�@�C���p�X��Ԃ�
}



// �s����񑗐M
void HttpController::post_actions(std::string& file_path) {
    // �|�X�g������e�擾
    std::ifstream actions_json(file_path);                      // �C���v�b�g�t�@�C���̎w��@json/actions.json
    nl::json actions;                                           // json�f�[�^���i�[����f�[�^�\��
    actions_json >> actions;                                    // json�t�@�C����ǂݍ���
    std::string body_data = actions.dump();                     // json�f�[�^��std::string�ɕϊ�

    // ���ʂ�ۑ�����t�@�C���p�X�̐ݒ�
    std::string result_file_path = json_directory;              // json/
    result_file_path += result_json_name;                       // json/post_result.json

    // ���s����R�}���h�����쐬����
    std::string send_text = "curl";                             // curl
    send_text += head_token;                                    // -H "Authorization: procon30_example_token"
    send_text += head_json;                                     // -H "Content-Type: application/json"
    send_text += " -X POST";                                    // -X POST
    send_text += url;                                           // localhost:8000/matches
    send_text += std::to_string(match_id) + "/action/";         // 7/action/
    send_text += " -d";                                         // -d
    send_text += " \"";                                         // "
    send_text += add_backslash(body_data);                      // HTTP�ʐM�̃{�f�B�f�[�^
    send_text += "\"";                                          // "
    send_text += " > " + result_file_path;                      // > json/post_result.json

    // �R�}���h�����s����
    int res = system(send_text.c_str());                        // curl�R�}���h���s
    if (res != 0)                                               // �R�}���h���s�Ɏ��s�����ꍇ
        Console << U"�s�����𑗐M�ł��܂���ł���";           // �G���[���b�Z�[�W���o��
}