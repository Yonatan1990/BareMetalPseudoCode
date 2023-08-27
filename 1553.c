
#include "brmMaster.h"
#include "xil_io.h"
#include "sleep.h"

void brmBCexample(void)

{

	 u16 data16;

	 print("Simple BC transmission\n\n\r");
	 // Notice to enable Sim All in the composer
	 brmSingleRegWrite (0x0003,0x0001); //perform SW reset
	 brmSingleRegWrite (0x0001,0x0040); //enable BC mode in config 1 register
	 // if we put 0x140 auto repeat enabled and it will transmitt forever. page 67
	 //in that case need to configure 102,103

	data16 = brmSingleRegRead (0x0001);
	xil_printf("read register: %0x \n\r",data16);

	//READ 40001 0040

	brmSingleRegWrite (0x0002,0x9C73); //Setup config 2 register
	brmSingleRegWrite (0x0007,0x8000); // write config register 3
	brmSingleRegWrite (0x0008,0x1060); //Setup config 4 register

	brmSingleRegWrite (0x000D,0x0004); //setup the Frame length to 400 us
	//setup memory stack
	// Block Status word (core)
	// Time tag (core)
	// Gap to next message
	// Setup Stack Memory pointers
	brmSingleMemWrite(0x0000,0x0000);
	brmSingleMemWrite(0x0001,0x0000);
	brmSingleMemWrite(0x0002,0x0000);
	brmSingleMemWrite(0x0003,0x01A0); //check
	brmSingleMemWrite(0x0004,0x0000);
	brmSingleMemWrite(0x0005,0x0000);
	brmSingleMemWrite(0x0006,0x0000);
	brmSingleMemWrite(0x0007,0x0160); //check
	brmSingleMemWrite(0x0008,0x0000);
	brmSingleMemWrite(0x0009,0x0000);
	brmSingleMemWrite(0x000A,0x0000);
	brmSingleMemWrite(0x000B,0x0120); //check

	brmSingleMemWrite(0x000C,0x0000);
	brmSingleMemWrite(0x000D,0x0000);
	brmSingleMemWrite(0x000E,0x0000);
	brmSingleMemWrite(0x000F,0x01C0); //check




   //read verify
	data16 = brmSingleMemRead(0x0003);
	xil_printf("check read mem_addr 0x0003 %0x \n\r",data16); // 01A0
	data16 = brmSingleMemRead(0x0007);
	xil_printf("check read mem_addr 0x0007 %0x \n\r",data16);// 0160
	data16 = brmSingleMemRead(0x000B);
	xil_printf("check read mem_addr 0x000B %0x \n\r",data16);// 0120

	//setup stack values pointers
	brmSingleMemWrite (0x0100,0x0000); // go to 0 address
	brmSingleMemWrite (0x0101,0xFFFB); //send 4 frames
	//brmSingleMemWrite (0x0101,0xFFFC); //send 3 frames

	//Define command list

	brmSingleMemWrite(0x01A0, 0x0000); // control word

	brmSingleMemWrite(0x01A1, 0xC023); // the command. rt 24. 3 words. sa 1. receive.
	brmSingleMemWrite(0x01A2, 0x0000); // fill up the memory
	brmSingleMemWrite(0x01A3, 0x1111);
	brmSingleMemWrite(0x01A4, 0x2222);
	brmSingleMemWrite(0x0160, 0x0080); // control word . Bus A.
	brmSingleMemWrite(0x0161, 0xCC24); // Command to another RT 25, SA1 ,transmit, 4 words
	brmSingleMemWrite(0x0120, 0x0080); // control word . Bus A
	brmSingleMemWrite(0x0121, 0xC423); // command to read data. r24. transmit. sa 1. 3 words

	brmSingleMemWrite(0x01C0, 0x0080); // control word . Bus A
	brmSingleMemWrite(0x01C1, 0xD438); // command to read data. 0xD438 is 24 words
	//11010-1-00001-11000 ---> rt26,transmit, sa1,24 words
	//Notice to update the right number of words also in the Composer
	//in order for the data to be shown on the Monitor


	brmSingleRegWrite(0x0003, 0x0002); //go for one frame

	// wait for 400 us
    sleep(1);
	data16 = brmSingleRegRead (0x0006);
		xil_printf("read register 0x0006:%0x \n\r",data16);
	// verify frame complete - 8009
	// now verify data is correct

		data16 = brmSingleMemRead (0x0100);
		xil_printf("read memory 0x0100:%0x\n\r",data16);

	//READ 00100 000C

	//Stack pointer after Frame finished
		data16 = brmSingleMemRead (0x0101);
		xil_printf("read memory 0x0101:%0x\n\r",data16);
	//READ 00101 FFFF
	//-----Results in memory-----
	xil_printf("--Results in memory-- \n\r");
	data16 = brmSingleMemRead(0x0000 );
	  xil_printf("check read mem_addr 0x0000: %0x \n\r",data16); // 0xA000
	data16 = brmSingleMemRead(0x0002 );
	  xil_printf("check read mem_addr 0x0002: %0x \n\r",data16); // 0x0000
	data16 = brmSingleMemRead(0x0003 );
	  xil_printf("check read mem_addr 0x0003: %0x \n\r",data16); // 0x01A0
	data16 = brmSingleMemRead(0x01A0 );
	  xil_printf("check read mem_addr 0x01A0: %0x \n\r",data16); // 0x0000
	data16 = brmSingleMemRead(0x01A1 );
	  xil_printf("check read mem_addr 0x01A1: %0x \n\r",data16); // 0xC023
	data16 = brmSingleMemRead(0x01A2 );
	  xil_printf("check read mem_addr 0x01A2: %0x \n\r",data16); // 0x0000
	data16 = brmSingleMemRead(0x01A3 );
	  xil_printf("check read mem_addr 0x01A3: %0x \n\r",data16); // 0x1111
	data16 = brmSingleMemRead(0x01A4 );
	  xil_printf("check read mem_addr 0x01A4: %0x \n\r",data16); // 0x2222
	data16 = brmSingleMemRead(0x01A5 );
	  xil_printf("check read mem_addr 0x01A5: %0x \n\r",data16); // 0x2222
	data16 = brmSingleMemRead(0x01A6 );
	  xil_printf("check read mem_addr 0x01A6: %0x \n\r",data16); // 0xC000

		//second command
	xil_printf("--second command-- \n\r");
	data16 = brmSingleMemRead(0x0004);
	  xil_printf("check read mem_addr 0x0004: %0x \n\r",data16); // 0x8010
	data16 = brmSingleMemRead(0x0006);
	  xil_printf("check read mem_addr 0x0006: %0x \n\r",data16); // 0x0000
	data16 = brmSingleMemRead(0x0007);
	  xil_printf("check read mem_addr 0x0007: %0x \n\r",data16); // 0x0160

	data16 = brmSingleMemRead(0x0160);
	xil_printf("check read mem_addr 0x0160: %0x \n\r",data16); // 0x0080
	data16 = brmSingleMemRead(0x0161);
	xil_printf("check read mem_addr 0x0161: %0x \n\r",data16); // 0xCC24
	data16 = brmSingleMemRead(0x0162);
	xil_printf("check read mem_addr 0x0162: %0x \n\r",data16); // 0xCC24
	data16 = brmSingleMemRead(0x0163);
	xil_printf("check read mem_addr 0x0163: %0x \n\r",data16); // 0xC800
	data16 = brmSingleMemRead(0x0164);
	xil_printf("check read mem_addr 0x0164: %0x \n\r",data16); // 0x0001
	data16 = brmSingleMemRead(0x0165);
	xil_printf("check read mem_addr 0x0165: %0x \n\r",data16); // 0x0002
	data16 = brmSingleMemRead(0x0166);
	xil_printf("check read mem_addr 0x0166: %0x \n\r",data16); // 0x0003
	data16 = brmSingleMemRead(0x0167);
	xil_printf("check read mem_addr 0x0167: %0x \n\r",data16); // 0x0004
	data16 = brmSingleMemRead(0x0168);
	xil_printf("check read mem_addr 0x0168: %0x \n\r",data16); //

	data16 = brmSingleMemRead(0x0120);
	  xil_printf("check read mem_addr 0x0120: %0x \n\r",data16); // 0x0080
	data16 = brmSingleMemRead(0x0121);
	  xil_printf("check read mem_addr 0x0121: %0x \n\r",data16); // 0xC423
	data16 = brmSingleMemRead(0x0122);
	  xil_printf("check read mem_addr 0x0122: %0x \n\r",data16); // 0xC423
	data16 = brmSingleMemRead(0x0123);
	  xil_printf("check read mem_addr 0x0123: %0x \n\r",data16); // 0xC000
	data16 = brmSingleMemRead(0x0124);
	  xil_printf("check read mem_addr 0x0124: %0x \n\r",data16); // 0xA001
	data16 = brmSingleMemRead(0x0125);
	  xil_printf("check read mem_addr 0x0125: %0x \n\r",data16); // 0xB002
	data16 = brmSingleMemRead(0x0126);
	  xil_printf("check read mem_addr 0x0126: %0x \n\n\r",data16); // 0xC003

	data16 = brmSingleMemRead(0x000C);
	  xil_printf("check read mem_addr 0x000C: %0x \n\r",data16); // 0x8010
	data16 = brmSingleMemRead(0x000E);
	  xil_printf("check read mem_addr 0x000E: %0x \n\r",data16); // 0x0000
	data16 = brmSingleMemRead(0x000F);
	  xil_printf("check read mem_addr 0x000F: %0x \n\r",data16); // 0x0160


//		data16 = brmSingleMemRead(0x01C0);
//		  xil_printf("check read mem_addr 0x01C0: %0x \n\r",data16); // 0x0080
//		data16 = brmSingleMemRead(0x01C1);
//		  xil_printf("check read mem_addr 0x01C1: %0x \n\r",data16); // 0x
//		data16 = brmSingleMemRead(0x01C2);
//		  xil_printf("check read mem_addr 0x01C2: %0x \n\r",data16); // 0x
//		data16 = brmSingleMemRead(0x01C3);
//		  xil_printf("check read mem_addr 0x01C3: %0x \n\r",data16); // 0x
//		data16 = brmSingleMemRead(0x01C4);
//		  xil_printf("check read mem_addr 0x01C4: %0x \n\r",data16); // 0x
//		data16 = brmSingleMemRead(0x01C5);
//		  xil_printf("check read mem_addr 0x01C5: %0x \n\r",data16); // 0x
//		data16 = brmSingleMemRead(0x01C6);
//		  xil_printf("check read mem_addr 0x01C6: %0x \n\r",data16); // 0x
//		data16 = brmSingleMemRead(0x01C7);
//		  xil_printf("check read mem_addr 0x01C7: %0x \n\r",data16); // 0x
//		data16 = brmSingleMemRead(0x01C8);
//		  xil_printf("check read mem_addr 0x01C8: %0x \n\r",data16); // 0x

		u16 data[50];
        int WordNum = 24;
        u16 base = 0x01C0;

		//brmBurstMemRead(0x01C0,WordNum,data);
		 for (int i=0;i<(WordNum+4);i++){
			  	  data[i] = brmSingleMemRead(base+i);
			  	  xil_printf("check read mem_addr %0x: %0x \n\r",(base+i),data[i]);
		 }


//	  u16 *data;
//	  int i;
//
//	  brmBurstMemRead(0x0160,8,data);
//	  for (i=0;i<8;i++){
//		  	  xil_printf("%0x \n\r",data[i]);
//	  }
//
//	  brmBurstMemRead(0x0120,7,data);
//	  for (i=0;i<8;i++){
//		  	  xil_printf("\n",data[i]);
//	  }


}


