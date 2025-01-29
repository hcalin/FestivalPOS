#include "RFID.h"

MFRC522 rfid(SS_PIN, RST_PIN); 
uint8_t nuidPICC[4];



/**
 * Helper routine to dump a byte array as hex values.
 */
void printHex(uint8_t *buffer, size_t bufferSize) {
  for (size_t i = 0; i < bufferSize; i++) {
    printf(" %02X", buffer[i]);
  }
}

/**
 * Helper routine to dump a byte array as dec values.
 */
void printDec(uint8_t *buffer, size_t bufferSize) {
  for (size_t i = 0; i < bufferSize; i++) {
    printf(" %03u", buffer[i]);
  }
}


void MFRC522_init()
{
	//printf("Constructor instantied\n");
	rfid.PCD_Init();
	//printf("PCD_Init called\n");
  //printf(rfid.PCD_PerformSelfTest() ? "Self test OK\n" : "Self test ERROR!\n");
  if (rfid.PCD_PerformSelfTest())
  {
    print_center((max_row/2) - 1, " RFID init was successfull.");
  }
  else
  {
    print_center((max_row/2) - 1, " RFID init was not successfull.");
    getch();
  }
  // Init array that will store new NUID
  rfid.PCD_Init();
}

int MFRC522_wait_for_read()
{
    // Reset the loop if no new card present on the sensor/reader. This saves
    // the entire process when idle.
    while(1){
    if (!rfid.PICC_IsNewCardPresent()) continue;
 
    // Verify if the NUID has been readed
    if (!rfid.PICC_ReadCardSerial()) continue;

    //printf("PICC type: ");
    MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
    //printf(rfid.PICC_GetTypeName(piccType).c_str());
    //printf("\n");

    // Check is the PICC of Classic MIFARE type
    if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
        piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
        piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
      //printf("Your tag is not of type MIFARE Classic.\n");
      return ERROR_TYPE_NOT_KNOWN;
    }

//    if (rfid.uid.uidByte[0] != nuidPICC[0] ||
//        rfid.uid.uidByte[1] != nuidPICC[1] ||
//        rfid.uid.uidByte[2] != nuidPICC[2] ||
//        rfid.uid.uidByte[3] != nuidPICC[3]) {
//      printf("A new card has been detected.\n");

      // Store NUID into nuidPICC array
      for (size_t i = 0; i < 4; i++) {
        nuidPICC[i] = rfid.uid.uidByte[i];
      }
//      printf("The NUID tag is:\n");
//      printf("In hex: ");
//      printHex(rfid.uid.uidByte, rfid.uid.size);
//      printf("\nIn dec: ");
//      printDec(rfid.uid.uidByte, rfid.uid.size);
//      printf("\n");
//    } else
//      printf("Card read previously.\n");

    // Halt PICC
    rfid.PICC_HaltA();

    // Stop encryption on PCD
    rfid.PCD_StopCrypto1();
    return READ_SUCCESSFULL; // end task
  }
}
