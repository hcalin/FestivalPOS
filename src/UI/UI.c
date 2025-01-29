#include "UI.h"

uint8_t window_type;
uint16_t max_row,max_col;
MEVENT event; // Mouse event
WINDOW *bar,*admin;


char* bar_items[ITEMS_IN_BAR] = {"Beer", "Wine" , "Palinka"};
char* admin_items[ITEMS_IN_ADMIN] = {"ADD/R USER", "ADD/R GOLD" , "MODIFY NAME"};
uint8_t bar_prices[ITEMS_IN_BAR] = {5, 5, 5};
uint16_t u16_total = 0U;
WINDOW *arr[ITEMS_IN_BAR];
WINDOW *total,*checkout;
WINDOW *backButton,*checkButton;

uint8_t bar_quantity[ITEMS_IN_BAR] = { 0, 0 , 0};

void UI_init()
{
	initscr();
	box(stdscr,0,0); // Outline box
	noecho();
	cbreak(); // Disable line buffering
	keypad(stdscr, TRUE); // Enable special keys
	mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL); // Enable mouse events
	curs_set(0); // Disable cursor
	getmaxyx(stdscr, max_row, max_col);
	
	refresh();
}

void UI_start()
{
  char ch;
  while ((ch = getch() != 'q'))
  {
	if ( ch == 1 ) 
	{
		print_center((max_row/2) - 1, "                            "); // clear screen ca la tara
		print_center((max_row/2) + 1, "                                                 ");
		refresh();
		break;
	}
		refresh();
  }
  
  UI_loop();
}

void UI_loop()
{
	
	while(1)
	{
		switch(window_type)
		{
			case BAR_ADMIN_MENU:
			// Print Bar | Admin window 
				UI_print_bar_admin();
			break;
			
			case BAR_MENU:
				UI_print_bar();
				UI_check_bar();
			break;
			case BAR_CHECKOUT:
				UI_print_checkout();
			break;
			case BAL_CHECK_MENU:
				UI_print_check_bal();
				break;
			case ADMIN_MAIN_MENU:
				UI_print_admin();
			break;
			default:
				print_center( 10, "ERROR, Please reset....");
				clear();
				refresh();
				napms(2000);
			break;
		}
	}
}

void UI_print_check_bal()
{
	DB_info_s DB_info;
	uint32_t nuid = 0;
	int retVal;
	char ch;
	
	
	print_center( 1, "SLOPE BAR *");
	print_center( 10, "Waiting for scan ...");
	
	retVal = MFRC522_wait_for_read();
	if (retVal == READ_SUCCESSFULL)
	{
		clear();
		refresh();
		print_center( 1, "SLOPE BAR *");
		checkout = newwin(20, 45, 4, 23);
		box(checkout, 0,0);
		wprint_center(checkout, 1, "< SCAN RESULTS >" );
		wrefresh(checkout);
		
		for(size_t idx = 0; idx < 4; idx++)
		{
			nuid |= (nuidPICC[idx] << ((3-idx) * 8)) ;
			//mvwprintw(stdscr, 10 + idx, 25, "%x %d ", nuidPICC[idx],idx);
		}
		//mvwprintw(stdscr, 10, 35 , "%x", nuid);
		
		retVal = DB_nuid_exists(nuid, &DB_info);
		if (retVal == 1U) // if RFID id exists in the database
		{
			//mvwprintw(checkout, 3, 10 , "Dear %s, %d %s", DB_info.name, DB_info.balance , DB_info.date_created);
			mvwprintw(checkout, 4, 9 , "Name .............. ");
			mvwprintw(checkout, 4, 29 , "%s",DB_info.name);
			mvwprintw(checkout, 6, 9 , "Balance ........... ");
			mvwprintw(checkout, 6, 29 , "%d lei",DB_info.balance);
			wprint_center(checkout, 13, "Created in:" );
			mvwprintw(checkout, 15, 13, "%s", DB_info.date_created);
		}
		else if (retVal == 0U)
		{
			wprint_center(checkout, 8, "RFID bracelet is not in our database." );
			wprint_center(checkout, 11, "That doesn't mean the beer is free..." );
		}
		else
		{
			wprint_center(checkout, 9, "Connection to the database failed. This is shit." );
		}

	}
	else
	{
		wprint_center(checkout, 9, "RFID TYPE NOT SUPPORTED BY THE READER" );
	}
	
	wrefresh(checkout);
	refresh();
	
	while ((ch = getch() != 'q')  )
	{
		if ( ch == 1 || ch == KEY_MOUSE ) 
		{
			if(getmouse(&event) == OK)
			{	
					clear();
					refresh();
					window_type = BAR_MENU;
					return;
			}
		}
	}
	
}

