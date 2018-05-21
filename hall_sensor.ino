#include <SoftwareSerial.h>// import the serial library

SoftwareSerial Aisha(11, 12); // TX, RX
//int ledpin=13; // led on D13 will show blink on / off
int hall_1=9;
int hall_2=10;
int hall_3=8;
int BluetoothData; // the data given from Computer
int HallState_1,HallState_2,HallState_3;
int change;
int Phs1,Phs2,Phs3;
String toSend="";
void setup(){
//   Serial.begin(9600);
   Aisha.begin(9600); //Bluetooth Module works at 9600 baudrate
//   pinMode(ledpin,OUTPUT); //led pin as output
   pinMode(hall_1,INPUT); //hall sensor 1 as input
   pinMode(hall_2,INPUT); //hall snesor 2 is also input
   pinMode(hall_3,INPUT);
//   pinMode(12, INPUT);
//   pinMode(11, OUTPUT);
}
 
void loop()
{
 if (Aisha.available()) //if data is sent from laptop
 {
   BluetoothData=Aisha.read(); //read it and store it in BluetoothData
//   Aisha.print("BTdata: ");
//   Aisha.println(BluetoothData);
 }
  
 Phs1=HallState_1;
 Phs2=HallState_2;
 Phs3=HallState_3;
 HallState_1 = digitalRead(hall_1);
 HallState_2 = digitalRead(hall_2);
 HallState_3 = digitalRead(hall_3);
// HallState_1 = analogRead(hall_1);
// HallState_2 = analogRead(hall_2);
// Aisha.print("HALL_01: ");
// Aisha.println(HallState_1);
// delay(1000);
// Aisha.print("HALL_02: ");
// Aisha.println(HallState_2);
 
if (Phs1!=HallState_1 || Phs2!=HallState_2 || Phs3!=HallState_3) //Check if new keys are pressed
 {
//if (HallState_1==LOW && HallState_2==LOW){
//}
//Aisha.write(1);
//Serial.println(1);
//if (HallState_1==HIGH && HallState_2==LOW && HallState_3 == HIGH){
//  toSend.concat("1");
//  toSend.concat("/");
//  toSend.concat("0");
//  toSend.concat("/");
//  toSend.concat("1");
//}
if (HallState_2==LOW){
  toSend.concat("1");
  toSend.concat("/");
  toSend.concat("0");
  toSend.concat("/");
  toSend.concat("1");
}
//Aisha.println(2);
//Aisha.write(2);
//Serial.println(2);
//else if (HallState_1==LOW && HallState_2==HIGH && HallState_3 == HIGH){
//  toSend.concat("0");
//  toSend.concat("/");
//  toSend.concat("1");
//  toSend.concat("/");
//  toSend.concat("1");
//}
else if (HallState_1==LOW){
  toSend.concat("0");
  toSend.concat("/");
  toSend.concat("1");
  toSend.concat("/");
  toSend.concat("1");
}
//Aisha.println(3);
//Aisha.write(3);
//Serial.println(3);
//else if (HallState_1==HIGH && HallState_2==HIGH){
//}
//Aisha.println(4);

//else if(HallState_1 == HIGH && HallState_2 == HIGH && HallState_3==LOW){
////  Aisha.println(5);
//  toSend.concat("1");
//  toSend.concat("/");
//  toSend.concat("1");
//  toSend.concat("/");
//  toSend.concat("0");
//}
else if(HallState_3==LOW){
//  Aisha.println(5);
  toSend.concat("1");
  toSend.concat("/");
  toSend.concat("1");
  toSend.concat("/");
  toSend.concat("0");
}
Aisha.println(toSend);
toSend="";
delay(100);
//Aisha.write(4);
//Serial.println(4);
 }
// Aisha.flush();
//
//if (BluetoothData=='y')digitalWrite(ledpin,HIGH);
//if (BluetoothData=='n')digitalWrite(ledpin,LOW);
}
