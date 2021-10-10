/* Sources: these sources are used for development not only in the main file but for all included files in this applications
including R package
https://github.com/wanduow/libflowmanager/tree/master/example
https://github.com/wanduow/libflowmanager
https://crocs.fi.muni.cz/public/projects/sts
https://rdrr.io/cran/CryptRndTest/

 *
 * Author: Ahmad Alaswad
 */

#define __STDC_FORMAT_MACROS

#include <stdio.h>
#include <assert.h>
#include <getopt.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>

#include <string.h>
#include <vector>
#include <iostream>
#include <fstream>

#include <sstream>

#include <libtrace.h>
#include <libflowmanager.h>
//for RTs
#include <RandomnessTests.h>
#include "boost/asio/buffer.hpp" //to be installed and tested
#include <stack>
#include "FlowTools.hpp"
#include "RTHandler.hpp"
#include "Generals.h"
#include <RInside.h> // install.packages("RInside")
#include <chrono>
#include <map>
using namespace std::chrono;
using namespace std;
using namespace ExtractConf;

int totalNumOfPackets = 0;
uint8_t mainthreshold = 5;

uint8_t numOfRTs = 0; 
char *RtsPlugIns;
bool train = true;
bool classify = true;

string pcap_file = "";

string resultsFile = "";

string trainingFile = "";

string GTfile = "";

string ClassificationScriptPy = "";

std::string python_object_path = "";

string reportTime = "";

uint8_t threshold = mainthreshold;
// int numbUnknownLabels = 0;
// int numbNonFoundFlowsInGT = 0;
// vector<string> unknownLabels;
std::map <string, string> GTdict;
vector<string> nonFoundFlowsInGT;
std::map <string, int> labels_counters;

uint64_t flow_counter = 0;
uint64_t expired_flows = 0;
int less_ThresholdC = 0;
int processed_flows = 0;
auto now = high_resolution_clock::now();
// auto rtsTotalDuration =  duration_cast<microseconds>(now - now);
auto gtfTotalDuration =  duration_cast<microseconds>(now - now);
auto annTotalDuration =  duration_cast<microseconds>(now - now);


FlowManager *fm = NULL;
/* This data structure is used to demonstrate how to use the 'extension' 
 * pointer to store custom data for a flow */
typedef struct builder
{
        uint64_t packets;
        uint8_t init_dir;
        uint32_t client_ip; /**< Source ip */
        uint32_t server_ip; /**< Destination ip */
        uint16_t client_p;  /**< Source Port */
        uint16_t server_p;  /**< Destination port */
        uint8_t proto;      /**< Transport layer protocol */
        uint64_t id_num;
        size_t payload_length;
        uint32_t remaining;
        const char **listOfPayloads; //  initial value of threshold, can not use threshold because it is a variable it has to be a constant
        size_t *payloadsLength;   // array size might be changed to max value of threshold later
        uint8_t numOfPayloads;
        //double pValues[100]; // array size might be changed to max value of (threshold * number of RTs) later
        double *aNNInput;    //p-values representation
        int label;
        int prediction;

} StructBuilder;

/* Initialises the custom data for the given flow. Allocates memory for a
 * CounterFlow structure and ensures that the extension pointer points at
 * it.
 */

void init_counter_flow(Flow *f, uint8_t dir)
{
        StructBuilder *stb = NULL;

        stb = (StructBuilder *)malloc(sizeof(StructBuilder));

        stb->init_dir = dir;
        stb->packets = 0;
        stb->numOfPayloads = 0;
        stb->label = -1;
        stb->prediction = -1;
        flow_counter++;
        f->extension = stb;
}



