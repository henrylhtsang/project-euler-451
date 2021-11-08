#include <iostream>
#include <vector>

using namespace std;

vector<int> primetable(int n);

int main(){

  vector<int> primes=primetable((int) 1000);

  return 0;
}



vector<int> primetable(int n){ // output a vector of primes less than or equal to n
  int num[n+1];
  for (int i=2; i<=n; i++) // set all to 1
    num[i]=1;


  for (int i=2; i<=n; i++){ // starting from the first prime
    for (int j=2; j<=n/i; j++){ // eliminate all multiples
      num[i*j] = 0;
    }
  }

  vector<int> primes;

  for (int i=2; i<=n; i++){
    if (num[i] == 1){
      primes.push_back(i);
    }
  }

  return primes;
}
