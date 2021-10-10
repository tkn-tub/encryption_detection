""" This script analyize the resutls from a binary classification file which contains in the header:
FlowID	Src_ip_address	Dst_ip_address	pro	s_p	d_p	label	pred	pckts
it produces as output the following
Descritptive info about the number of flows:
    Total number of flows
    Flows of encrypted traffic
    Flows of non-encrypted traffic
MAIN OUTPUT
Barchart of the performance comparison: for only 5 packets flows vs. including less than 5 packets flows
Used for T1F22 and T2F22 combined 
"""

from sklearn.metrics import precision_score, recall_score, f1_score, accuracy_score
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from configparser import ConfigParser

configFile = 'config.ini'
conf = ConfigParser()
conf.read(configFile)
resultsFile = conf['File_parameters']['ResultsFile']
results_barFile = conf['File_parameters']['ResultsBarchartFile']

font = {'family': 'serif',
    'color':  'darkred',
    'weight': 'normal',
    'size': 13,
    }

def get_results_analysis(resultsFile):

    datasetfile = pd.read_csv(resultsFile,sep='\t', comment='#')
    #/home/ahmad/structureBuilder/FinalResults/ResultsT2f8.csv
    #/home/ahmad/structureBuilder/FinalResults/ResultsT1c5.csv
    labels = datasetfile['label']
    predictions = datasetfile['pred']
    nonEncryptedCounter = 0
    y_label = []
    y_pred = []



    unknownLabels = 0
    for i, j in zip(labels, predictions):
        if i != -1 and i!= 254:
            if i == 0:
                nonEncryptedCounter+=1
            y_label.append(i)
            y_pred.append(j)
        elif i == -1:
            unknownLabels+=1
    flows = len(y_label)
    print("Total Number Of FLows (Including flows with less than 5 packets):\n", flows)
    print("Number of Non-encrypted Flows:\n", nonEncryptedCounter)
    print("Number of Encrypted Flows:\n", flows - nonEncryptedCounter)
    precision = precision_score(y_label, y_pred)
    recall = recall_score(y_label, y_pred)
    f1 = f1_score(y_label, y_pred)
    accuracy = accuracy_score(y_label, y_pred)
    return precision, recall, f1, accuracy

#This function analyze the results for flows have 5 packets execluding less than 5
def five_get_results_analysis(resultsFile):

    datasetfile = pd.read_csv(resultsFile,sep='\t', comment='#')
    labels = datasetfile['label']
    predictions = datasetfile['pred']
    packets = datasetfile['pckts']
    nonEncryptedCounter = 0
    y_label = []
    y_pred = []



    unknownLabels = 0
    for i, j, z in zip(labels, predictions, packets):
        if i != -1 and i!= 254 and z == 5:
            if i == 0:
                nonEncryptedCounter+=1
            y_label.append(i)
            y_pred.append(j)
        elif i == -1:
            unknownLabels+=1
    flows = len(y_label)
    print("Number Of FLows (Only 5 Packets):\n", flows)
    print("Number of Non-encrypted Flows:\n", nonEncryptedCounter)
    print("Number of Encrypted Flows:\n", flows - nonEncryptedCounter)
    precision = precision_score(y_label, y_pred)
    recall = recall_score(y_label, y_pred)
    f1 = f1_score(y_label, y_pred)
    accuracy = accuracy_score(y_label, y_pred)
    return precision, recall, f1, accuracy

def combined_barchart(rfile, resultsList, fiveresultsList):
    n_groups = 4
    performance = [float("{:,.2f}".format(ele*100)) for ele in resultsList]
    performanceF = [float("{:,.2f}".format(ele*100)) for ele in fiveresultsList]

    parameters = ["Precision", "Recall", "F1", "Accuracy"]
    # create plot
    fig, ax = plt.subplots()
    index = np.arange(n_groups)
    bar_width = 0.35
    opacity = 0.8

    rects1 = ax.bar(index - bar_width/2, performance, bar_width,
    alpha=opacity,
    color='b',
    label='Including less than 5 packets')

    rects2 = ax.bar(index + bar_width/2, performanceF, bar_width,
    alpha=opacity,
    color='g',
    label='Only 5 packets')

    ax.set_ylabel("Performance (%)", fontdict=font)
    ax.set_xlabel("Parameters", fontdict=font)
    #ax.set_title('Results Analysis', fontdict=font)
    ax.set_xticks(index)
    ax.set_xticklabels(parameters)
    ax.legend(loc=10)

    ax.bar_label(rects1, padding=3)
    ax.bar_label(rects2, padding=3)
    plt.legend(bbox_to_anchor =(0.8, 1.1), ncol = 3, fontsize=9)
    fig.tight_layout()
    plt.savefig(rfile)
resultsList = get_results_analysis(resultsFile)
fiveresultsList = five_get_results_analysis(resultsFile)

combined_barchart(results_barFile, resultsList, fiveresultsList)