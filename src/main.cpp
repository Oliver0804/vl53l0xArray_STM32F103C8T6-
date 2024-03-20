
#include <Arduino.h>
#include "I2CScanner.h"
#include <Wire.h>
#include <VL53L0X.h>

#define DEBUG 0

#define MAX_DEVICES 8                              // 定义设备的数量
#define BUFFER_SIZE 3                              // 定义每个设备用于计算平均值的缓冲区大小
uint16_t distanceBuffer[MAX_DEVICES][BUFFER_SIZE]; // 存储距离测量值的数组
int bufferIndex[MAX_DEVICES];                      // 记录每个设备当前存储距离的索引
int avgDistance[MAX_DEVICES];                      // 存储平均距离的数组
I2CScanner scanner;

// STM32 Arduino I2C pins
#define SDA_PIN PB6
#define SCL_PIN PB7

#include <Wire.h>

#include "TCA9548.h"

TCA9548 MP(0x70);
uint8_t searchAddress = 0x29; // vl53l0x default address

void sendAvgDistancesAsBytes(int avgDistance[])
{
  Serial.write(0xAA);
  Serial.write(0x55);
  for (int i = 0; i < MAX_DEVICES; i++)
  {
    // 分离出高位字节和低位字节
    byte highByte = (avgDistance[i] >> 8) & 0xFF; // 获取高位字节
    byte lowByte = avgDistance[i] & 0xFF;         // 获取低位字节

    // 发送两个字节
    Serial.write(highByte);
    Serial.write(lowByte);
  }
}

int tca9548init()
{
  Serial.println(__FILE__);
  Serial.print("TCA9548_LIB_VERSION: ");
  Serial.println(TCA9548_LIB_VERSION);
  Serial.println();

  Wire.begin();
  if (MP.begin() == false)
  {
    Serial.println("Could not connect to TCA9548 multiplexer.");
    return -1;
  }

  Serial.println("\nScan the channels of the multiplexer for searchAddress.\n");
  for (int chan = 0; chan < 8; chan++)
  {
    MP.selectChannel(chan);
    bool b = MP.isConnected(searchAddress);
    Serial.print("CHAN:  ");
    Serial.print(chan);
    Serial.print(" \t");
    Serial.println(b ? "vl53 found!" : "vl53 not found!");
  }
  Serial.println();

  return 0;
}
void tca9548select(uint8_t channel)
{
  MP.selectChannel(channel);
}

void setup()
{
  Serial.begin(230400);
  memset(bufferIndex, 0, sizeof(bufferIndex)); // 初始化索引数组

  tca9548init();
  tca9548select(0);
}

void loop()
{
  for (int device = 0; device < MAX_DEVICES; device++)
  {
    tca9548select(device);                     // 选择当前的设备
    uint16_t distance = VL53L0X_getDistance(); // 获取距离值

    if (distance != 65535)
    { // 检查距离是否有效
      // 将有效的距离值存储到缓冲区
      distanceBuffer[device][bufferIndex[device]] = distance;
      bufferIndex[device] = (bufferIndex[device] + 1) % BUFFER_SIZE; // 更新索引
    }

    // 计算平均距离
    uint32_t sum = 0;
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
      sum += distanceBuffer[device][i];
    }
    avgDistance[device] = sum / BUFFER_SIZE;
#if DEBUG
    // 输出平均距离
    Serial.print(device);
    Serial.print(": ");
    Serial.print(avgDistance[device]);
    if (device != MAX_DEVICES - 1)
    {
      Serial.print("  ");
    }
#endif
  }
  // Serial.println();
  sendAvgDistancesAsBytes(avgDistance);
  // delay(10);
}
