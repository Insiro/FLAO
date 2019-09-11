#define RED 9
#define GREEN 5
#define BLUE 6

void setup() {
  pinMode (RED, OUTPUT);//R
  pinMode (GREEN, OUTPUT);//G
  pinMode (BLUE, OUTPUT);//B
}

void loop() {
        analogWrite(RED, 100);
        analogWrite(BLUE, 50);
}
