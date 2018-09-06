#include"ext_ad9984.h"
#include"his_iic.h"
#include"his_gpio.h"

#define AD9984_FILE_PATH "./env/ad9984.dat"
typedef struct EXT_VGA_FORMAT_S
{
 WV_U16  total_line     ;
 WV_U16  total_pix      ;
 WV_U16  hor_freq       ;
 WV_U16  act_pix        ;
 WV_U16  act_line       ;
 WV_U16  vi_de_left     ;
 WV_U16  vi_de_top      ;
 WV_U16  ad_set         ;
 WV_U16  hsvs_pol       ;
 WV_U8   formati        ;
}EXT_VGA_FORMAT_S;

typedef struct EXT_AD9984_DEV_E 
{
    WV_THR_HNDL_T     thrHndl; 
    WV_U32      		open;
    WV_U32      		close; 
    
   HIS_IIC_HNDL      iicHndl; 
   WV_U8             iicBus;
   WV_U8             addr; 
   //stat
   WV_U8       init;
   WV_U32      formate; 
    
}EXT_AD9984_DEV_E;


static  EXT_AD9984_DEV_E   gAd9984Dve;

EXT_VGA_FORMAT_S  vga_format_table[] = {
  {525 , 800,3972, 640, 480, 144, 35, 0x28, 3, 0}, //640x480x60 pix_clk=25.175 line_rate = 31.469khz 3972 = 125000/31.5
  {520 , 832,3301, 640, 480, 168, 31, 0x60, 3, 0}, //640x480x72 pix_clk=31.500 line_rate = 37.861khz     
  {500 , 840,3333, 640, 480, 184, 19, 0x60, 3, 0}, //640x480x75 pix_clk=31.500 line_rate = 37.5khz
  {509 , 832,2888, 640, 480, 136, 28, 0x60, 3, 0}, //640x480x85 pix_clk=36.000 line_rate = 43.269khz
        		       
  {625 ,1024,3555, 800, 600, 206, 25, 0x60, 0, 0}, //800x600x56 pix_clk=36.000 line_rate = 35.156khz
  {622 ,1024,3349, 800, 600, 192, 23, 0x68, 0, 0}, //800x600x60g pix_clk=38.220 line_rate = 37.320khz
  {628 ,1056,3299, 800, 600, 222, 28, 0x68, 0, 0}, //800x600x60 pix_clk=40.000 line_rate = 37.879khz     
  {666 ,1040,2599, 800, 600, 190, 30, 0x68, 0, 0}, //800x600x72 pix_clk=50.000 line_rate = 48.077khz
  {625 ,1056,2666, 800, 600, 245, 25, 0x68, 0, 0}, //800x600x75 pix_clk=49.500 line_rate = 46.875khz
  {631 ,1048,2328, 800, 600, 222, 31, 0x70, 0, 0}, //800x600x85 pix_clk=56.250 line_rate = 53.674khz
  
  //  {622 ,1232,3349, 960, 600, 268, 21, 0x58}, //960x600x60 pix_clk=45.980 line_rate = 37.320khz
  
  //{795 ,1344,2620,1024, 768, 264, 26, 0xa0}, //1024x768x60g pix_clk=64.110 line_rate = 47.700khz
  {806 ,1344,2584,1024, 768, 301, 36, 0xa0, 3, 0}, //1024x768x60 pix_clk=65.000  line_rate = 48.363khz
  {806 ,1328,2213,1024, 768, 280, 35, 0xa8, 3, 0}, //1024x768x70 pix_clk=75.000  line_rate = 56.476khz     
  {800 ,1312,2082,1024, 768, 277, 32, 0xa8, 0, 0}, //1024x768x75 pix_clk=78.750  line_rate = 60.023khz
  {808 ,1376,1820,1024, 768, 309, 40, 0xb0, 0, 0}, //1024x768x85 pix_clk=94.500  line_rate = 68.677khz

  {895 ,1520,2327,1152, 864, 309, 31, 0xa8, 0, 0}, //1152x864x60 pix_clk=81.600  line_rate = 53.700khz
  {900 ,1600,1851,1152, 864, 389, 36, 0xb0, 0, 0}, //1152x864x75 pix_clk=108.000 line_rate = 67.500khz
  {907 ,1552,1621,1152, 864, 332, 43, 0xb0, 0, 0}, //1152x864x85 pix_clk=119.700 line_rate = 77.100khz

  //  {795 ,1680,2637,1280, 768, 112, 19, 0x60}, //1280x768x60g pix_clk=81.140 line_rate = 47.700khz

  {746 ,1664,2792,1280, 720, 315, 24, 0x98, 0, 0}, //1280x720x60  pix_clk=74.480 line_rate = 44.76khz
  {750 ,1650,2777,1280, 720, 306, 25, 0x98, 0, 0}, //1280x720x60  pix_clk=74.480 line_rate = 45.00khz
//  {750 ,1650,2777,1260, 720, 295, 26, 0x98, 0}, //1280x720x60  pix_clk=74.480 line_rate = 45.00khz  for guodian
  {752 ,1696,2216,1280, 720, 344, 31, 0xb0, 0, 0}, //1280x720x75  pix_clk=95.650 line_rate = 56.40khz

  {790 ,1440,2637,1280, 768, 112, 19, 0xa0, 1, 0}, //1280x768x60r pix_clk=68.250 line_rate = 47.396khz
  {798 ,1664,2616,1280, 768, 325, 28, 0xa8, 2, 0}, //1280x768x60  pix_clk=79.500 line_rate = 47.776khz
  {805 ,1696,2073,1280, 768, 338, 35, 0xb0, 2, 0}, //1280x768x75 pix_clk=102.250 line_rate = 60.289khz
  {809 ,1712,1821,1280, 768, 345, 39, 0xb0, 2, 0}, //1280x768x85 pix_clk=117.500 line_rate = 68.633khz
  
  {828 ,1680,2516,1280, 800, 339, 28, 0xa8, 1, 0}, //1280x800x60  pix_clk=83.460 line_rate = 49.68khz
  {835 ,1712,1996,1280, 800, 354, 35, 0xb0, 1, 0}, //1280x800x75  pix_clk=107.210 line_rate = 62.62khz
  
  {1000,1800,2083,1280, 960, 429, 40, 0xb0, 0, 0}, //1280x960x60  pix_clk=108.250 line_rate = 60.000khz
  {1011,1728,1454,1280, 960, 388, 51, 0xf0, 0, 0}, //1280x960x85  pix_clk=148.500 line_rate = 85.938khz

  {1066,1688,1953,1280,1024, 365, 42, 0xb0, 0, 0},//1280x1024x60  pix_clk=108.000 line_rate = 63.981khz
//  {1066,1688,1953,1280,1024, 362, 42, 0xb0, 0},//1280x1024x60  pix_clk=108.000 line_rate = 63.981khz for guodian
  {1066,1688,1562,1280,1024, 396, 41, 0xf0, 0, 0},//1280x1024x75  pix_clk=135.000 line_rate = 79.976khz
  {1072,1728,1371,1280,1024, 388, 48, 0xf0, 0, 0},//1280x1024x85  pix_clk=157.500 line_rate = 91.146khz
  
  {795 ,1792,2619,1360, 768, 372, 25, 0xa8, 0, 0},//1360x768x60  pix_clk=85.500 line_rate = 47.712khz
  {798 ,1792,2619,1366, 768, 364, 24, 0xa8, 0, 0},//1366x768x60  pix_clk=85.500 line_rate = 47.712khz
  
  //{1080,1560,1930,1400,1050, 112, 27, 0xb0, 1, 0},//1400x1050x60r pix_clk=101.000 line_rate = 64.744khz//*为了给出1680*1050*60r（r为vesa标准里的reduced blanking时序）
  //{1089,1864,1913,1400,1050, 232, 36, 0xb0, 2, 0},//1400x1050x60  pix_clk=121.750 line_rate = 65.317khz//*为了给出1680*1050*60（无r为vesa标准里的非reduced blanking时序）
  {1099,1896,1519,1400,1050, 248, 46, 0xf0, 2, 0},//1400x1050x75  pix_clk=156.000 line_rate = 82.278khz
  
  { 926,1600,2253,1440, 900, 112, 23, 0xa8, 1, 0},//1440x900x60r  pix_clk=88.750 line_rate = 55.469khz
  { 934,1904,2234,1440, 900, 388, 32, 0xa8, 2, 0},//1440x900x60  pix_clk=106.500 line_rate = 55.935khz
  { 942,1936,1769,1440, 900, 400, 39, 0xf0, 2, 0},//1440x900x75  pix_clk=136.750 line_rate = 70.635khz
  { 948,1952,1554,1440, 900, 256, 45, 0xf0, 2, 0},//1440x900x85  pix_clk=157.000 line_rate = 80.430khz
  
   
  //  {1000,2128,2082,1600, 900, 264, 31, 0xb0, 0},//1600x900x60  pix_clk=119.000 line_rate = 55.920khz
  { 932,2128,2235,1600, 900, 264, 31, 0xb0, 0, 0},//1600x900x60  pix_clk=119.000 line_rate = 55.920khz
  { 940,2160,1773,1600, 900, 460, 41, 0xf0, 0, 0},//1600x900x75  pix_clk=152.280 line_rate = 70.500khz
  
  {1250,2160,1666,1600,1200, 498, 50, 0xf0, 0, 0},//1600x1200x60  pix_clk=162.000 line_rate = 75.000khz
  
  {1080,1840,1932,1680,1050, 116, 27, 0xb0, 1},//1680x1050x60r pix_clk=119.000 line_rate = 64.674khz
  {1089,2240,1914,1680,1050, 460, 36, 0xf0, 2},//1680x1050x60  pix_clk=146.250 line_rate = 65.290khz//de_left=sync_width+back_porch+4(4为经验值)，de_top=sync_width+v_back_porch+0
 
  {1125,2200,1853,1920,1080, 196, 41, 0xf0, 1, 0},//1920x1080x60r pix_clk=154.000 line_rate = 74.038khz//*+4是我通过处理器环回测试过的
  
  {1235,2080,1688,1920,1200, 112, 32, 0xf0, 1, 0},//1920x1200x60r pix_clk=154.000 line_rate = 74.038khz//水平上没有加4，因为这个是以前的参数
  
  //{1125,2640,1853,1920,1080, 246, 32, 0xf0, 1, 0},//1920x1080x50r pix_clk=148.500 line_rate = 54.25khz//某个摄像头的参数，但是与上两行的60r的行频和场频参数相同了！
  {1125,2640,2222,1920,1080, 246, 32, 0xf0, 1, 0},//1920x1080x50r pix_clk=148.500 line_rate = 1125*50=56.25khz
  //{1125,2640,2222,1920,1080, 193, 41, 0xf0, 1, 0},//某个现场调试后给出的参数
  //{1125,2640,4444,1920,1080, 246, 32, 0xb8, 1}//1920x1080x25r pix_clk=74.250 line_rate = 28.125khz--don't work!
  //{1125,2200,3704,1920,1080, 246, 32, 0xb8, 1} //1920x1080x30r pix_clk=74.250 line_rate = 33.750khz--don't work!
  //as the same as yuv
  {1125,2200,3704,1920,1080, 240, 41, 0xa8, 1, 0},//1920x1080px30  pix_clk=74.25 line_rate = 33.75khz
  {1125,2640,4438,1920,1080, 240, 41, 0xa8, 1, 0} //1920x1080px25  pix_clk=74.25 line_rate = 33.75khz
};




