 #include <SPI.h>
  #include <nRF24L01.h>
  #include <RF24.h>
  //const uint64_t pipeOut = 0xE9E8F0F0E1LL; //IMPORTANT: The same as in the receiver 0xE9E8F0F0E1LL | Bu adres alıcı ile aynı olmalı
  
  RF24 radio(7, 8); // select CE,CSN pin | CE ve CSN pinlerin seçimi
  const byte address[6] = "00001";
  struct Signal {
  byte throttle;
  byte pitch;
  byte roll;
  byte yaw;
  byte aux1;
  byte aux2;
};
  Signal data;
  
  void ResetData() 
{
  data.throttle = 127;   // Motor stop | Motor Kapalı (Signal lost position | sinyal kesildiğindeki pozisyon)
  data.pitch = 127;    // Center | Merkez (Signal lost position | sinyal kesildiğindeki pozisyon)
  data.roll = 127;     // Center | merkez (Signal lost position | sinyal kesildiğindeki pozisyon)
  data.yaw = 127;     // Center | merkez (Signal lost position | sinyal kesildiğindeki pozisyon)
  data.aux1 = 127;    // Center | merkez (Signal lost position | sinyal kesildiğindeki pozisyon)
  data.aux2 = 127;    // Center | merkez (Signal lost position | sinyal kesildiğindeki pozisyon)
}
  void setup()
{
  Serial.begin(9600);
  //Start everything up
  radio.begin();
  radio.openWritingPipe(address);
  radio.stopListening(); //start the radio comunication for Transmitter | Verici olarak sinyal iletişimi başlatılıyor
 
}
  // Joystick center and its borders | Joystick merkez ve sınırları
  int mapJoystickValues(int val, int lower, int middle, int upper, bool reverse)
{
  val = constrain(val, lower, upper);
  if ( val < middle )
  val = map(val, lower, middle, 0, 128);
  else
  val = map(val, middle, upper, 128, 255);
  return ( reverse ? 255 - val : val );
}
  void loop()
{
  ResetData();
  // Control Stick Calibration | Kumanda Kol Kalibrasyonları
  // Setting may be required for the correct values of the control levers. | :Kontrol kolların doğru değerleri için ayar gerekebilir.
  data.throttle = mapJoystickValues( analogRead(A0), 12, 524, 1020, true )-4;  // "true" or "false" for signal direction | "true" veya "false" sinyal yönünü belirler
  data.roll = mapJoystickValues( analogRead(A3), 12, 524, 1020, true )-6;      // "true" or "false" for servo direction | "true" veya "false" servo yönünü belirler
  data.pitch = mapJoystickValues( analogRead(A2), 12, 524, 1020, true )-6;     // "true" or "false" for servo direction | "true" veya "false" servo yönünü belirler
  data.yaw = mapJoystickValues( analogRead(A1), 12, 524, 1020, true )-5;       // "true" or "false" for servo direction | "true" veya "false" servo yönünü belirler
  data.aux1 = mapJoystickValues( analogRead(A4), 12, 524, 1020, true );     // "true" or "false" for servo direction | "true" veya "false" servo yönünü belirler
  data.aux2 = mapJoystickValues( analogRead(A5), 12, 524, 1020, true );// "true" or "false" for servo direction | "true" veya "false" servo yönünü belirler
  Serial.print(data.throttle);
  Serial.print('\t');
  Serial.print(data.roll);
  Serial.print('\t');
  Serial.print(data.pitch);
  Serial.print('\t');
  Serial.print(data.yaw);
  Serial.print('\t');
  Serial.print(data.aux1);
  Serial.print('\t');
  Serial.print(data.aux2);
  Serial.println('\t');
  
  radio.write(&data, sizeof(Signal));
}
