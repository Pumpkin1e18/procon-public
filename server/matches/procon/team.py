# -*- coding: utf-8 -*-
# Teamクラス
class Team:
    # コンストラクタ
    def __init__(self, teamID, agentNum, agents):
        self.teamID = teamID
        self.agentNum = agentNum
        self.agents = agents
        self.tilePoint = 0
        self.areaPoint = 0
        self.token = ""
        self.auto_greedy = True
    
    # 試合状態取得APIのレスポンスの一部, 受信情報送信用辞書の作成
    def create_state_dictionary(self):
        agents_dic = []
        for agent in self.agents:
            agent_action = {}
            agent_action["agentID"] = agent.agentID
            agent_action["x"] = agent.x+1
            agent_action["y"] = agent.y+1
            agents_dic.append(agent_action)
        team_info = {}
        team_info["teamID"] = self.teamID
        team_info["tilePoint"] = self.tilePoint
        team_info["areaPoint"] = self.areaPoint
        team_info["agents"] = agents_dic
        return team_info
