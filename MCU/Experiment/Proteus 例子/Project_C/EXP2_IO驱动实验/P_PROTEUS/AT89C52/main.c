/*************************************************  
*文件名称： main.c 
*作 者：    
*版 本：    V1.00
*说 明：    IO驱动实验
*修改记录：  
--------------------------------------------------
*Descriptoon: LED流水灯 
*接线说明：P00~P07-D1~D8                    
**************************************************/
#include<reg52.h>
#include <intrins.h>

#define uchar unsigned char	 //数据类型宏定义
#define uint unsigned int
/*********************引脚定义********************/
#define LED   P0

/*********************延时函数********************/
void delayms(uchar ms)	
// 延时子程序
{						
	unsigned char i;
	while(ms--)
	{
		for(i = 0; i < 120; i++);
	}
}
/*********************主函数**********************/
void main(void)
{
	LED = 0xfe;						//初始化P0口
	while(1)
	{								//延时
		delayms(250);			
		LED = _crol_(LED,1);		//循环右移1位，点亮下一个LED
	}
}