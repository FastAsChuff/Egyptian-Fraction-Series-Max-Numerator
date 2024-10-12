#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// gcc largestnumerator2.c -o largestnumerator2.bin -O3 -march=native -Wall 
#define LNUM_MAXN 30
typedef struct {
  __int128 num, denom;
} numdenom_t;

__int128 gcdint128(__int128 A, __int128 B) {
  __int128 temp;
  if ((A <= 0) || (B <= 0)) return 0;
  while (1) {
    if (A < B) {
      temp = A;
      A = B;
      B = temp;
    }
    A = A % B;
    if (A == 0) return B;
  }
}

numdenom_t clistnum(int8_t *clist, uint32_t clistlen) {
  numdenom_t res;
  res.denom = 1;
  if (clistlen == 0) {
    res.num = 0;
    return res;
  }
  res.num = clist[0];
  uint32_t i = 1;
  while (i < clistlen) {
    __int128 temp = res.denom;
    res.denom *= i+1;
    res.num = res.num*(i+1) + clist[i]*temp;
    i += 1;
  }  
  __int128 g = gcdint128(res.num, res.denom);
  if (g > 0) {
    res.num /= g;
    res.denom /= g;
  }
  return res;
}

__int128 clistnumub(int8_t *clist, uint32_t clistlen, uint32_t n) {
  numdenom_t numdenom = clistnum(clist, clistlen);
  uint32_t i = clistlen;
  while (i < n) {
    __int128 temp = numdenom.denom;
    numdenom.denom *= i+1;
    numdenom.num = numdenom.num*(i+1) + temp;
    i += 1;
  }
  return numdenom.num;
}
  

_Bool clistnext(int8_t *clist, uint32_t *clistlen, uint32_t n) {
  uint32_t m = *clistlen;
  while (m > 0) {
    if (clist[m-1] == 1) {
      m -= 1;
    } else {
      clist[m-1] += 1;
      *clistlen = m;
      return false;
    }
  }
  *clistlen = m;
  return true;
}
  
int main(int argc, char*argv[]) {
  printf("Finds the largest possible numerator after reduction of the series of the first n <= 30 'Egyptian' fractions c/i where c in {-1,0,1}.\nFor 1 < n <= 30, find max(numerator(sum_{i = 1 to n} c_i/i))\n");  
  uint32_t n = 2;
  int8_t clist[LNUM_MAXN];
  for (int32_t i=0; i<n; i++) clist[i] = -1;
  uint32_t clistlen = n;
  __int128 maxnum = 1;
  int8_t maxnumclist[LNUM_MAXN];
  while (n <= LNUM_MAXN) {
    _Bool completed = false;
    while (!completed) {
      if (clistnumub(clist, clistlen, n) > maxnum) {
        if (clistlen == n) {
          numdenom_t numdenom = clistnum(clist, clistlen);
          if (numdenom.num > maxnum) {
            maxnum = numdenom.num;            
            for (int32_t i=0; i<n; i++) maxnumclist[i] = clist[i];
          }
        }
        if (clistlen < n) {
          clist[clistlen] = -1;
          clistlen++;
        } else {
          completed = clistnext(clist, &clistlen, n);
        }
      } else {
        completed = clistnext(clist, &clistlen, n);
      }
    }
    printf("[");          
    for (int32_t i=0; i<n-1; i++) printf("%i, ", maxnumclist[i]);
    printf("%i] %li\n", maxnumclist[n-1], (int64_t)maxnum);
    if (n < LNUM_MAXN) {
      maxnumclist[n] = 1;
      numdenom_t numdenom = clistnum(maxnumclist, n+1);
      if (numdenom.num > maxnum) {
        maxnum = numdenom.num;
      } else {        
        maxnumclist[n] = 0;
      }
    }
    n += 1;
  }
}
