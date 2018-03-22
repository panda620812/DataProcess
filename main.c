#include"stdio.h"
#include<stdlib.h>
#include<string.h>

# define u8 	unsigned char 
# define u16 	unsigned short
# define u32	unsigned int
# define u64 	unsigned long
# define FALSE 	0
# define TRUE 	1

#pragma pack(1)
typedef struct
{
    unsigned char     	startflag;
    unsigned char     	function;
    unsigned char     	usestate;//0未使用 用途1 2 3
	unsigned char       datalen;
	unsigned int 		datapilenum;//对应地址
	unsigned int 		Time;		//对应时间
    unsigned short     	sumcheck;
}IndexItemType;
#pragma pack()
IndexItemType IndexItem;
# define PILESIZE 10
# define AREASIZE 500
# define INDEXAREASIZE 100
# define DataAREASIZE (AREASIZE-INDEXAREASIZE)

# define PREDATABITLEN 7 //bit 	BYTES = PREDATABITLEN*PILESIZE
# define PREDATABYTESLEN PREDATABITLEN*PILESIZE  //bit 	BYTES = 

# define INDEX_SIZE 14
# define INDEX_NUM 	(INDEXAREASIZE/INDEX_SIZE)

# define PILENUM (DataAREASIZE / PILESIZE)//bit 数//簇数量 = 数据域Size/堆Size

	//标记检测 --- 是否与当前软件设置一致
	//版本号
	//开始
	//Index 条数 长度 起始结束地址
	//Data 	Area
	//簇大小
//bit 清除
void ByteU32ArrayBitClr(unsigned char* data, unsigned short index)
{
    data[index / 8] &= ~(1 << (index % 8));
}
//bit state Get
unsigned char ByteU8ArrayBitGet(unsigned char * data, unsigned short index)
{
    if (data[index / 8] & (1 << (index % 8)))
        return TRUE;
    else
        return FALSE;
}	
// bit Set
void ByteU8ArrayBitSet(u8* data, u32 index)
{
    data[index / 8] |=   (1 << (index % 8));
}
// bits Set
void BytesU8ArrayBitSet(u8* data, u32 index,unsigned char num)
{
    data[index / 8] |=   (1 << (index % 8));
}
unsigned char DataSaveArea[500] = {};	
unsigned char i = 0;

	


