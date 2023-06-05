# -*- coding: utf-8 -*-
import re
import time
import pickle
import os

# トークンを取得する
def get_token(request):
    token_error = (-1, -1, -1)
    # トークンの取得
    try:
        token = ''
        if 'HTTP_AUTHORIZATION' in request.META == False: return token_error
        token = request.META.get('HTTP_AUTHORIZATION')
        # トークンから「試合のID」と「チームのタイプ（REDかBLUE）」と「高専名」の情報を抜き取る
        pattern = r'^procon30_matchID-(\d+)_team(.+)_(.+)$'     # 例) procon30_matchID-7_teamRED_FudaiKosen
        result = re.search(pattern, token)
        if result == None: return token_error
        match_id = result.groups()[0]       # 例) 7
        team_type = result.groups()[1]      # 例) RED
        name = result.groups()[2]           # 例) FudaiKosen
        # 適切でないトークンはエラー処理
        if match_id.isdecimal() == False: return token_error
        match_id = int(match_id)
        if match_id <= 0 or 100 < match_id: return token_error
        if team_type != 'RED' and team_type != 'BLUE': return -token_error
    except:
        return token_error
    return match_id, team_type, name


# ボードを読み込む
def read_board(match_id):
    file_name = 'pickle/match' + str(match_id) + '.pickle'
    try:
        with open(file_name, 'rb') as f:
            board = pickle.load(f)
    except FileNotFoundError: return 1    # File Not Found Error
    except: return 2    # File Open Error
    return board


# ボードを保存する
def save_board(board):
    match_id = board.match_id
    os.makedirs('pickle/', exist_ok=True)   # exist_ok=Trueにすることで既に存在するディレクトリを指定してもエラーにならない
    file_name = 'pickle/match' + str(match_id) + '.pickle'
    with open(file_name, 'wb') as f:
        pickle.dump(board, f)


# ボードの情報を更新する
def update_board(board):
    elapsed_time = int(time.time())*1000 - board.startedAtUnixTime
    elapsed_turn = elapsed_time // (board.intervalMillis + board.turnMillis)
    diff_turn = elapsed_turn - board.turn       # 前のアクセスからどれだけターンが進んだか
    if elapsed_time < 0: return board       # 試合がまだ始まっていない場合何もしない
    
    if 100 < diff_turn: return 0
    for i in range(diff_turn):
        board.move()
    board.turn = elapsed_turn
    return board