/*
This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program. If not, see <https://www.gnu.org/licenses/>. 
*/

#include <Wire.h>

const int MPU = 0x68;

struct SensorData {
  int16_t AcX, AcY, AcZ;
  int16_t GyX, GyY, GyZ;
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
