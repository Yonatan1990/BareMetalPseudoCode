#include "xil_types.h"

u16 brmSingleRegWrite(u16 address,u16 data);
u16 brmSingleRegRead(u16 address);

u16 brmSingleMemWrite(u16 address,u16 data);
u16 brmSingleMemRead(u16 address);

void brmBurstMemWrite(u16 startAddress,u16 length, u16* data);
void brmBurstMemRead(u16 startAddress,u16 length, u16* data);









