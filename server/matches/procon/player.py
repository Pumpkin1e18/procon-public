# -*- coding: utf-8 -*-
import random

# 貪欲法に基づき行動するプレイヤー
class PlayerGreedy:
    # コンストラクタ
    def __init__(self):
        pass
    
    # 次の行動を貪欲法によりセットする
    def set_actions(self, board, team):     # teamA か teamB について処理する
        for agent in team.agents:       # team の全てのエージェントに対して処理する
            next_hands = {}
            # 「あるエージェント」 がいるマスから、その9近傍を探索する
            for dy in [-1, 0, 1]:
                for dx in [-1, 0, 1]:
                    # team の　「あるエージェント」　が　「ある9近傍のマス」　に行った時の処理
                    now = agent.y*board.W + agent.x     # 今いるマス
                    nxt = now + (dy*board.W + dx)       # 次に移動するマス
                    if board.ok(now, nxt) == False: continue
                    value = board.points[nxt]       # nxtマスのタイルポイント
                    nxt_tiled = board.tiled[nxt]    # nxtマスがどっちのチームのものか
                    if nxt_tiled == team.teamID: value = 0
                    will_flip = False
                    if nxt_tiled != 0 and nxt_tiled != team.teamID: will_flip = True
                    next_hands.setdefault(value, []).append((dx, dy, will_flip))
            # 探索結果を格納する
            # next_handsの例） {-3: [(1, -1, 0)], 5: [(-1, 0, 1)], 7: [(0, 1, 1), (1, 1, 0)]}
            # next_handの例） (7: [(0, 1, 1), (1, 1, 0)])
            next_hand = sorted(next_hands.items(), reverse=True)[0]
            # actionの例） (0, 1, 1)
            action = random.choice(next_hand[1])
            agent.dx = action[0]     # action[0]の例） 0
            agent.dy = action[1]     # action[1]の例） 1
            agent.action_type = "move"
            if action[2]: agent.action_type = "remove"      # action[2]の例） 1
            
            
