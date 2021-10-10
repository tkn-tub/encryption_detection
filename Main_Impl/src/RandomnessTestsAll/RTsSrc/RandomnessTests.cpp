#include <stdio.h>
#include <math.h>
#include <string.h>
#include "externs.h"
#include "cephes.h"
#include "../include/tools.h"
#include "RandomnessTests.h"

void Frequency4(int n, unsigned char* array, double& p_value)
{
	int int_sum, i, j;
	double	f, s_obs,/* p_value,*/ sum, sqrt2 = 1.41421356237309504880;
	
	int LUT_HW_size = 16;
	signed char *LUT_HW = LUT_HW_16;
	
	
	const int Tsize = 64;
	uint64_t *pblock;
	uint64_t help;
	const unsigned int mask = (1 << LUT_HW_size) - 1;
	

	
	
	sum = 0.0;
	int_sum = 0;

	pblock = (uint64_t*)array;
	help = *pblock;
	i = 0;
	for ( ; i < n + 1 - Tsize; i += Tsize) {
		for (j = 0; j < Tsize / LUT_HW_size; j++){
			int_sum += LUT_HW[help & mask];
			help >>= LUT_HW_size;
		}
		help = *(++pblock);
	}

	for (; i < n; i++) {
		help = get_nth_block4(array, i);
		int_sum += help & 1;
	}

	sum = int_sum - (n - int_sum);
	s_obs = fabs(sum) / sqrt(n);
	f = s_obs / sqrt2;
	p_value = erfc(f);

	//printf("Pval: %lf sum %lf\n", p_value, sum);

}

void BlockFrequency4(int M, int n, unsigned char* array, double& p_value)
{
	int		N, blockSum, block, /* n_, */ i,j, bitend;
	double  /*p_value, */sum, pi, v, chi_squared;

	int LUT_HW_size = 16;
	signed char *LUT_HW = LUT_HW_16;
	const int Tsize = 64;
	uint64_t* pblock;
	uint64_t help;
	const unsigned int mask = (1 << LUT_HW_size) - 1;




	N = n / M; 		/* # OF SUBSTRING BLOCKS      */
	//n_ = N*M;
	sum = 0.0;
	


	for (block = 0; block < N; block++){
		i = block*M;
		bitend = (block+1)*M;
		blockSum = 0;

		while (i % 8 != 0 && i < bitend){
			help = get_nth_block4(array, i);
			blockSum += help & 1;
			i++;
		}

		pblock = (uint64_t*)(array+i/8);
		help = *pblock;
		for (; i < bitend + 1 - Tsize; i += Tsize) {
			for (j = 0; j < Tsize / LUT_HW_size; j++){
				blockSum += LUT_HW[help & mask];
				help >>= LUT_HW_size;
			}
			help = *(++pblock);
		}

		for (; i < bitend; i++) {
			help = get_nth_block4(array, i);
			blockSum += help & 1;
		}
		pi = (double)blockSum / (double)M;
		v = pi - 0.5;
		sum = sum + v*v;
	}
	chi_squared = 4.0 * M * sum;
	p_value = cephes_igamc(N / 2.0, chi_squared / 2.0);

	//printf("%lf ",sum);
        //return p_value;

}

