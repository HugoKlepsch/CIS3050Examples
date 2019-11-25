/**
 * Embedding example from the Python docs chapter 4 "Embedding Python
 * in Another Application"
 *
 * https://docs.python.org/2.7/extending/embedding.html
 *
 * Referenced Nov 18, 2019
 */

#include <Python.h>

int
main(int argc, char *argv[])
{
    PyObject *pName, *pModule, *pFunc;
    PyObject *pArgs, *pValue;
    int i;

    if (argc < 3) {
        fprintf(stderr,"Usage: call pythonfile funcname [args]\n");
        return 1;
    }

    Py_Initialize();

    /**
     * add the current directory to the path; without this we
     * will not be able to find the module
     */
    PyObject *sysmodule = PyImport_ImportModule("sys");
    PyObject *syspath = PyObject_GetAttrString(sysmodule, "path");
    PyList_Append(syspath, PyString_FromString("."));
    Py_DECREF(syspath);
    Py_DECREF(sysmodule);


    /**
     * convert our string argument (which should be the Python
     * module name **WITHOUT** the .py extention) to a Python string
     */
    pName = PyString_FromString(argv[1]);
    if (pName == NULL || PyString_AsString(pName)[0] == '\0') {
        fprintf(stderr, "Could not convert '%s' to Python string\n", argv[1]);
        return 1;
    }
        
    
    /**
     * load the module (calls dlopen())
     */
    pModule = PyImport_Import(pName);
    Py_DECREF(pName);


    if (pModule == NULL) {
        PyErr_Print();
        fprintf(stderr, "Failed to load \"%s\"\n", argv[1]);
        return 1;
    } else {

        /**
         * use the module
         */

        pFunc = PyObject_GetAttrString(pModule, argv[2]);
        /* pFunc is a new reference */

        /** make sure that we are callable */
        if (pFunc && PyCallable_Check(pFunc)) {
            pArgs = PyTuple_New(argc - 3);
            for (i = 0; i < argc - 3; ++i) {
                pValue = PyInt_FromLong(atoi(argv[i + 3]));
                if (!pValue) {
                    Py_DECREF(pArgs);
                    Py_DECREF(pModule);
                    fprintf(stderr, "Cannot convert argument\n");
                    return 1;
                }
                /* pValue reference stolen here: */
                PyTuple_SetItem(pArgs, i, pValue);
            }
            pValue = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);
            if (pValue != NULL) {
                printf("Result of call: %ld\n", PyInt_AsLong(pValue));
                Py_DECREF(pValue);
            } else {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                PyErr_Print();
                fprintf(stderr,"Call failed\n");
                return 1;
            }
        } else {
            if (PyErr_Occurred())
                PyErr_Print();
            fprintf(stderr, "Cannot find function \"%s\"\n", argv[2]);
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    }
    Py_Finalize();
    return 0;
}

