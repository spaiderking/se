//
// DropboxAPI.cpp
// BlindStorage
//

#include "DropboxAPI.h"

DropboxAPI::DropboxAPI() : pythonfile("Dropbox"), pythonClass("DropboxAPI"){
	int i;
	Py_Initialize();
 	pName = PyString_FromString(pythonfile.c_str());
	pModule = PyImport_Import(pName);
	pDict = PyModule_GetDict(pModule);
	pClass = PyDict_GetItemString(pDict, pythonClass.c_str());

	if(PyCallable_Check(pClass))
		pInstance = PyObject_CallObject(pClass, NULL);
}

DropboxAPI::~DropboxAPI(){
	Py_DECREF(pModule);
	Py_DECREF(pName);
	Py_Finalize();
}

void DropboxAPI::upload(string filenames, char* file, size_t filesize){
	char function[] = "upload";
	
	char filenamesChars[filenames.size()];
	memcpy(filenamesChars, filenames.c_str(), filenames.size());
	
	pValue = PyObject_CallMethod(pInstance, function, "(ssi)", filenamesChars, file, filesize);

	if(pValue != NULL){
		printf("Return of call : %d\n", PyInt_AsLong(pValue));
		Py_DECREF(pValue);
	}
	else{
		PyErr_Print();
	}
}

void DropboxAPI::download(string filenames, uint32_t numFiles, char* file, size_t filesize){
	char function[] = "download";

	char filenamesChars[filenames.size()];
	memcpy(filenamesChars, filenames.c_str(), filenames.size());
	
	pValue = PyObject_CallMethod(pInstance, function, "(si)", filenamesChars, filesize);

	if(pValue != NULL){
		PyObject* objectsRepresentation = PyObject_Repr(pValue);
		const char* downloadedFile = PyString_AsString(objectsRepresentation);
		memcpy(file, downloadedFile, numFiles*filesize);
		
		printf("Return of call : %d\n", PyInt_AsLong(pValue));
		Py_DECREF(pValue);
	}
	else{
		PyErr_Print();
	}
}

int main(){
	char file[256*4];
	memset(file, 'e', 256*4);
	DropboxAPI dbapi;
	dbapi.upload("BLOCK23:BLOCK20:BLOCK22:BLOCK21", file, 256);

	DropboxAPI dbapi1;
	char* downloadfile = new char[256*4];
	dbapi1.download("BLOCK20:BLOCK21:BLOCK23:BLOCK22", 4, downloadfile, 256);
	printf("%s\n", downloadfile);
//	cout << "Size of the download file is " << downloadsize << endl;
	delete[] downloadfile;
	return 0;
}
