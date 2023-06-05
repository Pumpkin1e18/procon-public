# -*- coding: utf-8 -*-
# Agentクラス
class Agent:
    # コンストラクタ
    def __init__(self, agentID, x, y):
        self.agentID = agentID
        self.x = x
        self.y = y
        self.action_type = "stay"
        self.dx = 0
        self.dy = 0
        self.apply = 1

    # 転置
    def transpose(self):
        self.x, self.y = self.y, self.x

    # 試合状態取得APIのレスポンスの一部, 行動結果の送信用辞書の作成
    def create_result_dictionary(self, turn):
        action = {}
        action["actionID"] = self.agentID
        action["dx"] = self.dx
        action["dy"] = self.dy
        action["type"] = self.action_type
        action["apply"] = self.apply
        action["turn"] = turn
        return action
