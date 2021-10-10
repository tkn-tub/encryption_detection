
#include <stdio.h>
#include <assert.h>
#include <getopt.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include "Freq.h"
#include <string.h>
#include <vector>
//+++for printing with ss using stringstream type
#include <iostream>
#include <sstream>
#include <ctime>
#include <bits/stdc++.h>         // for mean and stdev function
#include "boost/asio/buffer.hpp" //to be installed and tested
#include <stack>
#include <algorithm>
#include <map>
#include <iterator>
#include <RInside.h> // install.packages("RInside")
#include "translator.h"
//#include <conio.h>
#include <Python.h>
#include "PyAPI.h"
//#include <RInside_C.h>
using namespace std;

int chhex(char ch)
{
    if (isdigit(ch))
        return ch - '0';
    if (tolower(ch) >= 'a' && tolower(ch) <= 'f')
        return ch - 'a' + 10;
    return -1;
}

void from_hex_to_uchar_array(unsigned char *dest, const char *source, int bytes_n)
{
    for (bytes_n--; bytes_n >= 0; bytes_n--)
        dest[bytes_n] = 16 * chhex(source[bytes_n * 2]) + chhex(source[bytes_n * 2 + 1]);
}

// unsigned char *hexstr_to_char(const char *hexstr)
// {
//     size_t len = strlen(hexstr);
//     // IF_ASSERT(len % 2 != 0)
//     //     return NULL;
//     size_t final_len = len / 2;
//     unsigned char *chrs = (unsigned char *)malloc((final_len + 1) * sizeof(*chrs));
//     for (size_t i = 0, j = 0; j < final_len; i += 2, j++)
//         chrs[j] = (hexstr[i] % 32 + 9) % 25 * 16 + (hexstr[i + 1] % 32 + 9) % 25;
//     chrs[final_len] = '\0';
//     return chrs;
// }

void update_payload_list(void *payload, size_t payload_length, char *arrTry)
{

    // stack<int> st;
    //char arrTry[100000];
    int *arr;
    arr = (int *)malloc((payload_length + 1) * sizeof(int));
    //char* arr2[2048];
    ostringstream arr2;
    for (int i = 0; i < payload_length; i += 4)
    {
        arr[i] = ((int *)payload)[i];
        //          cout << arr[i] ;
        arr2 << std::hex << arr[i];

        // st.push((( int *)payload)[i]);
    }
    // cout << "PRINTED PAYLOAD....\n"
    //      << arr2.str() << endl;
    string ssss = arr2.str();
    strcpy(const_cast<char *>(arrTry), ssss.c_str());
    //listOfPayloads[np] = arrTry ;
    free(arr);
}

// Function for calculating mean and standard deviation of p-values:
// input array of values, output stdev and mean
void mean_standardDeviation(double a[], int n, double &stdev, double &mean)
{
    // Compute mean (average of elements)
    double sum = 0;
    for (int i = 0; i < n; i++)
        sum += a[i];
    mean = sum / (double)n; // get mean

    // Compute sum squared
    // differences with mean.
    double sqDiff = 0;
    for (int i = 0; i < n; i++)
        sqDiff += (a[i] - mean) *
                  (a[i] - mean);
    stdev = sqrt(sqDiff / (double)n); //standard deviation
}
void groundtruth_file_as_dict(string GTfile, std::map <string, string>& GTdict)
{
    ifstream fin(GTfile);
    vector<string> row;
    string line, word;
    string translateLabel;
    while (getline(fin, line, '\n'))
    {

        row.clear();
        istringstream s(line);
        stringstream fiveTupleGT;
        stringstream labelDashString;
        while (getline(s, word, ','))
        {
            row.push_back(word);
        }
        fiveTupleGT << row[1] << "," << row[2] << "," << row[3] << "," << row[4] << ',' << row[5];
		labelDashString << row[row.size() - 3] << "_" << row[row.size() - 2];
		GTdict.insert(std::pair<string, string>(fiveTupleGT.str(), labelDashString.str()));

	}
	row.clear();
    fin.close();
}

