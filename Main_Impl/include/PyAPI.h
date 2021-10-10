#pragma once
#define PY_SSIZE_T_CLEAN

#include <Python.h>
#include <string>


class PyAPI
{

	PyObject  *object, *dict;

public:
	PyAPI(std::string python_object_path, std::string ml_module);
	PyAPI(std::string file_path, float size,std::string python_object_path, std::string ml_module );

	~PyAPI();
	PyObject *get_object();
	int  get_label(std::string sample);
};

