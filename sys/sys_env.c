#include"sys_env.h"

#define   SYS_ENV_FILE_NAME    "conf.ini"
#define   SYS_ENV_FILE_PATH    "./env/"


static WV_CONF_DEV_E  *pSysEnv;

/**********************************************************************

WV_S32  SYS_ENV_Register(WV_S8 type ,WV_S8 * pName,WV_S8 *pDefault,WV_S8  *pNote);

**********************************************************************/
WV_S32  SYS_ENV_Register(WV_S8 type ,WV_S8 * pName,WV_S8 *pDefault,WV_S8  *pNote)
{
 return WV_CONF_Register(pSysEnv,type ,pName,pDefault,pNote);  
}



/**********************************************************************

WV_S32  SYS_ENV_Get(WV_S8 *pName ,WV_S8 *pValue);

**********************************************************************/
WV_S32  SYS_ENV_Get(WV_S8 *pName ,WV_S8 *pValue) 
{
 return  WV_CONF_Get(pSysEnv,pName,pValue);  
 
}
/**********************************************************************

WV_S32  SYS_ENV_GetU32(WV_S8 *pName ,WV_U32 * pValue);

**********************************************************************/
WV_S32  SYS_ENV_GetU32(WV_S8 *pName ,WV_U32 * pValue)
{

 WV_S8  Value[WV_CONF_VALUE_MAX_LEN];
 WV_S32 ret = 0;
ret = WV_CONF_Get(pSysEnv,pName,Value);
if(ret == WV_EFAIL)
{
  WV_ERROR("no the env  %s\r\n",pName);
  return WV_EFAIL;
}
   
 WV_STR_S2v(Value,pValue); 
 
 return WV_SOK;
 
}


/**********************************************************************

WV_S32  SYS_ENV_Set(WV_S8 *pName ,WV_S8 *pValue);

**********************************************************************/

WV_S32  SYS_ENV_Set(WV_S8 *pName ,WV_S8 *pValue)

{
  return  WV_CONF_Set(pSysEnv,pName,pValue); 
} 



/**********************************************************************

WV_S32  SYS_ENV_SetU32(WV_S8 *pName ,WV_U32 dat);

**********************************************************************/

WV_S32  SYS_ENV_SetU32(WV_S8 *pName ,WV_U32 data)

{
   WV_S8  Value[WV_CONF_VALUE_MAX_LEN];
   sprintf(Value,"%d",data);
    
  return  WV_CONF_Set(pSysEnv,pName,Value); 
} 
/**********************************************************************

WV_S32  SYS_ENV_SetDefaultAll();

**********************************************************************/

WV_S32  SYS_ENV_SetDefaultAll()

{
 
  return  WV_CONF_SetDefalutAll(pSysEnv);
} 



/**********************************************************************

WV_S32  SYS_ENV_SetDefault();

**********************************************************************/

WV_S32  SYS_ENV_SetDefault(WV_S8 *pName)

{
 
  return  WV_CONF_SetDefalut(pSysEnv,pName);
} 

/**********************************************************************

WV_S32  SYS_ENV_Save();

**********************************************************************/
WV_S32  SYS_ENV_Save()
{
  return WV_CONF_Save(pSysEnv);  
}


