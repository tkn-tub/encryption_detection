import numpy as np
import pandas as pd
import keras
from configparser import ConfigParser
import matplotlib.pyplot as plt
import seaborn as sb
from tensorflow import keras
from keras.models import Sequential
from keras.layers import Dense
from tensorflow.keras.wrappers.scikit_learn import KerasClassifier
from sklearn.model_selection import cross_val_score
import statistics

#Extract parameters from config file
configFile = 'config.ini'
conf = ConfigParser()
conf.read(configFile)
print(conf.sections)
hiddenLayers = conf['NN_Parameters']['HiddenLayers']
hlStep = conf['NN_Parameters']['HlStep']
neurons = conf['NN_Parameters']['Neurons']
nStep = conf['NN_Parameters']['NStep']
epochsP = conf['NN_Parameters']['EpochsP']
trainingFile = conf['File_parameters']['File']
numOfFeatures = conf['File_parameters']['NumOfFeatures']
heatmap = conf['File_parameters']['Heatmap_file']
modelPath = conf['File_parameters']['ModelFile']
classes_num = conf['File_parameters']['classes']

classes = int(classes_num)
hiddenLayersNumber = int(hiddenLayers)
hiddenLayersStep = int(hlStep)
neuronsNumber = int(neurons)
neuronsStep = int(nStep)
Epochs = int(epochsP)
numberOfFeatures = int(numOfFeatures)
training_file = trainingFile
heatmapFile = heatmap

def create_design_matrix():
    hiddenLayers = []
    neurons = []
    for i in range(2, int(hiddenLayersNumber) + int(hiddenLayersStep), int(hiddenLayersStep)):
        hiddenLayers.append(i)
    for j in range(20, int(neuronsNumber)+ int(neuronsStep), int(neuronsStep)):
        neurons.append(j)

    hnTupleList = [] # list of tuples each tuple has two parameters (numberOfhiddenLayers, numberOfNeurons)
    for h in hiddenLayers:
        for n in neurons:
            hnTupleList.append((h,n))
    return hnTupleList

def get_hiddenlayers_neurons_lists():
    hiddenLayers = []
    neurons = []
    for i in range(2, int(hiddenLayersNumber) + int(hiddenLayersStep), int(hiddenLayersStep)):
        hiddenLayers.append(i)
    for j in range(20, int(neuronsNumber)+ int(neuronsStep), int(neuronsStep)):
        neurons.append(j)
    return hiddenLayers, neurons

def data_preprocessor(csv_file):
    datasetfile = pd.read_csv(csv_file, comment='#')

    labels = datasetfile['Label']
    features = datasetfile.drop(columns=['Label'])
    features = features.values.astype('float32')
    labelsP= labels.values.astype('float32')
    labels = keras.utils.to_categorical(labelsP, num_classes=classes)
    return features, labels

features, labels = data_preprocessor(training_file)

"""Creating compiled model with input tuple of numberOfHiddenLayers and NumberOfneurons
"""
def create_model(hntuple):
    h, n = hntuple
    model = Sequential()
    #Input layer and first hidden layer.
    model.add(Dense(n, kernel_initializer = 'uniform', input_shape=(numberOfFeatures,), activation='relu'))


    for i in range(h-1):
        #2nd hidden layer
        model.add(Dense(n, kernel_initializer = 'uniform', activation='relu'))

    # #Output layer
    model.add(Dense(classes, kernel_initializer = 'uniform', activation='softmax'))

    model.compile(loss='categorical_crossentropy', optimizer='adam', metrics=['accuracy'])
    #model.fit(features, labels_train, epochs=Epochs, batch_size=10)

    return model
"""This function traines and saves model accoding to a givin tuple contains
the numberOfHiddenLayers and numberOfNeurons"""
def save_model(hntuple):

    modelfile = modelPath + '_Model_' + str(hntuple[0]) + 'X' + str(hntuple[1]) + '.h5'
    model = create_model(hntuple)
    model.fit(features, labels, epochs=Epochs, batch_size=10)
    model.save(modelfile)
    print("\nModel with path" + modelfile +  " is saved..\n")
    return modelfile


"""This function validates model according to a given tuple contains
the numberOfHiddenLayers and numberOfNeurons
it uses 10 fold cross validation which gives an output of 10 values
its output is the average of these 10 values"""
def validate_model(hntuple):

    classifier = KerasClassifier(build_fn = create_model, hntuple=hntuple, batch_size = 10, epochs = Epochs)
    results = cross_val_score(estimator = classifier, X = features, y = labels, cv = 10)
    average = statistics.mean(results)
    return average

def heatmap_file(accuracyList, heatmapFIle):
    hiddenLayers, neurons = get_hiddenlayers_neurons_lists()
    accuracyListCounter = 0
    accuracies = []
    for i in range(len(hiddenLayers)):
        alist = []
        for j in range(len(neurons)):
            # print(accuracyList[accuracyListCounter])
            # print(i,j)
            # accuracies[i][j] = accuracyList[accuracyListCounter]
            # print(accuracies[i][j])
            alist.append(accuracyList[accuracyListCounter])
            accuracyListCounter+=1
        accuracies.append(alist)

    accuracies = np.array(accuracies)

    heat_map = sb.heatmap(accuracies, annot=True, cmap="YlGnBu", cbar_kws={'label': 'Model Performance', 'orientation': 'horizontal'}, fmt='.2f', annot_kws={'size': 6})
    heat_map.set_yticklabels(hiddenLayers, rotation=0)
    heat_map.set_xticklabels(neurons, rotation=0)
    plt.xlabel("Neurons")
    plt.ylabel("Hidden Layers")
    plt.savefig(heatmapFIle)

hnTupleList = create_design_matrix()

models_performance = {}
accuracies = []
for i in range(len(hnTupleList)):
    
    hntuple = hnTupleList[i]
    print("Validating tuple {} (hidden layers and neurons)..\n\n Iteration {} of {} ..\n\n\n".format(hntuple, i+1, len(hnTupleList)))
    accuracy = validate_model(hntuple)
    models_performance.update({hntuple: accuracy})
    accuracies.append(accuracy)
    print("Accuracy after validation is :\t{}".format(accuracy))

hntupleOfMaxAccuracy = max(models_performance, key=models_performance.get)
modelFile = save_model(hntupleOfMaxAccuracy)

print("Model Validation completed..\n Model SAVED with highest accuracy is:  {}\n".format(modelFile))
# for key, value in models_performance.items():
# 	    print("Tuple {} has accuracy of  {}\n".format(key, value))
accuraciesList = []
for i in accuracies:
    i*=100
    accuraciesList.append(i)


heatmap_file(accuraciesList, heatmapFile)
