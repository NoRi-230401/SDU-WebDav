# SDU-WebDav

SD-Updaterに対応したファイル管理（WebDav）ツールです。<br>
PC等から直接、M5Stack Core2のSD/SPIFFSにアクセスすることができるようになります。<br>


<b>（※注意）</b><br>
本ソフトウエア使用により、ファイル等の破損などで損害が発生しても責任を取ることができません。
ご了承の上、ご利用ください。<br>
<br>



## 設定ファイル
設定サンプルを変更してSD直下にコピーしてください。<br>
  
- wifi.txt<br>
  WiFi設定のファイル<br>
  １行目：SSID<br>
  ２行目：SSIDのPASSWD<br>

<br>  

## 対応ハードウエア
### 本体:　 M5Stack Core2
- M5Stack Core2 for AWS（動作確認している機種）<br>
- M5Stack Core2 <br>
- M5Stack Core2 v1.1　<br>
<br>

### SD:　 FAT32 format
SDは、相性等（？）で、動作が不安定になるものがありますので注意が必要です。</b><br>
<br><br>



## 使用方法
### 電源投入
電源投入直後のブート時に SD-UpdaterのLobby画面が現れます。<br>
その後、ソフトが立ち上がり、wifi等の設定が完了しますと接続情報が画面に表示されます。<br>
画像から、FileSystemは、SDを選択されていることが確認できます。（初期値）<br><br>
![画像](images/s-webdav01.jpg)<br><br>

<br>

### PCからのアクセス
PCは、win11での説明です。<br>
win11のOS標準のファイル・エクスプローラーを使用します。<br>
画面上部の位置を入力箇所に、全て半角文字で<br>
 - <b>"￥￥webDav￥DavWWWRoot"</b>　と入力してください。<br><br>

または、webDav(HOSTNAME)の代わりに、<b>IPアドレス</b>を入れることもできます。<br>
 - <b>(例)　"￥￥192.168.0.16￥DavWWWRoot"</b>

<br><br>
![画像](images/s-pc01.png )<br>

接続が完了しますと、ファイル・エクスプローラーで本体(5StackCore2)のSDまたは、SPIFFSのファイルを操作することができるようになります。<br><br>

 - 画面にショートカットを作成するとアクセスが簡単になります。<br>

 - デスクトップの「PC」アイコンを右クリックし、ネットワークドライブの割り当てで使用することもできます。<br>

 - スマホ(Android/iPhone)やPC(Mac/Linuex)等でもWebDavは、接続できるようです。<br>
 （未確認）<br>
 <br>

<br><br>

### Special Mode : (BtnB)の長押し<br>
Special Modeの画面になり、FileSystemの変更(SD <-> SPIFFS)等ができます。<br>
![画像](images/s-webdav02.jpg )<br>
<br>
FileSystemが変更されますと、一度リブートします。<br>
その後、変更後のFileSystemが有効になります。<br>

<br><br>

## 参照情報
以下の情報を参照しました。<br>

ESPWebDAVライブラリ<br>
https://github.com/d-a-v/ESPWebDAV<br>

たなかまさゆき さん(@tnkmasayuki)<br>
https://lang-ship.com/blog/work/esp32-webdav/<br>

リナちゃん@chrmlinux03<br>
https://qiita.com/chrmlinux03/items/5d65893760dad6f4bee0<br>

このソフトのGitHub<br>
https://github.com/NoRi-230401/SDU-WebDav<br>

<br><br><br>