void Runs4(int n, unsigned char* array, double& p_value)
{
	int		S=0, i, j;
	double	pi, erfc_arg/*, p_value*/;
	double V;

	//unsigned int NumberOfRuns = 0;
	
	int LUT_HW_size = 16;
	signed char *LUT_HW = LUT_HW_16;


	const int Tsize = 64;
	uint64_t *pblock;
	uint64_t help;
	const unsigned int mask = (1 << LUT_HW_size) - 1;

	S = 0;

	pblock = (uint64_t*)array;
	help = *pblock;
	
	for (i = 0; i < n + 1 - Tsize; i += Tsize) {
		for (j = 0; j < Tsize / LUT_HW_size; j++){
			S += LUT_HW[help & mask];
			help >>= LUT_HW_size;
		}
		help = *(++pblock);
	}

	for (; i < n; i++) {
		help = get_nth_block4(array, i);
		S += help & 1;
	}

	pi = (double)S / (double)n;

	if (fabs(pi - 0.5) > (2.0 / sqrt(n))) {
		p_value = 0.0;

	}
	else {

		V = 1;
		pblock = (uint64_t*)array;
		
		for (i = 0; i < n - Tsize - 1; i += Tsize)
		{
			help = *pblock;
			++pblock;

			help = help ^ (help >> 1) ^ (*pblock << (Tsize - 1));

			V += LUT_HW_16[help & 0xFFFF]; help >>= LUT_HW_size;
			V += LUT_HW_16[help & 0xFFFF]; help >>= LUT_HW_size;
			V += LUT_HW_16[help & 0xFFFF]; help >>= LUT_HW_size;
			V += LUT_HW_16[help & 0xFFFF]; 	
		}

		for (; i + 1 < n; i++)
		{
			if ((get_nth_block_effect(array, i ) & 1) != (get_nth_block_effect(array, i + 1) & 1))
				V++;
		}

		erfc_arg = fabs((double)V - 2.0 * n * pi * (1 - pi)) / (2.0 * pi * (1 - pi) * sqrt(2 * n));
		p_value = erfc(erfc_arg);
		//printf("Pval: %lf sum %lf", p_value, V);

	}

}
void
ApproximateEntropy4(int m, int n, unsigned char* array, double& p_value)
{
	int				i, k, len, mi;
	double			sum, numOfBlocks, ApEn[2], apen, chi_squared/*, p_value*/; //comment p_value since it is already a parapeter
	unsigned int	mask, help;
	int				*P;



	numOfBlocks = n;
	m++;
	mask = (1 << m) - 1;

	len = (1 << m);



	if ((P = (int*)calloc(len, sizeof(int))) == NULL) {

		return;
	}
	for (i = 0; i < len; i++)
		P[i] = 0;
	Histogram(0, P, m, n, array);// add array variable as parameter

	for (i = 1; i<m; i++) {
		k = get_nth_block4(array, n - m + i)&(mask >> i);
		//printf("%d ",k);
		k ^= (((unsigned int*)array)[0] << (m - i));
		//printf("%d ",k);
		k &= mask;
		//printf("%d ",k);
		P[k]++;
	}

	//DISPLAY FREQUENCY
	sum = 0.0;
	for (i = 0; i < len / 2; i++) {
		mi = Mirrored_int(i, m - 1);
		help = P[mi] + P[mi + len / 2];
		if (help > 0)
			sum += help*log(help / numOfBlocks);

		//printf("%i ",help);
	}


	sum /= numOfBlocks;
	ApEn[0] = sum;

	sum = 0.0;
	for (i = 0; i < len; i++) {
		mi = Mirrored_int(i, m);
		if (P[mi] > 0)
			sum += P[mi] * log(P[mi] / numOfBlocks);
		//printf("[%d: %d] ",i,P[Mirrored_int(i,m)]);

	}


	sum /= numOfBlocks;
	ApEn[1] = sum;



	//printf("\n");
	//printf("SUM: %lf \n\n",sum);

	//printf("%lf %lf",ApEn[0],ApEn[1]);
	apen = ApEn[0] - ApEn[1];
	chi_squared = 2.0*n*(log(2) - apen);
	p_value = cephes_igamc(pow(2, m - 2), chi_squared / 2.0);



	//printf("P-value %lf \n",p_value);

	free(P);
}