void match_flow_to_groundtruth(string fiveTupleFlow , std::map <string, string>& GTdict, int &label, vector<string>& nonFoundFlowsInGT, std::map <string, int>& labels_counters){
            
	if (GTdict.find(fiveTupleFlow) == GTdict.end()){
        label = 254;
        nonFoundFlowsInGT.push_back(fiveTupleFlow);
                // cout << "No match found for flow: " << fiveTupleFlow << " in GT file, please check the ips or port numbers are reversed or there is something wrong with GT file\n";

	}
	else {
		string labeldashstring = GTdict[fiveTupleFlow];
		label_translator(labeldashstring, label);

            if (labels_counters.find(labeldashstring) == labels_counters.end()){
                labels_counters.insert(std::pair<string, int>(labeldashstring, 1));
            }
            else {
                labels_counters[labeldashstring] += 1;
            }
    }
}
void print_undefined_label_flow_info(vector<string> NonFoundFlowsInGT, std::map <string, int>& labels_counters){

        cout << "Number of Flows which not found in GT file are: " << NonFoundFlowsInGT.size() << endl;
		if(NonFoundFlowsInGT.size() != 0)
				cout << "Non-Found Flows in GT file are: " << endl;
		for (int i = 0; i < NonFoundFlowsInGT.size(); i++)
		{
			cout << "Flow" << i+1 << ":\t" << NonFoundFlowsInGT[i] << endl;
		}
        cout << "---------------------------------------------------------------------------------------------\n\n";
        // print Labels summary
        int zeroLabels = 0;
        int oneLabels = 0;
        for (auto &e : labels_counters)
        {
            // istringstream s(e.first);
            // string word;
            // string translateLabel = "";
            // while (getline(s, word, '_'))
            // {
            //     translateLabel+=word;
            // }

            // int gtlabel = stoi(translateLabel);
            string labeldashstring = e.first;
            int label;
            label_translator(labeldashstring, label);
            if(label == 0){
            std::cout << "DEFINED Label, Non-encrypted, Class 0\t: App(" << e.first << ")\t\t\t Number of Flows\t" << e.second << '\n';
            zeroLabels += e.second;
            }
            else if(label == 1){
                std::cout << "DEFINED Label, Encrypted, Class 1\t: App(" << e.first << ")\t\t\t Number of Flows\t" << e.second << '\n';
                oneLabels += e.second;
            }
            else std::cout << "UNDEFINED Label, Unknown, Class -1\t:App(" << e.first << ")\t Number of Flows\t" << e.second << '\n';
        }
        cout << "---------------------------------------------------------------------------------------------\n\n";
        std::cout << "Total number of Flows of Non-encrypted traffic, Class 0 is\t: " << zeroLabels  << '\n' << '\n';
        std::cout << "Total number of Flows for Encrypted traffic, Class 1 is\t: " << oneLabels  << '\n';
}

string get_five_tuple_as_cs(uint32_t src_ip, uint32_t dst_ip, uint8_t protocol, uint16_t src_p, uint16_t dst_p)
{
	uint8_t *sip = (uint8_t *)&src_ip;
	uint8_t *dip = (uint8_t *)&dst_ip;
	std::stringstream fiveTupleFlow;
	fiveTupleFlow << (int)sip[3] << "." << (int)sip[2] << "." << (int)sip[1] << "." << (int)sip[0] << ',';
	fiveTupleFlow << (int)dip[3] << "." << (int)dip[2] << "." << (int)dip[1] << "." << (int)dip[0] << ',';
	fiveTupleFlow << unsigned(protocol) << ",";
	fiveTupleFlow << src_p << ",";
	fiveTupleFlow << dst_p;
	// cout << "print flow as strig\n"
	// 	 << fiveTupleFlow.str() << "\n";
	string ss = fiveTupleFlow.str();

	return ss;
}

