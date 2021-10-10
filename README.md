# Installations
In this section, I explain in details all the required libraries and packages for the implementation.

+ Libtrace and LibFlowManager libraries can be installed according to the in-structions in the corresponding documentations, which are publicly availablein GitHub
    - [Libtrace](https://github.com/LibtraceTeam/libtrace/wiki/Installing-Libtrace)
    - [LibFlowManager](https://github.com/wanduow/libflowmanager/wiki/Installing-Debian-Packaged-Version).
+ For embedding R in CPP, I used the RInside library along with Rcpp.  Thus,installing R programming language with the mentioned libraries is required.

    * Install R with the following commands:
        ```
        sudo  apt  update
        sudo  apt  install r-base
        ```
    * After installing R, the packages RInside and Rcpp can be installed fromR shell. As shown in the following commands.
        ```
        sudo R
        > install.packages("Rcpp")
        > install.packages("RInside")
        ```
    * install the CryptRndTest package: the package is archived along with most of its dependencies, so it can not be installed from R shell as the standard packages. There is a workaround for installation which has to be used for each dependency ofCryptRndTest until having all dependencies installed. [This_link](https://stackoverflow.com/questions/24194409/how-do-i-install-a-package-that-has-been-archived-from-cran)
+ For embedding Python inside CPP, the python3 has to be installed. The mainapplication is tested on python3.8.
    * It can be installed with the following command.
        ```
        sudo  apt  install  python3 .8
        ```
    * Some required python3 libraries can be installed with pip3 packagemanager:
        ```
        pip3 install seaborn2pip3 
        install --upgrade keras3
        pip3 install --upgrade keras4
        pip3 install --upgrade tensorflow5
        pip3 install matplotlib6
        pip3 install sklearn7
        pip3 install imblearn8
        pip3 install numpy9
        pip3 install pandas
        ```

# Configurations
+ An important manual configuration in the code must be done in the file Main_impl/src/ANNCLassification/PyAPI.cpp where the full path of the ANN python module must be hard coded in `line 17`.
    ```
    	// FULL PATH HAS TO BE HARD CODED
	PyRun_SimpleString("sys.path.append(\"/FULL_PATH/TO/Main_Impl\")");
    ```

+ The configurations can be done through the Configuration.ini file , which is read directly from the main application,  without the need of compilation after modifications, some hints on using the configuration file are listed:
    - The commented lines must start with the hash symbol (#).
    - The files’ paths must be absolute paths and NOT relative paths.
    - The variables’ names must be unique in the file and can not be used as values.
    - The file itself contains some important comments, which must be considered
+ Configurations for training is done with Main_Impl/ANNinPython/config.ini file.The following parameters should be specified to create multiple designs of ANN todo the training on them. These parameters define the range for creating a matrixthat contains the number of hidden layers and the number of Neurons (in eachhidden layer) of the dense network. Moreover, the number of features, classes, andthe path to save the best model can be configured in this file.
    -  HiddenLayers: The Maximum number of hidden layers
    -  HlStep: this is the increasing step of the number of hidden layers until reaching the maximum value, considering the starting value of hidden layers is 2 (hard-coded).
    -  Neurons: the Maximum number of neurons
    -  NStep: this is the increasing step of the number of neurons until reaching the maximum value considering the starting value of neurons is 20 (hard-coded).

    -  EpochsP: The Number of Epochs considered, avoiding overfitting (same used for training and validation).

    -  NumOfFeatures: the number of features used in training. This number must be consistent with the number of features in the training file.

    -  File: The path of the .csv file, which contains the training dataset, according to our Implementation should be (features, label)

    -  ModelFile: the path of the model file, the name includes a representative numbers of the chosen number of hidden layers and the number of neurons as <NumberofHiddenLayers"X"NumberOfNeurons> e.g. 4X60.
    -  Heatmap\_file: The path of the .pdf file to create the heatmap that shows the accuracies of the models' performances.
    -  Classes: the number of output classes, i.e. for binary classification there must be only two classes.
# MakeFile
This file must be checked before running the implementation where all the included libraries paths must be checked in the system before compilation to avoid any errors.
# Run
### Running the Main_Impl
After installing all the required libraries and packages, as well as setting up the configuration file, the cpp code can be compiled and run with the follwoing command, considering that the configuraiton file is passed as an argument to the compiled file.
**Note that the same output files in configuration file will be overritten when running the implementation.**
```
make
./bin/main Configuration.ini
```
### Running Training
Running the script can be done from the command line with the following command.
```
python3 training.py
```





