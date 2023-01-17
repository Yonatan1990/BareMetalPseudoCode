
// BRM_RTMT.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include <string>
//#include "windows.h"
#include "stld1553_operatingSystemDependencies.h"
#include "stld1553_returnCodes.h"
#include "stld1553.h"
#include "stld1553_driverInterface.h"
#include "SitalAPI.h"


// typedef unsigned int    U32BIT;
// typedef signed int      S32BIT;
typedef unsigned short  UINT16;
// typedef signed short    S16BIT;
// typedef unsigned char   U8BIT;


#include "CommonTypes.h"

U32BIT uiDeviceCount = 1;
//U32BIT uiDeviceID0;
//U32BIT uiDeviceID1;

 U32BIT uiDeviceID0 = 0;
 U32BIT uiDeviceID1 = 1;
 U32BIT uiDeviceID2 = 2;

S16BIT iResult;
UINT16 device0 = 0;



//#pragma region BC

// run BC and wait for EOF on interrupt callback
int counter = 0;
void _DECL funcExternalISR(S16BIT DevNum, U32BIT Status)
{
	//ISR implementation user defined callback routine
	//printf("\nAn interrupt %i has occurred \n", counter++);
}


void InitDevicesAsRtMt(U32BIT uiDeviceID)
{
	UINT16 NumberOfDevices = 0;
	iResult = sitalDevice_GetCount(&NumberOfDevices);
	printf("\nDevices detected : %i", NumberOfDevices);

	iResult = sitalDevice_Initialize(uiDeviceID,sitalAccess_CARD,sitalMode_RT_AND_MT | sitalModeVariant_ADVANCED,0,0,0);
	printf("\nsitalMode_RT_AND_MT 0 INIT %i", iResult);

//	iResult = sitalDevice_Initialize(uiDeviceID1,sitalAccess_CARD,sitalMode_RT_AND_MT | sitalModeVariant_ADVANCED,0,0,0);
//	printf("\nsitalMode_RT_AND_MT 1 INIT %i", iResult);
}

void InitDevicesAsRt(U32BIT uiDeviceID)
{
	UINT16 NumberOfDevices = 0;
	iResult = sitalDevice_GetCount(&NumberOfDevices);
	//goes to  openDriverIf()
	printf("\nDevices detected : %i\n\r", NumberOfDevices);

	iResult = sitalDevice_Initialize(uiDeviceID,sitalAccess_CARD,sitalMode_RT | sitalModeVariant_ADVANCED,0,0,0);
	printf("\n\rsitalMode_RT_AND_MT %x INIT: %i\n\r",uiDeviceID, iResult);

//	iResult = sitalDevice_Initialize(uiDeviceID1,sitalAccess_CARD,sitalMode_RT | sitalModeVariant_ADVANCED,0,0,0);
//	printf("\nsitalMode_RT_AND_MT 1 INIT %i", iResult);
}




void Set_mRT(U32BIT uiDeviceID)

{
//
	//	UINT16 length = 31;
		UINT16 SubAddress = 1;         // 10
		UINT16  DataWordsBuffer[32];
		UINT16  DataBlockID = 1;
		UINT16 wRtAddress = 24;         // 24


		for(int i = 0 ; i < 32 ; i++) DataWordsBuffer[i] = i*2; // some data to see on the BC's side...
		//printf("\n\rsital rt mt \n");

		iResult = sitalDevice_Initialize(uiDeviceID1,sitalAccess_CARD,sitalMode_MRT /* | sitalModeVariant_ADVANCED*/,0,0,0);
		//Create Data Block
		printf("Initialize result: %i\n\r",iResult);

		iResult = sital_mRt_EnableRts (1,0xFFFFFFFF);
		printf("Sital enable mRT result: %i\n\r",iResult);

		iResult = sital_mRt_Create_BusList(0);
		printf("Create buslist result: %i\n\r",iResult);

		iResult = sital_mRt_Create_BusList_Element(0,0xc540,0);
		printf("Create buslist element result: %i\n\r",iResult);

		iResult = sital_mRt_Create_Element_DataBlock (0,0,DataWordsBuffer,64);
		printf("Create element datablock result: %i\n\r",iResult);

		iResult = sital_mRt_Map_DataBlock_To_Element (0,0);
		printf("mRT map datablock result: %i\n\r",iResult);

		iResult = sital_mRt_Map_Element_To_BusList (0,0);
		printf("Map element to buslist result: %i\n\r",iResult);

		iResult = sital_mRt_Start (uiDeviceID1,0);
		printf("Sital mRT start result: %i\n\r",iResult);
//		printf("\Decode stack of device:%x\n\r",uiDeviceID);
//
//
//
//
}

