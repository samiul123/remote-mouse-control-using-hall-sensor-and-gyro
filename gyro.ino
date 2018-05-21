#include <Wire.h>
#include <I2Cdev.h>
#include <MPU6050.h>
#include<SoftwareSerial.h>
#include<String.h>
//#include <Mouse.h>

SoftwareSerial bluetooth(12, 13);
MPU6050 mpu;
int16_t ax, ay, az, gx, gy, gz;
int vx, vy;
float Acc_x,Acc_y,Acc_z;
float Sample_X;
float Sample_Y;
float Sample_Z;
unsigned char countx,county ;
signed long accelerationx[2], accelerationy[2];
signed long velocityx[2], velocityy[2];
signed long positionX[2];
signed long positionY[2];
signed int positionZ[2];
unsigned char direction;
float sstatex,sstatey;
int button1 = 6;
int button2 = 7;
int button3 = 5;
int buttonState1 = 0;
int buttonState2 = 0;
int buttonState3 = 0;
float prevSample_X, prevSample_Y, prevSample_Z;
String toSend="";
void Read_RawValue(){
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
//  Serial.print("AX: ");
//  Serial.print(ax);
//  Serial.print("AY: ");
//  Serial.print(ay);
//  Serial.print("AZ: ");
//  Serial.println(az);
  Sample_X = (float)ax;
  Sample_Y = (float)ay;
  Sample_Z = (float)az;
//  Serial.print("SampleX: ");
//  Serial.print(Sample_X);
//  Serial.print("SampleY: ");
//  Serial.print(Sample_Y);
//  Serial.print("SampleZ: ");
//  Serial.println(Sample_Z);
//  delay(1000);
}

/*******************************************************************************
 The purpose of the calibration routine is to obtain the value of the reference threshold.
 It consists on a 1024 samples average in no-movement condition.
********************************************************************************/
void Calibrate(void)
{
  unsigned int count1;
  count1 = 0;
  do{
  Read_RawValue();
  sstatex = sstatex + Sample_X;              // Accumulate Samples
  sstatey = sstatey + Sample_Y;
  count1++;
  }while(count1!=0x0400);                    // 1024 times
  sstatex=sstatex/1024;                       // division between 1024
  sstatey=sstatey/1024;
}



void setup() {
  bluetooth.begin(9600);
  Serial.begin(9600);
  Wire.begin();
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  mpu.initialize();
  if (!mpu.testConnection()) {
//    Serial.println("not");
//    while (1);
    mpu.setSleepEnabled(false);
  }
  Serial.println("Calibrating");
  Calibrate();
  Serial.println("Calibrating done");
  Serial.print("sstateX: ");
  Serial.println(sstatex);
  Serial.print("sstateY: ");
  Serial.println(sstatey);
}


/******************************************************************************************
This function allows movement end detection. If a certain number of acceleration samples are
equal to zero we can assume movement has stopped. Accumulated Error generated in the velocity
calculations is eliminated by resetting the velocity variables. This stops position increment
and greatly eliminates position error.
******************************************************************************************/
void movement_end_check(void)
{
  if (accelerationx[1]==0)   //we count the number of acceleration samples that equals zero
    { countx++;}
  else { countx =0;}

  if (countx>=25)          //if this number exceeds 25, we can assume that velocity is zero
   {
   velocityx[1]=0;
   velocityx[0]=0;
   }

   if (accelerationy[1]==0)   //we do the same for the Y axis
   { county++;}
  else { county =0;}

  if (county>=25)
   {
   velocityy[1]=0;
   velocityy[0]=0;
   }
}

/*****************************************************************************************/
/******************************************************************************************
This function transforms acceleration to a proportional position by integrating  the
acceleration data  twice. It also adjusts sensibility by  multiplying the "positionX"  and
"positionY" variables.
This integration algorithm carries error, which is compensated in the "movenemt_end_check"
subroutine. Faster  sampling frequency implies less error  but requires more memory. Keep  in
mind that the same process is applied to the X and Y axis.
*****************************************************************************************/

