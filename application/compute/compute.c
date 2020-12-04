#include "compute.h"
#include "adc.h"


int volatile_to_site_Table[10] = {0,0,0,0,0,   0,0,0,0,0};//用来构成电压到位置的映射表
int o1,o2,o3,o4,o5,o6,o7,o8,o9;
int volatile_to_site(float vola)
{
	int site = 0;
	
	//区域判断
	if(o2 > 8000 && o8 < 4000)//上区域
	{
		
	}
	else if(o2 < 4000 && o8 > 8000)//下
	{
		
	}
	
	if(o4 )
	
	return  site;
}

