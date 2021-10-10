

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     S T A T I S T I C A L  T E S T  F U N C T I O N  P R O T O T Y P E S 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void Frequency4(int n, unsigned char* array, double& p_value);
void BlockFrequency4(int M, int n, unsigned char* array, double& p_value);
void Runs4(int n, unsigned char* array, double& p_value);
void ApproximateEntropy4(int m, int n, unsigned char* array, double& p_value);
void CumulativeSums3(int n, unsigned char* array, double& p_value_forward);
void Serial4(int m, int n, unsigned char* array, double& p_value1);
void LongestRunOfOnes3(int n, unsigned char* array, double& p_value);
void DiscreteFourierTransform2(int n, unsigned char* array, double& p_value);