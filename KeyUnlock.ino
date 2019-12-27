#include <Servo.h>
#include <MsTimer2.h>
Servo key;

void rainbow();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  key.attach(6);
  pinMode(A1,INPUT);//A
  pinMode(A0,INPUT);//B

  pinMode(9,OUTPUT);//R
  pinMode(10,OUTPUT);//G
  pinMode(11,OUTPUT);//B
  pinMode(8,OUTPUT);//SD

  MsTimer2::set(200, corlor);
}

int r = 255,g = 0,b = 0;

//0 130 65
bool flag = true;
void loop() {
  // put your main code here, to run repeatedly:
  int A = digitalRead(A1),B = digitalRead(A0);
  digitalWrite(8,LOW);
  //rainbow();
  if(A != B){
    if(A == 0) {
      digitalWrite(9,HIGH);
      digitalWrite(10,HIGH);
      digitalWrite(11,LOW);
      digitalWrite(8,HIGH);
      key.write(0);
      //Serial.println(1000);
      delay(2000);
      key.write(65);
      delay(1000);
      digitalWrite(9,LOW);
      digitalWrite(10,HIGH);
      digitalWrite(11,LOW);
      delay(1000);
      flag = 1;
      MsTimer2::start();
    }
    else if(B == 0) {
      MsTimer2::stop();
      digitalWrite(9,HIGH);
      digitalWrite(10,HIGH);
      digitalWrite(11,LOW);
      digitalWrite(8,HIGH);
      key.write(120);
      //Serial.println(-1000);
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
      flag = 0;
    }
  }
  Serial.print(A);
  Serial.println("  ");
  Serial.println(B);
  delay(5);
}

int sel = 0;
void corlor(){
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

void rainbow(){
  if(r >=255 && b <= 0 && g < 255) g++;
  else if(g >= 255 && b <= 0 && r > 0) r--;
  else if(g >= 255 && r <= 0 && b < 255) b++;
  else if(b >= 255 && r <= 0 && g > 0) g--;
  else if(b >= 255 && g <= 0 && r < 255) r++;
  else if(r >= 255 && g <= 0 && b > 0) b--;
}