void SetRT_MT(U32BIT uiDeviceID)
{

//	UINT16 length = 31;
	UINT16 SubAddress = 10;         // 10
	UINT16  DataWordsBuffer[32];
	UINT16  DataBlockID = 0;
	UINT16 wRtAddress = 22;         // 24


	//wRtAddress = wRtAddress-uiDeviceID; // In you want to assign differnet RT to ID0,1..
	//printf(" Channel chosen for communication :%x",uiDeviceID);
	for(int i = 0 ; i < 32 ; i++) DataWordsBuffer[i] = i*2; // some data to see on the BC's side...
	//printf("\n\rsital rt mt \n");


	iResult = sitalRt_Address_Set(uiDeviceID, wRtAddress);
	printf("\n\r sitalRt_Address_Set:%i",iResult);
	//Create Data Block
	iResult = sitalRt_DataBlock_Create(uiDeviceID, DataBlockID, sitalRtDataBlockType_DOUBLE, &DataWordsBuffer[0], 32);
	printf("\n\r sitalRt_DataBlock_Create:%i",iResult);
	//Map the Data Block with relevant SA 10,11
	//SubAddress = SubAddress - uiDeviceID;
	iResult = sitalRt_DataBlock_MapToSubaddress(uiDeviceID, DataBlockID, SubAddress, sitalRtMessageType_ALL, sitalRtDataBlockIrq_END_OF_MESSAGE, true);
	printf("\n\r sitalRt_DataBlock_MapToSubaddress:%i",iResult);
	//RT start
	//iResult = sitalRtMt_Start(uiDeviceID);

	iResult = sitalRt_Start(uiDeviceID);

	printf("\n\r sitalRt_Start:%i\n",iResult);

	int counter = 0;

	printf("\Decode stack of device:%x\n\r",uiDeviceID);

//	while(counter < 10) // run the monitor until 100 messages recieved...
//    {
//  //      iResult = sitalMt_Message_GetFromStackDecoded(uiDeviceID, &dms, sitalMessageLocationAndRemoval_NEXT_PURGE, sitalMtStack_ACTIVE);
//		//printf("\n\r counter:%d", counter);
//		iResult = sitalRt_Message_GetFromStackDecoded(uiDeviceID, &dms, sitalMessageLocationAndRemoval_NEXT_PURGE);  //, sitalMtStack_ACTIVE);
//		//printf("\n\r iresult:%i", iResult);
//		//printf("\n\r decode function iResult:%i",iResult);
//		if(iResult == 1) // a message recieved
//        {
//            printf("msg:%d Cmd:%04X: D0=%04X  D1=%04X \n\r",counter, dms.wCommandWord1, dms.waData[0], dms.waData[1]);
//			counter++;
//		}
//        usleep(0);
//    }
}

void Set_MT(U32BIT uiDeviceID)
{

	UINT16 wRtAddress = 24;         // 24

	iResult = sitalMt_Start(uiDeviceID);

	printf("\n\r sitalRt_Start:%i\n",iResult);


}

