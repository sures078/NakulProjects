#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "batt.h"

// Uses the two global variables (ports) BATT_VOLTAGE_PORT and
// BATT_STATUS_PORT to set the fields of the parameter 'batt'.  If
// BATT_VOLTAGE_PORT is negative, then battery has been wired wrong;
// no fields of 'batt' are changed and 1 is returned to indicate an
// error.  Otherwise, sets fields of batt based on reading the voltage
// value and converting to precent using the provided formula. Returns
// 0 on a successful execution with no errors. This function DOES NOT
// modify any global variables but may access global variables.
//
// CONSTRAINT: Uses only integer operations. No floating point
// operations are used as the target machine does not have a FPU.
//
// CONSTRAINT: Limit the complexity of code as much as possible. Do
// not use deeply nested conditional structures. Seek to make the code
// as short, and simple as possible. Code longer than 40 lines may be
// penalized for complexity.

int set_batt_from_ports(batt_t *batt) {
  if (BATT_VOLTAGE_PORT < 0) //out of bounds
    return 1;

  batt->volts = BATT_VOLTAGE_PORT; //VOLTS

  //PERCENT CALCULATION
  if ((batt->volts - 3000)/8 > 100)
    batt->percent = 100;
  else if ((batt->volts - 3000)/8 < 0)
    batt->percent = 0;
  else
    batt->percent = (batt->volts - 3000)/8;

  //MODE
  if (BATT_STATUS_PORT & 0x1)
    batt->mode = 1;
  else
    batt->mode = 0;

  return 0;
}

// Alters the bits of integer pointed to by display to reflect the
// data in struct param 'batt'. Selects either to show Volts (mode=0)
// or Percent (mode=1). If Volts are displayed, only displays 3 digits
// rounding the lowest digit up or down appropriate to the last digit.
// Calculates each digit to display changes bits at 'display' to show
// the volts/percent according to the pattern for each digit. Modifies
// additional bits to show a decimal place for volts and a 'V' or '%'
// indicator appropriate to the mode. In both modes, places bars in
// the level display as indicated by percentage cutoffs in provided
// diagrams. This function DOES NOT modify any global variables but
// may access global variables. Always returns 0.
//
// CONSTRAINT: Limit the complexity of code as much as possible. Do
// not use deeply nested conditional structures. Seek to make the code
// as short, and simple as possible. Code longer than 85 lines may be
// penalized for complexity.
int set_display_from_batt(batt_t batt, int *display) {
  *display = 0; //resets display each time
  //BATTERY LEVELS (most significant portion)
  if (batt.percent >= 5)
    *display = 0b1 << 1; //1 shifts left 1 spot

  if (batt.percent >= 30)
    *display = (*display + 0b1) << 1; //add 1 and shift left 1
  else
    *display = *display << 1; //or just shift left 1 (same pattern followed)

  if (batt.percent >= 50)
    *display = (*display + 0b1) << 1;
  else
    *display = *display << 1;

  if (batt.percent >= 70)
    *display = (*display + 0b1) << 1;
  else
    *display = *display << 1;

  if (batt.percent >= 90)
    *display = (*display + 0b1) << 1;
  else
    *display = *display << 1;

  if (batt.mode == 1) { //PERCENT, VOLTS AND DECIMAL
    *display = (*display + 0b1) << 1;
    *display = *display << 1;
  }
  else {
    *display = *display << 1;
    *display = (*display + 0b1) << 1;
    *display = (*display + 0b1);
  }

  int numbers[11]; //ARRAY OF BIT MASKS REPRESENTING 0-9
  numbers[0] = 0b0111111;
  numbers[1] = 0b0000011;
  numbers[2] = 0b1101101;
  numbers[3] = 0b1100111;
  numbers[4] = 0b1010011;
  numbers[5] = 0b1110110;
  numbers[6] = 0b1111110;
  numbers[7] = 0b0100011;
  numbers[8] = 0b1111111;
  numbers[9] = 0b1110111;
  numbers[10] = 0b0000000; //NOTHING TO BE DISPLAYED

  int temp, firstNum, secondNum, thirdNum;

  if (batt.mode == 0) { //VOLTS
    firstNum = batt.volts/1000;
    temp = batt.volts % 1000;

    secondNum = temp/100;
    temp %= 100;

    temp += 5;
    thirdNum = temp/10;
  }

  temp = batt.percent/10; //PERCENT
  if (batt.mode == 1 && temp == 10) { //100%
    firstNum = 1;
    secondNum = 0;
    thirdNum = 0;
  }
  else if (batt.mode == 1 && temp == 0) { //0-9%
    firstNum = 10;
    secondNum = 10;
    thirdNum = batt.percent;
  }
  else if (batt.mode == 1){ //10-99%
    firstNum = 10;
    secondNum = temp;
    thirdNum = batt.percent % 10;
  }

  int threeNums[3] = {firstNum, secondNum, thirdNum};

  for (int i = 0; i < 3; i++) { //APPENDING BIT REPRESENTATIONS OF THE 3 NUMBERS
    *display = *display << 7; //shift left 7
    *display = *display + numbers[threeNums[i]]; //add number
  }
return 0;
}


// Called to update the battery meter display.  Makes use of
// set_batt_from_ports() and set_display_from_batt() to access battery
// voltage sensor then set the display. Checks these functions and if
// they indicate an error, does NOT change the display.  If functions
// succeed, modifies BATT_DISPLAY_PORT to show current battery level.
//
// CONSTRAINT: Does not allocate any heap memory as malloc() is NOT
// available on the target microcontroller.  Uses stack and global
// memory only.
int batt_update() {
  batt_t batt;
  int set = set_batt_from_ports(&batt); //using set function
  if (set == 1) //error checking
    return 1;
  set_display_from_batt(batt, &BATT_DISPLAY_PORT); //using display function and global variable for display
  return 0;
}