void output_files_handler(StructBuilder *stb)
{
        // int prediction = -1;
        string fiveTuple = get_five_tuple_as_cs( stb->client_ip, stb->server_ip, stb->proto, stb->client_p, stb->server_p);
        string commaSepFeatures = get_features_as_cs( stb->aNNInput, numOfRTs);
        auto start = high_resolution_clock::now();
        if (GTfile == "Encryption_Detection") {
        uint16_t tmp = (stb->server_p /100) * 100;// cout << "\nthis is temp \n" << tmp;
        if (tmp == 2000 || tmp == 6000 || tmp == 8000) stb->label = 0;
        else if (tmp == 3000 || tmp == 4000 || tmp == 7000) stb->label =1;
        }
        else if (GTfile == "Content_Distinguisher") {
        uint16_t tmp = (stb->server_p /100) * 100;// cout << "\nthis is temp \n" << tmp;
        if (tmp == 3000 || tmp == 4000 || tmp == 7000) stb->label =13;
        else if (tmp == 2000 || tmp == 6000 || tmp == 8000) stb->label = stb->server_p % tmp;// cout << "\nThis is the laebl" << stb->label ;
        }
        else if (GTfile == "Protocol_CLassifier") {
        uint16_t tmp = (stb->server_p /100) * 100;// cout << "\nthis is temp \n" << tmp;
        if (tmp == 2000) stb->label =0;
        if (tmp == 6000) stb->label =1;
        if (tmp == 8000) stb->label =2;
        if (tmp == 3000) stb->label =3;
        if (tmp == 4000) stb->label =4;
        if (tmp == 7000) stb->label =5;
        }
        else if (GTfile != "") match_flow_to_groundtruth(fiveTuple, GTdict, stb->label, nonFoundFlowsInGT, labels_counters);

        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        
        std::ofstream myfile;
        myfile.open(reportTime, ios::app);
        myfile << duration.count() << "\t";

          gtfTotalDuration +=duration;
        myfile.close();
 

        if(train && threshold == mainthreshold)
        {
                if (trainingFile == ""){
                perror("No training File specified, set the full path of trainingFile parameter\n If no training required set train parameter in Configuration.ini file to 0\n");
                exit(-1);
                }
        if (stb->label != -1 && stb->label != 254)
                {
                //string commaSepFeatures = get_features_as_cs( stb->aNNInput, numOfRTs);
                build_training_file(trainingFile, commaSepFeatures, stb->label, numOfRTs);
                }
        }
        auto startC = high_resolution_clock::now();
        if(classify)
        {


        stb->prediction = classification( ClassificationScriptPy, python_object_path, commaSepFeatures);

                if (stb->prediction == -1) 
                {
                perror("Classification does not work\n please check that the trained model in Configuration.ini file considers the same number of features for Classifiactoin\n");
                exit(-1);
                }
        }
        auto stopC = high_resolution_clock::now();
        auto durationC = duration_cast<microseconds>(stop - start);
        myfile.open(reportTime, ios::app);
        myfile << durationC.count() << "\t" << unsigned(threshold) << "\t" << unsigned(stb->id_num) << "\n";
        myfile.close();
        get_results_file(resultsFile, fiveTuple, stb->label, stb->prediction, stb->numOfPayloads, stb->id_num);
        free(stb->aNNInput); free(stb->payloadsLength);
        stb->aNNInput = NULL; stb->payloadsLength = NULL;
        //free(stb);
}
void after_threshold_processing(StructBuilder *stb)
{
	stb->aNNInput = (double *)malloc(sizeof(double) * ((numOfRTs * 2)));
        // Extract features from RTs plug-ins
	RTsHandler(stb->payloadsLength, stb->listOfPayloads, threshold, stb->aNNInput, reportTime, RtsPlugIns);

        for (int i=0; i<threshold; ++i) {
                free((void *)stb->listOfPayloads[i]);
	}
	free(stb->listOfPayloads);
	stb->listOfPayloads = NULL;
        // Label extractino, using features for training/classification, build the output files..
	output_files_handler(stb);
        processed_flows+=1;
}
/* Expires all flows that libflowmanager believes have been idle for too
 * long. The exp_flag variable tells libflowmanager whether it should force
 * expiry of all flows (e.g. if you have reached the end of the program and
 * want the stats for all the still-active flows). Otherwise, only flows
 * that have been idle for longer than their expiry timeout will be expired.
 */
