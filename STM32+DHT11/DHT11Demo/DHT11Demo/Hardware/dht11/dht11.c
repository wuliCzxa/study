#include "dht11.h"
#include "main.h"

void DHT11_IO_OUT(void) {
  GPIO_InitTypeDef  GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Pin = DHT11_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(DHT11_GPIO_PORT, &GPIO_InitStructure);
}

void DHT11_IO_IN(void) {
  GPIO_InitTypeDef  GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Pin = DHT11_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(DHT11_GPIO_PORT, &GPIO_InitStructure);
}

//复位DHT11
void DHT11_Rst(void) {
  DHT11_IO_OUT();   //SET OUTPUT
  GPIO_ResetBits(DHT11_GPIO_PORT, DHT11_GPIO_PIN);
  Delay(20000);     //拉低至少18ms
  GPIO_SetBits(DHT11_GPIO_PORT, DHT11_GPIO_PIN);
  Delay(30);      //主机拉高20~40us
}
//等待DHT11的回应
//返回1:未检测到DHT11的存在
//返回0:存在
u8 DHT11_Check(void) {
  u8 retry = 0;
  DHT11_IO_IN();//SET INPUT
  while (GPIO_ReadInputDataBit(DHT11_GPIO_PORT, DHT11_GPIO_PIN) && retry < 100) { //DHT11会拉低40~80us
    retry++;
    Delay(1);
  };
  if (retry >= 100)
    return 1;
  else
    retry = 0;
  while (!GPIO_ReadInputDataBit(DHT11_GPIO_PORT, DHT11_GPIO_PIN) && retry < 100) { //DHT11拉低后会再次拉高40~80us
    retry++;
    Delay(1);
  };
  if (retry >= 100)
    return 1;
  return 0;
}
//从DHT11读取一个位
//返回值：1/0
u8 DHT11_Read_Bit(void) {
  u8 retry = 0;
  while (GPIO_ReadInputDataBit(DHT11_GPIO_PORT, DHT11_GPIO_PIN) && retry < 100) { //等待变为低电平
    retry++;
    Delay(1);
  }
  retry = 0;
  while (!GPIO_ReadInputDataBit(DHT11_GPIO_PORT, DHT11_GPIO_PIN) && retry < 100) { //等待变高电平
    retry++;
    Delay(1);
  }
  Delay(40);//等待40us
  if (GPIO_ReadInputDataBit(DHT11_GPIO_PORT, DHT11_GPIO_PIN))
    return 1;
  else
    return 0;
}
//从DHT11读取一个字节
//返回值：读到的数据
u8 DHT11_Read_Byte(void) {
  u8 i, dat;
  dat = 0;
  for (i = 0; i < 8; i++) {
    dat <<= 1;
    dat |= DHT11_Read_Bit();
  }
  return dat;
}
//从DHT11读取一次数据
//temp:温度值(范围:0~50°)
//humi:湿度值(范围:20%~90%)
//返回值：0,正常;1,读取失败
u8 DHT11_Read_Data(u8 *temp, u8 *humi) {
  u8 buf[5];
  u8 i;
  DHT11_Rst();
  if (DHT11_Check() == 0) {
    for (i = 0; i < 5; i++) { //读取40位数据
      buf[i] = DHT11_Read_Byte();
    }
    if ((buf[0] + buf[1] + buf[2] + buf[3]) == buf[4]) {
      *humi = buf[0];
      *temp = buf[2];
    }
  } else
    return 1;
  return 0;
}
//初始化DHT11的IO口 DQ 同时检测DHT11的存在
//返回1:不存在
//返回0:存在
u8 DHT11_Init(void) {


  DHT11_Rst();  //复位DHT11
  return DHT11_Check();//等待DHT11的回应
}