void UI_print_admin()
{
	char ch;
	
	print_center( 1, "ADMIN MENU *");
	
	backButton = newwin(3, 8, 21 ,5);
	box(backButton, 0,0);
	wprint_center_box(backButton, "BACK");
	wrefresh(backButton);
	
	for (uint8_t idx = 0 ; idx < ITEMS_IN_ADMIN ; idx++)
	{
		if(arr[idx]) delwin(arr[idx]);
		arr[idx] = newwin(10, 25, 3, 5 + idx * 5 + idx * 25);
		box(arr[idx], 0,0);
		wprint_center_box(arr[idx], admin_items[idx]); 
		
		
		
		wrefresh(arr[idx]);
		refresh();
	}
		while ((ch = getch() != 'q')  )
	{
		if ( ch == 1 || ch == KEY_MOUSE ) 
		{
			if(getmouse(&event) == OK)
			{	
				if(wenclose(backButton, event.y , event.x))
				{
					for ( size_t idx = 0 ; idx < ITEMS_IN_ADMIN ; idx ++)
					{
						wclear(arr[idx]);
						wrefresh(arr[idx]);
					}
					wclear(backButton);
					wrefresh(backButton);
					clear();
					refresh();
					window_type = BAR_ADMIN_MENU;
					return;
				}
			}
		}
	}
}

void UI_print_checkout()
{
	char ch;
	
	print_center( 1, "SLOPE BAR *");

	checkout = newwin(20, 45, 4, 23);
	box(checkout, 0,0);
	wprint_center(checkout, 1, "< CHECKOUT >" );
	
	backButton = newwin(3, 8, 21 ,5);
	box(backButton, 0,0);
	wprint_center_box(backButton, "BACK");
	wrefresh(backButton);
	
	for ( size_t idx = 0 ; idx < ITEMS_IN_BAR ; idx ++)
	{
		if (bar_quantity[idx] != 0U)
		{
			mvwprintw(checkout, 3 + idx * 2 , 2 , "  [%dx] %s ",bar_quantity[idx], bar_items[idx]); 
			mvwprintw(checkout, 3 + idx * 2 , 20 , "..............");
			mvwprintw(checkout, 3 + idx * 2 , 35 , "%d lei" , bar_quantity[idx] * bar_prices[idx]);
		}
	}
	mvwprintw(checkout, 13, 27 , " TOTAL %d lei ", u16_total);
	wprint_center(checkout , 16, " Waiting for scan ...");
	wrefresh(checkout);
	while ((ch = getch() != 'q')  )
	{
		if ( ch == 1 || ch == KEY_MOUSE ) 
		{
			if(getmouse(&event) == OK)
			{	
				if(wenclose(backButton, event.y , event.x))
				{
					refresh();
					wclear(backButton);
					wrefresh(backButton);
					wclear(checkout);
					wrefresh(checkout);
					clear();
					refresh();
					/* Go back to BAR MENU and reset sum */ 
					window_type = BAR_MENU;
					for( size_t idx = 0 ; idx < ITEMS_IN_BAR; idx ++)
					bar_quantity[idx] = 0U;
					u16_total = 0U;
					break;
				}
			}
		}
	}
}

void UI_check_bar()
{
	char ch;
	
	while ((ch = getch() != 'q'))
	{
		if ( ch == 1 || ch == KEY_MOUSE ) 
		{
			if(getmouse(&event) == OK)
			{
				for( size_t idx = 0 ; idx < ITEMS_IN_BAR; idx ++)
				{
					if(wenclose(arr[idx], event.y , event.x))
					{
						if (event.x < (15 + 30 * idx ) )
						{
							if (bar_quantity[idx] != 0)
							{
								bar_quantity[idx] --;
								u16_total -=  bar_prices[idx];
							}
						}
						else
						{
							bar_quantity[idx] ++;
							u16_total += bar_prices[idx];
						}
						wrefresh(arr[idx]);
						mvwprintw(arr[idx], 8, 12, "%d    %d lei ", bar_quantity[idx], bar_quantity[idx] * bar_prices[idx]);

						wrefresh(arr[idx]);
						refresh();
						break;
					}
				}
				if(wenclose(total, event.y , event.x) && u16_total != 0U)
				{
					window_type = BAR_CHECKOUT;
					for( size_t idx = 0 ; idx < ITEMS_IN_BAR; idx ++)
					{
						wclear(arr[idx]);
						wrefresh(arr[idx]);
						//delwin(arr[idx]);
					}
					wclear(total);
					wrefresh(total);
					wclear(backButton);
					wrefresh(backButton);
					//delwin(total);
					clear();
					refresh();
					return;
				}
				if(wenclose(backButton, event.y , event.x))
				{
					window_type = BAR_ADMIN_MENU;
					for( size_t idx = 0 ; idx < ITEMS_IN_BAR; idx ++)
					{
						wclear(arr[idx]);
						wrefresh(arr[idx]);
						//delwin(arr[idx]);
					}
					
					wclear(total);
					wrefresh(total);
					wclear(backButton);
					wrefresh(backButton);
					//delwin(total);
					clear();
					refresh();
					
					/* Reset Bar quantity and TOTAL */
					for( size_t idx = 0 ; idx < ITEMS_IN_BAR; idx ++)
					bar_quantity[idx] = 0U;
					u16_total = 0U;
					
					return;
				}
				if(wenclose(checkButton, event.y , event.x))
				{
					window_type = BAL_CHECK_MENU;
					for( size_t idx = 0 ; idx < ITEMS_IN_BAR; idx ++)
					{
						wclear(arr[idx]);
						wrefresh(arr[idx]);
						//delwin(arr[idx]);
					}
					
					wclear(total);
					wrefresh(total);
					wclear(backButton);
					wrefresh(backButton);
					//delwin(total);
					clear();
					refresh();
					
					/* Reset Bar quantity and TOTAL */
					for( size_t idx = 0 ; idx < ITEMS_IN_BAR; idx ++)
					bar_quantity[idx] = 0U;
					u16_total = 0U;
					
					return;
				}
			}
		}
		mvwprintw(total, 1, 2 , "  TOTAL  %d lei ",u16_total);
		mvwprintw(total, 3, 1 , " Click to checkout");
		wrefresh(total);
		refresh();
	}
}

