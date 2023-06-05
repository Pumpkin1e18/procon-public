## 概要
第30回高専プロコンのソルバとゲームサーバです。

これは実際に本番で使用したものではなく後輩の勉強用にOpenSiv3Dを使って作り直したものです。思考エンジンを積んだ本番で使用したソースコードは[procon29](https://github.com/Pumpkin1e18/procon29-public)や[procon30](https://github.com/Pumpkin1e18/procon30-public)においてます。


## 使い方
サーバの実行に必要なpythonパッケージのインストール  
```
$ pip install numpy
$ pip install django
```
ローカルサーバの実行
```
$ cd procon30-opuct/server/
$ python manage.py runserver
```
ソルバの実行  
`solver/solver/App/solver.exe`をダブルクリックで実行

## デモ
<img src="resource\demo.gif" alt="demo" style="zoom:110%;" />

## 操作方法
1. 自分チームのエージェント（ネコで表現）を左クリックして選択する  
2. 移動したい場合: 移動したいマスを左クリックする（猫の向きで表現）  
   タイルをはがしたい場合: はがしたいタイルを右クリックする  

「自分と相手の色を交換する」のボタン以外はプロトタイプで使っていたボタンの名残で、現在のバージョンでは押しても何も起こらない
