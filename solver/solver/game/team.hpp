#pragma once
#include <Siv3D.hpp>
#include "agent.hpp"
#include "proconsettings.hpp"

class Team {
public:
    int team_id = 0;
    int agent_num = 2;
    String name = U"FudaiKosen";
    Array<Agent> agents;
    int tile_point = 0;
    int area_point = 0;
    bool is_my_team = false;
    int color = Procon::Red;

    Team();
};

using Teams = Array<Team*>;