unsigned char AreaInit(void)
{
	//写入	Area
	
	//写入	index
	
	//更新	Map

}
//dataArea  -> DataSaveArea
unsigned short IndexInit(unsigned char * bitmap,unsigned char *dataArea)
{

    unsigned char data_len = 0;
	unsigned char temp8 = 0;
	unsigned char temp8_2 = 0;
    unsigned char index_num = 0 ;
    for(;temp8 < INDEX_NUM;temp8 ++)//Index Scan
	{
        if(0XAA == DataSaveArea[temp8 * INDEX_SIZE])
		{
            //CRC   check
			index_num ++ ;
            data_len = ((dataArea[temp8 * INDEX_SIZE + 4])/PILESIZE);//   pre data len/PILESIZE get  //(unsigned char *)
            if(((dataArea[temp8 * INDEX_SIZE + 4])%PILESIZE)>0)
                data_len++;

            for (;temp8_2 < data_len;temp8_2++)
			{			
                ByteU8ArrayBitSet(bitmap,data_len + temp8_2);//bitmap FILL
			}
		}
        temp8++;//next index ++
	}
	return index_num;
}
//bitmap start adress       bitmaplengh         datalengh
//mallocadress = AreaMALLOC(BitMap,PILENUM,PREDATABITLEN);//保存位置
unsigned short AreaMALLOC(unsigned char * bitmap , unsigned short bitmaplengh ,unsigned char datalengh)
{
    unsigned short temp16       = 0;
    unsigned short returnflag   = 0;
    unsigned short adress       = 0;
    unsigned char  templengh    = 0;
	
	for (;temp16 < bitmaplengh;temp16 ++)
	{
		returnflag = ByteU8ArrayBitGet(bitmap,temp16);
		if(0 == returnflag)
		{
			adress = temp16;
            templengh ++;
			if(datalengh == templengh)
			{
                return adress +1 - datalengh ;
			}
		}
		else
		{
			adress = 0;
			templengh = 0;
		}			
	}
	return 0xFFFF;
}
//IndexCreat(datasaveArea,savedatabuf,adresslen,0);
unsigned char IndexCreat(unsigned char * indexstartadress,unsigned char *savedatabuf,unsigned short bitmapadress,unsigned char flag)
{
	time_t t;
	time(&t); 
    unsigned char index_num = 0;//Pre Index number
	unsigned short temp16;
	//NULL Index Find
	for(temp16 = 0;temp16 < INDEX_NUM;temp16++)
	{
		if((* (indexstartadress + temp16*INDEX_SIZE)) == 0xAA)
		{

		}
		else
		{
			if((*(indexstartadress + 2)) == 0x00)//使用标记位 IndexItem.usestate
			//额外的校验，待添加
			break;
		}
        temp16 += INDEX_SIZE;
	}
	//
	{
		IndexItem.startflag 	= 0xAA;
		IndexItem.function 		= 0x01;
		IndexItem.usestate 		= 0x01;
		IndexItem.datalen 		= ((strlen(savedatabuf))/PILESIZE);
		IndexItem.datapilenum 	= bitmapadress;
		IndexItem.Time 			= t;
		IndexItem.sumcheck		= 0x00FF;//待修改
	}
    memcpy(indexstartadress + temp16*INDEX_SIZE,&IndexItem,index_num);// --- 全局变量 Index_num
	index_num ++ ;
    printf("Index++ : %d\n",index_num);
}
/*
 DataSave(DataSaveArea , dataadress,BitMap,saveBuf,PREDATABYTESLEN);
*/
unsigned char DataSave(	unsigned char 	* datasaveArea,	// 数据域起始地址
						unsigned short 	adresslen,		// 数据保存地址到起始地址距离
						unsigned char	* bitmap,		// 位图地址
						unsigned char 	* savedatabuf,	// 保存数据
                        unsigned char 	datalen		// 数据长度
                        )	// 保存的位图位置----
{
/*
    1 datasave
    2 bitmap    refresh
    3 index     refresh
*/
//	unsigned short startadress = 0;
	unsigned char temp8_2 = 0;
	unsigned char temp8_3 = 0;

	//1	数据域起始 + 目录域大小 + 到地址存储区域大小
    memcpy(datasaveArea + INDEXAREASIZE + adresslen,savedatabuf,datalen);//待修改
    //2 位图填充
//	if((datalen%PILESIZE)!=0)
//		temp8_3 = 1;
//        DataSave(DataSaveArea , dataadress,BitMap,saveBuf,PREDATABYTESLEN);
    for (;temp8_2 < ((datalen/PILESIZE));temp8_2++)
        ByteU8ArrayBitSet(bitmap ,( adresslen/PILESIZE) + temp8_2);//bitmap FULL
    //3 目录创建
    IndexCreat(datasaveArea,savedatabuf,adresslen,0);
}

unsigned char DataDelete(	unsigned char 	* datasaveArea,	// 	数据域起始地址
							unsigned short 	index_num,		//	0 起始数
                            unsigned char	* bitmap		// 	位图地址
						)
{

	unsigned short bitmapadress = *((unsigned int *)(datasaveArea + index_num*INDEX_SIZE + 4));//保存的位置
	unsigned short bitmaplengh 	= (*((unsigned char *)(datasaveArea + index_num*INDEX_SIZE + 3)))/8;//（保存的数据长度/8）
	unsigned char temp;
	//Index Delete
	memset(datasaveArea,0,INDEX_SIZE);
	//bitmap Delete
	for(temp = 0;temp < bitmaplengh;temp++) 
		ByteU32ArrayBitClr(bitmap,bitmapadress + temp);
}