void CumulativeSums3(int n, unsigned char* array, double& p_value_forward)
{
	int		S, sup, inf, z, zrev, k, plus, minus, tmp, i, mask;
	double	sum1, sum2, p_value;

	int LUT_Cusum_size = 16;
	int LUT_Cusum_Bsize = 2;
	signed char *LUT_Cusum = LUT_Cusum_16;
	signed char *LUT_Cusum_max_positiv = LUT_Cusum_max_positiv_16;
	signed char *LUT_Cusum_max_negativ = LUT_Cusum_max_negativ_16;
	unsigned char *p_tmp, *p_end;

	if(0)
	{
		LUT_Cusum_size = 8;
		LUT_Cusum_Bsize = 1;
		LUT_Cusum = LUT_Cusum_8;
		LUT_Cusum_max_positiv = LUT_Cusum_max_positiv_8;
		LUT_Cusum_max_negativ = LUT_Cusum_max_negativ_8;
	}
	S = 0;
	sup = 0;
	inf = 0;

	mask = get_mask(LUT_Cusum_size);

	
	p_end = array + (n- (n%LUT_Cusum_size))/8;
	for (p_tmp = array; p_tmp < p_end; p_tmp += LUT_Cusum_Bsize){
		tmp = *((unsigned int*)p_tmp) & mask & mask;

		plus = LUT_Cusum_max_positiv[tmp];
		minus = LUT_Cusum_max_negativ[tmp];
		if (S + plus > sup) sup = S + plus;
		if (S + minus < inf) inf = S + minus;

		S += LUT_Cusum[tmp];
	}




	//LAST BLOCK
	if ( n % LUT_Cusum_size != 0)
	{
		for (i = n - (n % LUT_Cusum_size)  ; i < n; i++) {

			if (get_nth_block4(array, i) & 1) S++;
			else  S--;

			if (S > sup)
				sup++;
			if (S < inf)
				inf--;
		}
	}


	z = (sup > -inf) ? sup : -inf;

	zrev = (sup - S > S - inf) ? sup - S : S - inf;

	//printf("z = %d zrev=%d S= %d", z, zrev,S);

	// forward
	sum1 = 0.0;
	for (k = (-n / z + 1) / 4; k <= (n / z - 1) / 4; k++) {
		sum1 += cephes_normal(((4 * k + 1)*z) / sqrt(n));
		sum1 -= cephes_normal(((4 * k - 1)*z) / sqrt(n));
	}
	sum2 = 0.0;
	for (k = (-n / z - 3) / 4; k <= (n / z - 1) / 4; k++) {
		sum2 += cephes_normal(((4 * k + 3)*z) / sqrt(n));
		sum2 -= cephes_normal(((4 * k + 1)*z) / sqrt(n));
	}

	p_value_forward = 1.0 - sum1 + sum2;
	// // backwards is ignored from original impl.
	// sum1 = 0.0;
	// for (k = (-n / zrev + 1) / 4; k <= (n / zrev - 1) / 4; k++) {
	// 	sum1 += cephes_normal(((4 * k + 1)*zrev) / sqrt(n));
	// 	sum1 -= cephes_normal(((4 * k - 1)*zrev) / sqrt(n));
	// }
	// sum2 = 0.0;
	// for (k = (-n / zrev - 3) / 4; k <= (n / zrev - 1) / 4; k++) {
	// 	sum2 += cephes_normal(((4 * k + 3)*zrev) / sqrt(n));
	// 	sum2 -= cephes_normal(((4 * k + 1)*zrev) / sqrt(n));
	// }
	// p_value_backwards = 1.0 - sum1 + sum2;
	//printf("sum1 %lf   sum2 %lf\n\n",sum1,sum2);
}

