
#include "xil_io.h"
#include "xparameters.h"

#define REG_OFF 0x00000
#define MEM_OFF 0x20000

u16 brmSingleRegWrite(u16 address,u16 data)
{

			Xil_Out16(XPAR_AXI_BRAM_CTRL_0_S_AXI_BASEADDR+REG_OFF+address*2,data);

			//spiMster16Write(0,data); // at the time the next commnd execute, this data already are in spimaster
			//spiMster32Write(0x7FFC,(address<<16)|0x00004000);

}

u16 brmSingleRegRead(u16 address)
{
 	u16 data;


 	    data = Xil_In16(XPAR_AXI_BRAM_CTRL_0_S_AXI_BASEADDR+REG_OFF+address*2);

        return data;
}


u16 brmSingleMemWrite(u16 address,u16 data)
        {

	Xil_Out16(XPAR_AXI_BRAM_CTRL_0_S_AXI_BASEADDR+MEM_OFF+address*2,data);



		}

u16 brmSingleMemRead(u16 address)
{
 	u16 data;

 	    data = Xil_In16(XPAR_AXI_BRAM_CTRL_0_S_AXI_BASEADDR+MEM_OFF+address*2);

        return data;

}


//void brmBurstRegRead(u16 startAddress,u16 length, u16* data)
//{
//
//
//
//
//
//}

void brmBurstMemWrite(u16 startAddress,u16 length, u16* data)
{

	//u32 bram_addr = XPAR_AXI_BRAM_CTRL_0_S_AXI_BASEADDR+startAddress*2;

	u16 BytesLength = length*2;
	void* bram_addr = (void*)(XPAR_AXI_BRAM_CTRL_0_S_AXI_BASEADDR+startAddress*2);
	memcpy(bram_addr,data,BytesLength );

}


void brmBurstMemRead(u16 startAddress,u16 length, u16* data)
{
	u16 BytesLength = length*2;
	void* bram_addr = (void*)(XPAR_AXI_BRAM_CTRL_0_S_AXI_BASEADDR+startAddress*2);
	memcpy(data,bram_addr,BytesLength );

}



  //memcpy(data,XPAR_AXI_BRAM_CTRL_0_S_AXI_BASEADDR+startAddress*2,BytesLength );
