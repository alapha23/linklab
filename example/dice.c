/* dice.c */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DICE_N_FACES 6

int main() {
  int dice;
  int input;

  // Get user input and validate it
  printf("Enter the dice number (1-6): ");
  scanf("%d", &input);
  if (input < 1 || input > 6) {
    printf("Wrong number.\n");
    return 0;
  }

  // get a random number betwen 1 - 6
  srand(time(NULL));
  dice = (rand() % DICE_N_FACES) + 1;

  // check your luck
  if (dice != input) {
    printf("\nYOU LOSE!\n\n");
  }
  else {
    printf("\nYOU WIN!\n\n");
  }
  printf("Dice:  %d\n", dice);
  printf("Input: %d\n", input);

  return 0;
}

// vim: tabstop=2 expandtab shiftwidth=2 softtabstop=2
