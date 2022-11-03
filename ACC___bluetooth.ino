#include <SoftwareSerial.h>
#include "NewPing.h"
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
//SDA A5, SCl A4
Adafruit_MPU6050 mpu;

int IRSensor = 8; // connect ir sensor to arduino pin 8
int backLED = 12; // back led

SoftwareSerial Blue(2, 3);
long int data = 0;
long int password1 = 92;// light on
long int password2 = 79; // light off

char phone_no[]="01670724809";
char state = 0;

#define TRIGGER_PIN 5
#define ECHO_PIN 6
#define LED 13
const int buzzer = 10;
#define MAX_DISTANCE 400

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
float duration, distance;

void setup()
{
  pinMode (IRSensor, INPUT); // sensor pin INPUT
  pinMode (backLED, OUTPUT); // Led pin OUTPUT
  //for acc
  Serial.begin(9600);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit MPU6050 test!");
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.println("");
  delay(100);

  // for blue
  pinMode(buzzer, OUTPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  Serial.begin(9600);
  Blue.begin(9600);

}

void loop()
{

  //Serial.println(Blue.available());
  
  ///////IR Sensor for Back LED////////////
  int statusSensor = digitalRead (IRSensor);
  
  if (statusSensor == 1){
    digitalWrite(backLED, LOW); // LED LOW
  }
  
  else
  {
    digitalWrite(backLED, HIGH); // LED High
  }
  //////////////////////////////////////////
  
  if (Blue.available() > 0)
  {
    Serial.println(Blue.available());
    data = Blue.parseInt();

  }
  //Serial.print("data = ");
  //Serial.println(data);

  if (data == password1)
  {

    distance = sonar.ping_cm();

    // Send results to Serial Monitor
    //Serial.print("Distance = ");

    if (distance <= 35 && distance > 21 )
    {
      digitalWrite(LED, HIGH);
    }
    else if (distance <= 21) {
      digitalWrite(LED, HIGH);
      digitalWrite(buzzer, HIGH);
      Serial.print(distance);
      delay(1000);
      digitalWrite(buzzer, LOW);
      delay(1000);
    }
    else
    {
      digitalWrite(buzzer, LOW);
      digitalWrite(LED, LOW);
      //Serial.print(distance);
      //Serial.println(" cm");
    }
  }

  if ( data == password2)
  {
    digitalWrite(LED, LOW);
    digitalWrite(buzzer, LOW);
  }


  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  double x1 = a.acceleration.x;
  if (x1 >= 8.0) {
    /*Serial.println("AT+CMGF=1");
    delay(2000);
    Serial.print("AT+CMGS=\"");
    Serial.print(phone_no);
    Serial.write(0x22);
    Serial.write(0x0D);  // hex equivalent of Carraige return
    Serial.write(0x0A);  // hex equivalent of newline
    delay(2000);
    Serial.print("The car has flipped forward");
    delay(500);
    Serial.println (char(26));*/
    Serial.println("The car has flipped forward");
  }
  else if (x1 <= -6.0) {
    Serial.println("The car has flipped backward");
    /*Serial.println("AT+CMGF=1");
    delay(2000);
    Serial.print("AT+CMGS=\"");
    Serial.print(phone_no);
    Serial.write(0x22);
    Serial.write(0x0D);  // hex equivalent of Carraige return
    Serial.write(0x0A);  // hex equivalent of newline
    delay(2000);
    Serial.print("The car has flipped backward");
    delay(500);
    Serial.println (char(26));*/
  }

  double y1 = a.acceleration.y;

  if (y1 >= 7.0) {
    Serial.println("The car has flipped left");
    /*Serial.println("AT+CMGF=1");
    delay(2000);
    Serial.print("AT+CMGS=\"");
    Serial.print(phone_no);
    Serial.write(0x22);
    Serial.write(0x0D);  // hex equivalent of Carraige return
    Serial.write(0x0A);  // hex equivalent of newline
    delay(2000);
    Serial.print("The car has flipped left");
    delay(500);
    Serial.println (char(26));*/
  }
  else if (y1 <= -8.0) {
    Serial.println("The car has flipped right");
    /*Serial.println("AT+CMGF=1");
    delay(2000);
    Serial.print("AT+CMGS=\"");
    Serial.print(phone_no);
    Serial.write(0x22);
    Serial.write(0x0D);  // hex equivalent of Carraige return
    Serial.write(0x0A);  // hex equivalent of newline
    delay(2000);
    Serial.print("The car has flipped right");
    delay(500);
    Serial.println (char(26));*/
  }


  //Serial.print("Temperature: ");
  //Serial.print(temp.temperature);
  //Serial.println(" degC");
  
}