/**********************************************************************************

WV_S32 EXT_9984_IicRead(EXT_SI7127_DEV_E * pDev ,WV_U8 sel,WV_U8 reg,WV_U8 *pData);

**********************************************************************************/
WV_S32 EXT_9984_IicRead(EXT_AD9984_DEV_E * pDev ,WV_U8 reg,WV_U8 *pData)
{ 
   
	   WV_CHECK_RET( HIS_IIC_Read8 (pDev-> iicHndl,pDev-> addr, reg, pData, 1) ); 

       //  WV_printf("AD9984 Rd  %#x  = %#x(%d)\r\n",reg,*pData,*pData);
  return WV_SOK;	
}
/**********************************************************************************

WV_S32 EXT_9984_Write(EXT_AD9984_DEV_E * pDev ,WV_U8 data);

**********************************************************************************/
WV_S32 EXT_9984_IicWrite(EXT_AD9984_DEV_E * pDev,WV_U8 reg,WV_U8 data)
{ 
    
        WV_CHECK_RET( HIS_IIC_Write8 (pDev-> iicHndl,pDev-> addr, reg,&data, 1) );
     //   WV_printf("AD9984 Write  %#x  = %#x(%d)\r\n",reg,data,data);
  return WV_SOK;	
}

/**********************************************************************************

WV_S32 EXT_9984_AutoOffset(EXT_AD9984_DEV_E * pDev );

**********************************************************************************/
WV_S32 EXT_9984_AutoOffset(EXT_AD9984_DEV_E * pDev )
{
 
	EXT_9984_IicWrite(pDev,0x0b,0x00);       //Red offset Msb[7:0]   
	EXT_9984_IicWrite(pDev,0x0c,0x80);       //Red offset Lsb[7:5] = roffset[1:0] lsb must be write for updata red gains   
	EXT_9984_IicWrite(pDev,0x0d,0x00);       //Green offset MSB  
	EXT_9984_IicWrite(pDev,0x0e,0x80);       //Green offset LSB  
	EXT_9984_IicWrite(pDev,0x0f,0x00);       //Blue offset MSB  
	EXT_9984_IicWrite(pDev,0x10,0x80);       //Blue offset LSB  
	EXT_9984_IicWrite(pDev,0x18,0x20);       //Blue offset LSB  
	EXT_9984_IicWrite(pDev,0x1b,0x7b);
	
	return WV_SOK;
}
/**********************************************************************************

WV_S32 EXT_9984_Conf_test(EXT_AD9984_DEV_E * pDev );

**********************************************************************************/
WV_S32 EXT_9984_Conf_test(EXT_AD9984_DEV_E * pDev )
{	

	WV_U8 buf[32];
	WV_U8 temp[2];

	FILE *fp;
	WV_U32 addr,val,data;
	//WV_U8 val,addr,data;	
	WV_S32 i=0;
	fp = fopen(AD9984_FILE_PATH,"rb");
	if(fp == NULL)
	{
		printf("open ad9984 conf file error \n");
		return 0;
	}
	
	for(i=0;i<2;i++)
	{
		
		if(fgets(buf,sizeof(buf),fp) == NULL)
		{
			fclose(fp);
			return 0;
		}	
		if(buf[0] == ' ')
		{
			fclose(fp);
			return -1;

		}

		temp[0]=buf[0];
		temp[1]=buf[1];
		
		sscanf(temp,"%x",&addr);
		//printf("addr=%02x[%d],[addr=%p][val=%p]\n",addr,addr,&addr,&val);
		//addr = data;
		if(buf[2] == ' ')
		{
			temp[0]=buf[3];
			temp[1]=buf[4];
			sscanf(temp,"%x",&val);
			//val = data;		
		}	
		//printf("addr=%02x[%d]\n",addr,addr);
		printf("*********ad9984,addr=%02x,val=%02x************\n",addr,val);
		EXT_9984_IicWrite(pDev,addr,val);  
	}

	fclose(fp);
  	//EXT_9984_IicWrite(pDev,0x1f,0x92);       //Output select1//解决部分vga输入板卡有抖动现象  // 0x93  0x92
    //EXT_9984_IicWrite(pDev,0x20,0x45);       //Output select2  //phase 90// 调整相位 0x45 
	
	
}
/**********************************************************************************

WV_S32 EXT_9984_Conf(EXT_AD9984_DEV_E * pDev );

**********************************************************************************/
WV_S32 EXT_9984_Conf(EXT_AD9984_DEV_E * pDev )
{
	WV_U8  temp_byte;
	WV_U16  temp=1688;
	EXT_9984_IicRead(pDev,0x00,&temp_byte); //reg 00 chip_id
	temp_byte = (temp>>4);
	EXT_9984_IicWrite(pDev,0x01,temp_byte);  //PLLDIV_MSB
  temp_byte = (temp&0xf)<<4;
  EXT_9984_IicWrite(pDev,0x02,temp_byte);  //PLLDIV_LSB
  EXT_9984_IicWrite(pDev,0x03,0xb0);       //VCO&cpmp 
  EXT_9984_IicWrite(pDev,0x04,0x80);       //phase   
  EXT_9984_IicWrite(pDev,0x05,0x50);       //Red gain Msb[6:0] = rgain[8:2]    EXT_9984_IicWrite(pDev,0x05,0x50);
  EXT_9984_IicWrite(pDev,0x06,0x00);       //Red gain Lsb[7:6] = rgain[1:0] lsb must be write for updata red gains   
  EXT_9984_IicWrite(pDev,0x07,0x50);       //Green gain MSB  
  EXT_9984_IicWrite(pDev,0x08,0x00);       //Green gain LSB  
  EXT_9984_IicWrite(pDev,0x09,0x50);       //Blue gain MSB  
  EXT_9984_IicWrite(pDev,0x0a,0x00);       //Blue gain LSB  
  EXT_9984_IicWrite(pDev,0x0b,0x40);       //Red offset Msb[7:0]   
  EXT_9984_IicWrite(pDev,0x0c,0x00);       //Red offset Lsb[7:5] = roffset[1:0] lsb must be write for updata red gains   
  EXT_9984_IicWrite(pDev,0x0d,0x40);       //Green offset MSB  
  EXT_9984_IicWrite(pDev,0x0e,0x00);       //Green offset LSB  
  EXT_9984_IicWrite(pDev,0x0f,0x40);       //Blue offset MSB  
  EXT_9984_IicWrite(pDev,0x10,0x00);       //Blue offset LSB    
  EXT_9984_IicWrite(pDev,0x11,0x20);       //SYNC Separator Threshold   
  EXT_9984_IicWrite(pDev,0x12,0x98);       //HSYNC Ctrl [7]=source override [6]=source   
  EXT_9984_IicWrite(pDev,0x13,0x20);       //HSOUT Duration 
  EXT_9984_IicWrite(pDev,0x14,0x98);       //VSync Ctrl
  EXT_9984_IicWrite(pDev,0x15,0x05);       //VSync Duration
  EXT_9984_IicWrite(pDev,0x16,0x00);       //Precoast
  EXT_9984_IicWrite(pDev,0x17,0x00);       //PostCoast  
  EXT_9984_IicWrite(pDev,0x18,0x20);       //Coast&Clamp Ctrl  
  EXT_9984_IicWrite(pDev,0x19,0x08);       //Clamp Placement  
  EXT_9984_IicWrite(pDev,0x1a,0x28);       //Clamp Duration  
  EXT_9984_IicWrite(pDev,0x1b,0x53);       //Auto_Clamp enable and offset updata every 3 field
  EXT_9984_IicWrite(pDev,0x1c,0xff);       //test register
  EXT_9984_IicWrite(pDev,0x1d,0x79);       //sog ctrl
  EXT_9984_IicWrite(pDev,0x1e,0xa4);       //input and power
  //EXT_9984_IicWrite(pDev,0x1f,0x90);       //Output select1
  EXT_9984_IicWrite(pDev,0x1f,0x92);       //Output select1//解决部分vga输入板卡有抖动现象  // 0x93  0x92
  //EXT_9984_IicWrite(pDev,0x20,0x04);       //Output select2  //phase 90
  EXT_9984_IicWrite(pDev,0x20,0x45);       //Output select2  //phase 90// 调整相位 0x45 
  EXT_9984_IicWrite(pDev,0x21,0x20);       //
  EXT_9984_IicWrite(pDev,0x22,0x32);       //
  EXT_9984_IicWrite(pDev,0x23,0x0a);       //sync filter window
  EXT_9984_IicWrite(pDev,0x28,0xbf);       //
  EXT_9984_IicWrite(pDev,0x29,0x02);       //
  EXT_9984_IicWrite(pDev,0x2c,0x00);       //offset hold
  EXT_9984_IicWrite(pDev,0x2d,0xe8);       //
  EXT_9984_IicWrite(pDev,0x2e,0xe0);       //
  EXT_9984_IicWrite(pDev,0x3c,0x00);       //autogain hold enable

  EXT_9984_Conf_test(pDev);
  EXT_9984_AutoOffset(pDev ); 
	return WV_SOK;
}

