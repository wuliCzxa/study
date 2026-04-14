#include <reg51.h>
void delay(unsigned int ms)
{
	unsigned	int	i,	j;
	for(i=0;i<ms;i++)
		{
		for(j=0;j<100;j++);
		}
}

void main()
{
	unsigned char i=0;
	while(1)
	{
		P1=~(1<i);
		delay(500);
		i=(i+1)%8;
	}
}