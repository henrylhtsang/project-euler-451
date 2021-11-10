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
  int n = 2000;
  vector<int> primes = primetable(n);

  vector<int> modinv (n+1, 0);

  for (int i=3; i<=n; i++){
    int temp = bruteforce(i);
    cout << i << " " << temp << endl;
    int k = i;
    while (k != 1){
      int p = findprimefactor(k, primes);
      int pk = findprimepower(k, p);
      k = k / pk;
      if (p != pk)
        if (p != 2)
          if (temp % pk != pk - 1 && temp % pk != 1)
            cout << "prime: " << p << " " << pk << " " << temp % pk << endl;
    }
  }
  return 0;
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
