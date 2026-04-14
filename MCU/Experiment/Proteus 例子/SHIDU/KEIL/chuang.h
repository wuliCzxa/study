#ifndef __CHUANG_H__
#define __CHUANG_H__


uchar code turn[]={0xF2,0xF6,0xF4,0xFc,0xF8,0xF9,0xF1,0xF3};
uchar i1,i2;

void zhuan( char del)
{
	if(del<0)
	{
		i2=-del;
	}
	else 
		i2=del;
			
			i1 = i1 > 0 ? i1-1 : 7;	
			P1=turn[i1];
			delay(40);
}

#endif