/**********************************************************************************

WV_S32 EXT_9984_Dect(EXT_AD9984_DEV_E * pDev );

**********************************************************************************/
WV_S32 EXT_9984_Dect(EXT_AD9984_DEV_E * pDev )
{

   WV_U8 temp_byte;
   WV_U16  temp;
   WV_S32  i;
   WV_U32  horFreq;
   
   EXT_FPGA_GetVgaHnum(&horFreq);
   horFreq = horFreq *5 /4; // clk 100M  to 125M  
  if(pDev->init == 0){ 
  	EXT_9984_Conf(pDev );
  	pDev->init = 1;
  	pDev->formate = sizeof(vga_format_table)/sizeof(EXT_VGA_FORMAT_S);
  	return WV_SOK;
  	}
  	
     EXT_9984_IicRead(pDev,0x26,&temp_byte); //HSYNC COUNTER
	 temp = (temp_byte<<4);
     EXT_9984_IicRead(pDev,0x27,&temp_byte); //HSYNC COUNTER
	 temp = (temp_byte>>4) + temp;
	 for (i=0;i<sizeof(vga_format_table)/sizeof(EXT_VGA_FORMAT_S);i++)
		{  
			if ((temp>=vga_format_table[i].total_line-1) && (temp<=vga_format_table[i].total_line+1)){
				if(horFreq >= vga_format_table[i].hor_freq -10  && horFreq <= vga_format_table[i].hor_freq +10){

				break;
				}
			}
		}
		
	if(i ==   pDev->formate)
	{
	   return WV_SOK;
    }	
		
	 else if ( i!= (sizeof(vga_format_table)/sizeof(EXT_VGA_FORMAT_S)))
		{
		  temp_byte = vga_format_table[i].ad_set;
		   EXT_9984_IicWrite(pDev,0x03, temp_byte);
		   temp = vga_format_table[i].total_pix;
		   temp_byte = temp >> 4;
		   EXT_9984_IicWrite(pDev,0x01, temp_byte);
		   temp_byte = temp <<4;
		   EXT_9984_IicWrite(pDev,0x02, temp_byte); 
		   
		   
		 //  EXT_FPGA_Write(0x2d,0); 
		   EXT_FPGA_Write(0x29,vga_format_table[i].act_pix);
		   EXT_FPGA_Write(0x2a,vga_format_table[i].act_line); 
		   EXT_FPGA_Write(0x2b,vga_format_table[i].vi_de_top);
		   EXT_FPGA_Write(0x2c,vga_format_table[i].vi_de_left-7); 
		   EXT_FPGA_Write(0x2d,1); 
		   
           pDev->formate = i;
		   printf("*********set 9984 [%d]**************\n",i);
		} 
    else
      {
          temp_byte = vga_format_table[0].ad_set;
		  EXT_9984_IicWrite(pDev,0x03, temp_byte);
		  temp = vga_format_table[0].total_pix;
		  temp_byte = temp >> 4;
		  EXT_9984_IicWrite(pDev,0x01, temp_byte);
		  temp_byte = temp <<4;
		   EXT_9984_IicWrite(pDev,0x02, temp_byte);
           pDev->formate = 255;
       }

  return  WV_SOK;
}



