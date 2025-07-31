#include "UI.h"


char* admin_items[ITEMS_IN_ADMIN] = {"ADD/R USER", "ADD/R GOLD" , "MODIFY NAME"};

extern WINDOW *arr[ITEMS_IN_BAR];
extern uint8_t window_type;
extern WINDOW *backButton,*checkout;


void UI_print_admin()
{
	uint32_t nuid = 0;
	uint8_t retVal;
	char ch;
	DB_info_s DB_info;
	
	print_center( 1, "ADMIN MENU *");
	
	UI_create_back_button();
	
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
					UI_clear_back_button();
					window_type = BAR_ADMIN_MENU;
					return;
				}
				for( size_t idx = 0 ; idx < ITEMS_IN_BAR; idx ++)
				{
					if(wenclose(arr[idx], event.y , event.x))
					{
						switch(idx)
						{
							case 0: //ADD/R USER BUTTON
								UI_clear_back_button();
								wprint_center(stdscr , 16, " Waiting for scan ...");
								
								while (retVal = MFRC522_wait_for_read() == 0xFFU)
								{
									;
								}
								//UI_clear_back_button();
								UI_create_checkout();
								UI_create_back_button();

								if (retVal == READ_SUCCESSFULL)
								{
									for(size_t idx = 0; idx < 4; idx++)
									{
										nuid |= (nuidPICC[idx] << ((3-idx) * 8)) ;
									}	
									if (DB_nuid_exists(nuid, &DB_info) == 1U) // NUID exists
									{
										wprint_center(checkout, 3, "RFID exists in the system already.");										
										mvwprintw(checkout, 5, 9 , "Name .............. ");
										mvwprintw(checkout, 5, 29 , "%s",DB_info.name);
										mvwprintw(checkout, 7, 9 , "Balance ........... ");
										mvwprintw(checkout, 7, 29 , "%.2f lei",DB_info.balance);
										wprint_center(checkout, 13, "Created in:" );
										wprint_center(checkout, 9, ">> CLICK TO DELETE << " );
										mvwprintw(checkout, 15, 13, "%s", DB_info.date_created);
										mvwprintw(checkout, 17, 18, "%x", nuid);
										wrefresh(checkout);
										refresh();
										UI_admin_del_nuid(nuid);										
									}
									else
									{
										wprint_center(checkout, 5, "RFID is not in the system.");
										wprint_center(checkout, 7, "Click on this box to add it");
										wprint_center(checkout, 9, "or click back to fuck off");
										wprint_center(checkout, 16, ">> CLICK ME TO ADD <<");
										wrefresh(checkout);
										refresh();
										UI_admin_add_nuid(nuid);
									}
									

								}
							break;
															
							// ADD/R GOLD
							case 1:
								UI_clear_back_button();
								wprint_center(stdscr , 16, " Waiting for scan ...");
								
								while (retVal = MFRC522_wait_for_read() == 0xFFU)
								{
									;
								}

								if (retVal == READ_SUCCESSFULL)
								{
									clear();
									refresh();
									UI_create_back_button();
									for(size_t idx = 0; idx < 4; idx++)
									{
										nuid |= (nuidPICC[idx] << ((3-idx) * 8)) ;
									}	
									if (DB_nuid_exists(nuid, &DB_info) == 1U) // NUID exists
									{
										//wprint_center(stdscr, 2, "RFID exists in the system already.");										
										mvwprintw(stdscr, 7, 9 , "Name .............. ");
										mvwprintw(stdscr, 7, 29 , "%s",DB_info.name);
										mvwprintw(stdscr, 9, 9 , "Balance ........... ");
										mvwprintw(stdscr, 9, 29 , "%.2f lei",DB_info.balance);
										mvwprintw(stdscr, 15, 13, "%s", DB_info.date_created);
										mvwprintw(stdscr, 17, 18, "%x", nuid);
										wrefresh(stdscr);
										refresh();	
										UI_draw_numpad(nuid, DB_info);						
									}
								}
							break;
							
							default:
							break;
						}
						wrefresh(arr[idx]);
						refresh();
						break;
					}
				}
			}
		}
	}
}

void UI_admin_add_nuid(uint32_t nuid)
{
	char ch;
	
	while ((ch = getch() != 'q'))
	{
		if ( ch == 1 || ch == KEY_MOUSE ) 
		{
			if(getmouse(&event) == OK)
			{

				if(wenclose(checkout, event.y , event.x))
				{
					wclear(checkout);
					wrefresh(checkout);
					if (DB_add_user(nuid) == 0U) // Adding successfull
					{
						wprint_center(checkout, 7, "RFID added successfull.");
					}
					else
					{
						wprint_center(checkout, 7, "RFID could not be added.");
					}
					wrefresh(checkout);
					refresh();
					break;
				}
			}
		}
	}
}

void UI_admin_del_nuid(uint32_t nuid)
{
	char ch;
	
	while ((ch = getch() != 'q'))
	{
		if ( ch == 1 || ch == KEY_MOUSE ) 
		{
			if(getmouse(&event) == OK)
			{

				if(wenclose(checkout, event.y , event.x))
				{
					wclear(checkout);
					wrefresh(checkout);
					if (DB_delete_user(nuid) == 0U) // Adding successfull
					{
						wprint_center(checkout, 11, "RFID deleted successfull.");
					}
					else
					{
						wprint_center(checkout, 7, "RFID could not be deleted.");
					}
					wrefresh(checkout);
					refresh();
					break;
				}
			}
		}
	}	
}

void UI_create_checkout()
{
	checkout = newwin(20, 45, 4, 23);
	box(checkout, 0,0);
	wprint_center(checkout, 1, "< SCAN RESULTS >" );
	wrefresh(checkout);
	refresh();
}

void UI_create_back_button()
{
	backButton = newwin(3, 8, 21 ,5);
	box(backButton, 0,0);
	wprint_center_box(backButton, "BACK");
	wrefresh(backButton);
	refresh();
}

void UI_clear_back_button()
{
	wclear(backButton);
	wrefresh(backButton);
	delwin(backButton);
	clear();
	refresh();
}
