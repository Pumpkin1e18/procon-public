# -*- coding: utf-8 -*-
import random
import numpy as np
from .team import Team
from .agent import Agent

# Procon29
#盤面のサイズ 10*10 ~ 20*20
#各マスの点数 -16 ~ 16
#最大ターン数は事前に分かる
#最大12試合,24チームが一度に行う
#負のポイントは20％程度(確か)


# -16 ~ 16 からランダムに整数を得る
def get_num():
    score = random.randint(0, 16)
    if(random.randint(1, 10) <= 2):score = -score
    return score


# ランダムボードの作成
def create_board(match_id):
    # ボードの作成
    H = random.randint(10, 20)      # タイルが縦方向にいくつ並んでるか
    W = random.randint(10, 20)      # タイルが横方向にいくつ並んでるか
    top_of_board = np.array([get_num() for i in range((H//2)*W)]).reshape(H//2, W)
    down_of_board = np.flipud(top_of_board).copy()
    points = np.vstack([top_of_board, down_of_board])
    if H%2 == 1:    # Hが奇数なら一行追加
        middle_of_board = np.array([get_num() for i in range(W)])
        points = np.vstack([top_of_board, middle_of_board, down_of_board])
    # エージェントの配置
    tiled = np.zeros(H*W).reshape(H, W)     # そのタイルにエージェントが配置されていたらチームIDが入る
    agentNum = random.randint(2, 8)     # 1チームのエージェントの数
    teamA_agents = []       # teamAのエージェントオブジェクトの配列
    teamB_agents = []
    teamA_id = 2*match_id - 1     # teamAのチームid
    teamB_id = teamA_id + 1
    startAgentID = random.randint(1, 100)
    agentID = startAgentID
    while agentID < startAgentID + agentNum:    # エージェントのオブジェクトをチームのagents配列に入れる
        teamA_y = random.randrange(0, H)   # エージェントIDがagentIDのエージェントのy座標
        teamA_x = random.randrange(0, W)
        if H%2 == 1 and teamA_y == H//2: continue
        if tiled[teamA_y, teamA_x] == 1: continue
        teamB_y = (H-1)-teamA_y     # エージェントIDがagentID+agentNumのエージェントのy座標
        teamB_x = teamA_x
        teamA_agents.append(Agent(agentID, teamA_x, teamA_y))
        teamB_agents.append(Agent(agentID+agentNum, teamB_x, teamB_y))
        tiled[teamA_y, teamA_x] = teamA_id
        tiled[teamB_y, teamB_x] = teamB_id
        agentID += 1
    # 1/2の確率で盤面を転置する
    if random.randint(0, 1) == 0:
        H, W = W, H
        points = points.T
        tiled = tiled.T
        for agent in teamA_agents: agent.transpose()
        for agent in teamB_agents: agent.transpose()
    # チームの作成
    teamA = Team(teamA_id, agentNum, teamA_agents)
    teamB = Team(teamB_id, agentNum, teamB_agents)
    return H, W, points, tiled, agentNum, teamA, teamB

# get_board()