void InitDeviceAsBC()
{
	unsigned int uiDeviceCount = 1;
    unsigned int uiDeviceID = 0;

	U16BIT wMajorVersionDll, wMinorVersionDll, wBuildNumberDll, wRevisionNumberDll;

	sitalStld1553_GetLibraryVersion(&wMajorVersionDll, &wMinorVersionDll, &wBuildNumberDll, &wRevisionNumberDll);
	printf ("Library Version: %i.%i.%i.%i\n", wMajorVersionDll, wMinorVersionDll, wBuildNumberDll, wRevisionNumberDll);
	sitalDeviceCapabilitiesStructure capStruct;

	U16BIT NumberOfDevices = 0;
	S16BIT iResult = sitalDevice_GetCount(&NumberOfDevices);
	printf ("Device Count: %i\n", NumberOfDevices);
	for(int idx = 0 ; idx < 1/*NumberOfDevices*/ ; idx++){
		uiDeviceID = idx;
		iResult = sitalDevice_Initialize(uiDeviceID, sitalAccess_CARD, sitalMode_BC, 0, 0, 0);
		if(iResult != 0)
		{
			printf("sitalDevice_InitializeBC.Error: %i", iResult);
			return;
		}
		iResult = sitalDevice_TimeTag_SetResolution(uiDeviceID, sitalTimeTagResolution_64US);
		if(iResult != 0)
		{
			printf("TimeTag_SetResolution Error: %i", iResult);
			return;
		}
		printf ("Time Tag resolution of %i\n", sitalTimeTagResolution_64US);


		iResult = sitalDevice_Irq_Manipulate(uiDeviceID, 1, sitalInterruptRegister2_BC_IRQ3, &funcExternalISR);
		if(iResult != 0)
		{
			printf("sitalDevice_Irq_Manipulate Error: %i", iResult);
			return;
		}
		printf ("sitalDevice_Irq_Manipulate %i\n", iResult);
	}
}