unsigned char DataFindFun()
{
	//
	
}
unsigned char DataSortFun()
{
	
	
}unsigned char DatapppFun()
{
	
	
}

unsigned char main()
{
	unsigned short 	IndexSumNum;
	unsigned short 	DeleteIndexNum = 0;
	unsigned char 	temp = 0;
    unsigned char 	BitMap[10] = {0};
	unsigned char 	saveBuf[60] = {1,2,3,4,5,4};
	
	unsigned short 	mallocadress;// bitmap
	unsigned short 	dataadress;// dataArea
	
    IndexSumNum = IndexInit(BitMap,DataSaveArea);
	
    printf("IndexSumNum     %d \n",IndexSumNum);
    printf("DataAREASIZE    %d \n",DataAREASIZE);
    printf("PILENUM/8       %d \n",PILENUM/8);

    //Bit MAP 打印
	for(temp = 0;temp < (PILENUM / 8);temp++)
        printf("BitMap %d %d\n",temp,*(BitMap + temp));

    mallocadress = AreaMALLOC(BitMap,PILENUM,PREDATABITLEN);//保存位置
    if(0XFFFF != mallocadress)
    {
        dataadress = ( mallocadress) * PILESIZE;					//保存位置据保存起始的距离---这个还需要修改下，毕竟重复操作了
        printf("dataadress : %d\n",dataadress);
        DataSave(DataSaveArea , dataadress,BitMap,saveBuf,PREDATABYTESLEN);
    } //Savedata;

    mallocadress = AreaMALLOC(BitMap,PILENUM,PREDATABITLEN);//保存位置
    if(0XFFFF != mallocadress)
    {
        dataadress = ( mallocadress) * PILESIZE;					//保存位置据保存起始的距离
        printf("dataadress : %d\n",dataadress);
        DataSave(DataSaveArea , dataadress,BitMap,saveBuf,PREDATABYTESLEN);
    }

    mallocadress = AreaMALLOC(BitMap,PILENUM,PREDATABITLEN);//保存位置
    dataadress = ( mallocadress) * PILESIZE;					//保存位置据保存起始的距离
//  DataSave(DataSaveArea , dataadress,BitMap,saveBuf,PREDATABYTESLEN);

    for(temp = 0;temp < (PILENUM / 8);temp++)
        printf("BitMap %d %d\n",temp,*(BitMap + temp));
	
    printf("IndexSumNum %d \n",IndexInit(BitMap,DataSaveArea));
	//查询
	
	//排序
	
	//删除

    DataDelete(DataSaveArea,DeleteIndexNum,BitMap);
    i = 0;
    printf("IndexSumNum %d \n",IndexInit(BitMap,&DataSaveArea));
    return 1;

}





/***************************************************************************************************
*\Function      BitPointSet
*\Description   将src指向数据的第pos位置有效。
*\Parameter     src     数据地址
*\Parameter     pos     位
*\Parameter     width   数据宽度
*\Return        void
*               创建函数。
***************************************************************************************************/
// void BitPointSet(void* src, u8 pos, u8 width)
// {
    // switch (width)
    // {
        // case sizeof(u8):
            // (*(u8*)src) |= (1UL<<(pos));
            // break;

        // case sizeof(u16):
            // (*(u16*)src) |= (1UL<<(pos));
            // break;

        // case sizeof(u32):
            // (*(u32*)src) |= (1ULL<<(pos));
            // break;

        // case sizeof(u64):
            // (*(u64*)src) |= (1ULL<<(pos));
            // break;

        // default:

            // break;
    // }
// }

