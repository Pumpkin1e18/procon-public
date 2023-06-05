#include "game/HttpController.hpp"


// フォーマットを整えたjsonファイルを json/formatted/ に保存する
void HttpController::format_json(std::string directory_name, std::string file_name) {
    std::ifstream input_file(directory_name + file_name);       // インプットファイルの指定　json/info.json
    directory_name += "formatted/";
    std::ofstream output_file(directory_name + file_name);      // アウトプットファイルの指定　json/formatted/info.json
    nl::json json;                                              // jsonデータを格納するデータ構造
    input_file >> json;                                         // フォーマットされていないjsonファイルを読み込む
    output_file << json.dump(4);                                // フォーマットされたjsonファイルを書き込む
}



// ダブルクォーテーション（"）の前にバックスラッシュ（/）を付け足す
std::string HttpController::add_backslash(std::string& data) {
    std::regex e("\"");                                         // "
    std::string res = std::regex_replace(data, e, "\\\"");      // \"
    return res;
}



// num を 1～100のどれかのランダムな数字 に変換する
std::string HttpController::convert_num(std::string& data) {
    std::mt19937 mt((int)time(0));                              // メルセンヌ・ツイスタの32ビット版
    std::regex e("num");                                        // num
    std::string s = std::to_string(mt() % 100 + 1);             // 1～100のどれか
    std::string res = std::regex_replace(data, e, s);           // num -> 1～100のどれか
    Console << U"試合ID: " << Unicode::Widen(s);
    Console << Unicode::Widen(res);
    return res;
}



// 設定ファイルを読み込む
void HttpController::read_settings() {
    std::ifstream settings_json(settings_file);                 // 読み込むファイルの指定
    nl::json settings;                                          // jsonデータを格納するデータ構造

    // 設定ファイルの情報を変数に代入
    settings_json >> settings;                                  // 設定ファイルから読み込む
    ip_address = settings["ip_address"];                        // localhost:8000
    token = settings["token"];                                  // procon30_matchID-num_teamRED_FudaiKosen
    token = convert_num(token);                                 // procon30_matchID-7_teamRED_FudaiKosen
    // その他の変数に値を代入
    head_token = R"( -H "Authorization: )" + token + "\"";
    head_json = R"( -H "Content-Type: application/json")";
    url = " http://" + ip_address + "/matches/";
    // Console << Unicode::Widen(ip_address);
    // Console << Unicode::Widen(token);
}



// コンストラクタ
HttpController::HttpController() {
    // 設定ファイルを読み込む
    read_settings();

    // jsonファイルを格納するフォルダを作成する
    std::string directory_path = "md json\\formatted";          // mkdir 
    system(directory_path.c_str());                             // jsonフォルダ作成
}



// 試合事前情報取得
std::string HttpController::get_info() {
    // 前処理
    if (ip_address == "")read_settings();                       // 初期化処理がされていなければする
    std::string file_path = json_directory + info_json_name;    // json/info.json

    // 実行するコマンド文を作成する
    std::string send_text = "curl";                             // curl
    send_text += head_token;                                    // -H "Authorization: procon30_example_token"
    send_text += url;                                           // localhost:8000/matches
    send_text += " > " + file_path;                             // > json/info.json

    // コマンドを実行して、フォーマットされたjsonファイルも保存する
    int res = system(send_text.c_str());                        // curlコマンド実行
    if (res != 0)                                               // コマンド実行に失敗した場合
        Console << U"試合事前情報を取得できませんでした";       // エラーメッセージを出力
    format_json(json_directory, info_json_name);                // フォーマットを整えたjsonファイルを保存する
    return file_path;                                           // 保存したファイルパスを返す
}



// 試合状態取得
std::string HttpController::get_state() {
    // 前処理
    if (ip_address == "")read_settings();                       // 初期化処理がされていなければする
    std::string file_path = json_directory + state_json_name;   // json/state.json

    // 実行するコマンド文を作成する
    std::string send_text = "curl";                             // curl
    send_text += head_token;                                    // -H "Authorization: procon30_example_token"
    send_text += url;                                           // localhost:8000/matches
    send_text += std::to_string(match_id) + "/";                // 7/
    send_text += " > " + file_path;                             // > json/state.json

    // コマンドを実行して、フォーマットされたjsonファイルも保存する
    int res = system(send_text.c_str());                        // curlコマンド実行
    if (res != 0)                                               // コマンド実行に失敗した場合
        Console << U"試合状態を取得できませんでした";           // エラーメッセージを出力
    format_json(json_directory, state_json_name);               // フォーマットを整えたjsonファイルを保存する
    return file_path;                                           // 保存したファイルパスを返す
}



// 行動情報送信
void HttpController::post_actions(std::string& file_path) {
    // ポストする内容取得
    std::ifstream actions_json(file_path);                      // インプットファイルの指定　json/actions.json
    nl::json actions;                                           // jsonデータを格納するデータ構造
    actions_json >> actions;                                    // jsonファイルを読み込む
    std::string body_data = actions.dump();                     // jsonデータをstd::stringに変換

    // 結果を保存するファイルパスの設定
    std::string result_file_path = json_directory;              // json/
    result_file_path += result_json_name;                       // json/post_result.json

    // 実行するコマンド文を作成する
    std::string send_text = "curl";                             // curl
    send_text += head_token;                                    // -H "Authorization: procon30_example_token"
    send_text += head_json;                                     // -H "Content-Type: application/json"
    send_text += " -X POST";                                    // -X POST
    send_text += url;                                           // localhost:8000/matches
    send_text += std::to_string(match_id) + "/action/";         // 7/action/
    send_text += " -d";                                         // -d
    send_text += " \"";                                         // "
    send_text += add_backslash(body_data);                      // HTTP通信のボディデータ
    send_text += "\"";                                          // "
    send_text += " > " + result_file_path;                      // > json/post_result.json

    // コマンドを実行する
    int res = system(send_text.c_str());                        // curlコマンド実行
    if (res != 0)                                               // コマンド実行に失敗した場合
        Console << U"行動情報を送信できませんでした";           // エラーメッセージを出力
}