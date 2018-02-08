#include <stdio.h>

# define IndexItemSize         10
# define IndexItemMaxNUM     10

# define PrePileSize         10
# define DataItemSize         50
# define DataAreaSize         400
# define BITMAPLEN         10

# define PileNUM         DataAreaSize/PrePileSize

# define RIGHT         1
# define LEFT         2

//单条最大长度 255
#pragma pack(1)
typedef struct
{
    unsigned char       startflag[2];
    unsigned char       function;
    char                datalen;
    char*               dataadress;
    unsigned short      sumcheck;
    // unsigned char;
}IndexItemType;
#pragma pack()

//存储结构
//start            function    datalen        data    crc
//AA BB            03            len                    xx xx
//


//前面100作为索引使用
//后面作为数据域使用
unsigned char DataSaveArea[500] = {0xAA};

unsigned char  DataSave(unsigned char* data_save_start_adress);
unsigned char  IndexCreate();
unsigned char  BitNumReturn(unsigned char bitvalue,unsigned char * vector);
unsigned char  *MallocCreate(unsigned char* bitmapadress,unsigned char spacelen,unsigned char* dataadress);
unsigned char DataFind();
unsigned char DataReplace();
unsigned char DataDelete();
unsigned char DataAdd();
unsigned char  DataScan(unsigned char* dataadress,unsigned char* dataendadress,unsigned char* bitmapadress);
unsigned int CRC16_Checkout ( unsigned char *puchMsg, unsigned int usDataLen );
IndexItemType IndexItem;
IndexItemType *IndexItem2;

unsigned char bitmap[BITMAPLEN] = {0};

int main(int argc, char *argv[])
{
    IndexItem2 = DataSaveArea;
    IndexItem2->startflag[0] = 0xAA;
    IndexItem2->function = 0xCC;
    IndexItem2->datalen = 0x80;
    IndexItem2->dataadress = &DataSaveArea[100];
    IndexItem2->sumcheck = 11;

    printf("%d  \n",argc);
    printf("sizeof(IndexItem) %d  \n",sizeof(IndexItem));

    unsigned char * index_adress;
    unsigned char * index_end_adress;
    unsigned char * data_save_start_adress;
    unsigned short save_data_num;
    unsigned char save_index_num;

	unsigned char temp;
	

	//扫描
    save_index_num = DataScan(&DataSaveArea,index_end_adress,bitmap);
    printf("save_index_num: %d\n",save_index_num);
	for(temp = 0;temp<10;temp++)
	{
		printf("bitmap %d:%d\n",temp,bitmap[temp]);		
	}
	
   return 0;
}
void BitmapProcess(unsigned char * bitmap,unsigned short NUM)
{
    //
    *bitmap << 1;
    *bitmap += 1;
}