/***************************************************************************************************
*\Function      BitPointClr
*\Description   将src指向数据的第pos位置无效。
*\Parameter     src     数据地址
*\Parameter     pos     位
*\Parameter     width   数据宽度
*\Return        void
*               创建函数。
***************************************************************************************************/
// void BitPointClr(void* src, u8 pos, u8 width)
// {
    // switch (width)
    // {
    // case sizeof(u8):
        // (*(u8*)src) &= ~(1UL<<(pos));
        // break;

    // case sizeof(u16):
        // (*(u16*)src) &= ~(1UL<<(pos));
        // break;

    // case sizeof(u32):
        // (*(u32*)src) &= ~(1ULL<<(pos));
        // break;

    // case sizeof(u64):
        // (*(u64*)src) &= ~(1ULL<<(pos));
        // break;

    // default:

        // break;
    // }
// }

/***************************************************************************************************
*\Function      BitPointNot
*\Description   将src指向数据的第pos位取反。
*\Parameter     src     数据地址
*\Parameter     pos     位
*\Parameter     width   数据宽度
*\Return        void
*               创建函数。
***************************************************************************************************/
// void BitPointNot(void* src, u8 pos, u8 width)
// {
    // switch (width)
    // {
    // case sizeof(u8):
        // (*(u8*)src) ^= (1UL<<(pos));
        // break;

    // case sizeof(u16):
        // (*(u16*)src) ^= (1UL<<(pos));
        // break;

    // case sizeof(u32):
        // (*(u32*)src) ^= (1ULL<<(pos));
        // break;

    // case sizeof(u64):
        // (*(u64*)src) ^= (1ULL<<(pos));
        // break;

    // default:

        // break;
    // }
// }

/***************************************************************************************************
*\Function      BitGet
*\Description   返回value第POS位的值。
*\Parameter     value     数据地址
*\Parameter     pos     位
*\Return        u8
*               创建函数。
***************************************************************************************************/
u8 BitGet(u64 value, u8 pos)
{
    return (value >> pos) & 1;
}

/***************************************************************************************************
*\Function      BitValueSet
*\Description   返回value的第pos位置有效后的值。
*\Parameter     value       数据地址
*\Parameter     pos         位
*\Return        u32
*               创建函数。
***************************************************************************************************/
u64 BitValueSet(u64 value, u8 pos)
{
    return (value) |= (1ULL<<(pos));
}

/***************************************************************************************************
*\Function      BitValueClr
*\Description   返回value第pos位置无效后的值。
*\Parameter     value     数据地址
*\Parameter     pos       位
*\Return        u32
*               创建函数。
***************************************************************************************************/
u64 BitValueClr(u64 value, u8 pos)
{
    return (value) &= ~(1ULL<<pos);
}

/***************************************************************************************************
*\Function      BitValueNot
*\Description   返回value第POS位取反后的值。
*\Parameter     value       数据地址
*\Parameter     pos         位
*\Return        u32
*               创建函数。
***************************************************************************************************/
u64 BitValueNot(u64 value, u8 pos)
{
    return (value) ^= (1ULL<<pos);
}

/***************************************************************************************************
*\Function      BitValueGetSetNum   二分法
*\Description   返回有效位的数目。
*\Parameter     value     数据地址
*\Return        u8
*               创建函数。
***************************************************************************************************/
u8 BitValueGetSetNum(u32 value)
{
    value = (value & 0x55555555) + ((value >> 1)  & 0x55555555);
    value = (value & 0x33333333) + ((value >> 2)  & 0x33333333);
    value = (value & 0x0F0F0F0F) + ((value >> 4)  & 0x0F0F0F0F);
    value = (value & 0x00FF00FF) + ((value >> 8)  & 0x00FF00FF);
    value = (value & 0x0000FFFF) + ((value >> 16) & 0x0000FFFF);
    return (u8)value;
}

