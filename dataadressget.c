# include "stdio.h"
//地址提取

typedef struct
{
    unsigned char     	startflag;
    unsigned char     	function;
    unsigned char     	usestate;
	unsigned char       datalen;
    // char*            dataadress;
	unsigned int 		DataPileNum;
    unsigned short     	sumcheck;

    // unsigned char;
}IndexItemType;
int main ()
{
	IndexItemType IndexItem;
	IndexItem.DataPileNum = 100;
	
	unsigned char *temp = (unsigned char*)&IndexItem; 
	
	unsigned int temp2;
	temp2 = *(unsigned int *)(temp + 4);
	
	printf("temp = %d",temp2);
	
}