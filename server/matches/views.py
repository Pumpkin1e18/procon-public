# jsonファイルでのやり取り
# https://qiita.com/t-iguchi/items/ac9638dbdbe509515148
# curlでのpostのボディ情報の受け取り方
# https://cloud6.net/so/python/1488833
# ヘッダのauthorization情報の受け取り方
# https://www.it-swarm.jp.net/ja/python/django%E3%81%A7%E3%81%99%E3%81%B9%E3%81%A6%E3%81%AE%E3%83%AA%E3%82%AF%E3%82%A8%E3%82%B9%E3%83%88%E3%83%98%E3%83%83%E3%83%80%E3%83%BC%E3%82%92%E5%8F%96%E5%BE%97%E3%81%99%E3%82%8B%E3%81%AB%E3%81%AF%E3%81%A9%E3%81%86%E3%81%99%E3%82%8C%E3%81%B0%E3%82%88%E3%81%84%E3%81%A7%E3%81%99%E3%81%8B%EF%BC%9F/971100933/
# 配列を返す方法
# https://stackoverflow.com/questions/25963552/json-response-list-with-django
# jsonの文字化け回避
# https://qiita.com/MuuKojima/items/3ea452fd69756db9e815
# csrf対策を無効化する
# https://djangobrothers.com/blogs/django_csrf/
# post例
# curl -H "Authorization: procon30_matchID-7_teamA" -H "Content-Type: application/json" -X POST http://localhost:8000/matches/1/action/ -d "{\"actions\":[{\"agentID\":9, \"dx\":1, \"dy\":1, \"type\":\"move\"} , {\"agentID\":10, \"dx\":1, \"dy\":-1, \"type\":\"move\"}]}"

# Django関連のモジュール
from django.shortcuts import render
from django.http import JsonResponse
from django.http import HttpResponse
from django.views.decorators.csrf import csrf_exempt
# プロコン関連のモジュール
from .procon.board import Board
from .procon.player import PlayerGreedy
from .procon.sub_func import get_token, read_board
from .procon.sub_func import save_board, update_board
# その他pythonで一般的に使われるモジュール
import json


# /matches/
# 試合事前情報取得
@csrf_exempt
def get_info(request):
    match_id, team_type, name = get_token(request)
    if match_id == -1: return HttpResponse('InvalidToken')
    board = Board(match_id)
    res = board.create_info_dictionary(team_type)
    # ボードを保存する
    save_board(board)
    return JsonResponse(res)


# /matches/{matcheID}/
# 試合状態取得API
@csrf_exempt
def get_state(request, matchID):
    # トークンを読みこむ
    match_id, team_type, name = get_token(request)
    if match_id == -1: return HttpResponse('InvalidToken')
    if match_id != matchID: return HttpResponse('InvalidToken')
    # ボードを読み込み、盤面を最新のものに更新する
    board = read_board(match_id)
    if board == 1: return HttpResponse('File Not Found Error')
    if board == 2: return HttpResponse('File Open Error')
    board = update_board(board)
    board.calc_score(save=True)
    # レスポンスを作成する
    res = board.create_state_dictionary()
    # ボードを保存する
    save_board(board)
    return JsonResponse(res)


# /matches/{matchID}/action
# 行動更新API
@csrf_exempt
def post_action(request, matchID):
    # トークンを読みこむ
    match_id, team_type, name = get_token(request)
    if match_id == -1: return HttpResponse('InvalidToken')
    if match_id != matchID: return HttpResponse('InvalidToken')
    # ボードを読み込む
    board = read_board(match_id)
    if board == 1: return HttpResponse('File Not Found Error')
    if board == 2: return HttpResponse('File Open Error')
    # ポストのボディを取得する
    dic = json.loads(request.body)
    actions = dic['actions']
    # 盤面を最新のものに更新し、行動情報を更新する
    board = update_board(board)
    player_greedy = PlayerGreedy()
    if board.teamA.auto_greedy: player_greedy.set_actions(board, board.teamA)
    if board.teamB.auto_greedy: player_greedy.set_actions(board, board.teamB)
    board.update_actions(actions, team_type)
    # ボードを保存する
    save_board(board)
    return HttpResponse('OK')

    
