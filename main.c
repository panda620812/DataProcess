#include <stdio.h>

# define IndexItemSize         10
# define IndexItemMaxNUM     10

# define PrePileSize         10
# define DataItemSize         50
# define DataAreaSize         400

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
char DataSaveArea[500] = {};

unsigned char  DataSave(unsigned char* data_save_start_adress);
unsigned char  IndexCreate();

unsigned char  MallocCreate();
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

    unsigned char bitmap[50] = {0};

   // unsigned short  DataScan(unsigned char* dataadress,unsigned char* dataendadress,unsigned char* bitmapadress);
    printf("save_index_num: %d\n",save_index_num);






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
unsigned short  DataScan(unsigned char* datastartadress,unsigned char* dataendadress,unsigned char* bitmapadress)
{
    // unsigned short itemnum = 0;
    // unsigned short crccalsulate = 0;
    // unsigned short crcget = 0;
    // unsigned char * adress = dataadress;

    // if(0 == *adress)
    // {
        // dataendadress = adress;
        // return itemnum;
    // }
    // while (0 != *adress)
    // {
        // if((0xAA == *adress)&&(0xBB == *(adress + 1)))
        // {
            // crcget = adress[IndexItemSize-1] + adress[IndexItemSize-2];
            // crccalsulate = CRC16_Checkout(adress,sizeof(IndexItemSize - 2));
            // if(crccalsulate == crcget)
            // {
                // itemnum ++;
                // adress += 12;
            // }
        // }
    // }
    // dataendadress = adress +1;

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
    // unsigned short bitmapadress;    	//bitmap adress
    unsigned short bitmapnum;    		//bitmap adress

    unsigned char remainder;//余数
    unsigned char pilenum;//簇数

    while(itemnum)//按照目录数检索
    {
        itemnum --;
//
        if(*adress == 0)
        {
        }
        else if(*adress == 0xAA)
        {
            crcget = adress[IndexItemSize-1] + adress[IndexItemSize-2];
            crccalsulate = CRC16_Checkout(adress,sizeof(IndexItemSize - 2));
            if(crccalsulate == crcget)
            {
                *adress += IndexItemSize;		//下条索引
                itemcount ++;					//当前数据数++
                datalen = *(adress + 3) + 6;	//存储的数据长度
                pilenum = datalen / PrePileSize;  //数据簇数量

                saveprelen = ((adress + 4)) - datastartadress;	//保存的数据起始地址到数据域开始长度
				saveprelen = saveprelen / PrePileSize;					//当前数据域前簇数量
                if(0 == (saveprelen/8))									//
                {
                    temp = saveprelen/8;
					
					while(pilenum)
					{
						pilenum -- ;
					}
					
					
					
					remainder = saveprelen%8;
                    bitmapnum = temp/8;// bitmap 编号
                    while( remainder )
					{
						remainder -- ;
						bitmapadress = bitmapadress +  bitmapnum;
						*bitmapadress += 1;
						*bitmapadress >>1;
					}
					while(temp)
					{
						temp --;
						itmapadress ++;
						*itemadress = 0xff;
					}
                    //temp 超界判断
                }
                else
                {

                }

                //  -> DATALEN
                // itemnum ++;
                // adress += 12;
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
		
		*adress += IndexItemSize;//增加1条距离
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
//存储空间查找
unsigned char  MallocCreate(unsigned char* bitmapadress,unsigned char bitmapnum,unsigned char bitnum)
{
	
	
    return 1;
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





