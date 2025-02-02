 ![SLOPE festival POS](https://i.ibb.co/qF0mF7ms/Untitled-design.png)
![GitHub Logo](https://github.githubassets.com/images/modules/logos_page/GitHub-Mark.png)

 Hello dear friends.

 This is a POS device that uses ncurses as an UI and sqlite3 as a database. You can select which and how much beverage do you want before scannning your bracelet. Each bracelet will be stored in the database with a respective amount. There will be two SQL tables, one for users and one for payments. The communication between the RFID sensor and Rpi is on SPI.
 The values from the database can be modified from the admin menu only by admin bracelets of course. 

Users table:
0|id|INTEGER|0||1
1|RFID|INTEGER|1||0
2|Name|TEXT|1||0
3|sold|REAL|1|0|0
4|isAdmin|BOOLEAN|1|0|h 
5|date_created|DATETIME|1|CURRENT_TIMESTAMP|0

Payments table:
0|id|INTEGER|0||1
1|RFID|INTEGER|1||0
2|amount|REAL|1||0
3|time|DATETIME|1|CURRENT_TIMESTAMP|0
 

Preq: 
-Install bcm2835 library
-Install ncurses library
-Install sqlite3 

//To DO: autocheck for libraries

Configure GPIO pins as in the schematic and be double checked by the pins set in the code. 

HW units: 
https://www.robofun.ro/raspberry-pi-23/display-7-raspberry-pi-touch-screen-capacitive-touch.html
https://www.optimusdigital.ro/ro/wireless-rfid/67-modul-cititor-rfid-mfrc522.html?srsltid=AfmBOoof0RCAOfBLTV58f6GUO2kaHhuLQpCF_YiiOYzYbA6a9fUcCkZ2
3d printed casing
Raspberry PI 4B
