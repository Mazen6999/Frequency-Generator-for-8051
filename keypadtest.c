#include <reg51.h>

sbit row1 = P3^0;  // Define row pins
sbit row2 = P3^1;
sbit row3 = P3^2;
sbit row4 = P3^3;
sbit col1 = P3^4;  // Define column pins
sbit col2 = P3^5;
sbit col3 = P3^6;
sbit col4 = P3^7;

char get_key(void);  // Function prototype

void main() {
    char key = '\0';  // Initialize key variable
    while(1) {  // Infinite loop
        key = get_key();  // Get key value
        if (key != '\0') {  // If key is pressed
            // Do something with the key value
        }
    }
}

int get_key(void) {
    int key = '\0';  // Initialize key variable
    // Scan rows for a pressed key
	
	while(1){
		
		  
    row1 = 0; row2 = 1; row3 = 1; row4 = 1;
    if (col1 == 0){ key = 'c';break;}
    if (col2 == 0){ key = 0;break;}
    if (col3 == 0){ key = '=';break;}
    if (col4 == 0){ key = '+';break;}
    row1 = 1; row2 = 0; row3 = 1; row4 = 1;
    if (col1 == 0){ key = 1;break;}
    if (col2 == 0){ key = 2;break;}
    if (col3 == 0){ key = 3;break;}
    if (col4 == 0){ key = '-';break;}
    row1 = 1; row2 = 1; row3 = 0; row4 = 1;
    if (col1 == 0){ key = 4;break;}
    if (col2 == 0){ key = 5;break;}
    if (col3 == 0){ key = 6;break;}
    if (col4 == 0){ key = 'x';break;}
    row1 = 1; row2 = 1; row3 = 1; row4 = 0;
    if (col1 == 0){ key = 7;break;}
    if (col2 == 0){ key = 8;break;}
    if (col3 == 0){ key = 9;break;}
    if (col4 == 0){ key = '/';break;}
	
	
		
	
	}
		
    return key;  // Return key value
}