/**********************************************************************************

WV_S32 EXT_9984_GetResolution(EXT_AD9984_DEV_E * pDev );

**********************************************************************************/
WV_S32 EXT_9984_GetResolution(EXT_AD9984_DEV_E * pDev ,WV_U32 *pWidth,WV_U32 *pHeight,WV_U32 *pFrameRate)
{  
   if(pDev->formate ==  sizeof(vga_format_table)/sizeof(EXT_VGA_FORMAT_S)) return WV_EFAIL;
   *pWidth = vga_format_table[pDev->formate]. act_pix;
   *pHeight = vga_format_table[pDev->formate]. act_line;
   *pFrameRate  = 0;
   return WV_SOK; 
}
 
/**********************************************************************************

WV_S32 EXT_9984_Open(EXT_AD9984_DEV_E * pDev);

**********************************************************************************/
WV_S32 EXT_9984_Open(EXT_AD9984_DEV_E * pDev)
{  
   HIS_IIC_Open(&pDev-> iicHndl,pDev->iicBus); 
   //EXT_9984_Dect(pDev);  
   
   
    
   return WV_SOK; 
}
/**********************************************************************************

WV_S32 EXT_9984_Close(EXT_AD9984_DEV_E * pDev);

**********************************************************************************/
WV_S32 EXT_9984_Close(EXT_AD9984_DEV_E * pDev)
{  
   HIS_IIC_Close(&pDev-> iicHndl); 
   return WV_SOK; 
}

