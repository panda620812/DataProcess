#include <stdio.h>

# define IndexItemSize         10
# define IndexItemMaxNUM     10

# define PrePileSize         10
# define DataItemSize         50
# define DataAreaSize         400
# define BITMAPLEN         10

# define PileNUM         DataAreaSize/PrePileSize

//单条最大长度 255
#pragma pack(1)
typedef struct
{
    unsigned char     startflag[2];
    unsigned char     function;
    char             datalen;
    char*            dataadress;
    unsigned short     sumcheck;
    // unsigned char;
}IndexItemType;
#pragma pack()

//存储结构
//start            function    datalen        data    crc
//AA BB            03            len                    xx xx
//


//前面100作为索引使用
//后面作为数据域使用
unsigned char DataSaveArea[500] = {};

unsigned char  DataSave(unsigned char* data_save_start_adress);
unsigned char  IndexCreate();

unsigned char  MallocCreate(unsigned char* bitmapadress,unsigned char bitmapnum,unsigned char bitnum);
unsigned char DataFind();
unsigned char DataReplace();
unsigned char DataDelete();
unsigned char DataAdd();
unsigned short  DataScan(unsigned char* dataadress,unsigned char* dataendadress,unsigned char* bitmapadress);
unsigned int CRC16_Checkout ( unsigned char *puchMsg, unsigned int usDataLen );
IndexItemType IndexItem;



int main(int argc, char *argv[])
{
    printf("%d  \n",argc);
    printf("sizeof(IndexItem) %d  \n",sizeof(IndexItem));

    unsigned char * index_adress;
    unsigned char * index_end_adress;
    unsigned char * data_save_start_adress;
    unsigned short save_data_num;
    unsigned short save_index_num;

	unsigned char temp;
	
    unsigned char bitmap[BITMAPLEN] = {0xAA};
	//扫描
	DataScan(DataSaveArea,index_end_adress,bitmap);
    printf("save_index_num: %d\n",save_index_num);
	for(temp = 0;temp<10;temp++)
	{
		printf("bitmap :%d\n",bitmap[temp]);		
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
unsigned short  DataScan(unsigned char* datastartadress,unsigned char* dataendadress,unsigned char* bitmap_adress)
{
    unsigned short itemnum = IndexItemMaxNUM;
    unsigned short itemcount;

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
	
	unsigned char bitsave8_1[] = {0x80,0xC0,0xE0,0xF0,0xF8,0xFC,0xFE,0xFF};
	unsigned char bitsave8_2[] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

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
                datalen = *(adress + 3) + 6;		//存储的数据长度
               

                saveprelen = *((unsigned char **)((adress + 4))) - datastartadress;		//保存的数据起始地址到数据域开始长度
				// saveprelen = saveprelen / PrePileSize;									//当前数据域前簇数量
                
				remainder1 	= (saveprelen/PrePileSize)%8;
				pilenum1   	= (saveprelen/PrePileSize)/8; //bitmap 位置
                *(bitmapadress + pilenum1) = bitsave8_1[remainder1];//写入bitmap值
				
				remainder2  = ((datalen - remainder2)% PrePileSize)%8;  	//多余数据簇数量
				pilenum2 	= ((datalen - remainder2)/ PrePileSize)/8;  	//数据簇数量
				
                temp =1;
				while(pilenum2)
				{
					pilenum2 -- ;
					*(bitmapadress + pilenum1 + temp) = 0xFF;
					temp++;					
				}
				*(bitmapadress + pilenum1 + temp) = bitsave8_2[remainder1];
				
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
//存储空间查找                        //地址 总长度 bit数
unsigned char  MallocCreate(unsigned char* bitmapadress,unsigned char spacelen,unsigned char surplus)
{
    unsigned char len = 0;
    unsigned char bitmapnum;
	//存储所需空间
	unsigned char needspace = (spacelen / PrePileSize);
    if((spacelen % PrePileSize)>0)
		needspace++;
	
	while(len < BITMAPLEN)
    {	//空间寻找	
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
	
    return 0;
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





