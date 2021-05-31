#define BUTTON 2
#define SPEAKER 3
#define BALL5 9
#define BALL4 10
#define BALL3 11
#define BALL2 12
#define BALL1 13
#define DURATION 500000
#define DIST 0

unsigned long duration = DURATION;
int level = 0;
int strike = 0;
int last = 0;

void setup(){
  pinMode(BALL1,INPUT);
  pinMode(BALL2,INPUT);
  pinMode(BALL3,INPUT);
  pinMode(BALL4,INPUT);
  pinMode(BALL5,INPUT);
  pinMode(SPEAKER,OUTPUT);
  pinMode(BUTTON,INPUT_PULLUP);
  Serial.begin(9600);
}
boolean ball(int pin){
  tone(SPEAKER,880,20);
  pinMode(pin,OUTPUT);
  digitalWrite(pin,HIGH);
  last=pin;
  unsigned long start = micros();

  while(micros()-start<duration){
    if(digitalRead(BUTTON)==LOW){
      return true;
    }
  }
  digitalWrite(pin,LOW);
  pinMode(pin,INPUT);
  return false;
}

void loop(){
  int t;
  float k,v;
  
  Serial.print("Level:");
  Serial.println(level);
  Serial.print("Strike:");
  Serial.println(strike);
  Serial.println("Hit the button tostart");

//ゲームスタート時のレベル指定用条件式と、ボタンの設定
  if(level==0 || strike==0 || last==0){
    while(digitalRead(BUTTON)==HIGH){}
    while(digitalRead(BUTTON)==LOW){

//距離センサの値を読み取り、その値から実際の距離を算出する

      t=analogRead(DIST);
      v=(float)t*(float)0.0048;
      k=(float)25.352*pow(v,-0.999);
      
//距離が10cm以下の時は10cm、80cm以上の時は80cmに固定する
      if(k<10)
        k=10;
      else if(k>80)
        k=80;
   
//距離が30cm以下の時、0.25s毎にレベルを1上げていく
      if(k<30 && level!=20){
        ++level;
        duration*=0.8;
        strike=0;
        Serial.print(k,0);
        Serial.println("[cm]");
        Serial.print("Level:");
        Serial.println(level);
        Serial.print("Strike:");
        Serial.println(strike);
      }
      
      delay(250);
      
    }
  
  }
  delay(50);

//ボタンが押されてから離されるのを待つ
  while(digitalRead(BUTTON)==HIGH){}
  while(digitalRead(BUTTON)==LOW){}
  delay(50);

  if(ball(BALL1) || ball(BALL2) || ball(BALL3) || ball(BALL4)){
    ++strike;
    tone(SPEAKER,50,500);
    delay(700);
  }
  else if(ball(BALL5)){
    strike=0;
    ++level;
    duration*=0.8;
    tone(SPEAKER,544,300);
    delay(300);
    tone(SPEAKER,440,300);
    delay(300);
  }
  else{
    ++strike;
    tone(SPEAKER,50,500);
    delay(700);
  }
  
  while(digitalRead(BUTTON)==LOW){}
    Serial.println();
    digitalWrite(last,LOW);
    pinMode(last,INPUT);

  if(strike==3){
    Serial.print("Last Level:");
    Serial.println(level);
    Serial.println("Hit the button tostart over");
    tone(SPEAKER,50,500);
    delay(700);
    tone(SPEAKER,50,500);
    while(digitalRead(BUTTON)==HIGH){}
    duration=DURATION;
    level=strike=last=0;
  }
//level20をクリアしたとき、Perfect!!と表示し、ゲームを最初からにリセット
  if(level==21){
    Serial.println("Perfect!!");
    duration=DURATION;
    level=strike=last=0;
  }
}
