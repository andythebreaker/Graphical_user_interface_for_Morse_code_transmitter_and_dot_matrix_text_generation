#include <Servo.h>

#define ONECLICK myservo.write(31); \
  delay(1000); \
   myservo.write(13); \
   delay(1000); \

   #define TWOCLICK myservo.write(31); \
  delay(3000); \
   myservo.write(13); \
   delay(1000); \

Servo myservo;

void testallchar();

void setup()
{
  myservo.attach(9, 500, 2400); 
testallchar();
}

void loop()
{
  /*delay(500);
  myservo.write(29); 
  delay(500);*/
  myservo.write(13); 
}
void testallchar(){
  //56
// int morse_ary_A[] = {
ONECLICK

TWOCLICK

delay(3000);

// int morse_ary_B[] = {
TWOCLICK

ONECLICK

ONECLICK

ONECLICK

delay(3000);

// int morse_ary_C[] = {
TWOCLICK

ONECLICK

TWOCLICK

ONECLICK

delay(3000);

// int morse_ary_D[] = {
TWOCLICK

ONECLICK

ONECLICK

delay(3000);

// int morse_ary_E[] = {
ONECLICK

delay(3000);

// int morse_ary_F[] = {
ONECLICK

ONECLICK

TWOCLICK

ONECLICK

delay(3000);

// int morse_ary_G[] = {
TWOCLICK

TWOCLICK

ONECLICK

delay(3000);

// int morse_ary_H[] = {
ONECLICK

ONECLICK

ONECLICK

ONECLICK

delay(3000);

// int morse_ary_I[] = {
ONECLICK

ONECLICK

delay(3000);

// int morse_ary_J[] = {
ONECLICK

TWOCLICK

TWOCLICK

TWOCLICK

delay(3000);

// int morse_ary_K[] = {
TWOCLICK

ONECLICK

TWOCLICK

delay(3000);

// int morse_ary_L[] = {
ONECLICK

TWOCLICK

ONECLICK

ONECLICK

delay(3000);

// int morse_ary_M[] = {
TWOCLICK

TWOCLICK

delay(3000);

// int morse_ary_N[] = {
TWOCLICK

ONECLICK

delay(3000);

// int morse_ary_O[] = {
TWOCLICK

TWOCLICK

TWOCLICK

delay(3000);

// int morse_ary_P[] = {
ONECLICK

TWOCLICK

TWOCLICK

ONECLICK

delay(3000);

// int morse_ary_Q[] = {
TWOCLICK

TWOCLICK

ONECLICK

TWOCLICK

delay(3000);

// int morse_ary_R[] = {
ONECLICK

TWOCLICK

ONECLICK

delay(3000);

// int morse_ary_S[] = {
ONECLICK

ONECLICK

ONECLICK

delay(3000);

// int morse_ary_T[] = {
TWOCLICK

delay(3000);

// int morse_ary_U[] = {
ONECLICK

ONECLICK

TWOCLICK

delay(3000);

// int morse_ary_V[] = {
ONECLICK

ONECLICK

ONECLICK

TWOCLICK

delay(3000);

// int morse_ary_W[] = {
ONECLICK

TWOCLICK

TWOCLICK

delay(3000);

// int morse_ary_X[] = {
TWOCLICK

ONECLICK

ONECLICK

TWOCLICK

delay(3000);

// int morse_ary_Y[] = {
TWOCLICK

ONECLICK

TWOCLICK

TWOCLICK

delay(3000);

// int morse_ary_Z[] = {
TWOCLICK

TWOCLICK

ONECLICK

ONECLICK

delay(3000);

// int morse_ary_0[] = {
TWOCLICK

TWOCLICK

TWOCLICK

TWOCLICK

TWOCLICK

delay(3000);

// int morse_ary_1[] = {
ONECLICK

TWOCLICK

TWOCLICK

TWOCLICK

TWOCLICK

delay(3000);

// int morse_ary_2[] = {
ONECLICK

ONECLICK

TWOCLICK

TWOCLICK

TWOCLICK

delay(3000);

// int morse_ary_3[] = {
ONECLICK

ONECLICK

ONECLICK

TWOCLICK

TWOCLICK

delay(3000);

// int morse_ary_4[] = {
ONECLICK

ONECLICK

ONECLICK

ONECLICK

TWOCLICK

delay(3000);

// int morse_ary_5[] = {
ONECLICK

ONECLICK

ONECLICK

ONECLICK

ONECLICK

delay(3000);

// int morse_ary_6[] = {
TWOCLICK

ONECLICK

ONECLICK

ONECLICK

ONECLICK

delay(3000);

// int morse_ary_7[] = {
TWOCLICK

TWOCLICK

ONECLICK

ONECLICK

ONECLICK

delay(3000);

// int morse_ary_8[] = {
TWOCLICK

TWOCLICK

TWOCLICK

ONECLICK

ONECLICK

delay(3000);

// int morse_ary_9[] = {
TWOCLICK

TWOCLICK

TWOCLICK

TWOCLICK

ONECLICK

delay(3000);

// int morse_ary_DOT[] = {
ONECLICK

TWOCLICK

ONECLICK

TWOCLICK

ONECLICK

TWOCLICK

delay(3000);

// int morse_ary_LB[] = {
TWOCLICK

ONECLICK

TWOCLICK

TWOCLICK

ONECLICK

delay(3000);

// int morse_ary_RB[] = {
TWOCLICK

ONECLICK

TWOCLICK

TWOCLICK

ONECLICK

TWOCLICK

delay(3000);

// int morse_ary_PLUS[] = {
ONECLICK

TWOCLICK

ONECLICK

TWOCLICK

ONECLICK

delay(3000);

// int morse_ary_SP[] = {
TWOCLICK

ONECLICK

TWOCLICK

ONECLICK

TWOCLICK

ONECLICK

delay(3000);

// int morse_ary_UQ[] = {
ONECLICK

ONECLICK

TWOCLICK

ONECLICK

TWOCLICK

delay(3000);

// int morse_ary_COMA[] = {
TWOCLICK

TWOCLICK

ONECLICK

ONECLICK

TWOCLICK

TWOCLICK

delay(3000);

// int morse_ary_DASH[] = {
TWOCLICK

ONECLICK

ONECLICK

ONECLICK

ONECLICK

TWOCLICK

delay(3000);

// int morse_ary_EQ[] = {
TWOCLICK

ONECLICK

ONECLICK

ONECLICK

TWOCLICK

delay(3000);

// int morse_ary_UEXC[] = {
TWOCLICK

TWOCLICK

ONECLICK

ONECLICK

ONECLICK

TWOCLICK

delay(3000);

// int morse_ary_QM[] = {
ONECLICK

ONECLICK

TWOCLICK

TWOCLICK

ONECLICK

ONECLICK

delay(3000);

// int morse_ary_AND[] = {
ONECLICK

TWOCLICK

ONECLICK

ONECLICK

ONECLICK

delay(3000);

// int morse_ary_DD[] = {
ONECLICK

ONECLICK

TWOCLICK

TWOCLICK

ONECLICK

TWOCLICK

delay(3000);

// int morse_ary_MONY[] = {
ONECLICK

ONECLICK

ONECLICK

TWOCLICK

ONECLICK

ONECLICK

TWOCLICK

delay(3000);

// int morse_ary_EXC[] = {
TWOCLICK

ONECLICK

TWOCLICK

ONECLICK

TWOCLICK

TWOCLICK

delay(3000);

// int morse_ary_SQUT[] = {
ONECLICK

TWOCLICK

TWOCLICK

TWOCLICK

TWOCLICK

ONECLICK

delay(3000);

// int morse_ary_COLN[] = {
TWOCLICK

TWOCLICK

TWOCLICK

ONECLICK

ONECLICK

ONECLICK

delay(3000);

// int morse_ary_DQUT[] = {
ONECLICK

TWOCLICK

ONECLICK

ONECLICK

TWOCLICK

ONECLICK

delay(3000);

// int morse_ary_AT[] = {
ONECLICK

TWOCLICK

TWOCLICK

ONECLICK

TWOCLICK

ONECLICK

delay(3000);

// int morse_ary_SLAH[] = {
TWOCLICK

ONECLICK

ONECLICK

TWOCLICK

ONECLICK

delay(3000);

}
