#include <cstdio>
#include <string>
#include "RFID/RFID.h"
#include "UI/UI.h"
#include "DB/DB.h"

int main() {

  /* Initialize UI */
  UI_init();
  
  /* MFRC522 init function + self test */ 
  MFRC522_init();

  /* Database init */
  DB_init();
  
  //---Init finished. Go to UI----//
  
  /* Start UI. Go to main menu */
  UI_start();
  
  while (1) {
    MFRC522_wait_for_read();
  }
  
  endwin();
  return 0;
}
