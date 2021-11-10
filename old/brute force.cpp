#include <iostream>
#include <vector>

using namespace std;

vector<int> primetable(int n);


int main(){
  int n = 100;
  vector<int> square (n+1, 0);

  for (int i=1; i<=n; i++)
    square[i] = i*i;

  long ans = 0;

  for (int i=3; i<=n; i++){
    for (int j = i-2; j>=1; j--){
      if (square[j] % i == 1){
        cout << i << " " << j << endl;
        ans = ans + j;
        break;
      }
    }
  }


  cout << ans << endl;
  return 0;
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