void UI_print_bar()
{
	print_center( 1, "SLOPE BAR *");
	if(total) delwin(total);
	total = newwin(5,21,20,75);
	box(total, 0,0);
	
	for (uint8_t idx = 0 ; idx < ITEMS_IN_BAR ; idx++)
	{
		if(arr[idx]) delwin(arr[idx]);
		arr[idx] = newwin(10, 25, 3, 5 + idx * 5 + idx * 25);
		box(arr[idx], 0,0);
		wprint_center_box(arr[idx], bar_items[idx]); 
		
		mvwaddstr(arr[idx], 5, 2, "-");
		mvwaddstr(arr[idx], 5, 22, "+");
		
		
		wrefresh(arr[idx]);
		refresh();
	} 
	
	/* Back button */
	backButton = newwin(3, 8, 21 ,5);
	box(backButton, 0,0);
	wprint_center_box(backButton, "BACK");
	wrefresh(backButton);
	
	/* Check balance */
	checkButton = newwin(3, 13, 21 , 42);
	box(checkButton, 0,0);
	wprint_center_box(checkButton, "CHECK BAL");
	wrefresh(checkButton);
}

void UI_print_bar_admin()
{
	char ch;

	print_center( 2, "Main menu" );

	
	bar = newwin(15, 25, 5, 15);
	admin = newwin(15,25, 5, 57);
	
	box(bar,0,0);
	box(admin,0,0);
	
	
	wprint_center_box(bar, "BAR WINDOW");
	wprint_center_box(admin, "ADMIN WINDOW");
	
	wrefresh(bar);
	wrefresh(admin);
	refresh();

	/* Waiting for input */
  while ((ch = getch() != 'q'))
  {
	if ( ch == 1 || ch == KEY_MOUSE ) 
	{
		if(getmouse(&event) == OK)
		{
			if(wenclose(bar, event.y , event.x))
			{
				window_type = BAR_MENU;
				/* Clear bar_admin menu & go to bar menu*/
				wclear(bar);
				wrefresh(bar);
				wclear(admin);
				wrefresh(admin);
				delwin(bar);
				delwin(admin);
				clear();
				refresh();
				break;
			}
			else if(wenclose(admin, event.y , event.x))
			{
				window_type = ADMIN_MAIN_MENU;
				/* Clear bar_admin menu & go to admin menu*/
				wclear(bar);
				wrefresh(bar);
				wclear(admin);
				wrefresh(admin);
				delwin(bar);
				delwin(admin);
				clear();
				refresh();
				break;
			}
		}
	}
	refresh();
  }
}

void print_center(uint16_t row, char *title)
{
	size_t len;
	uint16_t indent;
	
	len = strlen(title);
	indent = (max_col - len) / 2; // Center the string
	
	/* Show the string */
	mvaddstr(row,indent,title);
	
	refresh();
}

void wprint_center(WINDOW * win, uint16_t row, char *title)
{
	size_t len;
	uint16_t indent,height, length;
	
	getmaxyx(win, height, length);
	len = strlen(title);
	indent = (length - len) / 2; // Center the string
	
	/* Show the string */
	mvwaddstr(win,row,indent,title);
	
	refresh();
}

void wprint_center_box(WINDOW * win, char *title)
{
	size_t len;
	uint16_t indent,height, length;
	
	getmaxyx(win, height, length);
	len = strlen(title);
	indent = (length - len) / 2; // Center the string
	
	/* Show the string */
	mvwaddstr(win,height/2,indent,title);
	
	refresh();
}

