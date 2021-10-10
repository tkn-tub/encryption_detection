
from sklearn.metrics import precision_score, recall_score, f1_score, accuracy_score
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from configparser import ConfigParser


configFile = 'TwoFilesConf.ini'
conf = ConfigParser()
conf.read(configFile)
resultsFile16 = conf['File_parameters']['ResultsFile_16']
resultsFile22 = conf['File_parameters']['ResultsFile_22']

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


def combined_barchart(rfile, resultsList16, resultsList22):
    n_groups = 4
    # performance = [float("{:,.2f}".format(ele*100)) for ele in resultsList]
    # performanceF = [float("{:,.2f}".format(ele*100)) for ele in fiveresultsList]
    performance16 = [float("{:,.2f}".format(ele*100)) for ele in resultsList16]
    performance22 = [float("{:,.2f}".format(ele*100)) for ele in resultsList22]
    #parameters = ["Precision", "Recall", "F1", "Accuracy"]
    parameters = ["Precision", "Recall", "F1", "Accuracy"]
    # create plot
    fig, ax = plt.subplots()
    index = np.arange(n_groups)
    bar_width = 0.35
    opacity = 0.8

    rects1 = ax.bar(index - bar_width/2, performance16, bar_width,
    alpha=opacity,
    color='b',
    label='16Features')

    rects2 = ax.bar(index + bar_width/2 , performance22, bar_width,
    alpha=opacity,
    color='g',
    label='22Features')


    ax.set_ylabel("Performance (%)", fontdict=font)
    ax.set_xlabel("Parameters", fontdict=font)
    #ax.set_title('Results Analysis', fontdict=font)
    ax.set_xticks(index)
    ax.set_xticklabels(parameters, fontdict=font)
    #ax.legend(loc=10)

    ax.bar_label(rects1, padding=3)
    ax.bar_label(rects2, padding=3)
    plt.legend(bbox_to_anchor =(0.8, 1.1), ncol = 3, fontsize=9)
    fig.tight_layout()
    plt.savefig(rfile)

resultsList16 = get_results_analysis(resultsFile16)
resultsList22 = get_results_analysis(resultsFile22)


combined_barchart(results_barFile, resultsList16, resultsList22)

