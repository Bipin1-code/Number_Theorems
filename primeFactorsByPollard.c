
//We are apply Pollard's Rho algorithm for to find the factors of a number.
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

uint64_t gcd(uint64_t a, uint64_t b){
  while(b != 0){
    uint64_t temp = b;
    b = a % b;
    a = temp;
  }
  return a;
}

//Function for modular multiplication( avoiding overflow)
uint64_t mulmod(uint64_t a, uint64_t b, uint64_t mod){
  uint64_t result = 0;
  a %= mod;
  while(b > 0){
    if(b % 2 == 1)
      result = (result + a) % mod;
    a = (a * 2) % mod;
    b /= 2;
  }
  return result;
}

//Function for modular exponentiation (Power mod)
uint64_t power_mode(uint64_t base, uint64_t exp, uint64_t mod){
  /*
    (a pow b) mod n = (a pow (b/2) mod n) pow 2 mod n
    Key observation:
    1. if b is even,
         a pow b = (a pow (b/2) * a pow (b/2)) mod n
    2. if b is odd,
         a pow b = (a pow (b-1) * a) mod n
         where a pow (b - 1) is even and can be computed recursively. 
  */
  uint64_t result = 1;
  base %= mod;
  while(exp > 0){
    if(exp % 2 == 1)
      result = mulmod(result, base, mod);
    base = mulmod(base, base, mod);
    exp /= 2;
  }
  return result;
}

//Pollard's Rho Function
uint64_t pollard_rho(uint64_t n){
  if(n % 2 == 0) return 2;
  uint64_t x = 2, y = 2, d = 1;
  uint64_t c = rand() % n;

  /*
     x = (mulmod(x, x, n) + c) % n;
     it means:
         x = (((x pow 2) mod n) + c) mod n
         where
         ->n is the original number being factorized.
         ->"mod n" keeps values bounded and detects cycles (*which keeps x within the range [0, n-1].)
    */

  while(d == 1){
    x = (mulmod(x, x, n) + c) % n;
    y = (mulmod(y, y, n) + c) % n;
    y = (mulmod(y, y, n) + c) % n;
    //updates y twice to move faster than x, implementing FLody's cycle-finding algorithm.
    d = gcd((x > y) ? (x - y) : (y - x), n);
  }
  return d;
}

//Millar-Rabin Primality test
/*
  Millar-Rabin update the Fermat's Little Theorem:
  Fermat's Little Theorem:
  If n is prime, for any 'a' such that: 1 <= a <= (n - 1)
   a pow (n-1) = 1 (mod n)
  but Carmichael number also passed this test.
  For to stop these imposter numbers Millar comes with the idea of find the power of 2
    n - 1 = (2 pow s) * d
    where: d is odd number
           s is the largest power of 2 which divides the (n - 1)
           and range becomes 2 <= a <= (n - 2)
           which is meaningful and skip the useless cases like 1 and n-1
 */
int isPrime(uint64_t n, int iterations){
  if(n < 2) return 0;
  if(n == 2 || n == 3) return 1;
  if(n % 2 == 0) return 0;

  uint64_t d = n - 1; 
  int s = 0;  //inital Power of two
  while(d % 2 == 0){
    d /= 2;
    s++;  //updating the power when d gets divided with 2
  }

  for(int i = 0; i < iterations; i++){
    uint64_t a = 2 + rand() % (n - 3);
    uint64_t x = power_mode(a, d, n);
    if(x == 1 || x == n - 1) continue;
    int is_composite = 1;
    for(int j = 0; j < s - 1; j++){
      x = mulmod(x, x, n);
      if(x == n - 1){
        is_composite = 0;
        break;
      }
    }
    if(is_composite) return 0;
  }
  return 1;
}

void findPrimeFactors(uint64_t n){
  if(n == 1) return;
  if(isPrime(n, 5)){
    printf("%llu ", n);
    return;
  }
  uint64_t factor = pollard_rho(n);
  findPrimeFactors(factor);
  findPrimeFactors(n / factor);
}

int main(){
  uint64_t num;
  printf("Enter a number:\n");
  fflush(stdout);
  scanf("%llu", &num);

  printf("Prime factors of %llu: ", num);
  findPrimeFactors(num);
  printf("\n");
  return 0;
}
