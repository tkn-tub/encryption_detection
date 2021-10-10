""" Comparison between three results: 
Protocol Classifier
Lpi_th2p6 output
TIE: the weka ouput is used
produces three pdf files for the performance parameters
precision, recall and f1
"""


from sklearn.metrics import confusion_matrix
from sklearn.metrics import precision_score, recall_score, f1_score, accuracy_score
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from configparser import ConfigParser

configFile = 'config_Proto_2thp6.ini'
conf = ConfigParser()
conf.read(configFile)

resultsFile = conf['File_parameters']['ResultsFile']
resultsFile1 = conf['File_parameters']['ResultsFile1']

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

    # conf_matrix = confusion_matrix(y_true=y_label, y_pred=y_pred)
    precision = precision_score(y_label, y_pred, average=None)
    recall = recall_score(y_label, y_pred, average=None)
    f1 = f1_score(y_label, y_pred, average=None)
    accuracy = accuracy_score(y_label, y_pred)
    return precision, recall, f1, accuracy

precision, recall, f1, accuracy = get_results_analysis(resultsFile)



print("Class \t\tPrecision \t\tRecall \t\tF1")

for i in range(len(precision)):
    print("Class{}:\t{}\t{}\t{}\n".format( i, precision[i], recall[i], f1[i]))


print("Accuracy is :", accuracy)



def combined_barchart(rfile, resultsList, th2p6_List):
    n_groups = 6

    performance = [float("{:,.1f}".format(ele*100)) for ele in resultsList]
    performanceth2p6 = [float("{:,.1f}".format(ele*100)) for ele in th2p6_List]

    parameters = ["FTP", "NETCAT", "HTTP", "SCP", "HTTPS", "SFTP"]
    # create plot
    fig, ax = plt.subplots()
    index = np.arange(n_groups)
    bar_width = 0.20
    opacity = 0.8

    rects1 = ax.bar(index - 3 * bar_width/3, performance, bar_width,
    alpha=opacity,
    color='b',
    label='RTs_ANN_5_Packets')

    rects2 = ax.bar(index , performanceth2p6, bar_width,
    alpha=opacity,
    color='g',
    label='RTs_ANN_2_Packets')


    if metric == "precision":
        print("we are here")
        ax.set_ylabel("Precision (%)", fontdict=font)
    if metric == "recall":
        ax.set_ylabel("Recall (%)", fontdict=font)
    if metric == "f1":
        ax.set_ylabel("F1 (%)", fontdict=font)
    ax.set_xlabel("Classes", fontdict=font)

    ax.set_xticks(index)
    ax.set_xticklabels(parameters, fontdict=font2, rotation= 45)
    #ax.legend(loc=10)

    ax.bar_label(rects1, padding=3, size= 5)
    ax.bar_label(rects2, padding=3, size= 5)
    #ax.annotate(fontsize= )
    plt.legend(bbox_to_anchor =(0.9, 1.15), ncol = 3, fontsize=12)
    fig.tight_layout()
    plt.savefig(rfile)

precision1, recall1, f11, accuracy1 = get_results_analysis(resultsFile1)

for i in range(len(precision1)):
    print("Class{}:\t{}\t{}\t{}\n".format( i, precision1[i], recall1[i], f11[i]))



print("Accuracy is :", accuracy1)


combined_barchart(results_barFile, precision, precision1)
metric = "recall"

combined_barchart(results_barFile1, recall, recall1)
metric = "f1"

combined_barchart(results_barFile2, f1, f11)