void SetBCDataRtToRT()
{

	U16BIT uwCmdWord1, uwCmdWord2;
	S16BIT iResult;

	uwCmdWord1 = 0x820; // receive command to rt 1 // sould put the receive first
	uwCmdWord2 = 0x6420; // transmit command to rt 12

	//iResult = sitalBc_Message_CreateRtToRt (0,0,0,1,1,32,12,1,0,sitalBcControlWord_RT_TO_RT);
	//printf("sitalBc_Message_CreateRtToRt: %x\n", iResult);

	unsigned int uiDeviceCount = 1;
	    unsigned int uiDeviceID = 0;

		//Set a Single Frame
		S16BIT swaCommandIds[sitalBcCounter_COMMANDS];
		U16BIT wCommandIdCount = 0;
		S16BIT DataBlockID, MessageID, CmdID;
		U16BIT wMinorFrameTime, wMajorFrameTime;
		U16BIT sCmdWord1, sCmdWord2;
		U16BIT BcControlWord, MsgGap;
		U16BIT NumberOfRows, WordCount, MsgCount;
		U16BIT DataWordsBuffer [31];
		BOOLEAN AtLeastOneMsgEn;

		NumberOfRows = 1;//1 messages
		wMajorFrameTime = 20000;//50Hz / 20,000Ms
		U16BIT FrameID = 0;
		wMinorFrameTime = 0;
	    MsgCount = 0;

		//Create Datablocks and messages

		for(int i = 0 ; i < NumberOfRows ; i++)
		{

			printf("\nMassage %d\n",i);
			DataBlockID = i;
			MessageID = i;
			CmdID = i;

			WordCount = 32;//Set to 10 words
			for(int j = 0 ; j < WordCount -1 ; j++)
			{
				DataWordsBuffer[j] = j + 100;
			}
			//Create Data Block with Data
			printf("\nDataBlock_Create\n");
			iResult = sitalBc_DataBlock_Create(uiDeviceID, DataBlockID, sitalBcDataBlockSizeOption_DOUBLE,  NULL, WordCount);
			if(iResult != 0)
			{
				printf("sitalBc_DataBlock_Create.Error: %i\n", iResult);
				return;
			}
			MsgGap = 0;
			BcControlWord = 0;
			int * cmdWord;
			BcControlWord += sitalBcControlWord_BUS_A; /*sitalBcControlWord_BUS_B*/
			printf("BC Control Word %i \n", BcControlWord);
			//printf("Calculate Command Words\n");

			U16BIT destinationSubaddress = 1;
			U16BIT deDestination = 0;//RT0
			//CalculateCommandWords(cmdWord, WordCount);

	//		sCmdWord1 = (U16BIT)&cmdWord;

	//		uwCmdWord1 = 0x82A + i * 0x1000;
			 uwCmdWord1 = 0xC540; // 0xC540 - RT2BC rt24 SA 10, WC 32  //0x82A + i * 0x1000; trassmit RT-->BC

			//uwCmdWord1 = 0x6420; // transmit command from bc to RT12 to transmit to RT1  and 1 SA


			sCmdWord2 = 0;
			uwCmdWord2 = 0;

			printf("Creating Message %i\n", i);
			iResult = sitalBc_Message_Create(uiDeviceID, (MessageID + i), DataBlockID, BcControlWord, uwCmdWord1, uwCmdWord2, MsgGap, 0, 0, 0, 0, 0, 0);

			printf("Message result %i\n", iResult);
			if(iResult == -1000)
			{
				printf("sitalReturnCode_INVALID_MESSAGE.Error : %i\n", iResult);
				return;
			}
			if(iResult != 0)
			{
				printf("sitalBc_Message_Create.Error: %i\n", iResult);
				return;
			}
			//Create Exec Command for each Msg
			printf("sitalBc_Command_1_Create\n");
			iResult = sitalBc_Command_Create(uiDeviceID, CmdID, sitalOpcode_EXECUTE_MESSAGE, sitalOpcodeCondition_ALWAYS, (MessageID + i), 0, 0);
			if(iResult != 0)
			{
				printf("sitalBc_Command_1_Create.Error: %i\n", iResult);
				return;
			}
			swaCommandIds[MsgCount] = CmdID;
			MsgCount++;
		}
		//Create Minor Frame
		printf("Create Minor frame\n");
		wCommandIdCount = MsgCount;
		iResult = sitalBc_Frame_Create(uiDeviceID, FrameID, sitalBcFrameType_MINOR, swaCommandIds, wCommandIdCount, wMinorFrameTime, 0);
		if(iResult != 0)
		{
			printf("Minor sitalBc_Frame_Create.Error: %i\n", iResult);
			return;
		}
		//Create Call Minor Frame Command
		printf("Create frame command_2\n");
		CmdID += 1;
		iResult = sitalBc_Command_Create(uiDeviceID, CmdID, sitalOpcode_CALL_SUBROUTINE, sitalOpcodeCondition_ALWAYS, 0, 0, 0);
		if(iResult != 0)
		{
			printf("sitalBc_Command_2_Create.Error: %i\n", iResult);
			return;
		}
		//Create Major Frame
		printf("Create Major frame\n");
		swaCommandIds[0] = CmdID;
		wCommandIdCount = 1;
		FrameID += 1;
		iResult = sitalBc_Frame_Create(uiDeviceID, FrameID, sitalBcFrameType_MAJOR, swaCommandIds, wCommandIdCount, wMajorFrameTime, 0);
		if(iResult != 0)
		{
			printf("Major sitalBc_Frame_Create.Error: %i\n", iResult);
			return;
		}

		//BC Start

	//	sleep(10);

		/*
		* Here you can use the Register Read / Write, Memory Read Write functions
		* _EXTERN U16BIT _DECL sitalDevice_Register_Read (S16BIT swDevice, U16BIT wRegisterAddress);
		* _EXTERN S16BIT _DECL sitalDevice_Register_Write (S16BIT swDevice, U16BIT wRegisterAddress, U16BIT wRegisterValue);
		* _EXTERN U16BIT _DECL sitalDevice_Memory_Read (S16BIT swDevice, U16BIT wMemoryAddress);
		* _EXTERN S16BIT _DECL sitalDevice_Memory_Write (S16BIT swDevice, U16BIT wMemoryAddress, U16BIT wMemoryValue);
		*/

	 //   iResult = sitalBc_Start(uiDeviceID, FrameID, 1);
		iResult = sitalBc_Start(uiDeviceID, FrameID, 25000);   //30);
	    printf("sitalBc_Start: %i\n", iResult);
	    if(iResult != 0)
		{
			printf("sitalBc_Start.Error: %i\n", iResult);
			return;
		}



}




