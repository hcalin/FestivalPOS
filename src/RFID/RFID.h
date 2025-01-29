#ifndef _RFID_H_
#define _RFID_H_
#include "MFRC522.h"
#include "UI.h"



// The GPIO pin that's connected to the MFRC522's reset pin
#define RST_PIN RPI_V2_GPIO_P1_15
// The GPIO pin that's connected to the MFRC522's SDA pin,
// sometimes labeled SS or CE or CS.
// Doesn't have to be one of the CE pins on the pi
#define SS_PIN RPI_V2_GPIO_P1_24

extern MEVENT event;
//extern MFRC522 rfid(SS_PIN, RST_PIN); 

#define READ_SUCCESSFULL 0U
#define ERROR_TYPE_NOT_KNOWN 1U


void MFRC522_init();
int MFRC522_wait_for_read();

#endif