void Serial4(int m, int n, unsigned char* array, double& p_value1)
{
	double	/*p_value1, p_value2,*/ psim0, psim1, psim2, del1, del2;

	int				i, k, len;
	double			sum /*, numOfBlocks */;
	unsigned int	mask, help;
	int             *P;

	//numOfBlocks = n;
	mask = (1 << m) - 1;
	len = (1 << m);

	if ((P = (int*)calloc(len, sizeof(int))) == NULL) {
		printf("Serial Test:  Insufficient memory available.\n");
		return;
	}
	for (i = 0; i < len; i++)
		P[i] = 0;
	/* COMPUTE FREQUENCY */
	Histogram(0, P, m, n, array); // add array variable as parameter

	for (i = 1; i<m; i++) {
		k = get_nth_block4(array, n - m + i)&(mask >> i);
		//printf("%d ",k);
		k ^= (((unsigned int*)array)[0] << (m - i));
		//printf("%d ",k);
		k &= mask;
		//printf("%d ",k);
		P[k]++;
	}

	if (m > 0)
	{
		sum = 0.0;
		for (i = 0; i < len; i++) {
			help = P[Mirrored_int(i, m)];
			if (help > 0)
				sum += (double)help*help;
			//printf("%d %d ",(1 << m)-1+i,help);	
		}
		sum = (sum * (1 << m) / (double)n) - (double)n;
		//printf("SUM = %lf   \n",sum);
	}
	else sum = 0.0;
	psim0 = sum;


	if (m - 1 > 0)
	{
		sum = 0.0;
		for (i = 0; i < len / 2; i++) {
			help = P[Mirrored_int(i, m - 1)] + P[Mirrored_int(i, m - 1) + len / 2];
			if (help > 0)
				sum += (double)help*help;
			//printf("%d %d ",(1 << (m-1))-1+Mirrored_int(i,m-1),help);	
		}
		sum = (sum * (1 << (m - 1)) / (double)n) - (double)n;
		//printf("SUM = %lf   \n",sum);

	}
	else sum = 0.0;
	psim1 = sum;

	if (m - 2 > 0)
	{
		sum = 0.0;
		for (i = 0; i < len / 4; i++) {
			help = P[Mirrored_int(i, m - 2)] + P[Mirrored_int(i, m - 2) + len / 4] + P[Mirrored_int(i, m - 2) + 2 * len / 4] + P[Mirrored_int(i, m - 2) + 3 * len / 4];
			if (help > 0)
				sum += (double)help*help;
			//printf("%d %d ",(1 << (m-2))-1+Mirrored_int(i,m-2),help);	
		}
		sum = (sum * (1 << (m - 2)) / (double)n) - (double)n;
		//printf("SUM = %lf   \n",sum);

	}
	else sum = 0.0;
	psim2 = sum;

	//printf("%lf %lf %lf\n",psim0,psim1,psim2);
	del1 = psim0 - psim1;
	del2 = psim0 - 2.0*psim1 + psim2;
	p_value1 = cephes_igamc(pow(2, m - 1) / 2, del1 / 2.0);
	//p_value2 = cephes_igamc(pow(2, m - 2) / 2, del2 / 2.0); ignore one of the p-values from original implementation

	free(P);
}

void LongestRunOfOnes3(int n, unsigned char* array, double& p_value)
{
	unsigned int    mask, tmp, processed_bits, block;

	double			/*p_value,*/ chi2, pi[7];
	int				run, v_n_obs, N, i, K, M, V[7];
	unsigned int	nu[7] = { 0, 0, 0, 0, 0, 0, 0 };

	
	int LUT_Lrun_size = 16;
	int LUT_Lrun_Bsize = 2;

	signed char *LUT_Lrun_start = LUT_Lrun_start_16;
	signed char *LUT_Lrun_end = LUT_Lrun_end_16;
	signed char *LUT_Lrun_max = LUT_Lrun_max_16;
	
	unsigned char *p_tmp, *p_end;

	if (n < 128) {
		return;
	}
	if (n < 6272) {
		K = 3;
		M = 8;
		V[0] = 1; V[1] = 2; V[2] = 3; V[3] = 4;
		pi[0] = 0.21484375;
		pi[1] = 0.3671875;
		pi[2] = 0.23046875;
		pi[3] = 0.1875;
	}
	else if (n < 750000) {
		K = 5;
		M = 128;
		V[0] = 4; V[1] = 5; V[2] = 6; V[3] = 7; V[4] = 8; V[5] = 9;
		pi[0] = 0.1174035788;
		pi[1] = 0.242955959;
		pi[2] = 0.249363483;
		pi[3] = 0.17517706;
		pi[4] = 0.102701071;
		pi[5] = 0.112398847;
	}
	else {
		K = 6;
		M = 10000;
		V[0] = 10; V[1] = 11; V[2] = 12; V[3] = 13; V[4] = 14; V[5] = 15; V[6] = 16;
		pi[0] = 0.0882;
		pi[1] = 0.2092;
		pi[2] = 0.2483;
		pi[3] = 0.1933;
		pi[4] = 0.1208;
		pi[5] = 0.0675;
		pi[6] = 0.0727;
	}




	if (M == 8)
	{
		LUT_Lrun_size = 8;
		LUT_Lrun_Bsize = 1;

		LUT_Lrun_start = LUT_Lrun_start_8;
		LUT_Lrun_end = LUT_Lrun_end_8;
		LUT_Lrun_max = LUT_Lrun_max_8;
	}

	
	N = n / M;
	run = 0;
	block = 1;
	processed_bits = 0;
	mask = get_mask(LUT_Lrun_size);
	v_n_obs = 0;
	
	p_end = array + M*N/8;
	for (p_tmp = array; p_tmp < p_end; p_tmp += LUT_Lrun_Bsize)
	{
		tmp =  *((unsigned int*)p_tmp) & mask;
		run += LUT_Lrun_start[tmp];

		if (run > v_n_obs){
			v_n_obs = run;
		}
		if (LUT_Lrun_max[tmp] > v_n_obs){
			v_n_obs = LUT_Lrun_max[tmp];
		}

		if (tmp != mask) run = LUT_Lrun_end[tmp];
		processed_bits += LUT_Lrun_size;

		if (processed_bits == M*block){
			if (v_n_obs < V[0])nu[0]++;
			else if (v_n_obs > V[K])nu[K]++;
			else nu[v_n_obs - V[0]]++;
			block++;
			v_n_obs = run = 0;
		}
	}

	
	chi2 = 0.0;
	for (i = 0; i <= K; i++)
		chi2 += ((nu[i] - N * pi[i]) * (nu[i] - N * pi[i])) / (N * pi[i]);

	p_value = cephes_igamc((double)(K / 2.0), chi2 / 2.0);

}

