#include <Arduino.h>

#include <Wire.h>

#define u8 unsigned char

int long testcount = 0;
int errorcount = 0;

u8 cmd[5] = {0x5a, 0x05, 0x00, 0x01, 0x60};         // Get measure value cmd
u8 FScmd[4] = {0x5a, 0x04, 0x10, 0x6E};             // Factory settings
u8 UartModecmd[5] = {0x5A, 0x05, 0x0A, 0x00, 0x69}; // Factory settings
u8 Savecmd[4] = {0x5A, 0x04, 0x11, 0x6F};           // Factory settings

void Get_LidarDatafromIIC(int address);
void SetTTLMode(int address);

void setup()
{
  Wire.begin();                 // join i2c bus (address optional for master)
  Serial.begin(115200);         // start serial for output
  pinMode(LED_BUILTIN, OUTPUT); // LED
  // SetTTLMode(0x10);
}

void loop()
{
  Get_LidarDatafromIIC(0x11);
  delay(50);
}

void SetTTLMode(int address)
{
  u8 i = 0;
  u8 j = 0;
  Wire.beginTransmission(address);
  for (i = 0; i < 5; i++)
  {
    Wire.write(UartModecmd[i]);
  }
  for (j = 0; j < 4; j++)
  {
    Wire.write(Savecmd[j]);
  }
  Wire.endTransmission(); // Send a START Sign
}

void Get_LidarDatafromIIC(int address)
{
  char i = 0;
  char j = 0;
  byte rx_buf[9] = {0};
  Wire.beginTransmission(address); // Begin a transmission to the I2C Slave device with the given address.
  for (j = 0; j < 5; j++)
  {
    Wire.write(cmd[j]);
  }
  Wire.endTransmission();       // Send a START Sign
  Wire.requestFrom(address, 9); // request 7 bytes from slave device address
  // print the result via serial
  Serial.print("Address=0x");
  Serial.print(address, HEX);
  Serial.print(":   ");
  while (Wire.available())
  {
    rx_buf[i] = Wire.read(); // received one byte
    Serial.print("0x");
    Serial.print(rx_buf[i], HEX);
    Serial.print(";");
    i++;
    if (i >= 9)
    {
      i = 0;
      Serial.print("---------->");
      Serial.print("Distance=");
      Serial.print(rx_buf[3] * 256 + rx_buf[2]);
      Serial.print(";");
      Serial.print("Strength=");
      Serial.print(rx_buf[5] * 256 + rx_buf[4]);
    }
  }
  Serial.print("\r\n");
}
