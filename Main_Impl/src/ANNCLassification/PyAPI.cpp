#include "PyAPI.h"
#include <string>
#include <sstream>
#include <iostream>

#include <fstream>



PyAPI::PyAPI(std::string python_object_path , std::string ml_module )
{	
	PyObject *module_name , *module;
	PyObject *func;
	Py_Initialize();
    PyRun_SimpleString("import sys\n");
	// FULL PATH HAS TO BE HARD CODED
	PyRun_SimpleString("sys.path.append(\"/FULL_PATH/TO/Main_Impl\")");

    module_name = PyUnicode_DecodeFSDefault("ANNinPython.multiClassification");
	module = PyImport_Import(module_name);
	if (module == nullptr)
	{
		PyErr_Print();
		std::cerr << "Module can not be imported, check the module name and/or the full path to the module.\n Make sure that the absolute path to the module is correct line 16 of PyAPI.cpp file";
        
	}
	Py_DECREF(module_name);

	dict = PyModule_GetDict(module);
	if (dict == nullptr)
	{
		PyErr_Print();
		std::cerr << "Fails to get the dictionary.\n";

	}

	func = PyDict_GetItemString(dict, "load_object");
	object = PyObject_CallFunctionObjArgs(func, PyUnicode_FromString(python_object_path.c_str()), NULL);
	if(object == NULL)
        {
            PyErr_Print();
		    std::cerr << "Can not call function from module.\n";
        }
    Py_DECREF(module);
	
}


PyAPI::~PyAPI()
{
	
	Py_DECREF(object);
    Py_DECREF(dict);	
	
	Py_Finalize();
}


PyObject *PyAPI::get_object()
{
	return object;
}

int  PyAPI::get_label(std::string sample)
{
	
	PyObject *func ;
    func = PyDict_GetItemString(dict, "predict_class");
	PyObject *pValue;
	PyObject *data = PyUnicode_InternFromString(sample.c_str());


    pValue = PyObject_CallFunctionObjArgs(func, object, data, NULL);
	if (pValue != NULL)
	{
		
	    
		// std::cout << PyLong_AsLong(pValue)<<std::endl;
		return PyLong_AsLong(pValue);
		//Py_DECREF(pValue);
	}
	if(PyErr_Occurred())
			PyErr_Print();
            
	
	return -1;

}

