#define RED 9
#define GREEN 10
#define BLUE 11
#define PUMP1 2 //oil PUMP1 
#define PUMP2 4 //oil PUMP2
#define PUMP3 7 //oil PUMP3
#define PUMP4 8 //oil PUMP4
#define PUMP5 12 //water PUMP1
#define HUMIDIFIER 1 //가습기 모듈
#define LightSensor 6 //광측센서

void setup() {
  pinMode (RED, OUTPUT);//R
  pinMode (GREEN, OUTPUT);//G
  pinMode (BLUE, OUTPUT);//B
  pinMode (PUMP1, OUTPUT);
  pinMode (PUMP2, OUTPUT);
  pinMode (PUMP3, OUTPUT);
  pinMode (PUMP4, OUTPUT);
  pinMode (PUMP5, OUTPUT);
  pinMode (HUMIDIFIER, OUTPUT);
  pinMode (LightSensor, INPUT);
}

void loop() {
 Sunny();
 delay(3000);
 Cloudy();
 delay(3000);
 Rain();
 delay(3000);
 Sleep();

for(int j=0; j<100; j++){
 int Light = 0;
 Light = analogRead (LightSensor);
 Serial.print(Light);
 }
}


///////////////////////////////////////////////////////////////////////////////////
void Sunny() {
  digitalWrite(PUMP1, HIGH);
  delay(5000);
  digitalWrite(PUMP1, LOW);
  digitalWrite(PUMP5, HIGH);
  delay(5000);
  digitalWrite(PUMP5, LOW); 
  
    int blue_count= 0;
    int green_count = 0;
    int green_now = 0;
    for(blue_count=0; blue_count<255 ; blue_count++){
      analogWrite(BLUE, blue_count);
      delay(20);
      green_now++;
      if(green_now == 2){
        green_now = 0;
        green_count++;
        delay(20);
        analogWrite(GREEN, green_count);
      }
    }
    green_now = 0;
    for(blue_count = 255 ; blue_count >0 ; blue_count--){
      analogWrite(BLUE,blue_count);
      delay(20);
      green_now++;
      if(green_now == 2) {
        green_now = 0;
        green_count--;
        analogWrite(GREEN, green_count);
        delay(20);
      }
    }
  }
/////////////////////////////////////////////////////////////////////////////////// 

void Cloudy() {
  digitalWrite(PUMP2, HIGH);
  delay(5000);
  digitalWrite(PUMP2, LOW);
  digitalWrite(PUMP5, HIGH);
  delay(5000);
  digitalWrite(PUMP5, LOW);

  int blue_count= 0;
  int red_count = 0;
  int red_now = 0;
  
  for(blue_count=0; blue_count<255 ; blue_count++){
   analogWrite(BLUE, blue_count);
   delay(20);  
   red_now++;
   if(red_now == 1){
    red_now = 0;
    red_count++;
    delay(20);
    analogWrite(RED, red_count);
   }
  }
    red_now = 0;
 for(blue_count = 255 ; blue_count >0 ; blue_count--){
  analogWrite(BLUE,blue_count);
  delay(20);
  red_now++;
  if(red_now == 1){
    red_now = 0;
    red_count--;
    analogWrite(RED, red_count);
    delay(20);
  }
 }
}

///////////////////////////////////////////////////////////////////////////////////
void Rain() {
  digitalWrite(PUMP3, HIGH);
  delay(5000);
  digitalWrite(PUMP3, LOW);
  digitalWrite(PUMP5, HIGH);
  delay(5000);
  digitalWrite(PUMP5, LOW);
  

 int rain_color = 0;
 for(rain_color = 0 ; rain_color <=100 ; rain_color++)
 {
  analogWrite(RED,rain_color);
  analogWrite(GREEN,rain_color);
  analogWrite(BLUE,rain_color); delay(100);
 }
 delay(500);
 for(rain_color = 100 ; rain_color>=1 ; rain_color--)
 {
  analogWrite(RED,rain_color); 
  analogWrite(GREEN,rain_color); 
  analogWrite(BLUE,rain_color); delay(100);
 }
 
}
///////////////////////////////////////////////////////////////////////////////////
void Sleep() {
  digitalWrite(PUMP4, HIGH);
  delay(5000);
  digitalWrite(PUMP4, LOW);
  digitalWrite(PUMP5, HIGH);
  delay(5000);
  digitalWrite(PUMP5, LOW);
  
}