void SetBCDataBCToRT()
{
	unsigned int uiDeviceCount = 1;
    unsigned int uiDeviceID = 0;

	//Set a Single Frame
	S16BIT swaCommandIds[sitalBcCounter_COMMANDS];
	U16BIT wCommandIdCount = 0;
	S16BIT DataBlockID, MessageID, CmdID;
	U16BIT wMinorFrameTime, wMajorFrameTime;
	U16BIT sCmdWord1, sCmdWord2;
	U16BIT uwCmdWord1, uwCmdWord2;
	U16BIT BcControlWord, MsgGap;
	U16BIT NumberOfRows, WordCount, MsgCount;
	U16BIT DataWordsBuffer [31];
	BOOLEAN AtLeastOneMsgEn;
	S16BIT iResult;

	NumberOfRows = 1;//3 messages
	wMajorFrameTime = 20000;//50Hz / 20,000Ms
	U16BIT FrameID = 0;
	wMinorFrameTime = 0;
    MsgCount = 0;

	//Create Datablocks and messages

	for(int i = 0 ; i < NumberOfRows ; i++)
	{

		printf("\nMassage %d\n",i);
		DataBlockID = i;
		MessageID = i;
		CmdID = i;

		WordCount = 32;//Set to 10 words
		for(int j = 0 ; j < WordCount -1 ; j++)
		{
			DataWordsBuffer[j] = j + 100;
		}
		//Create Data Block with Data
		printf("\nDataBlock_Create\n");
		iResult = sitalBc_DataBlock_Create(uiDeviceID, DataBlockID, sitalBcDataBlockSizeOption_DOUBLE, &DataWordsBuffer[0], WordCount);
		if(iResult != 0)
		{
			printf("sitalBc_DataBlock_Create.Error: %i\n", iResult);
			return;
		}
		MsgGap = 0;
		BcControlWord = 0;
		int * cmdWord;
		BcControlWord += sitalBcControlWord_BUS_A; /*sitalBcControlWord_BUS_B*/
		printf("BC Control Word %i \n", BcControlWord);
		//printf("Calculate Command Words\n");

		U16BIT destinationSubaddress = 1;
		U16BIT deDestination = 0;//RT0
		//CalculateCommandWords(cmdWord, WordCount);

//		sCmdWord1 = (U16BIT)&cmdWord;

//		uwCmdWord1 = 0x82A + i * 0x1000;b
		//uwCmdWord1 = 0xC540; // 0xC540 - RT2BC rt24 SA 10, WC 32  //0x82A + i * 0x1000; trassmit RT-->BC

		uwCmdWord1 = 0xC540; // receive command from bc to rt, RT 24  and 10 SA
		//uwCmdWord2 = 0xC940; // receive command from bc to rt, RT 25  and 10 SA

		//uwCmdWord1 = 0x6420; // transmit command from bc to RT12 to transmit to RT1  and 1 SA


		sCmdWord2 = 0;
		uwCmdWord2 = 0;

		printf("Creating Message %i\n", i);
		iResult = sitalBc_Message_Create(uiDeviceID, (MessageID + i), DataBlockID, BcControlWord, uwCmdWord1, uwCmdWord2, MsgGap, 0, 0, 0, 0, 0, 0);

		printf("Message result %i\n", iResult);
		if(iResult == -1000)
		{
			printf("sitalReturnCode_INVALID_MESSAGE.Error : %i\n", iResult);
			return;
		}
		if(iResult != 0)
		{
			printf("sitalBc_Message_Create.Error: %i\n", iResult);
			return;
		}
		//Create Exec Command for each Msg
		printf("sitalBc_Command_1_Create\n");
		iResult = sitalBc_Command_Create(uiDeviceID, CmdID, sitalOpcode_EXECUTE_MESSAGE, sitalOpcodeCondition_ALWAYS, (MessageID + i), 0, 0);
		if(iResult != 0)
		{
			printf("sitalBc_Command_1_Create.Error: %i\n", iResult);
			return;
		}
		swaCommandIds[MsgCount] = CmdID;
		MsgCount++;
	}
	//Create Minor Frame
	printf("Create Minor frame\n");
	wCommandIdCount = MsgCount;
	iResult = sitalBc_Frame_Create(uiDeviceID, FrameID, sitalBcFrameType_MINOR, swaCommandIds, wCommandIdCount, wMinorFrameTime, 0);
	if(iResult != 0)
	{
		printf("Minor sitalBc_Frame_Create.Error: %i\n", iResult);
		return;
	}
	//Create Call Minor Frame Command
	printf("Create frame command_2\n");
	CmdID += 1;
	iResult = sitalBc_Command_Create(uiDeviceID, CmdID, sitalOpcode_CALL_SUBROUTINE, sitalOpcodeCondition_ALWAYS, 0, 0, 0);
	if(iResult != 0)
	{
		printf("sitalBc_Command_2_Create.Error: %i\n", iResult);
		return;
	}
	//Create Major Frame
	printf("Create Major frame\n");
	swaCommandIds[0] = CmdID;
	wCommandIdCount = 1;
	FrameID += 1;
	iResult = sitalBc_Frame_Create(uiDeviceID, FrameID, sitalBcFrameType_MAJOR, swaCommandIds, wCommandIdCount, wMajorFrameTime, 0);
	if(iResult != 0)
	{
		printf("Major sitalBc_Frame_Create.Error: %i\n", iResult);
		return;
	}

	//BC Start

//	sleep(10);

	/*
	* Here you can use the Register Read / Write, Memory Read Write functions
	* _EXTERN U16BIT _DECL sitalDevice_Register_Read (S16BIT swDevice, U16BIT wRegisterAddress);
	* _EXTERN S16BIT _DECL sitalDevice_Register_Write (S16BIT swDevice, U16BIT wRegisterAddress, U16BIT wRegisterValue);
	* _EXTERN U16BIT _DECL sitalDevice_Memory_Read (S16BIT swDevice, U16BIT wMemoryAddress);
	* _EXTERN S16BIT _DECL sitalDevice_Memory_Write (S16BIT swDevice, U16BIT wMemoryAddress, U16BIT wMemoryValue);
	*/

 //   iResult = sitalBc_Start(uiDeviceID, FrameID, 1);
	iResult = sitalBc_Start(uiDeviceID, FrameID, 25000);   //30);
    printf("sitalBc_Start: %i\n", iResult);
    if(iResult != 0)
	{
		printf("sitalBc_Start.Error: %i\n", iResult);
		return;
	}
}

