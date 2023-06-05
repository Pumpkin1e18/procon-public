#pragma once
#include <Siv3D.hpp>

class Agent {
public:
    int agent_id = 0;
    int x = 0;
    int y = 0;
    std::string action_type = "stay";
    int dx = 0;
    int dy = 0;
    int apply = 1;

    Agent();
    Agent(int id_);
    Agent(int id_, int x_, int y_);
    void set_action(int dx_, int dy_, bool will_flip);
};

using Agents = HashTable<int, Agent*>;   // Agents[agent_id, Agent*]