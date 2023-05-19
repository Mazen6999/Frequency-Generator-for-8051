#include <reg51.h>

// Define the segments of the 7-segment display
sbit SEG_A = P2^0;
sbit SEG_B = P2^1;
sbit SEG_C = P2^2;
sbit SEG_D = P2^3;
sbit SEG_E = P2^4;
sbit SEG_F = P2^5;
sbit SEG_G = P2^6;
sbit SEG_DP = P2^7;


// Define the pins for the common cathodes of the 4 digits
sbit DIGIT1 = P1^0;
sbit DIGIT2 = P1^1;
sbit DIGIT3 = P1^2;
sbit DIGIT4 = P1^3;



/*
 * Title: Variable Frequency Generator
 * Author: Osama Mazhar
 * Date: 21st Jan 2012
 * Email: osamazhar@yahoo.com
 */
 
  
#include <reg51.h>

#define KEYPAD P3												
#define ldata P0 				//sfr ldata = 0x0B0; for P3

sbit rs = P3^5;
sbit rw = P3^6;
sbit en = P3^7;
sbit busy = P0^7;
sbit Clock = P3^0;
sbit Status = P3^1;




void MSDelay(unsigned int);
unsigned char KeypadRead();
unsigned int GetInput();
void TimerH(unsigned char, unsigned char);
void TimerL(unsigned char, unsigned char);

unsigned char code keypad[4][4] = {'7','8','9','/','4','5','6','X','1','2','3','-','.','0','=','+'};



void main()
	{
	 float Frequency;
	 unsigned char colloc;

	 Status = 0;
	 KEYPAD = 0xF0;			// make higher bits of keypad port that is column bits input and low row bits outputs
	 
	 
main:
	 Frequency = GetInput();
	 								
	 
	 Status = 1;
	 while(1)						//2 Machine Cycle
		{
		 
		 colloc = KEYPAD;				//2 MC		 
	 	 colloc &= 0xF0;				//2 MC		 
		 if(colloc == 0xE0)				//4 MC
		 	{
	 	 	 MSDelay(5);
		 	 colloc = KEYPAD;			//read the columns
		 	 colloc &= 0xF0;			//mask unused bits
			 if(colloc == 0xE0)
			 	{
				 Status = 0;
				 goto main;	 
				}
			}												
		}
	}							//main braces

unsigned int GetInput()
	{
	 unsigned char L, cnt, Key[5];
	 unsigned int Entry = 0;
	 cnt = 0;
	 do
		{
Re:		 Key[cnt] = KeypadRead();
		 L = Key[cnt];
		 if(L == '+' | L == '-')
		 	goto Re;
		 if(L == '=')
		 	continue;
		 Key[cnt] = Key[cnt] - 48;
		 cnt++;

		}
	 while(L != '=' & cnt != 5);
	 switch(cnt)
	 	{
		 case(1):
		 	{
			 Entry = Key[0];
			 break;
			}
		 case(2):
		 	{
			 Entry = (Key[0] * 10) + Key[1];
			 break;
			}
		 case(3):
		 	{
			 Entry = (Key[0] * 100) + (Key[1] * 10) + Key[2];
			 break;
			}
		 case(4):
		 	{
			 Entry = (Key[0] * 1000) + (Key[1] * 100) + (Key[2] * 10) + Key[3];
			 break;
			}
		 case(5):
		 	{
			 Entry = (Key[0] * 10000) + (Key[1] * 1000) + (Key[2] * 100) + (Key[3] * 10) + Key[4];
			 break;
			}
		}
	 return Entry;
	}

unsigned char KeypadRead()
	{
		unsigned char colloc, rowloc; 		
		do
		   {												
		   KEYPAD = 0xF0;				// ground all rows at once
		   colloc = KEYPAD;				// read the port for columns
		   colloc &= 0xF0;				// mask row bits
		   }
		while(colloc != 0xF0);				// check until all keys are released
		
		do
		   {
		  	 do
		   		{
				MSDelay(8);  			//call delay
				colloc = KEYPAD;		//see if any key is pressed
				colloc &= 0xF0;			// mask unsused bits
				}
		 	 while(colloc == 0xF0);			// keep checking for keypress

			MSDelay(8);				// call delay for debounce
			colloc = KEYPAD;			// read columns
			colloc &= 0xF0;				// mask unused bits
			}
			while(colloc == 0xF0);			// wait for keypress

			while(1)
				{
					KEYPAD &= 0xF0;		// masking row bits
					KEYPAD |= 0x0E;		// now ground row 0 0E = 00001110b ORing won't affect column data
					colloc = KEYPAD;	// read columns
					colloc &= 0xF0;		// mask row bits
					if(colloc != 0xF0)	// column detected
						{
						 rowloc = 0;	// save row location
						 break;
						}
					KEYPAD &= 0xF0;
					KEYPAD |= 0x0D;		// ground row 1 0D = 00001101b  ORing won't affect column data
					colloc = KEYPAD;	// read columns
					colloc &= 0xF0;		// mask row bits
					if(colloc != 0xF0)	// column detected
						{
						 rowloc = 1;	// save row location
						 break;
						}
					KEYPAD &= 0XF0;
					KEYPAD |= 0x0B;		// ground row 2 0B = 00001011b
					colloc = KEYPAD;	// read columns
					colloc &= 0xF0;		// mask row bits
					if(colloc != 0xF0)	// column detected
						{
						 rowloc = 2;	// save row location
						 break;
						}
					KEYPAD &= 0XF0;
					KEYPAD |= 0x07;		// ground row 3 07 = 00000111b
					colloc = KEYPAD;	// read columns
					colloc &= 0xF0;		// mask row bits
					if(colloc != 0xF0)	// column detected
					rowloc = 3;		// save row location
					break;
				}
		//check columns and send result to 
		if(colloc == 0xE0)				// 0E = 00001110
			return keypad[rowloc][0];
		else if(colloc == 0xD0)				// 0D = 00001101
			return keypad[rowloc][1];
		else if(colloc == 0xB0)				// 0B = 00001011
			return keypad[rowloc][2];
		else
			return keypad[rowloc][3];
		}
	
		
void MSDelay(unsigned int count) 
{// mSec Delay 12 Mhz 
     unsigned int i;		       		
     while(count)
		{
         i = 122; 
		 while(i>0)
		 	i--;
         count--;
    	}
}