void build_training_file(string trainingFile, string features, int label, uint8_t numbOfRTs)
{

	std::ofstream myfile;
	myfile.open(trainingFile, ios::app);
	ifstream pFile(trainingFile);
	if (pFile.peek() == std::ifstream::traits_type::eof())
	{
		for (int i = 0; i < (numbOfRTs * 2); i++)
		{
			myfile << "f" << i + 1 << ",";
		}
		myfile << "Class"
			   << "\n";
	}
	myfile << features << label << "\n";
	myfile.close();
}

string get_features_as_cs(double* aNNInput, uint8_t numbOfRTs)
{
	std::stringstream features;

	for (int j = 0; j < (numbOfRTs * 2); j++)
	{
		features << aNNInput[j];
		features << ",";
	}
	return features.str();
}

int classification(string ClassificationScriptPy, string python_object_path, string features)
{
    
    static PyAPI *py_api = new PyAPI(python_object_path, ClassificationScriptPy);
    int prediction = py_api->get_label(features);
    // cout << "Returned value form python is  ";


	return prediction;
}
void get_results_file(string resultsFile, string fiveTuple, int label, int prediction, uint8_t packetsNumber, uint64_t id_num)
{
	vector<string> row;
	string word;
    istringstream s(fiveTuple);
	while (getline(s, word, ','))
	{
		row.push_back(word);
	}
	std::ofstream myfile;
	myfile.open(resultsFile, ios::app);

	myfile << unsigned(id_num) << "\t" << row[0] << "\t" << row[1] << "\t" << row[2] << "\t" << row[3] << "\t" << row[4] << "\t" << label << "\t" << prediction << "\t" << unsigned(packetsNumber) << "\n";
	myfile.close();
}

