/*=============================================================================
 * N vs M quest success calculator
 * (Created for Honest Dan's Adventure: https://www.twitch.tv/honestdangames )
 *
 * Compilation: This is written in C and can be compiled with gcc.
 *    gcc nvm.c -o NvM.exe
 *
 * Usage: Running the program requires two parameters for N and M respectively
 *    ./NvM.exe <N> <M>
 *
 * Author: Otay
 * (8/13/2018)
 *===========================================================================*/
 
/**
 * Problem description:
 *   A quest consists of N players fighting M monsters.  Each player has 3 
 *   health, and each monster has 3 health.  The chance for a player to remove
 *   one health from a monster is 60%, and otherwise they lose one health.  
 *   This calculator will find the percent chance that N players succeed 
 *   agaisnt M monsters.
 *
 * The 1 vs 1 case example:
 * 
 *   We sum the chance of several independent possibilities
 *   1) The player strikes 3 times in a row
 *   2) The player strikes 3 times, and the enemy strikes back once
 *   3) The player strikes 3 times, and the enemy strikes back twice
 *   
 *   There are ten ways these 3 possibilities may occur.
 *   Let W denote a successful strike, and F denote a monster strike.
 *   
 *   1) W W W
 * 
 *   2) W W F W
 *      F W W W
 *      W F W W
 * 
 *   3) W W F F W
 *      W F W F W
 *      W F F W W
 *      F W W F W
 *      F W F W W
 *      F F W W W 
 * 
 *   We use the number of occurences as coefficients.  
 *   Therefore the equation is given by,
 *   
 *   (.6 ^ 3) + 3(0.6 ^ 3 * 0.4) + 6(0.6 ^ 2 * 0.4 ^ 2)
 * 
**/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Set to true for verbose debugging
#define DEBUG 0

// The chance for a strike to succeed in combat
#define WIN_CHANCE     0.6
#define LOSS_CHANCE    1 - WIN_CHANCE

// Total health for each entire team
#define ENEMY_HEALTH   m*3
#define PLAYER_HEALTH  n*3

long long unsigned int nCk(int n, int k);

int main(int argc, const char ** argv[]) {
  // Check validity of command line arguments
  if (argc != 3) {
    fprintf(stderr, "Two integer arguments are required for N vs M\n");
    exit(-1);
  }
  
  // Obtain player and monster count from command line arguments
  int n, m;
  n = atoi((char *)argv[1]);
  m = atoi((char *)argv[2]);
  if (n == 0 || m == 0) {
    fprintf(stderr, "N and M values must be positive integers\n");
    exit(-1);
  }
  
  // Calculate the chance of success
  double p = 0.f;
  for (int i = 0; i < PLAYER_HEALTH; i++) {
    long long unsigned int coef = nCk(ENEMY_HEALTH - 1 + i, ENEMY_HEALTH - 1);
    double chance = pow(WIN_CHANCE, ENEMY_HEALTH) * pow(LOSS_CHANCE, i);
    p += coef * chance;
    
    // Coefficients may overflow when N and M are greater than 10
    // You can observe this by setting DEBUG to 1
    if (DEBUG) printf("coef %llu\n", coef);
  }
  
  // Calculate the chance of failure
  double q = 0.f;
  for (int i = 0; i < ENEMY_HEALTH; i++) {
    long long unsigned int coef = nCk(PLAYER_HEALTH - 1 + i, PLAYER_HEALTH - 1);
    double chance = pow(LOSS_CHANCE, PLAYER_HEALTH) * pow(WIN_CHANCE, i);
    q += coef * chance;
    
    // Coefficients may overflow when N and M are greater than 10
    // You can observe this by setting DEBUG to 1
    if (DEBUG) printf("coef %llu\n", coef);
  }
  
  printf("Chance of success: %G%%\n", p*100);
  printf("Chance of failure: %G%%\n\n", q*100);
  printf("rating of uncertainty: %G%%\n\n", (1-(p+q))*100);
  
  return p;
}

// recursive n choose k formula
long long unsigned int nCk(int n, int k) {
  if (k == 0) return 1;
  return (n * nCk(n - 1, k - 1)) / k;
}










