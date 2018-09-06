#include "his_gpio.h"
#include "hi_comm_sys.h"
#include "mpi_sys.h"
 
#define HIS_GPIO_REG_BASE  0X12150000
#define HIS_GPIO_REG_DIR   0X400
#define HIS_GPIO_REG_IS    0X404
#define HIS_GPIO_REG_IBE   0X408
#define HIS_GPIO_REG_IEV   0X40C
#define HIS_GPIO_REG_IE    0X410
#define HIS_GPIO_REG_RIS   0X414
#define HIS_GPIO_REG_MIS   0X418
#define HIS_GPIO_REG_IC    0X41C




/*******************************************************************************************************

WV_S32  HIS_GPIO_Cfg(HIS_GPIO_DEV_E  pin);

*******************************************************************************************************/

WV_S32  HIS_GPIO_SetCfg(HIS_GPIO_DEV_E  pin ,WV_U32 mode)
{
	WV_U32 addr,data,bank,num; 

	bank  = pin.bank;
	num   = pin.num; 
    // 
   
	if(num > 7 || bank > 15)
		{
		WV_ERROR("no the PIN [%u.%u]\r\n",num, bank);
		return  WV_EFAIL;
		} 
  
	
	
	addr =HIS_GPIO_REG_BASE +(bank<<16) + HIS_GPIO_REG_DIR;

	WV_CHECK_FAIL( HI_MPI_SYS_GetReg(addr,&data) ); 
	if(mode == HIS_GPIO_MODE_INPUT)
		{
		data= data & (~(1<<num));
		} 
	else
		{
		data= data | (1<<num);
		}  
	WV_CHECK_FAIL ( HI_MPI_SYS_SetReg(addr,data) ); 
	
	//printf("\nHIS_GPIO_SetCfg bank[%d],pin[%d],mode=%d \n",bank,num,mode); 
	return WV_SOK;
}

/*******************************************************************************************************

WV_S32  HIS_GPIO_Set(HIS_GPIO_DEV_E  pin);

*******************************************************************************************************/

WV_S32  HIS_GPIO_Set(HIS_GPIO_DEV_E  pin,WV_U32 value)
{
	WV_U32 addr,data,bank,num;
	bank  = pin.bank;
	num   = pin.num;  
	// 
	if(num > 7 || bank > 15)
	{
		WV_ERROR("no the PIN [%u.%u]\r\n",num, bank);
		return  WV_EFAIL;
	} 


	addr =HIS_GPIO_REG_BASE +(bank<<16) + (1<<(num+2)); 
	if(value == 1)
	{
	  data  = 1 << num; 
	}
	else
	{
	  data = 0; 
	}  
	WV_CHECK_FAIL (HI_MPI_SYS_SetReg(addr,data) );  
	return WV_SOK;  
}
 

/*******************************************************************************************************

WV_S32  HIS_GPIO_Get(HIS_GPIO_DEV_E  pin,WV_U8 *pValue;

*******************************************************************************************************/
WV_S32  HIS_GPIO_Get(HIS_GPIO_DEV_E  pin,WV_U32 *pValue)
{
	WV_U32 addr,data,bank,num;
	bank  = pin.bank;
	num   = pin.num;  
	// 
	if(num > 7 || bank > 15)
		{
		WV_ERROR("no the PIN [%u.%u]\r\n",num, bank);
		return  WV_EFAIL;
		}  
	addr =HIS_GPIO_REG_BASE +(bank<<16) + (1<<(num+2));  
	WV_CHECK_FAIL (HI_MPI_SYS_GetReg(addr,&data) ); 
	*pValue = data;
	return WV_SOK;  
}