void write_headers_Files(string trainingFile, char *RtsPlugIns, string reportTime, string resultsFile , string pcap_file){


       // current date/time based on current system
   time_t now = time(0);
   
   // convert now to string form
   char* dt = ctime(&now);

    std::ofstream myfile;
	myfile.open(resultsFile, ios::app);
	ifstream pFile(resultsFile);
	if (pFile.peek() == std::ifstream::traits_type::eof())
	{
        myfile << "#The local date and time: \t" << dt << "\n"
        << "#RESULTS , Flows in pcap file" << "\n#" << pcap_file << "\n"
        << "#This file contains the flow five tuple, label and prediction result" << "\n"
         << "#five tuple are the biflow info which is examined for training OR classification" << "\n" 
         << "#label : can have values according to translator function see translator.cpp" << "\n" 
         << "#label = -1 this is undefined label(unknown)" << "\n"
          << "#label = 254 this means the flow is not found in GTfile " << "\n" 
          << "#prediction : can have values according to the classificaton classes defined " << "\n"
           << "#in ANN classification with its trained model" << "\n" 
           << "#prediction = -1 this means the value is not defined - should be only in training phase" << "\n";
			myfile 
               << "FlowID"
               << "\t"
			   << "Src_ip_address"
			   << "\t"
			   << "Dst_ip_address"
			   << "\t"
			   << "pro"
			   << "\t"
			   << "s_p"
			   << "\t"
			   << "d_p"
			   << "\t"
			   << "label"
			   << "\t"
			   << "pred"
               	<< "\t"
			   << "pckts"
			   << "\n";
    }
    std::ofstream trainfile;
	trainfile.open(trainingFile, ios::app);
	ifstream trpFile(trainingFile);
	if (trpFile.peek() == std::ifstream::traits_type::eof() && trainingFile != "")
	{
        trainfile << "#The local date and time: \t" << dt << "\n"
        << "#Training , Flows in pcap file" << "\n#" << pcap_file << "\n"
        << "#This file contains the features extracted from the Randomness tests and the corresponding label" << "\n"
         << "#The features are basically a statistaical representation (mean and standard deviation) of the p-values" << "\n" 
         << "#Each RT on each input(payload) generates one p-vlaue, for mulitple p-values the mean(M) and standard(S) deviation are calculated as featuers" << "\n" 
         << "#label = 0 for non-encrypted data" << "\n"
          << "#label = 1 for encrypted data" << "\n" 
          << "#Labels are extracted from GTfile specified in Configuration.ini file and translated based on translator.cpp file" << "\n"
           << "#NOTE: If there is no output, check the GTfile parameter in Configuration.ini file" << "\n" 
           << "#Randmoness Tests should be in this order if used (uncommented in Configuration file):\n"
           << "#FrequencyTest, BlockFreqTest, SerialTest, CumulativeSumTest, ApproximateEntropyTest, DescreteFourierTransformTest, LongestRunsOfOnesTest, RunsTest, TopologicalBinaryTest, GreatesCommonDivisorTest" << "\n"
            << "#Example: FreqM and FreqS represent the mean and standard deviation values of FrequencyTest" << "\n";
if (RtsPlugIns[0] == 't') trainfile << "FreqM," << "FreqS,";
if (RtsPlugIns[1] == 't') trainfile << "BlockFreqM," << "BlockFreqS,";
if (RtsPlugIns[2] == 't') trainfile << "SerM," << "SerS,";
if (RtsPlugIns[3] == 't') trainfile << "CumSumM," << "CumSumS,";
if (RtsPlugIns[4] == 't') trainfile << "ApEntM," << "ApEntS,";
if (RtsPlugIns[5] == 't') trainfile << "DftM," << "DftS,";
if (RtsPlugIns[6] == 't') trainfile << "LrunsM," << "LrunsS,";
if (RtsPlugIns[7] == 't') trainfile << "RunsM," << "RunsS,";
if (RtsPlugIns[8] == 't') trainfile << "TBTM," << "TBTS,";
if (RtsPlugIns[9] == 't') trainfile << "GCDM," << "GCDS,";
if (RtsPlugIns[10] == 't') trainfile << "BckstackM," << "BckstackS,";
            trainfile << "Label" << "\n";
    }
        trainfile.close();
    std::ofstream timefile;
    timefile.open(reportTime, ios::app);
	ifstream tipFile(reportTime);
	if (tipFile.peek() == std::ifstream::traits_type::eof())
	{
        timefile << "#The local date and time: \t" << dt << "\n"
        << "#TIME CONSUMTION CALCUALATIONS, Flows in pcap file" << "\n#" << pcap_file << "\n"
            << "#This file contains the Time report for implementing functions below" << "\n"
         << "#All Randomness Tests plugin functions including RTs in R seperated" << "\n" 
         << "#RTs preprocessing (which includes the initialization of R instance in the first time)" << "\n" 
         << "#Ground truth label extraction in which the caputred flow is matched with the ones saved in GT file" << "\n"
          << "#Classification time using ANN prediction" << "\n" 
          << "#NOTE: this time calculation is for each processed flow" << "\n"
           << "#in ANN classification with its trained model(the model is loaded only once in the first flow)" << "\n" 
           << "#" << "\n";

        timefile 
                 << "RTpre"
                 << "\t"
                 << "Freq"
                 << "\t"
                 << "Blockfreq"
                 << "\t"
                 << "Ser"
                 << "\t"
                 << "Cumsum"
                 << "\t"
                 << "ApEnt"
                 << "\t"
                 << "Dft"
                 << "\t"
                 << "Lruns"
                 << "\t"
                 << "Runs"
                 << "\t"
                 << "TBT"
                 << "\t"
                 << "GCD"
                 << "\t"
                 << "Bckstack"
                 << "\t"
                 << "GTmatch"
                 << "\t"
                 << "Classification"
                 << "\t"
                 << "Packets"
                 << "\t"
                 << "FlowID"
                 << "\n";
    }
    timefile.close();
}