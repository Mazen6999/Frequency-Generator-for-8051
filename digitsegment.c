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
sbit DIGIT1 = P0^0;
sbit DIGIT2 = P0^1;
sbit DIGIT3 = P0^2;
sbit DIGIT4 = P0^3;

sbit row1 = P1^3;  // Define row pins
sbit row2 = P1^0;
sbit row3 = P1^1;
sbit row4 = P1^2;
sbit col1 = P1^4;  // Define column pins
sbit col2 = P1^5;
sbit col3 = P1^6;
sbit col4 = P1^7;

// intterupt
sbit intr = P3^2;

//output pin
sbit FREQ_OUT = P3^7;

void delay_ms(unsigned int ms);
void segments_init();
int get_key(void);
void T1Delay(float time);
void setwave ();
void settimer();
void starttimer();
void resettimer();
void stoptimeINT();
void stoptimer();

// Define the characters to be displayed on the 7-segment display
unsigned char char_table[] = {
    0x40, // 0
    0x79, // 1
    0x24, // 2
    0x30, // 3
    0x19, // 4
    0x12, // 5
    0x02, // 6
    0x78, // 7
    0x00, // 8
    0x10  // 9
};

		int segconv[4];
		double freq;
		unsigned int temp;
		int seg_table[4]={0,0,0,0};
		char incseg=0;
		char hold=0;
		int key;


void main()
{
		
		
	// Initialize the pins for the segments and digits as output
    SEG_A = 0;SEG_B = 0;SEG_C = 0;SEG_D = 0;SEG_E = 0;SEG_F = 0;SEG_G = 0;SEG_DP = 0;
    DIGIT1 = 0;DIGIT2 = 0;DIGIT3 = 0;DIGIT4 = 0;FREQ_OUT = 0;
		EX0= 1 ;
		EX1= 1	; // enable EXT interrupts 0
		IT1= 1  ;        ; // activates on falling edge from 1 to 0
		EA= 1   ;       ; //enable global interrupts
		ET1=1   ;         ;//enable timer interrupts o
	
    P2 = char_table[0];
	
    while (1) {
			
			char i;
			float freq=0;
			// scan keypad
			
			
			
			
		getkey:	 key = 69;  // Initialize key variable
		// Scan rows for a pressed key
		segments_init();
		
    while(1){
		row1 = 0; row2 = 1; row3 = 1; row4 = 1;
    if (col2 == 0 && hold == 0){ key = 0;hold=1;delay_ms(300);break;}

    row1 = 1; row2 = 0; row3 = 1; row4 = 1;
    if (col1 == 0 && hold == 0){ key = 1;hold=1;delay_ms(300);break;}
    if (col2 == 0 && hold == 0){ key = 2;hold=1;delay_ms(300);break;}
    if (col3 == 0 && hold == 0){ key = 3;hold=1;delay_ms(300);break;}

    row1 = 1; row2 = 1; row3 = 0; row4 = 1;
    if (col1 == 0 && hold == 0){ key = 4;hold=1;delay_ms(300);break;}
    if (col2 == 0 && hold == 0){ key = 5;hold=1;delay_ms(300);break;}
    if (col3 == 0 && hold == 0){ key = 6;hold=1;delay_ms(300);break;}

    row1 = 1; row2 = 1; row3 = 1; row4 = 0;
    if (col1 == 0 && hold == 0){ key = 7;hold=1;delay_ms(300);break;}
    if (col2 == 0 && hold == 0){ key = 8;hold=1;delay_ms(300);break;}
    if (col3 == 0 && hold == 0){ key = 9;hold=1;delay_ms(300);break;}
		break;
		}
			
			
			// check keypad input
			if(key == 69){hold=0; goto getkey;}
			  
			// If key is pressed
			seg_table[0]=key;
			segconv[incseg]=seg_table[0];
			if(incseg>0){
			for(i=0;i<incseg;i++){seg_table[incseg-i]=segconv[i];}
			}
			incseg++;
				
	
		}
}

void setwave ()
{
			
			freq = seg_table[0]+seg_table[1]*10+seg_table[2]*100+seg_table[3]*1000;
			freq = freq*2;
		  freq=(1000000.0/(freq*1.085))-9;
		  freq=65536-freq;
		  temp=(int)freq;
			TL1=temp%256;
			TH1=temp/256;	
}
void settimer ()
{
	TMOD=0x10;
	TR1=1;
}
void stoptimer ()
{
	TR1=0;
	seg_table[0]=0;seg_table[1]=0;seg_table[2]=0;seg_table[3]=0;
	incseg=0;
}

// Function to generate frequency
void freqGEN  (void) interrupt 0
{	
		setwave();
		settimer();
	
}


void stoptimeINT  (void) interrupt 2
{	
		stoptimer();
		
}


void timer1_intr (void) interrupt 3
{
	
	TL1=temp%256;
	TH1=temp/256;	
	FREQ_OUT = ~ FREQ_OUT;
}




// Function to introduce a delay in milliseconds
void delay_ms(unsigned int ms)
{
    unsigned int i, j;
    for (i = 0; i < ms; i++) {
        for (j = 0; j < 123; j++) {
            // This loop introduces a delay of approximately 1ms
            // Adjust the value of j based on your clock frequency
        }
    }
}


// Fuction to write on 7-segment
void segments_init()
{
	char i;
	for (i = 0; i < 4; i++)
	{   
		switch (i) {
			
			case 0:
			// Turn on the first digit
			P2 = char_table[seg_table[3]];
			DIGIT1 = 1;
			DIGIT2 = 0;
			DIGIT3 = 0;
			DIGIT4 = 0;
			break;

			
			case 1:
			// Turn on the second digit
			P2 = char_table[seg_table[2]];
			DIGIT1 = 0;
			DIGIT2 = 1;
			DIGIT3 = 0;
			DIGIT4 = 0;
			break;		
			
		
			case 2:
			// Turn on the third digit
			P2 = char_table[seg_table[1]];
			DIGIT1 = 0;
			DIGIT2 = 0;
			DIGIT3 = 1;
			DIGIT4 = 0;
			break;	
			
			
			case 3:
			// Turn on the fourth digit
			P2 = char_table[seg_table[0]];
			DIGIT1 = 0;
			DIGIT2 = 0;
			DIGIT3 = 0;
			DIGIT4 = 1;
			break;
				}
		
		
		delay_ms(1);
	
	}
	
}