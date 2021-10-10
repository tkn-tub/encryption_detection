
from sklearn.metrics import precision_score, recall_score, f1_score, accuracy_score
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from configparser import ConfigParser


configFile = '/home/ahmad/git-thesis/ResultsAnalysis/Comparison/Binary/AllResultsBinary.ini'
conf = ConfigParser()
conf.read(configFile)
resultsFile_Mein = conf['File_parameters']['ResultsFile_Mein']
resultsFile_Lpi = conf['File_parameters']['ResultsFile_Lpi']
resultsFile_Weka = conf['File_parameters']['ResultsFile_Weka']
results_barFile = conf['File_parameters']['ResultsBarchartFile']

font = {'family': 'serif',
    'color':  'darkred',
    'weight': 'normal',
    'size': 13,
    }

def get_results_analysis(resultsFile):

    datasetfile = pd.read_csv(resultsFile,sep='\t', comment='#')
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
    precision = precision_score(y_label, y_pred)
    recall = recall_score(y_label, y_pred)
    f1 = f1_score(y_label, y_pred)
    accuracy = accuracy_score(y_label, y_pred)
    return precision, recall, f1, accuracy

def combined_barchart(rfile, resultsList, DPI_List, arff_List):
    n_groups = 4

    performance = [float("{:,.1f}".format(ele*100)) for ele in resultsList]
    performanceDPI = [float("{:,.1f}".format(ele*100)) for ele in DPI_List]
    performanceWeka = [float("{:,.1f}".format(ele*100)) for ele in arff_List]
    parameters = ["Precision", "Recall", "F1", "Accuracy"]
    # create plot
    fig, ax = plt.subplots()
    index = np.arange(n_groups)
    bar_width = 0.20
    opacity = 0.8

    rects1 = ax.bar(index - 3 * bar_width/3, performance, bar_width,
    alpha=opacity,
    color='b',
    label='RTs_ANN')

    rects2 = ax.bar(index , performanceDPI, bar_width,
    alpha=opacity,
    color='g',
    label='LPI')

    rects3 = ax.bar(index + 3* bar_width/3, performanceWeka, bar_width,
    alpha=opacity,
    color='r',
    label='MLP')

    ax.set_ylabel("Performance (%)", fontdict=font)
    ax.set_xlabel("Parameters", fontdict=font)
    ax.set_xticks(index)
    ax.set_xticklabels(parameters, fontdict=font)

    ax.bar_label(rects1, padding=3, size= 5)
    ax.bar_label(rects2, padding=3, size= 5)
    ax.bar_label(rects3, padding=3, size= 5)
    plt.legend(bbox_to_anchor =(0.9, 1.1), ncol = 3, fontsize=12)
    fig.tight_layout()
    plt.savefig(rfile)

resultsList_Mein = get_results_analysis(resultsFile_Mein)

resultsList_Lpi = get_results_analysis_comma(resultsFile_Lpi)
resultsList_arrf = get_results_analysis_comma(resultsFile_Weka)

combined_barchart(results_barFile, resultsList_Mein, resultsList_Lpi, resultsList_arrf)

