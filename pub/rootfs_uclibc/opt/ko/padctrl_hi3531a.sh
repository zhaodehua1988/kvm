#!/bin/sh

echo "run $0 begin!!!";

#SPI
himm 0x120F0948 0x78;   #SPI_SCLK
himm 0x120F094C 0x28;   #SPI_SDO

#I2S
himm 0x120F09BC 0x18;   #I2S2_BCLK_RX
himm 0x120F09C0 0x38;   #I2S2_WS_RX
himm 0x120F09CC 0x18;   #I2S2_MCLK

himm 0x120F0934 0x38;   #I2S0_WS_RX
himm 0x120F0940 0x38;   #I2S1_WS_RX
himm 0x120F09AC 0x38;   #I2S3_WS_TX

#VI
himm 0x120F0848 0x58;   #VI_ADC_REFCLK0
himm 0x120F0894 0x58;   #VI_ADC_REFCLK1
himm 0x120F08E0 0x58;   #VI_ADC_REFCLK2
himm 0x120F092C 0x58;   #VI_ADC_REFCLK3

#VDAC
himm 0x120F09B4 0x50;   #VGA_HS
himm 0x120F09B8 0x50;   #VGA_VS

#VO BT1120
himm 0x120F0964 0x40;   #VOU_1120CLK
himm 0x120F0968 0x18;   #VOU_1120DAT0
himm 0x120F096C 0x18;   #VOU_1120DAT1
himm 0x120F0970 0x18;   #VOU_1120DAT2
himm 0x120F0974 0x18;   #VOU_1120DAT3
himm 0x120F0978 0x18;   #VOU_1120DAT4
himm 0x120F097C 0x18;   #VOU_1120DAT5
himm 0x120F0980 0x18;   #VOU_1120DAT6
himm 0x120F0984 0x18;   #VOU_1120DAT7
himm 0x120F0988 0x18;   #VOU_1120DAT8
himm 0x120F098C 0x18;   #VOU_1120DAT9
himm 0x120F0990 0x18;   #VOU_1120DAT10
himm 0x120F0994 0x18;   #VOU_1120DAT11
himm 0x120F0998 0x18;   #VOU_1120DAT12
himm 0x120F099C 0x18;   #VOU_1120DAT13
himm 0x120F09A0 0x18;   #VOU_1120DAT14
himm 0x120F09A4 0x18;   #VOU_1120DAT15

#VO 级联输出
himm 0x120F09E4 0x40;   #VOU_SLV_DAT15
himm 0x120F09E8 0x40;   #VOU_SLV_DAT1
himm 0x120F09EC 0x40;   #VOU_SLV_DAT0
himm 0x120F09F0 0x40;   #VOU_SLV_DAT9
himm 0x120F09F4 0x40;   #VOU_SLV_DAT14
himm 0x120F09F8 0x40;   #VOU_SLV_DAT8
himm 0x120F09FC 0x0;    #VOU_SLV_CLK
himm 0x120F0A00 0x40;   #VOU_SLV_DAT2
himm 0x120F0A04 0x40;   #VOU_SLV_DAT3
himm 0x120F0A08 0x40;   #VOU_SLV_DAT5
himm 0x120F0A0C 0x40;   #VOU_SLV_DAT4
himm 0x120F0A10 0x40;   #VOU_SLV_DAT11
himm 0x120F0A14 0x40;   #VOU_SLV_DAT10
himm 0x120F0A18 0x40;   #VOU_SLV_DAT7
himm 0x120F0A1C 0x40;   #VOU_SLV_DAT6
himm 0x120F0A20 0x40;   #VOU_SLV_DAT12
himm 0x120F0A24 0x40;   #VOU_SLV_DAT13





echo "run $0 end!!!";