/**********************************************************************************

WV_S32  EXT_AD9984_Detec();

**********************************************************************************/
WV_S32  EXT_AD9984_Detec()
{

   return   EXT_9984_Dect(&gAd9984Dve);
}


/**********************************************************************************

WV_S32  EXT_AD9984_GetInfo(WV_U32 *width,WV_U32 *height,WV_U32 * frameRate);

**********************************************************************************/
WV_S32  EXT_AD9984_GetInfo(WV_U32 *width,WV_U32 *height,WV_U32 * frameRate) 
{

   return   EXT_9984_GetResolution(&gAd9984Dve,width,height,frameRate);
}
 
/**********************************************************************************

void *EXT_9984_Proc(Void * prm)
 **********************************************************************************/																																																																																										
void *EXT_9984_Proc(void * prm)
{
 
	EXT_AD9984_DEV_E   * pDev;  

	pDev = ( EXT_AD9984_DEV_E *) prm; 
	EXT_9984_Open(pDev);
	pDev-> open  = 1;
	pDev-> close  = 0;  
	while(pDev -> open == 1)
	{ 
	  EXT_9984_Dect(pDev);
	  usleep(100000); 
	} 
	EXT_9984_Close(pDev); 
	return NULL;
 }

/****************************************************************************

WV_S32 EXT_AD9984_CMDGet(WV_S32 argc, WV_S8 **argv,WV_S8 *prfBuff)

****************************************************************************/
WV_S32 EXT_AD9984_CMDGet(WV_S32 argc, WV_S8 **argv, WV_S8 * prfBuff)
{
	     
	if(argc<1) 
		{
		prfBuff += sprintf(prfBuff,"ad9984 get <stat>\r\n");
		return WV_SOK;
		}
    if(strcmp(argv[0],"stat") ==0)
	{
	   prfBuff += sprintf(prfBuff,"ad9984  stat:\r\n");
	     WV_U8 temp_byte;
         WV_U16  temp; 
	   EXT_9984_IicRead(&gAd9984Dve,0x26,&temp_byte); //HSYNC COUNTER
	  temp = (temp_byte<<4);
      EXT_9984_IicRead(&gAd9984Dve,0x27,&temp_byte); //HSYNC COUNTER
	   temp = (temp_byte>>4) + temp;
	  prfBuff += sprintf(prfBuff,"totol pix : %d\r\n",temp); 
	   
	  EXT_9984_IicRead(&gAd9984Dve,0x01,&temp_byte); //pll 
	  temp = (temp_byte<<4);
      EXT_9984_IicRead(&gAd9984Dve,0x02,&temp_byte); //pll
	  temp = (temp_byte>>4) + temp; 
	   
	  prfBuff += sprintf(prfBuff,"pll div : %d\r\n",temp);  
	  EXT_9984_IicRead(&gAd9984Dve,0x03,&temp_byte); //clock generate ctrl
	  prfBuff += sprintf(prfBuff,"clk control : %#x\r\n",temp_byte);
	  
	    prfBuff += sprintf(prfBuff,"input  mode :\r\n"); 
	    prfBuff += sprintf(prfBuff,"total_line  :%d\r\n",vga_format_table[gAd9984Dve. formate].total_line);
	    prfBuff += sprintf(prfBuff,"total_pix   :%d\r\n",vga_format_table[gAd9984Dve. formate].total_pix);  
	    prfBuff += sprintf(prfBuff,"hor_freq    :%d\r\n",vga_format_table[gAd9984Dve. formate].hor_freq);
	    prfBuff += sprintf(prfBuff,"act_pix     :%d\r\n",vga_format_table[gAd9984Dve. formate].act_pix);
	    prfBuff += sprintf(prfBuff,"act_line    :%d\r\n",vga_format_table[gAd9984Dve. formate].act_line);
	    prfBuff += sprintf(prfBuff,"vi_de_left  :%d\r\n",vga_format_table[gAd9984Dve. formate].vi_de_left);
	    prfBuff += sprintf(prfBuff,"vi_de_top   :%d\r\n",vga_format_table[gAd9984Dve. formate].vi_de_top);
	    prfBuff += sprintf(prfBuff,"ad_set      :%#x\r\n",vga_format_table[gAd9984Dve. formate].ad_set);
	    prfBuff += sprintf(prfBuff,"hsvs_pol    :%d\r\n",vga_format_table[gAd9984Dve. formate].hsvs_pol);
	    prfBuff += sprintf(prfBuff,"formati     :%d\r\n",vga_format_table[gAd9984Dve. formate].formati);
	   return WV_SOK;
	   } 
     prfBuff += sprintf(prfBuff,"no the cmd!!!!!!\r\n"); 
  return WV_SOK;
}



