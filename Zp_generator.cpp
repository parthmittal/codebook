#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

const int max_P = 1e9 + 55;
typedef long long ll;
int generator(int p);
int modexp(int a, int b, int p);
int is_prime(int x);

int main(int argc, char **argv)
{
	vector<int> powers = {18, 19, 20, 21, 22, 23};
	for (auto &ceil_log_2N : powers) {
		cout << "good for 2N <= " << (1 << ceil_log_2N) << ':' << endl;
		for (int k = 1; k <= 100; ++k) {
			int res = (k << ceil_log_2N) + 1;
			if (is_prime(res)) {
				printf("%d = %d * %d + 1: %d\n", res, k, 1 << ceil_log_2N, generator(res));
			}
		}
	}
}

int generator(int p)
{
	for (int i = 1; i < p; ++i) {
		int R = sqrt(p - 1);
		int ok = 1;
		for (int j = 2; j <= R; ++j) {
			if ((p - 1) % j == 0) {
				if (modexp(i, j, p) == 1 || modexp(i, p / j, p) == 1) {
					ok = 0;
					break;
				}
			}
		}
		if (ok) {
			return i;
		}
	}
	return -1;
}

int modexp(int a, int b, int p)
{
	ll ans = 1, res = a;
	while(b) {
		if (b % 2) {
			ans *= res;
			ans %= p;
		}
		res *= res;
		res %= p;
		b /= 2;
	}
	return ans;
}

int is_prime(int x)
{
	int R = sqrt(x);
	for (int i = 2; i <= R; ++i) {
		if (x % i == 0) {
			return 0;
		}
	}
	return 1;
}