/****************************************************************************

WV_S32 HIS_GPIO_CmdConf(WV_S32 argc, WV_S8 **argv,WV_S8 *prfBuff)

****************************************************************************/
WV_S32 HIS_GPIO_CmdConf(WV_S32 argc, WV_S8 **argv,WV_S8 *prfBuff)
{
  HIS_GPIO_DEV_E  pin;
  WV_U32 data,mode;
  WV_S32 ret;
  
  if(argc < 3)
  {
    prfBuff += sprintf(prfBuff,"gpio conf <bank> <num> <mode>  \r\n  mode  0 :out   1 : input \r\n");
    return WV_SOK;  
  }
  WV_STR_S2v(argv[0],&data);
  pin.bank = data;
  
  WV_STR_S2v(argv[1],&data);
   pin.num = data;
  
  WV_STR_S2v(argv[2],&data);
  mode= data;  
  
  ret =  HIS_GPIO_SetCfg(pin,mode);
  if(ret == WV_EFAIL)
  { 
     prfBuff += sprintf(prfBuff,"cofig pin[%d.%d] = %d erro\r\n",pin.bank,pin.num,mode);
     return WV_SOK;
   } 
  prfBuff += sprintf(prfBuff,"cofig pin[%d.%d] = %d \r\n",pin.bank,pin.num,mode);
  return WV_SOK; 
}


/****************************************************************************

WV_S32 HIS_GPIO_CmdSet(WV_S32 argc, WV_S8 **argv,WV_S8 *prfBuff)

****************************************************************************/
WV_S32 HIS_GPIO_CmdSet(WV_S32 argc, WV_S8 **argv,WV_S8 *prfBuff)
{
  HIS_GPIO_DEV_E  pin;
  WV_U32 data,value;
  WV_U32  ret;
  
  if(argc < 3)
  {
    prfBuff += sprintf(prfBuff,"gpio conf <bank> <num> <mode>  \r\n  mode  0 :out   1 : input \r\n");
    return WV_SOK;  
  }
  WV_STR_S2v(argv[0],&data);
  pin.bank = data;  
  WV_STR_S2v(argv[1],&data);
   pin.num = data;
  
  WV_STR_S2v(argv[2],&data);
  value  = data;  
  
 
  ret =  HIS_GPIO_Set(pin,value);
  
  if(ret ==WV_EFAIL)
  {
     prfBuff += sprintf(prfBuff,"set pin[%d.%d] = %d erro\r\n", pin.bank,pin.num,value);
     return WV_SOK;
  }
  prfBuff += sprintf(prfBuff,"set pin[%d.%d] = %d \r\n",pin.bank,pin.num,value);
  return WV_SOK; 
}



/****************************************************************************

WV_S32 HIS_GPIO_CmdGet(WV_S32 argc, WV_S8 **argv,WV_S8 *prfBuff)

****************************************************************************/
WV_S32 HIS_GPIO_CmdGet(WV_S32 argc, WV_S8 **argv,WV_S8 *prfBuff)
{
  HIS_GPIO_DEV_E  pin;
  WV_U32 data,value;
  WV_S32 ret;
  
  if(argc < 2)
  {
    prfBuff += sprintf(prfBuff,"gpio conf <bank> <num> <mode>  \r\n  mode  0 :out   1 : input \r\n");
    return WV_SOK;  
  }
  WV_STR_S2v(argv[0],&data);
  pin.bank = data;  
  WV_STR_S2v(argv[1],&data);
  pin.num = data;
   
     ret =  HIS_GPIO_Get(pin,&value);  
    
  if(ret ==WV_EFAIL)
  {
     prfBuff += sprintf(prfBuff,"Get pin[%d.%d] erro\r\n", pin.bank,pin.num);
     return WV_SOK;
  }
  prfBuff += sprintf(prfBuff,"Get ping[%d.%d] = %d \r\n",pin.bank,pin.num,value);
  return WV_SOK; 
}




/****************************************************************************

WV_S32 HIS_GPIO_Init()

****************************************************************************/
WV_S32 HIS_GPIO_Init()
{
  
	WV_CMD_Register("gpio",NULL,"gpio cmd",NULL);
	WV_CMD_Register("gpio","conf","gpio config",HIS_GPIO_CmdConf); 
	WV_CMD_Register("gpio","set","gpio set value",HIS_GPIO_CmdSet);
	WV_CMD_Register("gpio","get","gpio get value",HIS_GPIO_CmdGet);


	return WV_SOK;
}

/****************************************************************************

WV_S32 HIS_GPIO_DeInit()

****************************************************************************/
WV_S32 HIS_GPIO_DeInit()
{
   return WV_SOK;
}
