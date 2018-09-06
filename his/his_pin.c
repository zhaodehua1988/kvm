#include"his_pin.h"
#include "hi_comm_sys.h"
#include "mpi_sys.h"

/*******************************************************************************************************

WV_S32  HIS_PINMUX_Init();

*******************************************************************************************************/
WV_S32  HIS_PINMUX_Init()
{
  WV_S32  i;
  WV_U32  addr;
  
  //gpio_5_ 0..7
  addr  = 0x120f009c;
  for(i =0;i<8;i++)
  {
  	WV_CHECK_FAIL ( HI_MPI_SYS_SetReg(addr,1) );
  	addr+=4; 
  }
  
  //gpio_6_ 0..7
  addr  = 0x120f00c0;
  for(i =0;i<8;i++)
  {
  	WV_CHECK_FAIL ( HI_MPI_SYS_SetReg(addr,1) );
  	addr+=4; 
  }  
  
    //gpio_7_ 0..7
  addr  = 0x120f00e8;
  for(i =0;i<8;i++)
  {
  	WV_CHECK_FAIL ( HI_MPI_SYS_SetReg(addr,1) );
  	addr+=4; 
  } 
  
  
    //gpio_8_ 0..7
  addr  = 0x120f010c;
  for(i =0;i<8;i++)
  {
  	WV_CHECK_FAIL ( HI_MPI_SYS_SetReg(addr,1) );
  	addr+=4; 
  } 
       //gpio_9_ 2..5
  addr  = 0x120f0160; //
  for(i =0;i<4;i++)
  {
  	WV_CHECK_FAIL ( HI_MPI_SYS_SetReg(addr,0) );
  	addr+=4; 
  }
  
      //gpio_10_ 0..7
  addr  = 0x120f0178; //
  for(i =0;i<8;i++)
  {
  	WV_CHECK_FAIL ( HI_MPI_SYS_SetReg(addr,0) );
  	addr+=4; 
  }
  
         //gpio_14_ 4..7
  addr  = 0x120f02e4; //
  for(i =0;i<4;i++)
  {
  	WV_CHECK_FAIL ( HI_MPI_SYS_SetReg(addr,0) );
  	addr+=4; 
  }
  // mclk
  addr  = 0x13140000 + 0x0140  + 8*2;
  WV_CHECK_FAIL ( HI_MPI_SYS_SetReg(addr,0x003254E7) );
 // iiC pad ctrl  
  addr  = 0x120f09dc;
  WV_CHECK_FAIL ( HI_MPI_SYS_SetReg(addr,0x08) );// 

 addr  = 0x120f09e0;
  WV_CHECK_FAIL ( HI_MPI_SYS_SetReg(addr,0x08) );//
  
  
  // gpio 7 3
addr  = 0x120f08f4;
  WV_CHECK_FAIL ( HI_MPI_SYS_SetReg(addr,0x08) );//
    
   
  //spi mux

    addr  = 0x120f0148;
  WV_CHECK_FAIL ( HI_MPI_SYS_SetReg(addr,1) );

    addr  = 0x120f014c;
  WV_CHECK_FAIL ( HI_MPI_SYS_SetReg(addr,1) );

    addr  = 0x120f0150;
  WV_CHECK_FAIL ( HI_MPI_SYS_SetReg(addr,1) );
  // UART 1  
  addr  = 0x120f0170;
  WV_CHECK_FAIL ( HI_MPI_SYS_SetReg(addr,2) );
  addr  = 0x120f0174;
  WV_CHECK_FAIL ( HI_MPI_SYS_SetReg(addr,2) );  
  
  // mclk
   addr  = 0x13140000 + 0x0140 + 8*0;
  WV_CHECK_FAIL ( HI_MPI_SYS_SetReg(addr,0x003254E7) );  
     addr  = 0x13140000 + 0x0140+8*1;
  WV_CHECK_FAIL ( HI_MPI_SYS_SetReg(addr,0x003254E7) );  
     addr  = 0x13140000 + 0x0140+8*2;
  WV_CHECK_FAIL ( HI_MPI_SYS_SetReg(addr,0x003254E7) ); 
  
  //vi clk;
  addr  = 0x120f0000;  //vi0clk
  WV_CHECK_FAIL ( HI_MPI_SYS_SetReg(addr,0x0) ); //clk
  
 addr  = 0x120f0024;  //vi1clk
  WV_CHECK_FAIL ( HI_MPI_SYS_SetReg(addr,0x1) );  //gpio 
  
  
   addr  = 0x120f0048;  //vi1clk
  WV_CHECK_FAIL ( HI_MPI_SYS_SetReg(addr,0x0) );  //gpio 
  
 
    addr  = 0x120f004c;  //vi2clk
  WV_CHECK_FAIL ( HI_MPI_SYS_SetReg(addr,0x0) );  //VI 2 CLK  
  
  
    addr  = 0x120f0070;  //vi3clk
  WV_CHECK_FAIL ( HI_MPI_SYS_SetReg(addr,0x1) );  //gpio
  
 addr  = 0x120f0094;  //vi3clk
  WV_CHECK_FAIL ( HI_MPI_SYS_SetReg(addr,0x0) );  //gpio 
  
  WV_U32  data;
  addr  = 0x120f0000;
   
  return WV_SOK;
}

/*******************************************************************************************************

WV_S32  HIS_PINMUX_DeInit();

*******************************************************************************************************/
WV_S32  HIS_PINMUX_DeInit()
{
return WV_SOK;
}
