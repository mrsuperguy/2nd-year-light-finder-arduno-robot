//LDR pair
#define pinLdrRight A0
#define pinLdrLeft A3
short ldrRightValue;
short ldrLeftValue;

//LED pair
#define pinLedRight 11
#define pinLedLeft 10
byte ledRightValue;
byte ledLeftValue;

//Motors
#define pinLeftForward 3
#define pinLeftReverse 2
#define pinRightForward 4
#define pinRightReverse 5

//ultrasonic
#define pinTrig 7
#define pinEcho 6

long duration;
int distance;

#define pinBuzz 13

#define pinPot A6
short threshold;
short avgLdrValue;

#define pinBtn 12

void setup() {
  pinMode(pinLdrRight, INPUT);
  pinMode(pinLdrLeft, INPUT);

  pinMode(pinLeftForward, OUTPUT);
  pinMode(pinLeftReverse, OUTPUT);
  pinMode(pinRightForward, OUTPUT);
  pinMode(pinRightReverse, OUTPUT);

  pinMode(pinTrig, OUTPUT);
  pinMode(pinEcho, INPUT);

  pinMode(pinBuzz, OUTPUT);

  pinMode(pinPot, INPUT);

  pinMode(pinBtn, INPUT_PULLUP);
  
  Serial.begin(9600);

  while (digitalRead(pinBtn) == HIGH) {

    // The right hand LED represents the average of the 2 LDRs
    // The left hand LED represents the potentiometer
    // Calibration is done by shining the light source onto the robot from a distance and adjusting
    // the potentiometer so that the left hand LED is a bit dimmer than the right hand one. 
    // If the two values are equal, the buzzer will sound to announce this.
    
    threshold = analogRead(pinPot);
    avgLdrValue = (analogRead(pinLdrLeft) + analogRead(pinLdrRight)) / 2;
    ldrRightValue = map (avgLdrValue, 0, 1023, 0, 255);
    ldrLeftValue = map (threshold, 0, 1023, 0, 255);
    analogWrite(pinLedRight, map (avgLdrValue, 0, 1023, 0, 255));
    analogWrite(pinLedLeft, map (threshold, 0, 1023, 0, 255));

    if (((ldrRightValue - ldrLeftValue) <= 15) && ((ldrRightValue - ldrLeftValue) >= 0)) {
      Buzz(5);
    }
/*
    Serial.println("Average LDR value:   " + String(ldrRightValue));
    Serial.println("Potentiometer value: " + String(ldrLeftValue));
    delay(150);*/
  }
}

void loop() {
/*
  for (int i = 0; i > 25; i++) {
    Buzz(i);
  }
*/  
  digitalWrite(pinTrig, LOW);
  delayMicroseconds(2);
  
  digitalWrite(pinTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinTrig, LOW);
  
  duration = pulseIn(pinEcho, HIGH);
  distance = duration * 0.034 / 2;

  if (distance <= 4) {
    //Serial.println("I found the light!!!!");
    for (int i = 0; i < 10; i++) {
      digitalWrite(pinLedRight, HIGH);
      Buzz(10);
      delay(100);
      digitalWrite(pinLedRight, LOW);
      digitalWrite(pinLedLeft, HIGH);
      Buzz(10);
      delay(100);
      digitalWrite(pinLedLeft, LOW);
      delay(100);

      switch (i){
        case 0:
          Right();
          break;
        case 5:
          Left();
      }
    }
  }
 
 ldrRightValue = analogRead(pinLdrRight);
 ldrLeftValue = analogRead(pinLdrLeft);

/*
 Serial.print("LDR right: ");
 Serial.println(ldrRightValue);
 Serial.print("LDR left: ");
 Serial.println(ldrLeftValue);
 Serial.println();
*/

 //These 2 statements allow for some noise without it interfering with the readings.
 if (ldrRightValue < threshold){
  ldrRightValue = 0;
 }
 if (ldrLeftValue < threshold) {
  ldrLeftValue = 0;
 }
 
 ledRightValue = map(ldrRightValue, 0, 1023, 0, 255);
 ledLeftValue = map(ldrLeftValue, 0, 1023, 0, 255);
 analogWrite(pinLedRight, ledRightValue);
 analogWrite(pinLedLeft, ledLeftValue);

 int ldrValueDifference = int(ldrLeftValue) - int(ldrRightValue); //If this is negative, the light source is coming from the right. And from the left if it's positive

 if ((ldrValueDifference <= 25) && (ldrValueDifference >= -25)) { //I'm allowing for the light source to not be dead in front of the robot for it to drive forward.
  if ((ldrLeftValue >= 60) && (ldrRightValue >= 60)) { //If the light the robot is detecting is actually bright enough, it drives forward
    //Buzz(100);
    Forward();
    //Serial.println("Moving towards light");
  }else {
    //Buzz(50);
    Left(); //If the light being detected by both LDRs is low it turns right to search for a light source.
    //Serial.println("Searching for light");
  }
 }else if(ldrValueDifference > 20){
  //Buzz(5);
  Left();
  //Serial.println("light is to the left");
 }else if(ldrValueDifference < -20){
  //Buzz(5);
  Right();
  //Serial.println("light is to the right");
 }
 
 delay(10);
 Break();
 delay(25);
 Stop();
}


void Forward()
{
  digitalWrite(pinLeftForward, HIGH);
  digitalWrite(pinLeftReverse, LOW);
  
  digitalWrite(pinRightForward, HIGH);
  digitalWrite(pinRightReverse, LOW);
}

void Reverse()
{
  digitalWrite(pinLeftForward, LOW); 
  digitalWrite(pinLeftReverse, HIGH);
  
  digitalWrite(pinRightForward, LOW); 
  digitalWrite(pinRightReverse, HIGH);
}

void Right()
{
  digitalWrite(pinLeftForward, HIGH); 
  digitalWrite(pinLeftReverse, LOW);
  
  digitalWrite(pinRightForward, LOW); 
  digitalWrite(pinRightReverse, HIGH);
}

void Left()
{
  digitalWrite(pinLeftForward, LOW); 
  digitalWrite(pinLeftReverse, HIGH);
  
  digitalWrite(pinRightForward, HIGH); 
  digitalWrite(pinRightReverse, LOW);
}

void Stop()
{
    digitalWrite(pinLeftForward, LOW);
    digitalWrite(pinLeftReverse, LOW);
    
    digitalWrite(pinRightForward, LOW);
    digitalWrite(pinRightReverse, LOW);
}

void Break()
{
    digitalWrite(pinLeftForward, HIGH);
    digitalWrite(pinLeftReverse, HIGH);
    
    digitalWrite(pinRightForward, HIGH);
    digitalWrite(pinRightReverse, HIGH);

}

void Buzz(int gap) {
  digitalWrite(pinBuzz, HIGH);
  delay(gap);
  digitalWrite(pinBuzz, LOW);
}