/****************************************************************************

WV_S32 EXT_AD9984_CMDSet(WV_S32 argc, WV_S8 **argv,WV_S8 *prfBuff)

****************************************************************************/
WV_S32 EXT_AD9984_CMDSet(WV_S32 argc, WV_S8 **argv, WV_S8 * prfBuff)
{
	    
	if(argc<1) 
		{
		prfBuff += sprintf(prfBuff,"ad9984 set <dtec>\r\n");
		return WV_SOK;
		}
    if(strcmp(argv[0],"dtec") ==0)
	{
	   prfBuff += sprintf(prfBuff,"ad9984  re detec:\r\n");  
	   EXT_9984_Dect(&gAd9984Dve);
	   return WV_SOK;
	}
	
	prfBuff += sprintf(prfBuff,"no the cmd!!!!!!\r\n"); 
  return WV_SOK;
}





/**********************************************************************************

WV_S32 EXT_AD9984_Init();

**********************************************************************************/
WV_S32 EXT_AD9984_Init()
{  
   
    WV_CMD_Register("ad9984","get","get ad9984 info",EXT_AD9984_CMDGet);
    WV_CMD_Register("ad9984","set","get ad9984 conf",EXT_AD9984_CMDSet);
    gAd9984Dve.iicBus  = 0;
    gAd9984Dve.addr  = 0x98;
    gAd9984Dve.init  = 0; 

    //EXT_9984_Open(&gAd9984Dve);
    WV_THR_Create(&(gAd9984Dve.thrHndl), EXT_9984_Proc , WV_THR_PRI_DEFAULT, WV_THR_STACK_SIZE_DEFAULT, &gAd9984Dve);  
    return WV_SOK; 
}

/**********************************************************************************

WV_S32 EXT_AD9984_DeInit();

**********************************************************************************/
WV_S32 EXT_AD9984_DeInit()
{    
  if(gAd9984Dve.open == 1)
			{
			gAd9984Dve.open = 0;
			while(gAd9984Dve.close == 1) ;
			WV_CHECK_RET( WV_THR_Destroy(&(gAd9984Dve.thrHndl))   );	
			} 
   return WV_SOK; 
}





