# M5Stack_DHT12_TemperatureToIFTTT

M5Stackとプロトタイピングキットについてくる温度湿度センサDHT12を使って、部屋の温度をIFTTTのWebhooksに投げるやつ

## ブログ書いた

- [M5StackとDHT12で作る部屋の温度湿度をIFTTTのWebhooksへ投げるやつ | Memorandum](https://nagatani.github.io/posts/m5stack-dht12-temptoifttt/)

## やってること

- NTPサーバーにつないで時刻調整
    - 時間単位で温度湿度をIFTTTに投げるので正確な時刻を設定する
- DHT12から温度湿度取得
    - 熱中症には温度だけじゃなく湿度も重要らしい
- HTTPのGETを使ってIFTTTのWebhooksへリクエストの送信
    - IFTTT側でSlackにデータを投げてくれる

## 使い方

M5Stack と、 DHT12を買う。

Arduinoの開発環境をセットアップ後、M5Stack用の諸々と専用ライブラリをセットアップする。

IFTTT Webhooksの設定をする。

`Config.cpp`にWifi接続設定と、IFTTT Webhooksの設定があるので変更する。

現在は、1hごとに温度湿度を取得し、Webhooksへ送信している。  
もし、温度上昇警告をする場合は、Webhooks送信呼び出しを温度上昇で文字色変更している箇所があるのでそのへんに移動すると良い。

コンパイルと書き込みをする。

以上
