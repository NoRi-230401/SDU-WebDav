# SDU-WebDav

SD-Updaterに対応したファイル管理（WebDav）ツールです。<br>
PC等から直接、M5Stack Core2のSD/SPIFFSにアクセスすることができるようになります。




## 設定ファイル
設定サンプルを変更してSD直下にコピーしてください。<br>
  
- wifi.txt<br>
  WiFi設定のファイル<br>
  １行目：SSID<br>
  ２行目：SSIDのPASSWD<br>

<br>  

## 対応ハードウエア
### 本体:　 M5Sstack Core2
- M5Stack Core2 for AWS（動作確認している機種）<br>
- M5Stack Core2 <br>
- M5Stack Core2 v1.1　<br>
<br>
SDは、相性があるので注意。<br>
<b>※本ソフトウエアの使用により、ファイル等が破損しても責任を取れませんのでご了承の上ご使用ください。</b>
<br><br><br>



## 使用方法
### 電源投入
電源投入直後のブート時に SD-UpdaterのLobby画面が現れます。<br>
その後、ソフトが立ち上がり、wifi等の設定が完了しますと接続情報が画面に表示されます。<br>
画像から、FileSystemは、SDを選択が確認できます。（初期値）<br>
![画像](images/s-webdav01.jpg)<br><br>

<br>

### PCからのアクセス
PCは、win11で説明します。
ファイルエクスプローラーの画面上部で場所の入力箇所に、全て半角の<br>
<b>"￥￥webDav￥￥DavWWWRoot"</b>　と入力（￥マークも半角で入力）<br>
webDav(HOSTNAME)の代わりに、<b>IPアドレス</b>を入れることもできます。<br><br>
![画像](images/s-pc01.png )<br>

接続が完了しますと、PCから通常の操作でM5StackCore2のSDまたは、SPIFFSのファイルを操作することができるようになります。<br><br>
PCアイコンを右クリックするとネットワークドライブとして使用することもできます。<br>
<br><br>

### Special Mode : (BtnB)の長押し<br>
Special Modeの画面になり、FileSystemの変更(SD <-> SPIFFS)等ができます。<br>
![画像](images/s-webdav02.jpg )<br>

FileSystemが変更されますと、一度リブートします。その後、変更後のFileSystemが有効になります。立上げ時に有効なFileSystemが表示されます。

<br><br>

## WebDavについて
私は、PC（Win11）でしか確認していませんが、スマホや他の機種のPCでも接続できそうです。<br>
以下の情報を基に作成しました。
<br><br>

GitHub<br>
https://github.com/d-a-v/ESPWebDAV<br>

たなかまさゆき さん(@tnkmasayuki)<br>
https://lang-ship.com/blog/work/esp32-webdav/#toc3<br><br>

リナちゃん@chrmlinux03<br>
https://qiita.com/chrmlinux03/items/5d65893760dad6f4bee0<br>

<br><br><br>