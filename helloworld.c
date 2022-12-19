/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>

#include "xil_printf.h"
#include "1553.h"
#include "brmMASTER.h"
#include "xil_io.h"
#include "xparameters.h"


int main()
{


    u16 data;


    xil_printf( "Hello from Standalone example main\n\r" );
    //some memory testing

    	int addr = 0x1A;

    	Xil_Out16(XPAR_AXI_BRAM_CTRL_0_S_AXI_BASEADDR+0x20000+addr*2,0x1111);

    	data = Xil_In16(XPAR_AXI_BRAM_CTRL_0_S_AXI_BASEADDR+0x20000+addr*2);

    	xil_printf("the data memory is:%0x\n\r",data);

   //some register testing
    	data = Xil_In16(XPAR_AXI_BRAM_CTRL_0_S_AXI_BASEADDR+0x00000+0x1A*2);
    	if (data == 0x0000)
    		printf("Some error in read/write registers\n\r");

//    	data = Xil_In16(XPAR_AXI_BRAM_CTRL_0_S_AXI_BASEADDR+REG_OFF+0x12*2);// C0010002 ---> 0x10002 ---> memory 0x0001
//
//    	xil_printf("the data register is:%0x\n\r",data);

  //  	perform brm version and registers list
//



    	  xil_printf("BRM1553D version register:%0x\n\r",brmSingleRegRead(0x1A));

    	  if (brmSingleRegRead(0xE)>>15)
    		  xil_printf("KEY is NOT attached\n\r");
    	  else
    		  xil_printf("KEY is attached\n\n\r");

    		for (int i = 0; i < 32; i++)   // write data first to spimstr
    			{
    			data = brmSingleRegRead(i);
    			xil_printf("Register Address %0x Data %0x\n\r",i,data);
    			}

    	  print("\n--Now trying to perform BC transmission--\n\r");

          brmBCexample();


    return 0;
}
