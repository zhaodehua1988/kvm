#include "wv_common.h" 
#include "sys.h" 
#include "his.h"
#include "tsk.h" 
#include "ext.h"
// for usb key svr uart
#include "kmv_svr.h"
#include "kmv_usb.h"
#include "kmv_serial.h"
#include "tsk_sw.h"
#include "tsk_uart.h"
#include "tsk_usb.h"
#include "ext_fpga.h"
/******************************************************************************
* function    : main()
* Description :  
******************************************************************************/


WV_S32  main()
{
	WV_S32 ret; 
	printf("********hello world **3****\n"); 
	 SYS_Init(); 
     HIS_SYS_Init();
     HIS_PINMUX_Init();
     HIS_GPIO_Init();
     HIS_IIC_Init();
	 //
     
	 EXT_AD9984_Init();
     EXT_SI7127_Init();
	 EXT_FPGA_Init();
	 EXT_DVI_Init();
	
     EXT_VGA_Init();
     EXT_3268_Init();
     
	// TSK_AI_Init();
	//  TSK_AO_Init(); 
   
	
    usleep(1000000);

    TSK_VDEC_Init();

	TSK_SRX_Init();

     TSK_VENC_Init(); 

	 //
	SVR_Open();
	KMV_SERIAL_CONF_Open(); 
	KMV_V_SERIAL_Open();
	TSK_SW_Init();	
	KMV_USB_DEV_Open();
	
	TSK_UART_Open();
	TSK_USB_Open();
	KMV_UDP_Server_Open();
	EXT_FPGA_Write(0x2f,0x6);
	while(1)
	{
		ret = WV_CMD_GetExit(); 
		if(ret == 1)
		{
			 break;
		}
		usleep(50000);
		 
	}

	//
	KMV_UDP_Server_Close();
	TSK_USB_Close();
	TSK_UART_Close();
	KMV_USB_DEV_Close();
	KMV_V_SERIAL_Close();
	KMV_SERIAL_CONF_Close();
	SVR_Close();
	//	
     
    EXT_DVI_DeInit();
    EXT_VGA_DeInit();	
    EXT_3268_DeInit();
    EXT_AD9984_DeInit();
    EXT_SI7127_DeInit();
    EXT_FPGA_DeInit();
	//
	
	  TSK_VENC_DeInit();	
	  TSK_VDEC_DeInit(); 
	 TSK_SRX_DeInit();	
	 
		
	// TSK_AO_DeInit();	 
	// TSK_AI_DeInit();
	 HIS_GPIO_DeInit();
    HIS_PINMUX_DeInit(); 	
    HIS_SYS_DeInit(); 		 	
	SYS_DeInit();
	printf("********    bye     ********\n");
	return 0;
}


