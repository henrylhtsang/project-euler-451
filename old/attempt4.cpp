#include <iostream>
#include <vector>
#include <numeric>
#include <climits>

using namespace std;

vector<int> primetable(int n);
// int carmichaelpk(int p, int r);
// int eulerpk(int p, int r);
// int modexp(long base, int exponent, long modulus);
// vector<int> carmichael(int n, vector<int> &primes);
// vector<int> euler(int n, vector<int> &primes);
int findprimefactor(int n, vector<int> &primes);
int findprimepower(int n, int p);
// int bruteforce(int n);
int xGCD(int a, int b, int &x, int &y);
// int reversechineseremainder(int a, int b, int u, int v);
int findmax(vector<int> & nums, int bound);
vector<int> prodvector(int a, int b, vector<int> & vec1, vector<int> & vec2);
vector<int> modinvcandidates(int n, vector<int> &primes);
// int reversechineseremaindercoeffs(int a, int b, int &x, int &y);

int main(){
  int n = 100000;
  vector<int> primes = primetable(n);

  vector<int> modinv (n+1, 0);

  // prime powers
  for (int i = 1; i<primes.size(); i++){
    int pow = 1;
    long pk = primes[i];
    while (pk <= n){
      modinv[pk] = 1;
      pk=pk*primes[i];
    }
  }

  // 2^k1 times p1^k2
  long p1 = 2;
  while (p1 <= n){
    vector<int> vec1 = modinvcandidates(p1, primes);
    for (int i = 1; i<primes.size(); i++){
      long p2 = primes[i];
      while (p1*p2 <= n){
        vector<int> vec2;
        vec2.push_back(1);
        vec2.push_back(p2-1);
        vector<int> vec3 = prodvector(p1, p2, vec1, vec2);
        modinv[p1*p2] = findmax(vec3, p1*p2-1);
        p2 = p2* primes[i];
      }
    }
    p1 = p1*2;
  }

  // p1^k1 times p2^k2
  for (int i = 1; i<primes.size(); i++){
    long p1 = primes[i];
    while (p1<=n){
      vector<int> vec1;
      vec1.push_back(1);
      vec1.push_back(p1-1);
      for (int j = i+1; j<primes.size(); j++){
        long p2 = primes[j];
        while (p1*p2 <= n){
          vector<int> vec2;
          vec2.push_back(1);
          vec2.push_back(p2-1);
          vector<int> vec3 = prodvector(p1, p2, vec1, vec2);
          modinv[p1*p2] = findmax(vec3, p1*p2-1);
          p2 = p2* primes[j];
        }
      }
      p1 = p1*primes[i];
    }
  }

  for (int i = 3; i<=n; i++){
    if (modinv[i] == 0){
      vector<int> vec1 = modinvcandidates(i, primes);
      modinv[i] = findmax(vec1, i-1);
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

vector<int> modinvcandidates(int n, vector<int> &primes){
  if (n==1){
    vector<int> vec1 (1, 1);
    return vec1;
  }

  int p = findprimefactor(n, primes);
  int pk = findprimepower(n, p);

  vector<int> vec1;
  if (p==2){
    for (int i=1; i<pk; i=i+2){
      if (i*i % pk == 1){
        vec1.push_back(i);
      }
    }
  }
  else{
    vec1.push_back(1);
    vec1.push_back(pk-1);
  }


  if (n != pk){
    vector<int> vec2 = modinvcandidates(n/pk, primes);
    vector<int> vec3 = prodvector(pk, n/pk, vec1, vec2);
    return vec3;
  }
  return vec1;
}


vector<int> prodvector(int a, int b, vector<int> & vec1, vector<int> & vec2){
  vector<int> vec3 (vec1.size() * vec2.size(), 0);
  int pos = 0;
  int x, y;
  xGCD(a, b, x, y);
  for (int i = 0; i < vec1.size(); i++){
    for (int j = 0; j < vec2.size(); j++){
      int temp = (b * y * vec1[i] + a * x * vec2[j] );
      while (temp < 0){
        temp+=(a*b);
      }
      vec3[pos] = temp % (a*b);
      pos++;
    }
  }
  return vec3;
}

int findmax(vector<int> & nums, int bound){
  int ans = INT_MIN;
  for (int i=0; i<nums.size(); i++){
    if (nums[i] < bound){
      ans = max(ans, nums[i]);
    }
  }

  return ans;
}


// int reversechineseremaindercoeffs(int a, int b, int &x, int &y){
//   xGCD(a, b, x, y);
//   // now we know xa + by = 1
//   return 0;
// }

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