/***************************************************************************************************
*\Function      BitValueU8Reverse
*\Description   返回8位value逆序后的值。
*\Parameter     value     数据地址
*\Return        u8
*               创建函数。
***************************************************************************************************/
u8 BitValueU8Reverse(u8 value)
{
    value = (value & 0x55) << 1  | (value & 0xAA) >> 1;
    value = (value & 0x33) << 2  | (value & 0xCC) >> 2;
    value = (value & 0x0F) << 4  | (value & 0xF0) >> 4;

    return value;
}

/***************************************************************************************************
*\Function      BitValueU16Reverse
*\Description   返回16位value逆序后的值。
*\Parameter     value     数据地址
*\Return        u16
*               创建函数。
***************************************************************************************************/
u16 BitValueU16Reverse(u16 value)
{
    value = (value & 0x5555) << 1  | (value & 0xAAAA) >> 1;
    value = (value & 0x3333) << 2  | (value & 0xCCCC) >> 2;
    value = (value & 0x0F0F) << 4  | (value & 0xF0F0) >> 4;
    value = (value & 0x00FF) << 8  | (value & 0xFF00) >> 8;

    return value;
}

/***************************************************************************************************
*\Function      BitValueU32Reverse
*\Description   返回32位value逆序后的值。
*\Parameter     value     数据地址
*\Return        u32
***************************************************************************************************/
u32 BitValueU32Reverse(u32 value)
{
    value = (value & 0x55555555) << 1  | (value & 0xAAAAAAAA) >> 1;
    value = (value & 0x33333333) << 2  | (value & 0xCCCCCCCC) >> 2;
    value = (value & 0x0F0F0F0F) << 4  | (value & 0xF0F0F0F0) >> 4;
    value = (value & 0x00FF00FF) << 8  | (value & 0xFF00FF00) >> 8;
    value = (value & 0x0000FFFF) << 16 | (value & 0xFFFF0000) >> 16;

    return value;
}

/***************************************************************************************************
*\Function      BitValueReverse
*\Description   返回value的低len位反转值。
*\Parameter     value   数据地址
*\Parameter     len     低len位(0~32)
*\Return        u32
*\Note          1) Example: BitValueReverse(0x3e23L, 3) == 0x3e26
*               创建函数。
***************************************************************************************************/
/*
u64 BitValueReverse(u64 value, u8 len)
{
    u64 t = value;
    u8  i = 0x00;

    for (i = 0; i < len; i++)
    {
        if (t & 1ULL)
            value |=  BitMask((len-1)-i);
        else
            value &= ~BitMask((len-1)-i);
        t >>= 1;
    }

    return value;
}
*/
/***************************************************************************************************
*\Function      ByteArrayBitSet
*\Description   设置字节数组中的某一位为1。
*\Parameter     data    字节数组
*\Parameter     index   要设置位的位置，从0开始。
*\Return        void
*               创建函数
***************************************************************************************************/
void ByteArrayBitSet(u8* data, u16 index)
{
    data[index >> 3] |= (1 << (index & 7));
}

/***************************************************************************************************
*\Function      ByteArrayBitClr
*\Description   清除字节数组中的某一位为0。
*\Parameter     data    字节数组
*\Parameter     index   要清除的位的位置，从0开始。
*\Return        void
*               创建函数
***************************************************************************************************/
void ByteArrayBitClr(u8* data, u16 index)
{
    data[index >> 3] &= ~(1 << (index & 7));
}