void brmRTexample(void)

{
     //#test reset procedure
     brmSingleRegWrite (0x0003,0x0001);
     brmSingleRegWrite (0x0000,0x0000);
     brmSingleRegWrite (0x0001,0x8F80);
     brmSingleRegWrite (0x0002,0x9073);
     brmSingleRegWrite (0x0007,0x87D9);
     brmSingleRegWrite (0x0008,0xC000);
     brmSingleRegWrite (0x0009,0x0B31);// # set to RT address 0x18!!!!
     brmSingleRegWrite (0x000A,0x0000);
     brmSingleRegWrite (0x000D,0x0000);
     brmSingleRegWrite (0x0018,0x3020);
     brmSingleRegWrite (0x001D,0x0000);

    //#Check Register's values
     brmSingleRegRead (0x0000);     //0x0000

    //#Clear the busy bit which is turned on by default for 1760.
     brmSingleRegWrite (0x0001, 0x8F80);
     brmSingleRegRead (0x0001);    //  0x8F80

     //#Enable End Of Message interrupts and mode code interrupts
     brmSingleRegWrite (0x0000, 0x0012);
     brmSingleMemWrite (0x0100, 0x0000);

     //# LUT set up address 140 to 19F
     //#RECEIVE BLOCKS
     brmSingleMemWrite (0x0140,0x0400);
     brmSingleMemWrite (0x0141,0x0420);
     brmSingleMemWrite (0x0142,0x0440);
     brmSingleMemWrite (0x0143,0x0460);
     brmSingleMemWrite (0x0144,0x0480);
     brmSingleMemWrite (0x0145,0x04A0);
     brmSingleMemWrite (0x0146,0x04C0);
     brmSingleMemWrite (0x0147,0x04E0);
     brmSingleMemWrite (0x0148,0x0500);
     brmSingleMemWrite (0x0149,0x0520);
     brmSingleMemWrite (0x014A,0x0540);
     brmSingleMemWrite (0x014B,0x0560);
     brmSingleMemWrite (0x014C,0x0580);
     brmSingleMemWrite (0x014D,0x05A0);
     brmSingleMemWrite (0x014E,0x05C0);
     brmSingleMemWrite (0x014F,0x05E0);

     brmSingleMemWrite (0x0150,0x0600);
     brmSingleMemWrite (0x0151,0x0620);
     brmSingleMemWrite (0x0152,0x0640);
     brmSingleMemWrite (0x0153,0x0660);
     brmSingleMemWrite (0x0154,0x0680);
     brmSingleMemWrite (0x0155,0x06A0);
     brmSingleMemWrite (0x0156,0x06C0);
     brmSingleMemWrite (0x0157,0x06E0);
     brmSingleMemWrite (0x0158,0x0700);
     brmSingleMemWrite (0x0159,0x0720);
     brmSingleMemWrite (0x015A,0x0740);
     brmSingleMemWrite (0x015B,0x0760);
     brmSingleMemWrite (0x015C,0x0780);
     brmSingleMemWrite (0x015D,0x07A0);
     brmSingleMemWrite (0x015E,0x07C0);
     brmSingleMemWrite (0x015F,0x07E0);

     //#TRANSMIT BLOCKS
     brmSingleMemWrite (0x0160 ,0x0800);
     brmSingleMemWrite (0x0161 ,0x0820);
     brmSingleMemWrite (0x0162 ,0x0840);
     brmSingleMemWrite (0x0163 ,0x0860);
     brmSingleMemWrite (0x0164 ,0x0880);
     brmSingleMemWrite (0x0165 ,0x08A0);
     brmSingleMemWrite (0x0166 ,0x08C0);
     brmSingleMemWrite (0x0167 ,0x08E0);
     brmSingleMemWrite (0x0168 ,0x0900);
     brmSingleMemWrite (0x0169 ,0x0920);
     brmSingleMemWrite (0x016A ,0x0940);
     brmSingleMemWrite (0x016B ,0x0960);
     brmSingleMemWrite (0x016C ,0x0980);
     brmSingleMemWrite (0x016D ,0x09A0);
     brmSingleMemWrite (0x016E ,0x09C0);
     brmSingleMemWrite (0x016F ,0x09E0);

     // these addresses are the same as Rx commands and will result in Wrap around data
     brmSingleMemWrite (0x0170,0x0600);
     brmSingleMemWrite (0x0171,0x0620);
     brmSingleMemWrite (0x0172,0x0640);
     brmSingleMemWrite (0x0173,0x0660);
     brmSingleMemWrite (0x0174,0x0680);
     brmSingleMemWrite (0x0175,0x06A0);
     brmSingleMemWrite (0x0176,0x06C0);
     brmSingleMemWrite (0x0177,0x06E0);
     brmSingleMemWrite (0x0178,0x0700);
     brmSingleMemWrite (0x0179,0x0720);
     brmSingleMemWrite (0x017A,0x0740);
     brmSingleMemWrite (0x017B,0x0760);
     brmSingleMemWrite (0x017C,0x0780);
     brmSingleMemWrite (0x017D,0x07A0);
     brmSingleMemWrite (0x017E,0x07C0);
     brmSingleMemWrite (0x017F,0x07E0);

     //bcr BLOCKS
     brmSingleMemWrite (0x0180,0x0400);
     brmSingleMemWrite (0x0181,0x0420);
     brmSingleMemWrite (0x0182,0x0440);
     brmSingleMemWrite (0x0183,0x0460);
     brmSingleMemWrite (0x0184,0x0480);
     brmSingleMemWrite (0x0185,0x04A0);
     brmSingleMemWrite (0x0186,0x04C0);
     brmSingleMemWrite (0x0187,0x04E0);
     brmSingleMemWrite (0x0188,0x0500);
     brmSingleMemWrite (0x0189,0x0520);
     brmSingleMemWrite (0x018A,0x0540);
     brmSingleMemWrite (0x018B,0x0560);
     brmSingleMemWrite (0x018C,0x0580);
     brmSingleMemWrite (0x018D,0x05A0);
     brmSingleMemWrite (0x018E,0x05C0);
     brmSingleMemWrite (0x018F,0x05E0);

     brmSingleMemWrite (0x0190,0x0600);
     brmSingleMemWrite (0x0191,0x0620);
     brmSingleMemWrite (0x0192,0x0640);
     brmSingleMemWrite (0x0193,0x0660);
     brmSingleMemWrite (0x0194,0x0680);
     brmSingleMemWrite (0x0195,0x06A0);
     brmSingleMemWrite (0x0196,0x06C0);
     brmSingleMemWrite (0x0197,0x06E0);
     brmSingleMemWrite (0x0198,0x0700);
     brmSingleMemWrite (0x0199,0x0720);
     brmSingleMemWrite (0x019A,0x0740);
     brmSingleMemWrite (0x019B,0x0760);
     brmSingleMemWrite (0x019C,0x0780);
     brmSingleMemWrite (0x019D,0x07A0);
     brmSingleMemWrite (0x019E,0x07C0);
     brmSingleMemWrite (0x019F,0x07E0);

     //#SA control words
     //#4210 - End of message interrupt for Rx + Tx + BCR
     //# now in burst mode
     //FILL 001A0 4210 32 # 32 writes with the same data 0x4210 from address 0x1A0 to 0x1BF
     //VERIFY_FILL 001A0 4210 32

     //#setup the Monitor pointers.
     brmSingleMemWrite (0x0102, 0x0C00);
     brmSingleMemWrite (0x00103, 0x0E00);

     //#enable the Monitor
     brmSingleRegWrite (0X0001,0X9F80);
     //#enable the monitor table for two sub addresses
     brmSingleMemWrite (0x02E0,0x0002);
     brmSingleMemWrite (0x02E1,0x0002);
     brmSingleMemWrite (0x02E3,0x0002);



}