void expire_counter_flows(double ts, bool exp_flag)
{
        Flow *expired;

        /* Loop until libflowmanager has no more expired flows available */
        while ((expired = fm->expireNextFlow(ts, exp_flag)) != NULL)
        {

                StructBuilder *stb = (StructBuilder *)expired->extension;
                if (stb->numOfPayloads < threshold && stb->numOfPayloads > 0)
                {
                        //stb->numOfPayloads = threshold;
                        threshold = stb->numOfPayloads;
                        after_threshold_processing(stb);
                        threshold = mainthreshold;
                        less_ThresholdC+=1;
                }
                
                // printf("Final count of packets: %" PRIu64 " total packets \n", stb->packets);

                /* Don't forget to free our custom data structure */
                free(stb);

                /* VERY IMPORTANT: release the Flow structure itself so
                 * that libflowmanager can now safely delete the flow */
                fm->releaseFlow(expired);

                expired_flows++;
        }
}
void payload_handler(StructBuilder *stb, void *payload, size_t payload_length)
{
	if (payload_length > 0 && payload != NULL)
	{
		// allocate memory for payloads, should be double of maximum payload_length expected
		if (stb->numOfPayloads == 0)
		{
			stb->listOfPayloads = (const char **)malloc(sizeof(char *) * threshold);
			stb->payloadsLength = (size_t *)malloc(sizeof(size_t) * threshold);

			for (int r = 0; r < threshold; r++)
				stb->listOfPayloads[r] = (char *)malloc(100000 * sizeof(char));
		}
		stb->payloadsLength[stb->numOfPayloads] = payload_length; // add the payload_length to payloadsLength array
		char *arrTry;
		arrTry = (char *)malloc(sizeof(char *) * (payload_length * 2));
		update_payload_list(payload, payload_length, arrTry);
		stb->listOfPayloads[stb->numOfPayloads] = arrTry;

		stb->numOfPayloads++;
		if (stb->numOfPayloads == threshold)
		{
			after_threshold_processing(stb);
                //this array is already freed before in this line free((void *)stb->listOfPayloads[i]);
                // free(arrTry);
                // arrTry = NULL;
		}

	}
}
void flow_Handler(StructBuilder *stb, libtrace_tcp_t *tcp, libtrace_udp_t *udp, void *payload, uint32_t remaining, size_t payload_length)
{
        void *flowPayload;
        // Extract paylod from TCP
        if (stb->numOfPayloads < threshold && tcp != NULL && payload_length > 0)
        {
                stb->proto = IPPROTO_TCP;
                flowPayload = trace_get_payload_from_tcp((libtrace_tcp_t *)payload, &remaining);
                // flowPayload = trace_get_payload_from_tcp(tcp, &remaining);

                if (flowPayload == NULL && remaining == 0){
                        printf("TCP header is incomplete:\t\t payload_length is %zu \n", payload_length);
                        perror("Existed payload is not detected");
                        exit(-1);
                        }
                payload_handler(stb, flowPayload, payload_length);

        }
                // Extract paylod from UDP
        else if (stb->numOfPayloads < threshold && udp != NULL && payload_length > 0)
        {
                stb->proto = IPPROTO_UDP;

                flowPayload = trace_get_payload_from_udp((libtrace_udp_t *)payload, &remaining);

                if (flowPayload == NULL && remaining == 0){
                        printf("UDP header is incomplete:\t\t payload_length is %zu \n", payload_length);
                        perror("Existed payload is not detected");
                        exit(-1);
                        }
                payload_handler(stb, flowPayload, payload_length);

        }
                // Extract paylod from IP
        else if (stb->numOfPayloads < threshold && stb->proto != 0 && payload_length > 0)
        {

                if (payload == NULL && remaining == 0){
                        printf("Header is incomplete:\t\t payload_length is %zu \n", payload_length);
                        perror("Existed payload is not detected");
                        exit(-1);
                        }
                payload_handler(stb, payload, payload_length);
        }
}