/***************************************************************************************************
*\Function      ByteArrayBitGet
*\Description   获取字节数组中的某一位的状态。
*\Parameter     data    字节数组
*\Parameter     index   希望获得的位的位置，从0开始
*\Return        bool    位为1时返回TRUE，位为0时返回FALSE。
*               创建函数
***************************************************************************************************/
unsigned char ByteArrayBitGet(u8* data, u16 index)
{
    if (data[index >> 3] & (1 << (index & 7)))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


/***************************************************************************************************
*\Function      ByteU32ArrayBitSet
*\Description   设置字节数组中的某一位为1。
*\Parameter     data    字节数组
*\Parameter     index   要设置位的位置，从0开始。
*\Return        void
*               创建函数
***************************************************************************************************/
void ByteU32ArrayBitSet(u32* data, u16 index)
{
    data[index / 32] |= (1 << (index % 32));
}


/***************************************************************************************************
*\Function      ByteU32ArrayBitGet
*\Description   获取字节数组中的某一位的状态。
*\Parameter     data    字节数组
*\Parameter     index   希望获得的位的位置，从0开始
*\Return        bool    位为1时返回TRUE，位为0时返回FALSE。
*               创建函数
***************************************************************************************************/
// bool ByteU32ArrayBitGet(u32* data, u16 index)
// {
    // if (data[index / 32] & (1 << (index % 32)))
    // {
        // return TRUE;
    // }
    // else
    // {
        // return FALSE;
    // }
// }
int findNumberOfTrailingZeros(unsigned int v)
{
    static const int MultiplyDeBruijnBitPosition[32] =
    {
        0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8,
        31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
    };
    return MultiplyDeBruijnBitPosition[((unsigned int)((v & -v) * 0x077CB531U)) >> 27];
}


static const u8 LSBTable[32] =
{
    0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8,
    31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
};

/***************************************************************************************************
*\Function      BitTrailingZeroCount
*\Description   从低位到高位寻找第一个置1的位索引。
*\Parameter     data    要找的目标
*\Parameter     pos     置1的位的索引(0～31）。
*\Return        bool    若参数正确且找到置1的位，则返回TRUE，否则返回FALSE。
*               创建函数。
***************************************************************************************************/
unsigned char  BitTrailingZeroCount(u32 data, u8* pos)
{
    if (!data || !pos)
    {
        return FALSE;
    }

    *pos = LSBTable[(((data & -(signed int)data) * 0x077CB531UL)) >> 27];

    return TRUE;
}

/***************************************************************************************************
*\Function      BitTrailingZeroCountWithStart
*\Description   从起始位start_pos起，从低位到高位寻找第一个置1的位索引，包含start_pos。
*\Parameter     data            要找的目标
*\Parameter     start_pos       起始位(0～31）
*\Parameter     pos             置1的位的索引(0～31）
*\Return        bool            若参数正确且找到置1的位，则返回TRUE，否则返回FALSE。
*\Note
*               创建函数。
***************************************************************************************************/
unsigned char  BitTrailingZeroCountWithStart(u32 data, u8 start_pos, u8* pos)
{
    /*屏蔽低位*/
    data &= (~0ULL) << (start_pos);

    if (!data || !pos)
    {
        return FALSE;
    }

    *pos = LSBTable[(((data & -(signed int)data) * 0x077CB531UL)) >> 27];

    return TRUE;
}

/***************************************************************************************************
*\Function      BitTrailingZeroCountWithEnd
*\Description   到终止位end_pos止，从低位到高位寻找第一个置1的位索引，包含end_pos。
*\Parameter     data            要找的目标
*\Parameter     start_pos       终止位(0～31）
*\Parameter     pos             置1的位的索引(0～31）
*\Return        bool            若参数正确且找到置1的位，则返回TRUE，否则返回FALSE。
*\Note          该函数有问题，对立即数位移变量时，当变量大于等于32时位移无效，导致0UL写法存在问题，待重写（121212）
*               创建函数。
***************************************************************************************************/
unsigned char BitTrailingZeroCountWithEnd(u32 data, u8 end_pos, u8* pos)
{
    unsigned char ok = FALSE;
    while(ok == FALSE);

    /*屏蔽低位*/
    data &= (~0ULL) >> (32 - end_pos + 1);

    if (!data || !pos)
    {
        return FALSE;
    }

    *pos = LSBTable[(((data & -(signed int)data) * 0x077CB531UL)) >> 27];
    return TRUE;
}


















