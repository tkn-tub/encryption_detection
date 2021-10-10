/* This plugin is for Frequency RT
input:
1-the payloadsLength array which contains the bytes length for each payload of the corresponding flow
2- the saved payloads of the flow (as unsigned char array)
Output:
The statistical representation of the p-values which are : Standard Deviation and Mean respectively
These 
*/
#include <RandomnessTests.h>
// #include "boost/asio/buffer.hpp" //to be installed and tested
// #include <stack>
#include "FlowTools.hpp"

void block_freq_plugin(size_t *payloadsLength, unsigned char **strArray, uint8_t threshold, std::vector<double> &stRepOfPvals)
{
    double pValues[24];
    double stdev, mean;
    int thresholdAsInt = static_cast<int>(threshold);
    for (uint8_t i = 0; i < threshold; i++)
    {
        size_t pln = payloadsLength[i]; //this is the actual payload_length NOT the legnth exists in listOfpayloads
        int n = static_cast<int>(pln * 8);
        int m; //default is 128 for blockfrequency
        if (pln < 16)
            m = 4;
        else if (pln < 128)
            m = 8;
        else if (pln < 512)
            m = 32;
        else if (pln < 1024)
            m = 64;
        else
            m = 128;

        double p_value = 0.12345;

        BlockFrequency4(m, n, (*(strArray + i)), p_value);
        pValues[i] = p_value;
        // printf("I AM INSIDE PLUG IN block_freq_plugin");
        // printf("BFQ PVALUE       %.6lf,\t\t\n", pValues[i]);
    }
    mean_standardDeviation(pValues, thresholdAsInt, stdev, mean);
    // printf("stdev PVALUE       %.6lf,\t\t\n", stdev);
    stRepOfPvals.push_back(stdev);
    // printf("mean PVALUE       %.6lf,\t\t\n", mean);
    stRepOfPvals.push_back(mean);
}