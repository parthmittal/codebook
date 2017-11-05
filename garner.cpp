//a: input array
//x: output array
//r: required, r[i][j] is (p[i]^-1)mod(p[j])
//p: array of moduli (must be pairwise coprime)
//k: length of arrays.
void garner(int *a, int *x, int *r[max_K], int *p, int k) {
	for (int i = 0; i < k; ++i) {
		x[i] = a[i];
		for (int j = 0; j < i; ++j) {
			x[i] = r[j][i] * (x[i] - x[j]);

			x[i] = x[i] % p[i];
			if (x[i] < 0)  x[i] += p[i];
		}
	}
}
