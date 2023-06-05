#include "game/Board.hpp"

Board::Board(){}

Board::Board(Team &my_team_, Team &op_team_) {
    // my_team と op_team にteamオブジェクトのポインタを設定する
    my_team = &my_team_;
    op_team = &op_team_;
    // 自分のチームかどうか判定する変数を設定する
    my_team->is_my_team = true;
    op_team->is_my_team = false;
    // チームの色を設定する
    set_my_team_color(Procon::Red);
    // Teams teams に my_team と op_team
    teams << my_team;
    teams << op_team;
}

void Board::set_info(std::string &file_path) {
    std::ifstream input_info(file_path);
    nl::json info;
    input_info >> info;
    if (info.is_array() == false) {
        match_id = info["id"];
        interval_millis = info["intervalMillis"];
        op_team->name = Unicode::Widen(info["matchTo"]);
        my_team->team_id = info["teamID"];
        turn_millis = info["turnMillis"];
        max_turn = info["turns"];
        return;
    }
    for (auto match : info) {
        match_id = match["id"];
        interval_millis = match["intervalMillis"];
        op_team->name = Unicode::Widen(match["matchTo"]);
        my_team->team_id = match["teamID"];
        turn_millis = match["turnMillis"];
        max_turn = match["turns"];
    }
}

// チームの色を設定する
void Board::set_my_team_color(int team_color) {
    my_team->color = team_color;
    op_team->color = (team_color == Procon::Red) ? Procon::Blue : Procon::Red;
}


void Board::set_team(Team &team, nl::json &team_state) {
    team.team_id = team_state["teamID"];
    team.tile_point = team_state["tilePoint"];
    team.area_point = team_state["areaPoint"];
    bool first = (team.agents.size() == 0 ? true : false);
    size_t team_num = team_state["agents"].size();
    team.agents.resize(team_num);
    int i = 0;
    for (auto &agent_state : team_state["agents"]) {
        int agent_id = agent_state["agentID"];
        Agent *agent = (first ? &(team.agents[i]) : agents[agent_id]);
        agent->agent_id = agent_id;
        agent->x = agent_state["x"]-1;
        agent->y = agent_state["y"]-1;
        if (first)agents[agent_id] = agent;
        i++;
    }
}

void Board::input_action(nl::json &action_state) {
    int agent_id = action_state["agentID"];
    Agent *agent = agents[agent_id];
    agent->dx = action_state["dx"];
    agent->dy = action_state["dy"];
    agent->action_type = action_state["type"];
    agent->apply = action_state["apply"];
}


void Board::set_state(std::string &file_path) {
    std::ifstream input_state(file_path);
    nl::json state;
    input_state >> state;
    // int型の変数代入
    height = state["height"];
    width = state["width"];
    started_unix_time = time_t(state["startedAtUnixTime"]);
    turn = state["turn"];
    // 二次元配列の変数代入
    std::vector<std::vector<int>> points_vec, tiled_vec;
    for (auto vec : state["points"])points_vec.push_back(vec);
    for (auto vec : state["tiled"])tiled_vec.push_back(vec);
    points = Grid<int>(width, height);
    tiled = Grid<int>(width, height);
    for (auto y : step(height))for (auto x : step(width)) {
        points[y][x] = points_vec[y][x];
        tiled[y][x] = tiled_vec[y][x];
    }
    // チームの情報代入
    for (auto team_state : state["teams"]) {
        Team *team = op_team;
        if (team_state["teamID"] == my_team->team_id) team = my_team;
        set_team(*team, team_state);
    }
    // 行動情報代入
    for (auto action_state : state["actions"]) {
        // input_action(action_state);
    }
}


std::string Board::write_actions() {
    std::string file_path = "json/actions.json";
    std::ofstream output_actions(file_path);
    nl::json actions_json;
    std::vector<nl::json> actions;
    for (auto &agent : my_team->agents) {
        nl::json action;
        action["agentID"] = agent.agent_id;
        action["type"] = agent.action_type;
        action["dx"] = agent.dx;
        action["dy"] = agent.dy;
        actions.push_back(action);
    }
    actions_json["actions"] = actions;
    output_actions << actions_json.dump(4);
    return file_path;
}