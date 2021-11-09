#include <iostream>
#include <vector>
#include <numeric>

using namespace std;

vector<int> primetable(int n);
int carmichaelpk(int p, int r);
int eulerpk(int p, int r);
int modexp(int base, int exponent, int modulus);
vector<int> carmichael(int n, vector<int> &primes);
vector<int> euler(int n, vector<int> &primes);


int main(){
  int n = 100000;
  vector<int> primes=primetable(n);

  // set up euler and carmichael
  vector<int> carmichaelnum  = carmichael(n, primes);
  cout << "ok" << endl;
  vector<int> eulernum = euler(n, primes);



  // compute modular inverse for each number

  int modinv[n+1]; // the largest modular inverse less than n-1

  for (int i = 3; i<=n; i++){
    modinv[i]=1;
    if (carmichaelnum[i] != eulernum[i]){
      int c = 0; // find first coprime that is not 2
      // note that first coprime is always a prime
      for (int j = 1; j<primes.size(); j++){ // skipping prime 2
        if (i % primes[j] == 0){
          c = primes[j];
          break;
        }
      }
      if (c==0)
        cout << "coprime is 0" << endl;

      modinv[i] = modexp(c, carmichaelnum[i]/2, i); // see wiki: Carmichael function
      modinv[i] = i-modinv[i];
    }
  }


  int ans = 0;
  for (int i = 3; i<=n; i++)
    ans += modinv[i];

  cout << ans << endl;
  return 0;
}

vector<int> euler(int n, vector<int> &primes){ // compute the euler function
  vector<int> eulernum (n+1, 0);
  int pk = 1; // this number is the prime power
  for (int i = 0; i<primes.size(); i++){
    pk = primes[i];
    int k = 1; // exponent of the prime power
    while (pk <= n){
      eulernum[pk] = eulerpk(primes[i], k);
      k++;
      pk = pk * primes[i];
    }
  }

  // hopefully now a double loop would do it

  for (int i = 3; i<n+1; i++){
    for (int j = i+1; j<=n/i; j++){
      if (eulernum[i*j] == 0)
        eulernum[i*j] = eulernum[i]*eulernum[j];
    }
  }

  return eulernum;
}

vector<int> carmichael(int n, vector<int> & primes){
  vector<int> carmichaelnum (n+1, 0);
  for (int i = 0; i<primes.size(); i++){
    int pk = primes[i]; // this number is the prime power
    int k = 1; // exponent of the prime power
    while (pk <= n){
      carmichaelnum[pk] = carmichaelpk(primes[i], k);
      k++;
      pk = pk * primes[i];
      cout << i << " " << pk << " " << n << endl;
    }

  }
  // hopefully now a double loop would do it

  cout << "ok" << endl;

  for (int i = 3; i<n; i++){
    for (int j = i+1; j<=n/i; j++){
      if (carmichaelnum[i*j] == 0)
        carmichaelnum[i*j] = lcm(carmichaelnum[i], carmichaelnum[j]);
    }
  }

  return carmichaelnum;
}





int modexp(int base, int exponent, int modulus){ // compute base^exponent mod modulus
  // I copy this from wikipedia
  if (modulus == 1)
    return 0;
  int result = 1;
  base = base % modulus;
  while (exponent > 0){
    if (exponent % 2 ==1)
      result = result * base % modulus;
    exponent = exponent >> 1;
    base = base* base % modulus;
  }

  return result;
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
  int num[n+1];
  for (int i=2; i<=n; i++) // set all to 1
    num[i]=1;

  vector<int> primes;

  for (int i=2; i<=n; i++){ // starting from the first prime
    if (num[i] == 1){
      primes.push_back(i);
      for (int j = 2; j <= n/i; j++){ // eliminate the multiples
        num[i*j] = 0;
      }
    }
  }

  return primes;
}