//数据扫描--已存储数据条数
//数据域起始地址 结束地址  位图起始地址
unsigned char  DataScan(unsigned char* datastartadress,unsigned char* dataendadress,unsigned char* bitmap_adress)
{
    unsigned short itemnum = IndexItemMaxNUM;
    unsigned char itemcount = 0;

    unsigned short temp = 0;
    unsigned char temp2 = 0;
    unsigned char temp3 = 0;

    // unsigned short indexnum = 0;
	//校验和
    unsigned short crccalsulate = 0;
    unsigned short crcget = 0;
	//数据起始地址
    unsigned char * adress = datastartadress;//Index start adress
	//每条数据存储长度
    unsigned short datalen;//存储的数据长度
    //每条保存的地址
	unsigned char *itemdataadress;    	//item 保存 数据位置
    unsigned short saveprelen;        	//开始到当前数据的长度
    unsigned char *bitmapadress = bitmap_adress;    	//bitmap adress
    unsigned short bitmapnum;    		//bitmap adress

    unsigned char remainder1;//余数1 针对preadress 之前的数据
    unsigned char remainder2;//余数	 之后	
    unsigned char pilenum1;//簇数    针对preadress 之前的数据  即 bitmap 位置
    unsigned char pilenum2;//簇数
	
	unsigned char bitsave8_1[] = {0,0x80,0xC0,0xE0,0xF0,0xF8,0xFC,0xFE,0xFF};
	unsigned char bitsave8_2[] = {0,0x01,0x03,0x07,0x0F,0x1F,0x3F,0x7F,0xFF};
	
	unsigned char addoneflag = 0;//
	
    while(itemnum)//按照目录数检索
    {
        itemnum --;
//
        if(*adress == 0)
        {
        }
        else if(*adress == 0xAA)
        {	
			//校验和---暂时屏蔽
            // crcget = adress[IndexItemSize-1] + adress[IndexItemSize-2];
            // crccalsulate = CRC16_Checkout(adress,sizeof(IndexItemSize - 2));
            //满足校验
			if(crccalsulate == crcget)
            {
                // *adress += IndexItemSize;		//下条索引
                itemcount ++;						//当前数据数++
                datalen = *(adress + 3) ;		//存储的数据长度+ 6

                saveprelen = *((unsigned char **)((adress + 4))) - datastartadress - 100;		//保存的数据起始地址到数据域开始长度
                // saveprelen = saveprelen / PrePileSize;                                       //当前数据域前簇数量
                if(saveprelen == 0)
				{
					remainder1 = 0;
					pilenum1 = 0;
				}
				else if(saveprelen < PrePileSize)
				{
					remainder1 = 1;
					pilenum1 = 0;
				}
				else
				{
					remainder1 	= (saveprelen/PrePileSize)%8;
					if(saveprelen%PrePileSize > 0)
						remainder1 ++;
					
					pilenum1 = (saveprelen/PrePileSize)/8; //bitmap 位置
				}
              //  *(bitmapadress + pilenum1) = bitsave8_1[remainder1];//写入bitmap值
			  //写入bitmap 
				if(datalen == 0)
				{
					//删除Index 操作
				}
				else if(datalen < PrePileSize)
				{
					remainder2 	= 1;
					pilenum2 	= 0;
					if((remainder1 == 1)&&(pilenum1 == 0))		//这种情况理论上不会出现
					{
						*(bitmapadress + pilenum1) = 0x03;						
					}
					else if((remainder1 > 1))
					{
						*(bitmapadress + pilenum1 + 1) = (bitsave8_2[remainder1]<<1) + 1;		
					}
					// *(bitmapadress + pilenum1) = bitsave8_2[remainder1];					
				}
                else    //datalen > PrePileSize
				{
					remainder2  = ((datalen ) / PrePileSize)%8;  	//多余数据簇数量--->修改为预先加上剩余簇处理更好点
					if(datalen % PrePileSize)
						remainder2 ++;
					pilenum2 	= ((datalen )/ PrePileSize)/8;  	//数据簇数量	
					
					if((remainder2 == 0))			// 正好1个8位簇&&(pilenum2 == 1)
					{
						temp = 	8 - remainder1;
						// for(temp2 = 0;temp2 < temp; temp2++)		
						// {
							*(bitmapadress + pilenum1) = ((*(bitmapadress + pilenum1))&(bitsave8_1[temp]));	//处理错误	！！	
						// }
						// pilenum2 --;
						if(pilenum2 == 1)	//1 pile
						{
							*(bitmapadress + pilenum1 + 1) = bitsave8_2[remainder1];	
						}
						else				// piles
						{
							for(temp2 = 1; temp2 < pilenum2;temp2 ++)
							{
								*(bitmapadress + pilenum1 + temp2) = 0xff;		
							}
							*(bitmapadress + pilenum1 + 1 + temp2) = bitsave8_2[remainder1];	
						}									
					}
					else if((remainder2 != 0))
					{
                        // if(remainder2 <= (8-remainder1))
                        if(remainder2 <= (8-remainder1))
						{
                            for(temp3 = 0;temp3 < (8-remainder1);temp3++)
							{
								(*(bitmapadress + pilenum1)) = (*(bitmapadress + pilenum1))<<1;
                                (*(bitmapadress + pilenum1))++;
								
							}	
							// *(bitmapadress + pilenum1) = ((*(bitmapadress + pilenum1))&(bitsave8_1[8-remainder1]));	
							temp = 	8 -  remainder1 - remainder2;//多余长度 或者是 剩余空间
							
							if(1 == pile)
							{
								
							}
							else
							{
								
							}
							
						}
						else
						{
							// *(bitmapadress + pilenum1) = ((*(bitmapadress + pilenum1))&(bitsave8_1[8-remainder1]));	
							for(temp3 = 0;temp3 < (8-remainder1);temp3++)
							{
								(*(bitmapadress + pilenum1)) = (*(bitmapadress + pilenum1))<<1;
                                (*(bitmapadress + pilenum1))++;
							}	
							temp = remainder2 - remainder1;//多余长度
						}
						// for(temp2 = 0;temp2 < temp; temp2++)		
						// {
							// *(bitmapadress + pilenum1) = ((*(bitmapadress + pilenum1))&(bitsave8_1[8-remainder1]));	//处理错误	！！
						// }
                        if((pilenum2 != 0)&&(remainder2 <= (8-remainder1)))
                        {
                            for(temp2 = 0; temp2 < (pilenum2);temp2 ++)
                            {
                                *(bitmapadress + pilenum1 + temp2 + 1) = 0xff;
                            }
                        }
                        if((temp != 0)&&(pilenum2>1))//多余长度的处理
						{
							for(temp3 = 0;temp3 < temp; temp3++)		
							{
								(*(bitmapadress + pilenum1 + temp2 + 1)) = (*(bitmapadress + pilenum1 + temp2 + 1)) << 1;
                                (*(bitmapadress + pilenum1 + temp2 + 1)) ++ ;
							}							
						}				
					}
					else
					{
						printf("???");
					}
					
					// 先保存整8簇
					// temp =1;
					// while(pilenum2)
					// {
						// pilenum2 -- ;
						// *(bitmapadress + pilenum1 + temp) = 0xFF;
						// temp++;					
					// }
					// *(bitmapadress + pilenum1 + temp) = bitsave8_2[remainder1];				
				}
                printf("*bitmapadress %d\n",*bitmapadress);
                printf("*bitmapadress %d\n",*(bitmapadress + 1));
                printf("*bitmapadress %d\n",*(bitmapadress + 2));
                printf("*bitmapadress %d\n",*(bitmapadress + 3));
                printf("*bitmapadress %d\n",*(bitmapadress + 4));
                printf("*bitmapadress %d\n",*(bitmapadress + 5));
                printf("*bitmapadress %d\n",*(bitmapadress + 6));
                printf("*bitmapadress %d\n",*(bitmapadress + 7));
			}
            else
            {
                printf("CRC Error\n");
                //如何处理
            }
            //(数据长度计算)
        }
        else
        {
            printf("Scan Error\n");
            //纠错处理
        }		
		*adress += IndexItemSize;//增加1条距离 下条索引
        printf("itemcount %d\n",itemcount);
    }
    return itemcount;
}
//剩余空间检查
//数据保存
unsigned char  DataSave(unsigned char* data_save_start_adress)
{
	//


    return 1;
}
//索引创建
unsigned char  IndexCreate()
{

    return 1;
}
//bitnumreturn 
unsigned char  BitNumReturn(unsigned char bitvalue,unsigned char * vector)
{
	unsigned char num;	
	switch(bitvalue)
	{
		case 0x01:	num = 7; *vector = LEFT;	break; 
		case 0x80:	num = 7; *vector = RIGHT;	break;
		case 0x03:	num = 6; *vector = LEFT;	break; 
		case 0xC0:	num = 6; *vector = RIGHT;	break;
		case 0x07:	num = 5; *vector = LEFT;	break; 
		case 0xE0:	num = 5; *vector = RIGHT;	break;
		case 0x0F:	num = 4; *vector = LEFT;	break; 
		case 0xF0:	num = 4; *vector = RIGHT;	break;
		case 0x1F:	num = 3; *vector = LEFT;	break; 
		case 0xF8:	num = 3; *vector = RIGHT;	break;
		case 0x3F:	num = 2; *vector = LEFT;	break; 
		case 0xFC:	num = 2; *vector = RIGHT;	break;
		case 0x7F:	num = 1; *vector = LEFT;	break; 
		case 0xFE:	num = 1; *vector = RIGHT;	break;
		default: 	num = 0;					break;
	}
	
    return num;
}
/*************
Parameters	:bitmapnum 	bitnum  vector *dataadress
Return 		:save dataadress
*************/
unsigned char *bitmapToadress(unsigned char bitmapnum,unsigned char bitnum,unsigned char vector,unsigned char *dataadress )
{
	char *returnadress;
    returnadress =dataadress + ( bitmapnum * 8 + bitnum)* PrePileSize;//
	//超界判断
	//多余pile处理
	//申请时应靠近已有的空间
	return returnadress;
}
/*************
Parameters	:bitmapnum 	bitnum  vector *dataadress
Return 		:save dataadress
*************/
//存储空间查找                        
	//bitma地址 所需空间长度 存储区域地址
