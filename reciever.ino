#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>
int ch_width_1 = 0;
int ch_width_2 = 0;
int ch_width_3 = 0;
int ch_width_4 = 0;
int ch_width_5 = 0;
int ch_width_6 = 0;
Servo ch1;
Servo ch2;
Servo ch3;
Servo ch4;
Servo ch5;
Servo ch6;
struct Signal {
byte throttle;      
byte pitch;
byte roll;
byte yaw;
byte aux1;
byte aux2;
};
Signal data;
//const uint64_t pipeIn = 0xE9E8F0F0E1LL;
RF24 radio(7, 8);
const byte address[6] = "00001"; 
void ResetData()
{
// Define the inicial value of each data input. | Veri girişlerinin başlangıç değerleri
// The middle position for Potenciometers. (254/2=127) | Potansiyometreler için orta konum
data.roll = 127;   // Center | Merkez
data.pitch = 127;  // Center | Merkez
data.throttle = 127; // Motor Stop | Motor Kapalı
data.yaw = 127;   // Center | Merkez
data.aux1 = 127;   // Center | Merkez
data.aux2 = 127;   // Center | Merkez
}
void setup()
{
  Serial.begin(9600);
  //Set the pins for each PWM signal | Her bir PWM sinyal için pinler belirleniyor.
  ch1.attach(2);
  ch2.attach(3);
  ch3.attach(4);
  ch4.attach(5);
  ch5.attach(6);
  //Configure the NRF24 module
  ResetData();
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.startListening(); //start the radio comunication for receiver | Alıcı olarak sinyal iletişimi başlatılıyor
} 
unsigned long lastRecvTime = 0;
void recvData()
{
if ( radio.available() ) {
radio.read(&data, sizeof(Signal));
lastRecvTime = millis();   // receive the data | data alınıyor
}
}
void loop()
{
recvData();
unsigned long now = millis();
if ( now - lastRecvTime > 1000 ) {
ResetData(); // Signal lost.. Reset data | Sinyal kayıpsa data resetleniyor
}
ch_width_4 = map(data.yaw,      0, 255, 1000, 2000);     // pin D5 (PWM signal)
ch_width_2 = map(data.pitch,    0, 255, 1000, 2000);     // pin D3 (PWM signal)
ch_width_3 = map(data.throttle, 0, 255, 1000, 2000);     // pin D4 (PWM signal)
ch_width_1 = map(data.roll,     0, 255, 1000, 2000);     // pin D2 (PWM signal)
ch_width_5 = map(data.aux1,     0, 255, 1000, 2000);     // pin D6 (PWM signal)
ch_width_6 = map(data.aux2,     0, 255, 1000, 2000);     // pin D7 (PWM signal)
Serial.print(ch_width_1+2);
Serial.print('\t');

Serial.print(ch_width_2+2);
Serial.print('\t');
Serial.print(ch_width_3+2);
Serial.print('\t');
Serial.print(ch_width_4+2);
Serial.print('\t');
Serial.print(ch_width_5+2);
Serial.print('\t');
Serial.println(ch_width_6+2);
// Write the PWM signal | PWM sinyaller çıkışlara gönderiliyor
ch1.writeMicroseconds(ch_width_1+2);
ch2.writeMicroseconds(ch_width_2+2);
ch3.writeMicroseconds(ch_width_3+2);
ch4.writeMicroseconds(ch_width_4+2);
ch5.writeMicroseconds(ch_width_5+2);
ch6.writeMicroseconds(ch_width_6+2);
}