void per_packet(libtrace_packet_t *packet)
{

        Flow *f;
        StructBuilder *stb = NULL;

        uint8_t dir;
        bool is_new = false;

        libtrace_tcp_t *tcp = NULL;
        libtrace_udp_t *udp = NULL;
        void *payload = NULL;
        libtrace_ip_t *ip = NULL;

        double ts;
        uint16_t l3_type;

        uint32_t src_ip; /**< Source ip */
        uint32_t dst_ip; /**< Destination ip */
        uint16_t src_p;  /**< Source Port */
        uint16_t dst_p;  /**< Destination port */
        uint8_t proto;   /**< Transport layer protocol */
        uint32_t remaining;
        uint64_t id_num;
        size_t payload_length = 0;

        /* get transport layer info to get five tuples, payload length, and the payload

*/
        ip = (libtrace_ip_t *)trace_get_layer3(packet, &l3_type, NULL);
        tcp = (libtrace_tcp_t *)trace_get_tcp(packet);
        udp = (libtrace_udp_t *)trace_get_udp(packet);
        // WITH THESE FUNCTIONS GOT THE ACTUAL RESULTS

        src_p = trace_get_source_port(packet);
        dst_p = trace_get_destination_port(packet);

        /* Libflowmanager only deals with IP traffic, so ignore anything
	 * that does not have an IP header */
        if (l3_type != 0x0800)
                return;
        if (ip == NULL)
                return;
        payload_length = trace_get_payload_length(packet);

        /* Many trace formats do not support direction tagging (e.g. PCAP), so
	 * using trace_get_direction() is not an ideal approach. The one we
	 * use here is not the nicest, but it is pretty consistent and 
	 * reliable. Feel free to replace this with something more suitable
	 * for your own needs!.
	 */
        if (ip->ip_src.s_addr < ip->ip_dst.s_addr)
                dir = 0;
        else
                dir = 1;

        // /* Ignore packets where the IP addresses are the same - something is
        //  * probably screwy and it's REALLY hard to determine direction */
        // if (ip->ip_src.s_addr == ip->ip_dst.s_addr)
        //         return;

        /* Match the packet to a Flow - this will create a new flow if
	 * there is no matching flow already in the Flow map and set the
	 * is_new flag to true. */
        f = fm->matchPacketToFlow(packet, dir, &is_new);

        /* Libflowmanager did not like something about that packet - best to
	 * just ignore it and carry on */
        if (f == NULL)
                return;

        /* If the returned flow is new, you will probably want to allocate and
	 * initialise any custom data that you intend to track for the flow */

        if (is_new)
        {
                /* initializing a flow structure containing flowid, and its fivetuple (in addition to counter flow)

        */
                init_counter_flow(f, dir);
                uint8_t *sip = (uint8_t *)&ip->ip_src.s_addr;
                uint8_t *dip = (uint8_t *)&ip->ip_dst.s_addr;
                FlowId id = f->id;
                flowid_hash id_key;
                stb = (StructBuilder *)f->extension;
                id_num = id_key.operator()(id);
                stb->id_num = id_num;
                stb->client_ip = ntohl(ip->ip_src.s_addr);
                stb->server_ip = ntohl(ip->ip_dst.s_addr);
                stb->client_p = src_p;
                stb->server_p = dst_p;
        }

        /* Cast the extension pointer to match the custom data type */

        else
                stb = (StructBuilder *)f->extension;
        /* Increment our packet counter */
        stb->packets++;
        payload = trace_get_transport(packet, &proto, &remaining);
        stb->proto = proto;
        // payloadP = packet->payload;
        flow_Handler(stb, tcp, udp, payload, remaining, payload_length);

        /* Tell libflowmanager to update the expiry time for this flow */
        ts = trace_get_seconds(packet);

        fm->updateFlowExpiry(f, packet, dir, ts);

        /* Expire all suitably idle flows */
        expire_counter_flows(ts, false);
}