unsigned char  *MallocCreate(unsigned char* bitmapadress,unsigned char spacelen,unsigned char* dataadress)
{
    unsigned char len = 0;
    unsigned char bitmapnum;
    unsigned char bitnum;
    unsigned char bitnum2;
	unsigned char *vector;//存储方向
	unsigned char *returnadress;
	unsigned char datasaverecord;
	
	//存储所需空间
	unsigned char needspace = (spacelen / PrePileSize);
    if((spacelen % PrePileSize)>0)
		needspace++;
	
	while(len < BITMAPLEN)
    {	

		//空间寻找	
		if(0xFF == *(bitmapadress + len))
		{
					
		}
		else
		{	//
			bitnum = BitNumReturn(*(bitmapadress + len),vector);
			if(bitnum >= needspace)
			{
				if(vector == LEFT)
					;
				else if(vector == RIGHT)
					bitnum = 8-bitnum;
				
				*returnadress=bitmapToadress(len,bitnum,0,dataadress);
				return returnadress;
			}
			else
			{	
				if(0xFF == *(bitmapadress + len+1))
				{}
				// bitnum2 = BitNumReturn(*(bitmapadress + len + 1),vector);
				
			}		
		}
		// if((8 - *(bitmapadress + len)) >= needspace)//剩余空间bitmap 1元素就满足时
		// {
			
			// return len; 			
		// }
		// else()
		// {
			
			
		// }
		// else
		// {
			
		// }
		len ++;
    }
	
    return 0;//返回map编号
}
//数据查找
unsigned char DataFind()
{

    return 1;
}
//数据替换
unsigned char DataReplace()
{

    return 1;
}
//数据删除---数据索引移动
unsigned char DataDelete()
{

    return 1;
}
//数据增加
unsigned char DataAdd()
{

    return 1;
}
//CRC
unsigned int CRC16_Checkout ( unsigned char *puchMsg, unsigned int usDataLen )
{
    unsigned int i,j,crc_reg,check;
    crc_reg = 0xFFFF;
    for(i=0;i<usDataLen;i++)
    {
        crc_reg = (crc_reg>>8) ^ puchMsg[i];
        for(j=0;j<8;j++)
        {
            check = crc_reg & 0x0001;
            crc_reg >>= 1;
            if(check==0x0001)
            {
                crc_reg ^= 0xA001;
            }
        }
    }
    return crc_reg;
}





