# 8MhzAVRlib
## 内蔵8Mhz仕様への変更手順
1. Arudino as ISPの設定に従って、２つのatmega328Pを接続します。  
  16Mhzのマイコン（マイコンA）と、内蔵8Mhz駆動予定のマイコン（マイコンB)と呼びます。
2. マイコンAにArduinoISPを書き込みます
3. 書込装置をArsuino as ISPに変更する  
   ボードをArudinoPro or Pro miniに変更する  
   プロセッサを8Mhz 328Pに変更する
   マイコンBにブートローダーを書き込む
4. マイコンBにヒューズを書き込む（Low:0xE2 Hi:0xDA Ex:0xFD)  
avrdude -c avrisp -P /dev/cu.usbmodem22101 -b 19200 -p m328p -U lfuse:w:0xE2:m -U hfuse:w:0xdA:m -U efuse:w:0xFD:m
5. テストとして、src/example01.cppをコンパイルして実行する（bps=57600)   

## setTimeout機能
Timerクラスを利用することで、1ms単位でsetTimeout機能を使うことができます。  
setTimeout関数の仕様はJavascriptの同関数と似ています。
内部のタイマー0を使用します。
#### 使用方法
Timer::setTimeout(実行したい関数ポインタ,待ち時間(ms))  
実行できる関数は引数と返り値がvoidの関数です。
1. ```#include "src/Timer.h"```
2. Timer::init()を実行する
3. Timer::setTimeout(hoge,1000);//1000ms後にhogeを実行する
4. sei();while(true){};//割り込みを許可してください。無限ループを実行することで、プログラムのを実行させつづけます。
