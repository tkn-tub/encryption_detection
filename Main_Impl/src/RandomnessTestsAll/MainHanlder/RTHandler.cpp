#include <RandomnessTests.h>
// #include "boost/asio/buffer.hpp" //to be installed and tested
// #include <stack>
#include <string.h>
#include <vector>
#include "FlowTools.hpp"
#include "Freq.h"
#include "BlockFreq.h"
#include "Serial.h"
#include "AppEntropy.h"
#include "CumSum.h"
#include "DFT.h"
#include "LongestRun.h"
#include "Runs.h"
#include "TBT.h"
#include "GCD.h"
#include "BookStack.h"
#include <RInside.h> // install.packages("RInside")
#include <chrono>
#include <iostream>
#include <fstream>

using namespace std::chrono;
using namespace std;

void RTsHandler(size_t *payloadsLength, const char **listOfPayloads, uint8_t threshold, double *aNNInput, string reportTime, char *RtsPlugIns)
{

    auto start1 = high_resolution_clock::now();

    // if(numOfRTs > 8) static RInside Rqm; // create an embedded R instance for RTs in R
//    unsigned char *stringsArray[24]; // prepare the listOfPayloads to unsigned char arrays fro RTs in C
    unsigned char **stringsArray; 
	stringsArray = (unsigned char **)malloc(sizeof(unsigned char *) * threshold);
    for (uint8_t i = 0; i < threshold; i++)
    {
        //Initialization of variables used for all RTs
        size_t pl = payloadsLength[i]; //this is the actual payload_length NOT the legnth exists in listOfpayloads

        stringsArray[i] = (unsigned char *)malloc(50000 * sizeof(unsigned char));
        from_hex_to_uchar_array((*(stringsArray + i)), (*(listOfPayloads + i)), pl); //send pointer to the corresponding payload
        
    }
    auto stop1 = high_resolution_clock::now();
    auto duration1 = duration_cast<microseconds>(stop1 - start1);


    std::vector<double> stRepOfPvals;

    auto start2 = high_resolution_clock::now();
    if (RtsPlugIns[0] == 't') freq_plugin(payloadsLength, stringsArray, threshold, stRepOfPvals);
    auto stop2 = high_resolution_clock::now();
    auto duration2 = duration_cast<microseconds>(stop2 - start2);


    auto start3 = high_resolution_clock::now();
    if (RtsPlugIns[1] == 't') block_freq_plugin(payloadsLength, stringsArray, threshold, stRepOfPvals);
    auto stop3 = high_resolution_clock::now();
    auto duration3 = duration_cast<microseconds>(stop3 - start3);


    auto start4 = high_resolution_clock::now();
    if (RtsPlugIns[2] == 't') serial_plugin(payloadsLength, stringsArray, threshold, stRepOfPvals);
    auto stop4 = high_resolution_clock::now();
    auto duration4 = duration_cast<microseconds>(stop4 - start4);


    auto start5 = high_resolution_clock::now();
    if (RtsPlugIns[3] == 't') cumsum_plugin(payloadsLength, stringsArray, threshold, stRepOfPvals);
    auto stop5 = high_resolution_clock::now();
    auto duration5 = duration_cast<microseconds>(stop5 - start5);


    auto start6 = high_resolution_clock::now();
    if (RtsPlugIns[4] == 't') app_entropy_plugin(payloadsLength, stringsArray, threshold, stRepOfPvals);
    auto stop6 = high_resolution_clock::now();
    auto duration6 = duration_cast<microseconds>(stop6 - start6);


    auto start7 = high_resolution_clock::now();
    if (RtsPlugIns[5] == 't') dft_plugin(payloadsLength, stringsArray, threshold, stRepOfPvals);
    auto stop7 = high_resolution_clock::now();
    auto duration7 = duration_cast<microseconds>(stop7 - start7);


    auto start8 = high_resolution_clock::now();
    if (RtsPlugIns[6] == 't') longest_run_plugin(payloadsLength, stringsArray, threshold, stRepOfPvals);
    auto stop8 = high_resolution_clock::now();
    auto duration8 = duration_cast<microseconds>(stop8 - start8);


    auto start9 = high_resolution_clock::now();
    if (RtsPlugIns[7] == 't') runs_plugin(payloadsLength, stringsArray, threshold, stRepOfPvals);
    auto stop9 = high_resolution_clock::now();
    auto duration9 = duration_cast<microseconds>(stop9 - start9);


    std::ofstream myfile;
    myfile.open(reportTime, ios::app);
    myfile << duration1.count() << "\t"<< duration2.count() << "\t"<< duration3.count() << "\t"<< duration4.count() << "\t"<< duration5.count() << "\t"<< duration6.count() << "\t"<< duration7.count() << "\t"<< duration8.count() << "\t"<< duration9.count() << "\t";
    // if (numOfRTs > 8) myfile << duration10.count() << "\t"<< duration11.count() << "\t"<< duration12.count() << "\t";
    // if(RtsPlugIns[8] == 't' || RtsPlugIns[9] == 't' || RtsPlugIns[10] == 't'){
            static RInside Rqm;
    auto start10 = high_resolution_clock::now();

    if (RtsPlugIns[8] == 't') tbt_plugin(Rqm, payloadsLength, listOfPayloads, threshold, stRepOfPvals);
    auto stop10 = high_resolution_clock::now();
    auto duration10 = duration_cast<microseconds>(stop10 - start10);


    auto start11 = high_resolution_clock::now();
    if (RtsPlugIns[9] == 't') gcd_plugin(Rqm, payloadsLength, listOfPayloads, threshold, stRepOfPvals);
    auto stop11 = high_resolution_clock::now();
    auto duration11 = duration_cast<microseconds>(stop11 - start11);


    auto start12 = high_resolution_clock::now();
    if (RtsPlugIns[10] == 't') book_stack_plugin(Rqm, payloadsLength, listOfPayloads, threshold, stRepOfPvals);
    auto stop12 = high_resolution_clock::now();
    auto duration12 = duration_cast<microseconds>(stop12 - start12);

    myfile << duration10.count() << "\t"<< duration11.count() << "\t"<< duration12.count() << "\t";

    // }
    myfile.close();

    for (int j = 0; j < stRepOfPvals.size(); j++)
    {
        aNNInput[j] = stRepOfPvals[j];
    }
    for (int i=0; i<threshold; ++i) {
  		free(stringsArray[i]);
	}
    free(stringsArray);
	stringsArray = NULL;
    stRepOfPvals.clear();
}

