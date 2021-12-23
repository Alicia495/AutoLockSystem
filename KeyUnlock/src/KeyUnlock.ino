#include <Arduino.h>
#include <Servo.h>
#include <MsTimer2.h>
#include <EEPROM.h>
Servo key;//鍵のサーボの名前
/*
ラズパイとの通信規格
上位4bit アドレス
下位4bit データ
アドレス                    データ
0000 鍵の操作　             0施錠　1 開錠
0001 LEDの点灯許可          0不許可　1許可
0010 LEDのカラーサイクル　   0-15

その他アドレス　未実装
*/
//eepromのアドれす2に最期に操作した鍵の状態を保存してます 0で施錠　1で解錠

void rainbow();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  key.attach(6);
  pinMode(A1,INPUT);//A
  pinMode(A0,INPUT);//B

  pinMode(9,OUTPUT);//LEDのR
  pinMode(10,OUTPUT);//LEDのG
  pinMode(11,OUTPUT);//LEDのB
  pinMode(8,OUTPUT);//SD

  pinMode(A3, INPUT);//switch

  MsTimer2::set(200, corlor);//200msごとに色を変える（解錠時）
}


//0 130 65
bool flag = true;//鍵の状態を示すフラグ
bool SW = false;
bool LED = false;
char colorCycle = 0;

void loop() {
  // put your main code here, to run repeatedly:
  bool A = digitalRead(A1),B = digitalRead(A0); //A1のみがlowになると解錠、A0のみがlowになると施錠します
  digitalWrite(8,LOW);//サーボ電源用の降圧レギュレータ作動用のピン
  if(Serial.available()){//ラズパイでの操作受付
    char rawData = Serial.read();
    char addr = (rawData >> 4);
    char data = (rawData & 0x0F);

    if(addr == 0b0000){//鍵の操作
      if(data == 1) doorUnlock();
      else doorLock();
    }
    else if(addr == 0b0001){//LED点灯の許可
      if(data == 0) LED =false;
      else LED = true;
    }
    else if(addr == 0b0010){//LEDのカラーパターンの選択
      colorCycle = data;
    }

  }

  if(A3 == LOW && SW == false){//ボタンが押されたら施錠、開錠操作
    if(EEPROM.read(2) == 0){
      doorUnlock();
    }
    else{
      doorLock();
    }
    SW = true;
    delay(100);
  }
  if(A3 == HIGH && SW == true){//チャタリング対策
    SW = false;
  }
}

void doorLock(){
  MsTimer2::stop();
  digitalWrite(9,HIGH);
  digitalWrite(10,HIGH);
  digitalWrite(11,LOW);
  
  digitalWrite(8,HIGH);
  key.write(120);
  
  delay(2000);
  
  key.write(65);
  digitalWrite(9,LOW);
  digitalWrite(10,LOW);
  digitalWrite(11,LOW);
  for(int i = 0;i < 5;i++){
    digitalWrite(9,HIGH);
    digitalWrite(10,LOW);
    digitalWrite(11,LOW);
    delay(200);
    digitalWrite(9,LOW);
    digitalWrite(10,LOW);
    digitalWrite(11,LOW);
    delay(200);
    
  }
  
  EEPROM.write(2,0);
  
  flag = false;
}

void doorUnlock(){
  digitalWrite(9,HIGH);
  digitalWrite(10,HIGH);
  digitalWrite(11,LOW);
  
  digitalWrite(8,HIGH);
  key.write(0);
  
  delay(2000);
  
  key.write(65);
  digitalWrite(9,LOW);
  digitalWrite(10,LOW);
  digitalWrite(11,LOW);
  for(int i = 0;i < 5;i++){
    digitalWrite(9,LOW);
    digitalWrite(10,HIGH);
    digitalWrite(11,LOW);
    delay(200);
    digitalWrite(9,LOW);
    digitalWrite(10,LOW);
    digitalWrite(11,LOW);
    delay(200);
    
  }

  flag = true;
  EEPROM.write(2,1);
  MsTimer2::start();
}

int sel = 0;
void corlor(){ //7色で光らせる用の関数 順番に立ち上がる
  if(sel ==0){
    digitalWrite(9,HIGH);
    digitalWrite(10,LOW);
    digitalWrite(11,LOW);
  }
  else if(sel == 1){
    digitalWrite(9,LOW);
    digitalWrite(10,HIGH);
    digitalWrite(11,LOW);
  }
  else if(sel == 2){
    digitalWrite(9,HIGH);
    digitalWrite(10,HIGH);
    digitalWrite(11,LOW);
  }
  else if(sel == 3){
    digitalWrite(9,LOW);
    digitalWrite(10,LOW);
    digitalWrite(11,HIGH);
  }
  else if(sel == 4){
    digitalWrite(9,HIGH);
    digitalWrite(10,LOW);
    digitalWrite(11,HIGH);
  }
  else if(sel == 5){
    digitalWrite(9,LOW);
    digitalWrite(10,HIGH);
    digitalWrite(11,HIGH);
  }
  else if(sel == 6){
    digitalWrite(9,HIGH);
    digitalWrite(10,HIGH);
    digitalWrite(11,HIGH);
  }
  sel++;
  if(sel > 6) sel = 0;
}

int r = 255,g = 0,b = 0;//なめらかな虹色を作るための変数

void rainbow(){ //なめらかな虹色になる値を吐く関数　今は使用してない
  if(r >=255 && b <= 0 && g < 255) g++;
  else if(g >= 255 && b <= 0 && r > 0) r--;
  else if(g >= 255 && r <= 0 && b < 255) b++;
  else if(b >= 255 && r <= 0 && g > 0) g--;
  else if(b >= 255 && g <= 0 && r < 255) r++;
  else if(r >= 255 && g <= 0 && b > 0) b--;
  else{
    r = 255;
    g = 0;
    b = 0;
  }
}
