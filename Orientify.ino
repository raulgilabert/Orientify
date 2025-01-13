#include <Wire.h>

const int MPU = 0x68;

// Definición de un struct para agrupar los datos del sensor
struct SensorData {
  int16_t AcX, AcY, AcZ;  // Acelerómetro
  int16_t GyX, GyY, GyZ;  // Giroscopio
};

enum Pos {
  LEFT,
  CENTER,
  RIGHT
};

Pos old_pos, current_pos;

SensorData getData(int address)
{
  SensorData sensorData;

  Wire.beginTransmission(address);
  Wire.write(0x3B);  
  Wire.endTransmission(false);
  Wire.requestFrom(address, 12, true);

  // Leer y almacenar los datos en el struct
  sensorData.AcX = Wire.read() << 8 | Wire.read();
  sensorData.AcY = Wire.read() << 8 | Wire.read();
  sensorData.AcZ = Wire.read() << 8 | Wire.read();
  sensorData.GyX = Wire.read() << 8 | Wire.read();
  sensorData.GyY = Wire.read() << 8 | Wire.read();
  sensorData.GyZ = Wire.read() << 8 | Wire.read();

  return sensorData;
}


void setup() {
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);  
  Wire.write(0);    
  Wire.endTransmission(true);
  Serial.begin(9600);
}

void loop() {
  old_pos = current_pos;
  SensorData sensorData = getData(MPU);

  if (sensorData.AcY < -6000) {
    current_pos = RIGHT;
  } else if (sensorData.AcY > 6000) {
    current_pos = LEFT;
  } else {
    current_pos = CENTER;
  }

  if (current_pos != old_pos) {
    switch (current_pos) {
      case LEFT:
        Serial.println(0x01);
        break;
      case CENTER:
        Serial.println(0x02);
        break;
      case RIGHT:
        Serial.println(0x00);
        break;
    } 
  }

  delay(1000);
}
