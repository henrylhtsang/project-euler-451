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


int main(){
  int n = 100;
  vector<int> primes = primetable(n);

  cout << "Prime table ok!" << endl;

  // set up euler and carmichael
  vector<int> carmichaelnum  = carmichael(n, primes);
  vector<int> eulernum = euler(n, primes);

  cout << "Euler and Carmichael functions ok!" << endl;

  // compute modular inverse for each number
  long long ans = 0;

  for (int i = 3; i<=n; i++){
    // cout << i << " " << carmichaelnum[i] << " " << eulernum[i] << endl;
    if (carmichaelnum[i] != eulernum[i]){
      int c = 0; // find first coprime that is not 2
      // note that first coprime is always a prime
      for (int j = 0; j<primes.size(); j++){ // skipping prime 2
        if (i % primes[j] != 0){
          c = primes[j];
          break;
        }
      }

      ans = ans + i - modexp((long) c, carmichaelnum[i]/2, (long) i); // see wiki: Carmichael function
      cout << i << " " << i - modexp((long) c, carmichaelnum[i]/2, (long) i) << endl;
    }
    else
      ans++;

  }

  cout << ans << endl;

  return 0;
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

vector<int> euler(int n, vector<int> &primes){ // compute the euler function
  vector<int> eulernum (n+1, 0);
  for (int i = 0; i<primes.size(); i++){
    int pk = primes[i]; // this number is the prime power
    int k = 1; // exponent of the prime power
    while (pk <= n){
      eulernum[pk] = eulerpk(primes[i], k);
      k++;
      if (INT_MAX / primes[i] < pk)
        break;
      pk = pk * primes[i]; // just standard overflow
    }
  }

  for (int i = 3; i<=n; i++){
    if (eulernum[i] == 0){
      int p = findprimefactor(i, primes);
      int k = findprimepower(i, p);
      eulernum[i] = eulernum[k] * eulernum[i/k];
    }
  }

  return eulernum;
}

vector<int> carmichael(int n, vector<int> &primes){
  vector<int> carmichaelnum (n+1, 0);
  for (int i = 0; i<primes.size(); i++){
    int pk = primes[i]; // this number is the prime power
    int k = 1; // exponent of the prime power
    while (pk <= n){
      carmichaelnum[pk] = carmichaelpk(primes[i], k);
      k++;
      if (INT_MAX / primes[i] < pk)
        break;
      pk = pk * primes[i];
    }
  }

  for (int i = 3; i<=n; i++){
    if (carmichaelnum[i] == 0){
      int p = findprimefactor(i, primes);
      int k = findprimepower(i, p);
      carmichaelnum[i] = lcm(carmichaelnum[k], carmichaelnum[i/k]);
    }
  }

  return carmichaelnum;
}





int modexp(long base, int exponent, long modulus){ // compute base^exponent mod modulus
  // I copy this from wikipedia
  if (modulus == 1)
    return 0;
  long result = 1;
  base = base % modulus;
  while (exponent > 0){
    if (exponent % 2 ==1)
      result = result * base % modulus;
    exponent = exponent >> 1;
    base = base* base % modulus;
  }

  return (int) result;
}

int carmichaelpk(int p, int r){ // carmichael function for prime powers
  if (p ==2 && r>=3)
    return eulerpk(p, r)/2;
  else
    return eulerpk(p, r);
}

int eulerpk(int p, int r){ // euler function for prime powers
  int ans = 1;
  for (int i=1; i<r; i++)
    ans = ans*p;
  return ans*(p-1);
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