// BC version for FreeRTOS demonstration . it does nor printing results on the terminal
void brmBC_FreeRTOS(void)

{

	 u16 data16;



	 brmSingleRegWrite (0x0003,0x0001); //perform SW reset
	 brmSingleRegWrite (0x0001,0x0040); //enable BC mode in config 1 register

	data16 = brmSingleRegRead (0x0001);


	//READ 40001 0040

	brmSingleRegWrite (0x0002,0x9C73); //Setup config 2 register
	brmSingleRegWrite (0x0007,0x8000); // write config register 3
	brmSingleRegWrite (0x0008,0x1060); //Setup config 4 register

	brmSingleRegWrite (0x000D,0x0004); //setup the Frame length to 400 us
	//setup memory stack
	// Block Status word (core)
	// Time tag (core)
	// Gap to next message
	// Setup Stack Memory pointers
	brmSingleMemWrite(0x0000,0x0000);
	brmSingleMemWrite(0x0001,0x0000);
	brmSingleMemWrite(0x0002,0x0000);
	brmSingleMemWrite(0x0003,0x01A0); //check
	brmSingleMemWrite(0x0004,0x0000);
	brmSingleMemWrite(0x0005,0x0000);
	brmSingleMemWrite(0x0006,0x0000);
	brmSingleMemWrite(0x0007,0x0160); //check
	brmSingleMemWrite(0x0008,0x0000);
	brmSingleMemWrite(0x0009,0x0000);
	brmSingleMemWrite(0x000A,0x0000);
	brmSingleMemWrite(0x000B,0x0120); //check

	brmSingleMemWrite(0x000C,0x0000);
	brmSingleMemWrite(0x000D,0x0000);
	brmSingleMemWrite(0x000E,0x0000);
	brmSingleMemWrite(0x000F,0x01C0); //check

	//setup stack values pointers
	brmSingleMemWrite (0x0100,0x0000);
	brmSingleMemWrite (0x0101,0xFFFB);


	//Define command list

	brmSingleMemWrite(0x01A0, 0x0000); // control word

	brmSingleMemWrite(0x01A1, 0xC023); // the command
	brmSingleMemWrite(0x01A2, 0x0000); // fill up the memory
	brmSingleMemWrite(0x01A3, 0x1111);
	brmSingleMemWrite(0x01A4, 0x2222);
	brmSingleMemWrite(0x0160, 0x0080); // control word . Bus A
	brmSingleMemWrite(0x0161, 0xCC24); // Command to another RT 25, SA1 ,transmit, 4 words
	brmSingleMemWrite(0x0120, 0x0080); // control word . Bus A
	brmSingleMemWrite(0x0121, 0xC423); // command to read data

	brmSingleMemWrite(0x01C0, 0x0080); // control word . Bus A
	brmSingleMemWrite(0x01C1, 0xD428); // command to read data. 8 words


	brmSingleRegWrite(0x0003, 0x0002); //go for one frame

	// wait for 400 us
    usleep(1000);  // sleep was not working
 	//data16 = brmSingleRegRead (0x0006);
		//xil_printf("read register 0x0006:%0x \n\r",data16);
	// verify frame complete - 8009
	// now verify data is correct

	//READ 00101 FFFF


}
