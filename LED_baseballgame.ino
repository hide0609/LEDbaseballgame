#define BUTTON 2
#define SPEAKER 3
#define BALL5 9
#define BALL4 10
#define BALL3 11
#define BALL2 12
#define BALL1 13
#define DURATION 500000

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
  Serial.print("Level:");
  Serial.println(level);
  Serial.print("Strike:");
  Serial.println(strike);
  Serial.println("Hit the button tostart");

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
}