void position(void)
{
	unsigned char count2 = 0;

    do{

    Read_RawValue();
    accelerationx[1]=accelerationx[1] + Sample_X; //filtering routine for noise attenuation
    accelerationy[1]=accelerationy[1] + Sample_Y; //64 samples are averaged. The resulting

     //average represents the acceleration of
     //an instant
    count2++;

    }while (count2!= 32);                        // 64 sums of the acceleration sample


    accelerationx[1]= (accelerationx[1] / 32);          // division by 64
    accelerationy[1]= (accelerationy[1] / 32);

    accelerationx[1] = accelerationx[1] - (int)sstatex; //eliminating zero reference
   //offset of the acceleration data
    accelerationy[1] = accelerationy[1] - (int)sstatey; // to obtain positive and negative
   //acceleration


    if ((accelerationx[1] <=3)&&(accelerationx[1] >= -3)) //Discrimination window applied
      {accelerationx[1] = 0;}                        // to the X axis acceleration
     //variable

    if ((accelerationy[1] <=3)&&(accelerationy[1] >= -3))
      {accelerationy[1] = 0;}

  //first X integration:
    //velocityx[1]= velocityx[0]+ accelerationx[0]+ ((accelerationx[1] -accelerationx[0])>>1);
    velocityx[1]= velocityx[0]+ ((accelerationx[1] -accelerationx[0])>>1) * .5;
  //second X integration:
	//positionX[1]= positionX[0] + velocityx[0] + ((velocityx[1] - velocityx[0])>>1);
    positionX[1]= positionX[0] + ((velocityx[1] - velocityx[0])>>1) * .5;
  //first Y integration:
	//velocityy[1] = velocityy[0] + accelerationy[0] + ((accelerationy[1] -accelerationy[0])>>1);
  velocityy[1] = velocityy[0] + ((accelerationy[1] -accelerationy[0])>>1) * .5;
  //second Y integration:
	//positionY[1] = positionY[0] + velocityy[0] + ((velocityy[1] - velocityy[0])>>1);
    positionY[1] = positionY[0] + ((velocityy[1] - velocityy[0])>>1) * .5;

    accelerationx[0] = accelerationx[1];  //The current acceleration value must be sent
											//to the previous acceleration
    accelerationy[0] = accelerationy[1];     //variable in order to introduce the new
											//acceleration value.


    velocityx[0] = velocityx[1];          //Same done for the velocity variable
    velocityy[0] = velocityy[1];


//    positionX[1] = positionX[1]<<20;      //The idea behind this shifting (multiplication)
//     ////is a sensibility adjustment.
//    positionY[1] = positionY[1]<<20;      //Some applications require adjustments to a
     ////particular situation
                                      ////i.e. mouse application
									  //
//    Serial.print("X: ");
//    Serial.print(positionX[1]);
//    Serial.print("Y: ");
//    Serial.println(positionY[1]);
      
      toSend.concat(positionX[1]);
      toSend.concat("/");
      toSend.concat(positionY[1]);
//      bluetooth.println(toSend);
//      toSend="";
//      delay(100);
//    positionX[1] = positionX[1]>>20;      //once the variables are sent them must return to
//    positionY[1] = positionY[1]>>20;      //their original state

    movement_end_check();

    positionX[0] = positionX[1];          //actual position data must be sent to the
    positionY[0] = positionY[1];      //previous position

    direction = 0;                        // data variable to direction variable reset
}


void loop() {
  position();
  buttonState1 = digitalRead(button1);
  buttonState2 = digitalRead(button2);
//  buttonState3 = digitalRead(button3);
 if (buttonState1 == HIGH) {
      toSend.concat("/");
      toSend.concat("1");
      toSend.concat("/");
      toSend.concat("0");
//      toSend.concat("/");
//      toSend.concat("0");
  }
  else if(buttonState2 == HIGH) {
      toSend.concat("/");
      toSend.concat("0");
      toSend.concat("/");
      toSend.concat("1");
//      toSend.concat("/");
//      toSend.concat("0");
  }
//  else if(buttonState3 == HIGH){
//      toSend.concat("/");
//      toSend.concat("0");
//      toSend.concat("/");
//      toSend.concat("0");
//      toSend.concat("/");
//      toSend.concat("1");
//  }
  bluetooth.println(toSend);
  toSend="";
  delay(100);
}

