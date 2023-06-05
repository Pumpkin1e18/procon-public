#include "game/Agent.hpp"

Agent::Agent() {}

Agent::Agent(int id_) { agent_id = id_; }

Agent::Agent(int id_, int x_, int y_) {
    agent_id = id_;
    x = x_;
    y = y_;
}

void Agent::set_action(int dx_, int dy_, bool will_flip) {
    dx = dx_;
    dy = dy_;
    if (will_flip == true) action_type = "remove";
    if (will_flip == false) action_type = "move";
    if (dx == 0 and dy == 0) action_type = "stay";
}