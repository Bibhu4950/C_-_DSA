#include<reg51.h>

sbit led=P1^0;
int main()
{
	while(1);
	led=~led;
}

