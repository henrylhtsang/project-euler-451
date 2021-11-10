#include <iostream>
#include <vector>
#include <numeric>
#include <climits>

using namespace std;

vector<int> primetable(int n);
int carmichaelpk(int p, int r);
int eulerpk(int p, int r);
int modexp(long base, int exponent, long modulus);
vector<int> carmichael(int n, vector<int> &primes);
vector<int> euler(int n, vector<int> &primes);
int findprimefactor(int n, vector<int> &primes);
int findprimepower(int n, int p);
int bruteforce(int n);
int xGCD(int a, int b, int &x, int &y);
int reversechineseremainder(int a, int b, int u, int v);


int main(){
  int n = 100;
  vector<int> primes = primetable(n);

  vector<int> modinv (n+1, 0);

  // handle 2's powers
  int k = 1; // exponent
  int pk = 2; //p^k
  while (pk <= n){
    modinv[pk] = bruteforce(pk);
    pk = pk*2;
  }

  // handle prime power
  for (int i = 1; i<primes.size(); i++){
    int k = 1; // exponent
    long pk = primes[i]; //p^k
    while (pk <= n){
      modinv[pk] = 1;
      pk = pk*primes[i];
    }
  }


  for (int i = 3; i<=n; i++){
    if (modinv[i] == 0){
      int p = findprimefactor(i, primes);
      int pk = findprimepower(i, p);

      // see which of the following is bigger
      // (pk-1, modinv[i/pk])
      // (modinv[pk], i/pk-1)

      if (p == 2 && pk != 4){
        if (pk == 2)
          modinv[i] = reversechineseremainder(2, i/2, 1, modinv[i/pk]);
        else
          modinv[i] = bruteforce(i);
      }
      else{
        int a = reversechineseremainder(pk, i/pk, pk-1, modinv[i/pk]);
        int b = reversechineseremainder(pk, i/pk, modinv[pk], i/pk - 1);
        int c =0, d=0;
        if (modinv[i/pk] != 1)
          int c = reversechineseremainder(pk, i/pk, pk-1, i/pk - modinv[i/pk]);
        if (modinv[pk] != 1)
          int d = reversechineseremainder(pk, i/pk, pk - modinv[pk], i/pk - 1);

        modinv[i] = max(max(max(a, b), c), d);
      }
    }
    cout << i << " " << modinv[i] << endl;
  }

  long ans = 0;
  for (int i = 3; i<=n; i++){
    ans += modinv[i];
  }

  cout << ans << endl;

  return 0;
}

int reversechineseremainder(int a, int b, int u, int v){
  // send (u, v) mod (Z_a, Z_b) to a number in Z_ab
  // assuming a and b are coprime
  int x, y;
  xGCD(a, b, x, y);
  // now we know xa + by = 1
  int ans = (b * y * u + a * x * v );
  while (ans < 0)
    ans+=(a*b);
  return ans % (a*b);
}

int xGCD(int a, int b, int &x, int &y) {
    if(b == 0) {
       x = 1;
       y = 0;
       return a;
    }

    int x1, y1, gcd = xGCD(b, a % b, x1, y1);
    x = y1;
    y = (x1 - (a / b) * y1 );
    return gcd;
}


int bruteforce(int n){
  for (long j = n-2; j>=1; j--){
    if (j * j % n == 1){
      return j;
    }
  }
  return 1;
}

int findprimepower(int n, int p){
  int k=p;
  n=n/p;
  while (n % p ==0){
    n=n/p;
    k=k*p;
  }
  return k;
}

int findprimefactor(int n, vector<int> &primes){
  for (int i=0; i<primes.size(); i++)
    if (n % primes[i] == 0)
      return primes[i];
  return -1;
}

// vector<int> euler(int n, vector<int> &primes){ // compute the euler function
//   vector<int> eulernum (n+1, 0);
//   for (int i = 0; i<primes.size(); i++){
//     int pk = primes[i]; // this number is the prime power
//     int k = 1; // exponent of the prime power
//     while (pk <= n){
//       eulernum[pk] = eulerpk(primes[i], k);
//       k++;
//       if (INT_MAX / primes[i] < pk)
//         break;
//       pk = pk * primes[i]; // just standard overflow
//     }
//   }
//
//   for (int i = 3; i<=n; i++){
//     if (eulernum[i] == 0){
//       int p = findprimefactor(i, primes);
//       int k = findprimepower(i, p);
//       eulernum[i] = eulernum[k] * eulernum[i/k];
//     }
//   }
//
//   return eulernum;
// }

// vector<int> carmichael(int n, vector<int> &primes){
//   vector<int> carmichaelnum (n+1, 0);
//   for (int i = 0; i<primes.size(); i++){
//     int pk = primes[i]; // this number is the prime power
//     int k = 1; // exponent of the prime power
//     while (pk <= n){
//       carmichaelnum[pk] = carmichaelpk(primes[i], k);
//       k++;
//       if (INT_MAX / primes[i] < pk)
//         break;
//       pk = pk * primes[i];
//     }
//   }
//
//   for (int i = 3; i<=n; i++){
//     if (carmichaelnum[i] == 0){
//       int p = findprimefactor(i, primes);
//       int k = findprimepower(i, p);
//       carmichaelnum[i] = lcm(carmichaelnum[k], carmichaelnum[i/k]);
//     }
//   }
//
//   return carmichaelnum;
// }





// int modexp(long base, int exponent, long modulus){ // compute base^exponent mod modulus
//   // I copy this from wikipedia
//   if (modulus == 1)
//     return 0;
//   long result = 1;
//   base = base % modulus;
//   while (exponent > 0){
//     if (exponent % 2 ==1)
//       result = result * base % modulus;
//     exponent = exponent >> 1;
//     base = base* base % modulus;
//   }
//
//   return (int) result;
// }

// int carmichaelpk(int p, int r){ // carmichael function for prime powers
//   if (p ==2 && r>=3)
//     return eulerpk(p, r)/2;
//   else
//     return eulerpk(p, r);
// }
//
// int eulerpk(int p, int r){ // euler function for prime powers
//   int ans = 1;
//   for (int i=1; i<r; i++)
//     ans = ans*p;
//   return ans*(p-1);
// }


vector<int> primetable(int n){ // output a vector of primes less than or equal to n
  vector<int> nums (n+1, 1);
  vector<int> primes;

  for (int i=2; i<=n; i++){ // starting from the first prime
    if (nums[i] == 1){
      primes.push_back(i);
      for (int j = 2; j <= n/i; j++){ // eliminate the multiples
        nums[i*j] = 0;
      }
    }
  }

  return primes;
}
