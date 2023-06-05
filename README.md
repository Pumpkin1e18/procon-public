# Procon30 OPUCT

第30回高専プロコンのソルバとゲームサーバです。これは実際に本番で使ったものではなく講義用にOpenSiv3Dを使って作り直したものです。ソルバに思考エンジンは積んでません。本番で使用したソースコードは [procon29](https://github.com/pumpkin1e18opuct/procon29) や [procon30](https://github.com/pumpkin1e18opuct/procon30) においてます。



## デモ
<img src="resource\demo.gif" alt="demo" style="zoom:110%;" />

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


## ソルバの仕様
自分チームのエージェント（猫で表現）を左クリックし、選択した状態で移動したいマスを左クリックすることで、そのエージェントの移動方向を指定することができる（移動方向は猫の向きで表現）。また、タイルをはがすように指示したい場合は、エージェントを選択した状態で、はがしたいマスを右クリックすることでできる。

ボタンをいくつも用意している。まず左上のボタンは飾りで、押しても何も起こらない。右下のボタンは手動でHTTP通信していた時の名残で、自動でHTTP通信をするようなった今、押してもゲームの進行上特に変わりはない。ただ、「自分と相手の色を交換する」というボタンは、相手と色を交換できる唯一意味のあるボタンになっている。

このソルバに思考エンジンは積んでいない。思考エンジンのソースコードが見たければ、実際に[procon29](https://github.com/pumpkin1e18opuct/procon29)や[procon30](https://github.com/pumpkin1e18opuct/procon30)で使ったリポジトリのソースコードを見に行ってほしい。