/****************************************************************************

WV_S32 SYS_ENV_CmdGet(WV_S32 argc, WV_S8 **argv,WV_S8 *prfBuff)

****************************************************************************/
WV_S32 SYS_ENV_CmdGet(WV_S32 argc, WV_S8 **argv,WV_S8 *prfBuff)
{
   WV_S32 i,j,len;
   WV_S8 lineBuf[WV_CONF_ITEM_MAX_LEN], * pTemp;
   if(argc > 0)
   {
      WV_S8  value[WV_CONF_VALUE_MAX_LEN];
      i =  SYS_ENV_Get(argv[0],value); 
      if(i == WV_EFAIL)
		{
		  prfBuff += sprintf(prfBuff,"no env %s \r\n",argv[0]);
		  return WV_SOK;     
		}
	  if(pSysEnv->pItem[i].type == 1)
	  {
	     memset(lineBuf, 0x20, WV_CONF_ITEM_MAX_LEN);  
	     len = strlen(pSysEnv->pItem[i].name);
	     memcpy(lineBuf,pSysEnv->pItem[i].name,len);
	     lineBuf[WV_CONF_NAME_MAX_LEN -1] = '=';
	     
	     pTemp = lineBuf + WV_CONF_NAME_MAX_LEN;
	     len = strlen(pSysEnv->pItem[i].value);
	     memcpy(pTemp,pSysEnv->pItem[i].value,len);
	     
	     lineBuf[WV_CONF_NAME_MAX_LEN + WV_CONF_VALUE_MAX_LEN -1] = ':'; 
	     pTemp =lineBuf + WV_CONF_NAME_MAX_LEN + WV_CONF_VALUE_MAX_LEN;
	     sprintf(pTemp,"%s\r\n",pSysEnv->pItem[i].note); 
	     lineBuf[WV_CONF_ITEM_MAX_LEN - 1] = 0;
	     prfBuff += sprintf(prfBuff,"%s",lineBuf);  
	     return WV_SOK;	       
	  } 
	  else
	  {
	  
		  for(j = i+1;j< pSysEnv->numItem;j++)
		  {
		     if(pSysEnv->pItem[j].type == 1)
		     {
				memset(lineBuf, 0x20, WV_CONF_ITEM_MAX_LEN);  
				len = strlen(pSysEnv->pItem[j].name);
				memcpy(lineBuf,pSysEnv->pItem[j].name,len);
				lineBuf[WV_CONF_NAME_MAX_LEN -1] = ':';

				pTemp = lineBuf + WV_CONF_NAME_MAX_LEN;
				len = strlen(pSysEnv->pItem[j].value);
				memcpy(pTemp,pSysEnv->pItem[j].value,len);

				lineBuf[WV_CONF_NAME_MAX_LEN + WV_CONF_VALUE_MAX_LEN -1] = '/'; 
				pTemp = lineBuf + WV_CONF_NAME_MAX_LEN + WV_CONF_VALUE_MAX_LEN;
				sprintf(pTemp,"%s\r\n",pSysEnv->pItem[j].note); 
				lineBuf[WV_CONF_ITEM_MAX_LEN - 1] = 0;
				prfBuff += sprintf(prfBuff,"%s",lineBuf);   
		     }
		     else
		     {
		       return WV_SOK;
		     } 
		      
		  }
	  }	  
       return WV_SOK;    
    } 
   prfBuff += sprintf(prfBuff,"env:\r\n"); 
   for( i = 0; i< pSysEnv-> numItem ;i++)
   {
     if(pSysEnv->pItem[i].type == 0 )
     {
      prfBuff += sprintf(prfBuff,"%s\r\n",pSysEnv->pItem[i].name);  
     }
     else
     {
         memset(lineBuf, 0x20, WV_CONF_ITEM_MAX_LEN);  
	     len = strlen(pSysEnv->pItem[i].name);
	     memcpy(lineBuf,pSysEnv->pItem[i].name,len);
	     lineBuf[WV_CONF_NAME_MAX_LEN -1] = '=';
	     
	     pTemp = lineBuf + WV_CONF_NAME_MAX_LEN;
	     len = strlen(pSysEnv->pItem[i].value);
	     memcpy(pTemp,pSysEnv->pItem[i].value,len);
	     
	     lineBuf[WV_CONF_NAME_MAX_LEN + WV_CONF_VALUE_MAX_LEN -1] = ':'; 
	     pTemp = lineBuf +WV_CONF_NAME_MAX_LEN + WV_CONF_VALUE_MAX_LEN;
	     sprintf(pTemp,"%s\r\n",pSysEnv->pItem[i].note); 
	     lineBuf[WV_CONF_ITEM_MAX_LEN - 1] = 0;
	     prfBuff += sprintf(prfBuff,"%s",lineBuf);     
     }
   }
    
 return WV_SOK;
}






/****************************************************************************

WV_S32 SYS_ENV_CmdSet(WV_S32 argc, WV_S8 **argv,WV_S8 *prfBuff)

****************************************************************************/
WV_S32 SYS_ENV_CmdSet(WV_S32 argc, WV_S8 **argv,WV_S8 *prfBuff)
{
   WV_S32 i;
   WV_S8  value[WV_CONF_VALUE_MAX_LEN];
   
   
   if(argc < 2) 
   {
      prfBuff += sprintf(prfBuff,"set env <name> <value>\r\n");   
      return WV_SOK; 
   }  
   
   i=SYS_ENV_Get(argv[0],value);
   if(i == WV_EFAIL)
    {
     prfBuff += sprintf(prfBuff,"no env %s \r\n",argv[0]);
      return WV_SOK;     
    }
    if(pSysEnv->pItem[i].type == 1)
     {
        i=SYS_ENV_Set(argv[0],argv[1]); 
        if(i == WV_EFAIL)
		{
		  prfBuff += sprintf(prfBuff,"set %s erro\r\n",argv[0]);
		 // return WV_SOK;     
		}
		else
		{
        	prfBuff += sprintf(prfBuff,"set %s  =  %s\r\n",pSysEnv->pItem[i].name,pSysEnv->pItem[i].value);  
        	
        }	
     } 
     else
     {
      prfBuff += sprintf(prfBuff,"%s is type module\r\n",argv[0]);
     }
      
 return WV_SOK;
}




