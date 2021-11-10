#include <iostream>
#include <vector>
#include <numeric>
#include <climits>

using namespace std;

vector<int> primetable(int n);
int findprimefactor(int n, vector<int> &primes);
int findprimepower(int n, int p);
int xGCD(int a, int b, long &x, long &y);
int findmax(vector<int> & nums, int bound);
vector<int> prodvector(int a, int b, vector<int> & vec1, vector<int> & vec2);
vector<int> modinvcandidates(int n, vector<int> &primes);
int carmichaelpk(int p, int r);
int eulerpk(int p, int r);
vector<int> carmichael(int n, vector<int> &primes);
vector<int> euler(int n, vector<int> &primes);

int main(){
  int n = 20000000;
  vector<int> primes = primetable(n); // create prime table

  vector<int> carmichaelnum  = carmichael(n, primes);
  vector<int> eulernum = euler(n, primes);

  vector<int> modinv (n+1, 0);


  for (int i = 3; i<=n; i++){
    if (carmichaelnum[i] == eulernum[i]){ // we ignore those cases
      // when Carmichael function and the Euler function agree
      // see the wiki article for Carmichael function for more details
      modinv[i] = 1;
    }
    else {
      vector<int> vec1 = modinvcandidates(i, primes);
      modinv[i] = findmax(vec1, i-1);
    }
    if (i % (n/100) == 0){ // progress bar
      cout << i << " Done" << endl;
    }
  }

  long ans = 0;
  for (int i = 3; i<=n; i++){
    ans += modinv[i];
    // cout << i << " " << modinv[i] << endl;
  }
  cout << ans << endl;

  return 0;
}

vector<int> euler(int n, vector<int> &primes){
  // build a table for euler function
  vector<int> eulernum (n+1, 0);
  for (int i = 0; i<primes.size(); i++){
    long pk = primes[i]; // this number is the prime power p^k for some k
    int k = 1; // exponent of the prime power
    while (pk <= n){
      eulernum[pk] = eulerpk(primes[i], k);
      k++;
      if (INT_MAX / primes[i] < pk)
        break;
      pk = pk * primes[i];
    }
  }

  for (int i = 3; i<=n; i++){
    if (eulernum[i] == 0){
      int p = findprimefactor(i, primes);
      int k = findprimepower(i, p);
      eulernum[i] = eulernum[k] * eulernum[i/k]; // it is a multiplicative function
    }
  }

  return eulernum;
}

vector<int> carmichael(int n, vector<int> &primes){
  // build a table for carmichael function
  vector<int> carmichaelnum (n+1, 0);
  for (int i = 0; i<primes.size(); i++){
    long pk = primes[i]; // this number is the prime power p^k for some k
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
      carmichaelnum[i] = lcm(carmichaelnum[k], carmichaelnum[i/k]); // see wiki for its definition
    }
  }

  return carmichaelnum;
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

vector<int> modinvcandidates(int n, vector<int> &primes){
  // list the modular inverses given n
  // for n=p^k, the structure is simple
  // see https://en.wikipedia.org/wiki/Multiplicative_group_of_integers_modulo_n#Structure
  // otherwise we need to use chinese remainder theorem
  // to break it down to a direct product of groups of prime power orders
  if (n==1){
    vector<int> vec1 (1, 1);
    return vec1;
  }

  int p = findprimefactor(n, primes);
  int pk = findprimepower(n, p);

  vector<int> vec1;
  if (pk == 2){
    vec1.push_back(1);
  }
  else if (pk==4){
    vec1.push_back(1);
    vec1.push_back(pk-1);
  }
  else if (p==2){
    vec1.push_back(1);
    vec1.push_back(pk/2-1);
    vec1.push_back(pk/2+1);
    vec1.push_back(pk-1);
  }
  else{
    vec1.push_back(1);
    vec1.push_back(pk-1);
  }

  if (n==pk){ // the case when n is a prime power
    return vec1;
  }

  vector<int> vec2 = modinvcandidates(n/pk, primes); // create the list of mod inv for n/pk
  vector<int> vec3 = prodvector(pk, n/pk, vec1, vec2); // merge them together
  return vec3;
}


vector<int> prodvector(int a, int b, vector<int> & vec1, vector<int> & vec2){
  // we are given:
  // vec1, a list of entries in Z_a
  // vec2, a list of entries in Z_b
  // we want to compute the value in Z_ab corresponding to (vec1[i], vec2[j])
  // this is guaranteed to exist by the chinese remainder theorem
  // in particular, we use the extended euclidean algorithm to find x and y
  vector<int> vec3 (vec1.size() * vec2.size(), 0);
  int pos = 0;
  long x, y;
  xGCD(a, b, x, y);
  for (int i = 0; i < vec1.size(); i++){
    for (int j = 0; j < vec2.size(); j++){
      long temp = ((b * y * vec1[i] + a * x * vec2[j]) % (a*b) + (a*b) ) % (a*b);
      vec3[pos] = temp;
      pos++;
    }
  }
  return vec3;
}

int findmax(vector<int> & nums, int bound){
  // given a vector nums, find the maximum entry
  int ans = INT_MIN;
  for (int i=0; i<nums.size(); i++){
    if (nums[i] < bound){
      ans = max(ans, nums[i]);
    }
  }

  return ans;
}


int xGCD(int a, int b, long &x, long &y) {
  // extended euclidean algorithm, copied from the internet
    if(b == 0) {
       x = 1;
       y = 0;
       return a;
    }

    long x1, y1, gcd = xGCD(b, a % b, x1, y1);
    x = y1;
    y = (x1 - (a / b) * y1 );
    return gcd;
}

int findprimepower(int n, int p){
  // given n and p, find p^k dividing n such that k is maximal
  int k = p;
  n = n/p;
  while (n % p ==0){
    n=n/p;
    k=k*p;
  }
  return k;
}

int findprimefactor(int n, vector<int> &primes){
  // given n, find a prime p dividing n
  if (n == 1){
    return -1;
  }
  for (int i=0; i<primes.size(); i++)
    if (n % primes[i] == 0)
      return primes[i];
  return -1;
}

vector<int> primetable(int n){
  // output a vector of primes less than or equal to n
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
