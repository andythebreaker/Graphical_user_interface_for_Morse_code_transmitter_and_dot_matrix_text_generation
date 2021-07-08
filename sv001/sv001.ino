#include <Servo.h>

#define ONECLICK myservo.write(32); \
  delay(1000); \
   myservo.write(13); \
   delay(1000); \

   #define TWOCLICK myservo.write(32); \
  delay(2700); \
   myservo.write(13); \
   delay(1000); \

Servo myservo;

void setup()
{
  myservo.attach(9, 500, 2400); 
ONECLICK
 TWOCLICK
ONECLICK
ONECLICK
}

void loop()
{
  /*delay(500);
  myservo.write(29); 
  delay(500);*/
  myservo.write(13); 
}