int main(int argc, char *argv[])
{
        auto start = high_resolution_clock::now();
        libtrace_t *trace;
        libtrace_packet_t *packet;

        bool opt_true = true;
        bool opt_false = false;
        double ts;
        lfm_plugin_id_t plugid = LFM_PLUGIN_STANDARD;

        int i;




         packet = trace_create_packet();
         if (packet == NULL)
         {
                 perror("Creating libtrace packet");
                 return -1;
        }
        // unknownLabels.clear();
	nonFoundFlowsInGT.clear();

        fm = new FlowManager();

        /* This tells libflowmanager to ignore any flows where an RFC1918
	 * private IP address is involved */
        //if (fm->setConfigOption(LFM_CONFIG_IGNORE_RFC1918, &opt_true) == 0)
        //      return -1;

        /* This tells libflowmanager not to replicate the TCP timewait
	 * behaviour where closed TCP connections are retained in the Flow
	 * map for an extra 2 minutes */
        if (fm->setConfigOption(LFM_CONFIG_TCP_TIMEWAIT, &opt_false) == 0)
                return -1;

        /* This tells libflowmanager to use the standard set of flow expiry
	 * rules, i.e. the original libflowmanager expiry rules.
	 *
	 * Other possible rulesets are:
	 *  LFM_PLUGIN_STANDARD_SHORT_UDP -- same as standard but with fast
	 *  expiry for short UDP flows. 
	 *  LFM_PLUGIN_FIXED_INACTIVE -- flows expire after a fixed period of
	 *  inactivity regardless of flow state or transport protocol.
	 *
	 * Expiry thresholds for the other rulesets can be set using the
	 * LFM_CONFIG_FIXED_EXPIRY_THRESHOLD config option. For the Short UDP
	 * ruleset, this will set the inactivity threshold for the short UDP
	 * flows (default is 10 seconds). For Fixed Inactive, this will set
	 * the inactivity threshold for all flows (default is 60 seconds).
	 */
        if (fm->setConfigOption(LFM_CONFIG_EXPIRY_PLUGIN, &plugid) == 0)
                return -1;

        optind = 1;

        for (i = optind; i < argc; i++)
        {

                printf("Processing flows based on the config file: \n%s\n", argv[i]);
                Info config;
                loadConfiguration(config,argv[i]);

                mainthreshold = config.mainthreshold;
                //numOfRTs = config.numOfRTs; 
                if(config.classify == 0) classify = false;
                pcap_file = config.pcap_file;
                resultsFile = config.resultsFile;
                trainingFile = config.trainingFile;
                GTfile = config.GTfile;
                ClassificationScriptPy = config.ClassificationScriptPy;
                python_object_path = config.python_object_path;
                reportTime = config.reportTime;
                threshold = mainthreshold;
                GTdict.clear();
                if (GTfile != "") groundtruth_file_as_dict(GTfile, GTdict);
                else printf("WARNING: no GTfile, No label is -1 for classification..\n");
                std::ofstream myfile;
                if( remove(reportTime.c_str()) == 0 )
                printf("Existed reportTime file is removed..\n");
                RtsPlugIns = config.RTsPlugin;
                // cout << RtsPlugIns[0] << endl;
                int i =0;
                while(RtsPlugIns[i]!= '\0'){
                        //string tmp = RtsPlugIns[i];
                        if (RtsPlugIns[i] == 't') 
                                numOfRTs++;
                        // cout << RtsPlugIns[i] << endl;
                        i++;
                        if (numOfRTs == 11) break;

                }
                // myfile.open(reportTime, ios::app);
                // myfile << "RTpre" << "\t"<< "Freq" << "\t"<< "Blockfreq" << "\t"<< "Ser" << "\t"<< "Cumsum" << "\t"<< "ApEnt" << "\t"<< "Dft" << "\t"<< "Lruns" << "\t"<< "Runs" << "\t"<< "TBT" << "\t"<< "GCD" << "\t"<< "Bckstack" << "\t"<< "GTmatch" << "\t"<< "Classification" << "\n";
                // myfile.close();
                if (config.Train == 0)
                {
                        train = false;
                }
                else{
                        train = true;
                        if( remove(trainingFile.c_str()) == 0 )
                        printf("Existed training file is removed, should be built in training phase\n");
                }
                if (remove(resultsFile.c_str()) == 0)
                        printf("Existed results file is removed to show the new results\n");
                
                if (config.interface != "")
                trace = trace_create(("pcapint:" + config.interface).c_str());
                else
                {
                        trace = trace_create(config.pcap_file.c_str());
                }

                write_headers_Files(trainingFile, RtsPlugIns, reportTime, resultsFile , pcap_file);
                // /* Bog-standard libtrace stuff for reading trace files */
                // trace = trace_create(argv[i]);

                if (!trace)
                {
                        perror("Creating libtrace trace");
                        return -1;
                }

                if (trace_is_err(trace))
                {
                        trace_perror(trace, "Opening trace file");
                        trace_destroy(trace);
                        continue;
                }

                if (trace_start(trace) == -1)
                {
                        trace_perror(trace, "Starting trace");
                        trace_destroy(trace);
                        continue;
                }
                while (trace_read_packet(trace, packet) > 0)
                {

                        ts = trace_get_seconds(packet);
                        per_packet(packet);
                        totalNumOfPackets++;
                }

                if (trace_is_err(trace))
                {
                        trace_perror(trace, "Reading packets");
                        trace_destroy(trace);
                        continue;
                }
                
                trace_destroy(trace);
        }

        trace_destroy_packet(packet);

        /* And finally, print something useful to make the exercise 
	 * worthwhile */
        printf("Final count: %" PRIu64 "\n", flow_counter);
        printf("Expired flows: %" PRIu64 "\n", expired_flows);
        cout << "Number of processed packets "<< totalNumOfPackets << endl;

        expire_counter_flows(ts, true);
        delete (fm);
        // cout << "Number of Flows with unknown Labels are: " << numbUnknownLabels << endl;
        print_undefined_label_flow_info(nonFoundFlowsInGT, labels_counters);
        cout << "---------------------------------------------------------------------------------------------\n\n";
        cout << "Number of processed Flows with less than "<< unsigned(mainthreshold) << " packets:\t" << less_ThresholdC << endl;
        cout << "Total number of processed Flows INCLUDING less than "<< unsigned(mainthreshold) << " packets:\t" << processed_flows << endl;

        // cout << "TOTAL Time consumption of RTs processing: "
        //  << rtsTotalDuration.count()/10000 << " MilliSeconds" << endl;

        cout << "TOTAL Time consumption of GTfile processing: "
         << gtfTotalDuration.count()/1000 << " MilliSeconds" << endl;

        cout << "TOTAL Time consumption of ANN (prediction) processing: "
         << annTotalDuration.count()/1000 << " MilliSeconds" << endl;

        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
            cout << "Time consumption of the implementation: "
         << duration.count()/1000000 << " Seconds" << endl;
        free(RtsPlugIns);
        return 0;
}