void SetBCDataBCToBCST()
{
	unsigned int uiDeviceCount = 1;
    unsigned int uiDeviceID = 1;

	//Set a Single Frame
	S16BIT swaCommandIds[sitalBcCounter_COMMANDS];
	U16BIT wCommandIdCount = 0;
	S16BIT DataBlockID, MessageID, CmdID;
	U16BIT wMinorFrameTime, wMajorFrameTime;
	U16BIT sCmdWord1, sCmdWord2;
	U16BIT uwCmdWord1, uwCmdWord2;
	U16BIT BcControlWord, MsgGap;
	U16BIT NumberOfRows, WordCount, MsgCount;
	U16BIT DataWordsBuffer [31];
	BOOLEAN AtLeastOneMsgEn;
	S16BIT iResult;

	NumberOfRows = 8;//8 messages
	wMajorFrameTime = 20000;//50Hz / 20,000Ms
	U16BIT FrameID = 0;
	wMinorFrameTime = 0;
    MsgCount = 0;

	//Create Datablocks and messages
	for(int i = 0 ; i < NumberOfRows ; i++)
	{
		DataBlockID = i;
		MessageID = i;
		CmdID = i;

		WordCount = 7;//Set to 7 words
		for(int j = 0 ; j < WordCount -1 ; j++)
		{
			DataWordsBuffer[j] = j + 100;
		}
		//Create Data Block with Data
		printf("Creating data block\n");
		printf("\nDataBlock_Create\n");
		iResult = sitalBc_DataBlock_Create(uiDeviceID, DataBlockID, sitalBcDataBlockSizeOption_DOUBLE, &DataWordsBuffer[0], WordCount);
		if(iResult != 0)
		{
			printf("sitalBc_DataBlock_Create.Error: %i\n", iResult);
			return;
		}
		MsgGap = 0;
		BcControlWord = 0;
		int * cmdWord;
		BcControlWord += sitalBcControlWord_BUS_B;
		printf("BC Control Word %i \n", BcControlWord);
		//printf("Calculate Command Words\n");

		U16BIT destinationSubaddress = 10;
		//U16BIT deDestination = 12;//RT
		S16BIT source = -1;//BC
		uwCmdWord1 = 0x826 + i * 0x2000;
		sCmdWord2 = 0;
		uwCmdWord2 = 0;

		printf("uwCmdWord1 %i\n", uwCmdWord1);

		printf("Creaing Message %i\n", i);
		iResult = sitalBc_Message_Create(uiDeviceID, (MessageID + i), DataBlockID, BcControlWord, uwCmdWord1, uwCmdWord2, MsgGap, 0, 0, 0, 0, 0, 0);

		printf("Message result %i\n", iResult);
		if(iResult == -1000)
		{
			printf("sitalReturnCode_INVALID_MESSAGE.Error : %i\n", iResult);
			return;
		}
		if(iResult != 0)
		{
			printf("sitalBc_Message_Create.Error: %i\n", iResult);
			return;
		}
		//Create Exec Command for each Msg
		printf("sitalBc_Command_Create\n");
		iResult = sitalBc_Command_Create(uiDeviceID, CmdID, sitalOpcode_EXECUTE_MESSAGE, sitalOpcodeCondition_ALWAYS, (MessageID + i), 0, 0);
		if(iResult != 0)
		{
			printf("sitalBc_Command_Create.Error: %i\n", iResult);
			return;
		}
		swaCommandIds[MsgCount] = CmdID;
		MsgCount++;
	}
	//Create Minor Frame
	printf("Create Minor frame\n");
	wCommandIdCount = MsgCount;
	iResult = sitalBc_Frame_Create(uiDeviceID, FrameID, sitalBcFrameType_MINOR, swaCommandIds, wCommandIdCount, wMinorFrameTime, 0);
	if(iResult != 0)
	{
		printf("Minor sitalBc_Frame_Create.Error: %i\n", iResult);
		return;
	}
	//Create Call Minor Frame Command
	printf("Create frame command\n");
	CmdID += 1;
	iResult = sitalBc_Command_Create(uiDeviceID, CmdID, sitalOpcode_CALL_SUBROUTINE, sitalOpcodeCondition_ALWAYS, 0, 0, 0);
	if(iResult != 0)
	{
		printf("sitalBc_Command_Create.Error: %i\n", iResult);
		return;
	}
	//Create Major Frame
	printf("Create Major frame\n");
	swaCommandIds[0] = CmdID;
	wCommandIdCount = 1;
	FrameID += 1;
	iResult = sitalBc_Frame_Create(uiDeviceID, FrameID, sitalBcFrameType_MAJOR, swaCommandIds, wCommandIdCount, wMajorFrameTime, 0);
	if(iResult != 0)
	{
		printf("Major sitalBc_Frame_Create.Error: %i\n", iResult);
		return;
	}

	//BC Start
	iResult = sitalDevice_Irq_Manipulate(uiDeviceID, 1, sitalInterruptRegister2_BC_IRQ3, &funcExternalISR);
    if(iResult != 0)
	{
		printf("sitalDevice_Irq_Manipulate.Error: %i\n", iResult);
		return;
	}
    iResult = sitalBc_Start(uiDeviceID, FrameID, 1);
    if(iResult != 0)
	{
		printf("sitalBc_Start.Error: %i\n", iResult);
		return;
	}
}
