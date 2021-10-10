""" This script analyize the resutls from a binary classification file which contains in the header:
FlowID	Src_ip_address	Dst_ip_address	pro	s_p	d_p	label	pred	pckts
it produces as output the following
Descritptive info about the number of flows:
    Total number of flows
    Flows of encrypted traffic
    Flows of non-encrypted traffic
MAIN OUTPUT
Barchart of the performance comparison when for only 5 packets flows vs. including less than 5 packets flows
Used for T1F16 and T2F16 combined or T1F22 and T2F22 combined 
(better to use it on the one which shows bigger differenece to discuss the point of having difference in two cases"""


from sklearn.metrics import confusion_matrix
from sklearn.metrics import precision_score, recall_score, f1_score, accuracy_score
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from configparser import ConfigParser

configFile = 'config_content.ini'
conf = ConfigParser()
conf.read(configFile)
resultsFileM = conf['File_parameters']['ResultsFile']
resultsFileW = conf['File_parameters']['ResultsFile1']
results_barFile = conf['File_parameters']['ResultsBarchartFile']
results_barFile1 = conf['File_parameters']['ResultsBarchartFile1']
results_barFile2 = conf['File_parameters']['ResultsBarchartFile2']
font = {'family': 'serif',
    'color':  'darkred',
    'weight': 'normal',
    'size': 15,
    }
font2 = {'family': 'serif',
    'color':  'darkred',
    'weight': 'normal',
    'size': 15,
    }
metric = "precision"
def get_results_analysis(resultsFile):

    datasetfile = pd.read_csv(resultsFile,sep='\t', comment='#')
    labels = datasetfile['label']
    predictions = datasetfile['pred']
    protocol = datasetfile['pro']
    packets = datasetfile['pckts']
    y_label = []
    y_pred = []



    unknownLabels = 0
    for i, j, p ,k in zip(labels, predictions, protocol, packets):
        if i != -1 and i!= 254 and p == 6:
            y_label.append(i)
            y_pred.append(j)
        elif i == -1:
            unknownLabels+=1

    print("number of unknown labels i.e. not analyized predictoins")
    print(unknownLabels)

    precision = precision_score(y_label, y_pred, average=None)
    recall = recall_score(y_label, y_pred, average=None)
    f1 = f1_score(y_label, y_pred, average=None)
    accuracy = accuracy_score(y_label, y_pred)
    return precision, recall, f1, accuracy


def get_results_analysis_comma(resultsFile):

    datasetfile = pd.read_csv(resultsFile,sep=',', comment='#')

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
    precision = precision_score(y_label, y_pred, average=None)
    recall = recall_score(y_label, y_pred, average=None)
    f1 = f1_score(y_label, y_pred, average=None)
    accuracy = accuracy_score(y_label, y_pred)
    return precision, recall, f1, accuracy

def combined_barchart(rfile, resultsList, fiveresultsList):
    n_groups = 14

    performance = [float("{:,.1f}".format(ele*100)) for ele in resultsList]
    performanceF = [float("{:,.1f}".format(ele*100)) for ele in fiveresultsList]

    parameters = ["au", "txt", "mp3", "pdf", "wav", "png", "xls", "csv", "webm", "mat", "zip", "jpg", "mp4", "Enc"]
    # create plot
    fig, ax = plt.subplots()
    index = np.arange(n_groups)
    bar_width = 0.40
    opacity = 0.8

    rects1 = ax.bar(index - bar_width/2, performance, bar_width,
    alpha=opacity,
    color='b',
    label='RTs_ANN')

    rects2 = ax.bar(index + bar_width/2, performanceF, bar_width,
    alpha=opacity,
    color='g',
    label='MLP')
    if metric == "precision":
        print("we are here")
        ax.set_ylabel("Precision (%)", fontdict=font)
    if metric == "recall":
        ax.set_ylabel("Recall (%)", fontdict=font)
    if metric == "f1":
        ax.set_ylabel("F1 (%)", fontdict=font)
    ax.set_xlabel("Classes", fontdict=font)
    #ax.set_title('Results Analysis', fontdict=font)
    ax.set_xticks(index)
    ax.set_xticklabels(parameters, fontdict=font2, rotation=45)
    #ax.legend(loc=10)

    ax.bar_label(rects1, padding=3, size= 7)
    ax.bar_label(rects2, padding=3, size= 7)
    #ax.annotate(fontsize= )
    plt.legend(bbox_to_anchor =(0.9, 1.15), ncol = 5, fontsize=12)
    fig.tight_layout()
    plt.savefig(rfile)
precision, recall, f1, accuracy = get_results_analysis(resultsFileM)
precisionWeka, recallWeka, f1Weka, accuracyWeka = get_results_analysis_comma(resultsFileW)

combined_barchart(results_barFile, precision, precisionWeka)
metric = "recall"
combined_barchart(results_barFile1, recall, recallWeka)
metric = "f1"

combined_barchart(results_barFile2, f1, f1Weka)