// THESE TWO FUNCTIONS BELOW USED IN DISCRETEFOURIERTRANSFORM2 TEST
// DECLARED IN dfft.c file

void  __ogg_fdrffti(int n, double *wsave, int *ifac);
void  __ogg_fdrfftf(int n, double *X, double *wsave, int *ifac);

void DiscreteFourierTransform2(int n, unsigned char* array, double& p_value)
{
	double	/*p_value,*/ upperBound, percentile, N_l, N_o, d, *m=NULL, *X=NULL, *wsave=NULL;
	int		i, count, ifac[15];

	if (((X = (double*)calloc(n, sizeof(double))) == NULL) ||
		((wsave = (double *)calloc(2 * n, sizeof(double))) == NULL) ||
		((m = (double*)calloc(n / 2 + 1, sizeof(double))) == NULL)) {
			printf("\t\tUnable to allocate working arrays for the DFT.\n");
			if( X != NULL )
				free(X);
			if( wsave != NULL )
				free(wsave);
			if( m != NULL )
				free(m);
			return;
	}
	for ( i=0; i<n; i++ )
	{
		X[i] = 2*((double)(get_nth_block4(array,i) & 1))-1;
	}
	
	__ogg_fdrffti(n, wsave, ifac);		/* INITIALIZE WORK ARRAYS */
	__ogg_fdrfftf(n, X, wsave, ifac);	/* APPLY FORWARD FFT */
	
	m[0] = sqrt(X[0]*X[0]);	    /* COMPUTE MAGNITUDE */
	
	for ( i=0; i<n/2; i++ )
		m[i+1] = sqrt(pow(X[2*i+1],2)+pow(X[2*i+2],2)); 
	count = 0;				       /* CONFIDENCE INTERVAL */
	upperBound = sqrt(2.995732274*n);
	for ( i=0; i<n/2; i++ )
		if ( m[i] < upperBound )
			count++;
	percentile = (double)count/(n/2)*100;
	N_l = (double) count;       /* number of peaks less than h = sqrt(3*n) */
	N_o = (double) 0.95*n/2.0;
	d = (N_l - N_o)/sqrt(n/4.0*0.95*0.05);
	p_value = erfc(fabs(d)/sqrt(2.0));
	//printf("%lf\n",p_value);
	free(X);
	free(wsave);
	free(m);
}