/****************************************************************************

WV_S32 SYS_ENV_CmdSet(WV_S32 argc, WV_S8 **argv,WV_S8 *prfBuff)

****************************************************************************/
WV_S32 SYS_ENV_CmdSetDefault(WV_S32 argc, WV_S8 **argv,WV_S8 *prfBuff)
{
    WV_S32 ret;
   WV_S8  value[WV_CONF_VALUE_MAX_LEN];
  
  
   if(argc > 0) 
   {
     
     ret = SYS_ENV_SetDefault(argv[0]);
     if(ret == WV_EFAIL)
     {
        prfBuff += sprintf(prfBuff,"argv[0]  default Erro!!!\r\n");   
     }
     else
     {
         WV_CHECK_FAIL(SYS_ENV_Get(argv[0] ,value) );
        prfBuff += sprintf(prfBuff,"%s =  %s \r\n",argv[0],value);   
     }
     
   } 
   else
   {
     ret =   SYS_ENV_SetDefaultAll();
     if(ret == WV_EFAIL)
     {
        prfBuff += sprintf(prfBuff,"default all Erro!!!\r\n");   
     }
     else
     {  
        prfBuff += sprintf(prfBuff,"default all ok \r\n");   
     } 
   }  
   return WV_SOK;
} 

/****************************************************************************

WV_S32 SYS_ENV_CmdSave(WV_S32 argc, WV_S8 **argv,WV_S8 *prfBuff)

****************************************************************************/
WV_S32 SYS_ENV_CmdSave(WV_S32 argc, WV_S8 **argv,WV_S8 *prfBuff)
{
  WV_S32 ret;
  prfBuff += sprintf(prfBuff,"save env .....\r\n");
  
  ret =  SYS_ENV_Save();
  
  if(ret == WV_SOK)
  {
  		prfBuff += sprintf(prfBuff,"save env ok\r\n");
  } 
  else
  {
   		prfBuff += sprintf(prfBuff,"save env erro!!!!\r\n");       
  }
  
  return WV_SOK; 
}



/**********************************************************************

WV_S32  SYS_ENV_Open();

**********************************************************************/
WV_S32  SYS_ENV_Open()
{ 
   pSysEnv =  (WV_CONF_DEV_E *)malloc( sizeof(WV_CONF_DEV_E));
   WV_CHECK_RET( WV_CONF_Creat(pSysEnv,SYS_ENV_FILE_NAME,SYS_ENV_FILE_PATH));
   WV_ASSERT_RET( WV_CONF_Load(pSysEnv));
   
   // for sys id
  SYS_ENV_Register(0 ,"BOARD",NULL,NULL);
  SYS_ENV_Register(1 ,"BD_Vendor",SOFTVER_VENDOR,"board Vendor");
  SYS_ENV_Register(1 ,"BD_TYPE",SOFTVER_TYPE,"board type");
  SYS_ENV_Register(1 ,"BD_Date",SOFTVER_DATE,"board date");
  SYS_ENV_Register(1 ,"BD_Number","0","board order number");  
	
   WV_CMD_Register("env",NULL,"sys env model cmd",NULL);
   WV_CMD_Register("env","get","get the sys env",SYS_ENV_CmdGet); 
   WV_CMD_Register("env","set","set the sys env",SYS_ENV_CmdSet);
   WV_CMD_Register("env","default","set the sys env default",SYS_ENV_CmdSetDefault);
   WV_CMD_Register("env","save","save all env",SYS_ENV_CmdSave);
   
 return  WV_SOK;
}

/**********************************************************************

WV_S32  SYS_ENV_Close();

**********************************************************************/
WV_S32  SYS_ENV_Close()
{  
 if(pSysEnv-> numItemBuf  != pSysEnv-> numItem)
  {
    WV_ASSERT_RET(WV_CONF_Save(pSysEnv));
  } 
   WV_CONF_Destry(pSysEnv);
  free(pSysEnv);
  pSysEnv = NULL; 
 return  WV_SOK;
}


