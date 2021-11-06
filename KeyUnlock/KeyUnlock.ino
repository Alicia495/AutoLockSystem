#include <Servo.h>
#include <MsTimer2.h>
Servo key;//鍵のサーボの名前

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

void loop() {
  // put your main code here, to run repeatedly:
  int A = digitalRead(A1),B = digitalRead(A0); //A1のみがlowになると解錠、A0のみがlowになると施錠します
  digitalWrite(8,LOW);//サーボ電源用の降圧レギュレータ作動用のピン
  if(A != B){
    if(A == 0) {
      digitalWrite(9,HIGH);
      digitalWrite(10,HIGH);
      digitalWrite(11,LOW);
      
      digitalWrite(8,HIGH);
      key.write(0);
      
      delay(2000);
      
      key.write(65);
      delay(1000);
      
      digitalWrite(9,LOW);
      digitalWrite(10,HIGH);
      digitalWrite(11,LOW);
      
      delay(1000);
      flag = true;
      MsTimer2::start();
    }
    else if(B == 0) {
      MsTimer2::stop();
      digitalWrite(9,HIGH);
      digitalWrite(10,HIGH);
      digitalWrite(11,LOW);
      
      digitalWrite(8,HIGH);
      key.write(120);
      
      delay(2000);
      
      key.write(65);
      
      delay(1000);
      
      digitalWrite(9,HIGH);
      digitalWrite(10,LOW);
      digitalWrite(11,LOW);
      
      delay(1000);
      
      digitalWrite(9,LOW);
      digitalWrite(10,LOW);
      digitalWrite(11,LOW);
      
      flag = false;
    }
  }

  else if(digitalRead(A3) == LOW){
    MsTimer2::start();
    Serial.write("ON");
    digitalWrite(8, HIGH);
    key.write(0);
    delay(1000);

    key.write(65);
    delay(1000);
  }
  Serial.println(flag);
  delay(5);
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
}
