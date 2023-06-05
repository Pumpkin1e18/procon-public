# -*- coding: utf-8 -*-
import random
import numpy as np
import time
from .boardcreator import create_board

# 1ターンは作戦ステップと遷移ステップから成る
# 作戦ステップで、各チームはエージェントの行動を決定してサーバに送信する
# 遷移ステップでは両チームが指示したエージェントの行動を反映し、フィールド情報を更新する

# タイル除去や移動など行動の指示が違っても、同じタイルに行動を行ったエージェントは全て停留扱い
# 行動を送信できなかった場合も停留扱い
# フィールド範囲外への行動やタイルがない場所へのタイル除去の行動等は無効扱い
# 相手チームのタイルがある場所への移動命令やデータの書式が不適切な場合も無効扱い
# つまり明らかにおかしい送信データを無効扱いし、そのような行動を除外した行動が競合すれば停留扱いになる


# Boardクラス
class Board:
    # コンストラクタ
    def __init__(self, match_id):
        H, W, points, tiled, agentNum, teamA, teamB = create_board(match_id)
        self.H = H      # タイルが縦方向にいくつ並んでるか
        self.W = W      # タイルが横方向にいくつ並んでるか
        self.match_id = match_id        # 試合ID
        self.intervalMillis = 5000      # 遷移ステップの時間
        self.turnMillis = 10000     # 行動送信の時間
        self.points = points    # タイルポイント
        self.tiled = tiled      # そのタイルにエージェントが配置されていたらチームIDが入る
        self.agentNum = agentNum    # 1チームのエージェントの数
        self.teamA = teamA      # teamAのエージェントオブジェクトの配列
        self.teamB = teamB
        self.points = self.points.ravel()   # 二次元配列から一次元配列にする
        self.tiled = self.tiled.ravel()
        self.turn = 0
        self.startedAtUnixTime = int(time.time()*1000 + 5*1000)  # UNIX時間（ミリ秒）
        

    # 不正な移動でないかを判定する
    def ok(self, now, nxt):
        if nxt < 0 or self.H*self.W <= nxt: return False       #範囲外アクセス防止
        if abs(nxt%self.W - now%self.W) > 1: return False      #ワープ防止
        return True
        

    # 領域ポイントを計算するためのDFS（深さ優先探索）
    def dfs(self, now, team_id):
        if self.visited[now] == True: return 0
        if self.tiled[now] == team_id: return 0
        self.visited[now] = True
        area_point = 0
        ng = False
        for dv4 in [-self.W, -1, 1, self.W]:
            nxt = now + dv4
            if self.ok(now, nxt) == False:
                ng = True
                continue
            ret = self.dfs(nxt, team_id)
            if ret == -1: ng = True
            area_point += ret
        if ng == True: return -1
        return area_point + abs(int(self.points[now]))
        

    # 得点を計算する
    def calc_score(self, save=False):
        score = []
        for team in self.teamA, self.teamB:
            self.visited = [False]*self.H*self.W
            tile_score = 0
            area_score = 0
            for i in range(self.H*self.W):
                area_score += max([self.dfs(i, team.teamID), 0])
                if self.tiled[i] == team.teamID: tile_score += int(self.points[i])
            score.append(tile_score + area_score)
            if save == True:
                team.tilePoint = tile_score
                team.areaPoint = area_score
        return score
        

    # 行動する
    def move(self):
        # 変数作成
        agents = []     # エージェントのオブジェクトが入る
        now_position = []       # 今の場所
        next_position = []      # 次の移動場所
        will_flip = []      # action_typeが"remove"かどうか
        will_stay = []      # action_typeが"stay"かどうか
        will_move = []      # action_typeが"move"かどうか
        count_agents = [0]*self.H*self.W       # その場所に行こうとしてるエージェントの人数
        for team in self.teamA, self.teamB:
            for agent in team.agents:
                agents.append(agent)
                now_position.append(agent.y*self.W + agent.x)
                next_position.append((agent.y+agent.dy)*self.W + (agent.x+agent.dx))
                will_flip.append(agent.action_type == "remove")
                will_stay.append(agent.action_type == "stay")
                will_move.append(agent.action_type == "move")
                print(agent.action_type)

        # エージェントを一人づつ処理していく
        for i in range(2*self.agentNum):
            agent = agents[i]
            agent.apply = 1
            teamID = {0: self.teamB.teamID, 1: self.teamA.teamID}
            opposed_teamID = teamID[i//self.agentNum]
            # 無効の行動の列挙
            is_invalid = False
            if self.ok(now_position[i], next_position[i]) == False:
                next_position[i] = now_position[i]
                is_invalid = True
            if will_flip == True and self.tiled[next_position[i]] == 0: is_invalid = True
            if will_flip == True and (agent.x == 0 and agent.y == 0): is_invalid = True
            if will_stay == True and (agent.x != 0 or agent.y != 0): is_invalid = True
            if will_move == True and self.tiled[next_position[i]] == opposed_teamID: is_invalid = True
            # 行動が無効判定されたのエージェントの処理
            if is_invalid:
                next_position[i] = now_position[i]
                agent.apply = -1
            # count_agents更新
            count_agents[next_position[i]] += 1
        
        # 移動できない人たちの処理
        for i in range(2*self.agentNum):
            # タイル除去に成功したエージェントの処理
            if count_agents[next_position[i]] <= 1 and will_flip[i] == True:
                self.tiled[next_position[i]] = 0
                next_position[i] = now_position[i]
            # 他のエージェントの競合したため停留
            if count_agents[next_position[i]] > 1:
                next_position[i] = now_position[i]
                agents[i].apply = 0
        
        # 最終的に移動する位置とその場所のタイルを更新
        for i in range(2*self.agentNum):
            teamID = {0: self.teamA.teamID, 1: self.teamB.teamID}
            self_teamID = teamID[i//self.agentNum]
            now_position[i] = next_position[i]
            self.tiled[next_position[i]] = self_teamID
            
        # エージェントの動きを適用する
        i = 0
        for team in self.teamA, self.teamB:
            for agent in team.agents:
                agent.y = int(now_position[i]//self.W)
                agent.x = int(now_position[i]%self.W)
                i += 1


    # 試合事前情報取得APIのレスポンスの作成
    def create_info_dictionary(self, team_type):
        self.turn = 0
        self.startedAtUnixTime = int(time.time()*1000 + 5*1000)  # UNIX時間（ミリ秒）
        team_index = {'RED': 0, 'BLUE': 1}
        opposed_team_type = {'RED': 'BLUE', 'BLUE':'RED'}
        info = {}
        info["id"] = self.match_id
        info["intervalMillis"] = self.intervalMillis
        info["matchTo"] = opposed_team_type[team_type] + '_Kosen'
        info["teamID"] = int(2*self.match_id + team_index[team_type] - 1)
        info["turnMillis"] = self.turnMillis
        info["turns"] = random.randint(30, 60)
        return info


    # 試合状態取得APIのレスポンスの作成
    def create_state_dictionary(self):
        teamA_dic = self.teamA.create_state_dictionary()
        teamB_dic = self.teamB.create_state_dictionary()
        agent_actions = []
        for team in self.teamA, self.teamB:
            for agent in team.agents:
                agent_actions.append(agent.create_result_dictionary(self.turn))
        state = {}
        state["height"] = self.H
        state["width"] = self.W
        state["turn"] = self.turn
        state["startedAtUnixTime"] = self.startedAtUnixTime
        state["points"] =  self.points.reshape(self.H, self.W).copy().tolist()
        state["tiled"] = self.tiled.reshape(self.H, self.W).copy().tolist()
        state["teams"] = [teamA_dic, teamB_dic]
        state["action"] = agent_actions
        return state


    # 行動更新API
    def update_actions(self, actions, team_type):
        # 前処理
        team = self.teamA
        if team_type == "B": team = self.teamB
        team.auto_greedy = False
        agents = []
        for agent in team.agents:
            agents.append(agent)
        # 本処理
        for action in actions:
            # 変数取り出し
            agent_id = action["agentID"]
            action_type = action["type"]
            dx = action["dx"]
            dy = action["dy"]
            # 変数検証
            if str(agent_id).isdecimal == False: continue
            if agent_id < agents[0].agentID: continue
            if agents[0].agentID + self.agentNum < agent_id: continue
            if action_type != "move" and action_type != "remove" and action_type != "stay": continue
            if str(dx).isdecimal == False or str(dy).isdecimal == False: continue
            if dx < -1 or 1 < dx: continue
            if dy < -1 or 1 < dy: continue
            # 代入
            for agent in team.agents:
                if agent.agentID != agent_id: continue
                agent.action_type = action_type
                agent.dx = int(dx)
                agent.dy = int(dy)
                print("agent_id", agent_id, ", dx:", agent.dx, ", dy:", agent.dy)
            
            
            