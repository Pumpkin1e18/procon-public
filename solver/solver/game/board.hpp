#pragma once
#include <Siv3D.hpp>
#include <fstream>
#include "nlohmann/json.hpp"
#include "team.hpp"
#include "agent.hpp"

namespace nl = nlohmann;

class Board {
private:
    void set_team(Team &team, nl::json &team_state);
    void input_action(nl::json &action_state);

public:
    int height = 10;
    int width = 10;
    int match_id = 0;
    int interval_millis = 0;
    int turn_millis = 0;
    int max_turn = 0;
    int turn = 0;
    time_t started_unix_time = 0;
    int agent_num = 2;
    Grid<int> points;
    Grid<int> tiled;
    Agents agents;
    Teams teams;
    Team *my_team;      // 自分のチーム
    Team *op_team;      // 相手のチーム（opposite team）

    Board();
    Board(Team &my_team_, Team &op_team_);
    void set_my_team_color(int team_color);
    void set_info(std::string &file_path);
    void set_state(std::string &file_path);
    std::string write_actions();
};