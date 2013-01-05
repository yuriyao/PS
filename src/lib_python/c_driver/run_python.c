/**
 *  运行python文件
 **/
#include <python2.6/Python.h>

int 
main(int argc, char **argv)
{
	FILE *fp;
	if(argc < 2)
	{
		printf("Usage: run a.py\n");
		exit(-1);
	}

	Py_SetProgramName(argv[0]);
	Py_Initialize();

	if((fp = fopen(argv[1], "r")) == NULL)
	{
		printf("Cannot open file %s\n", argv[1]);
		exit(-1);
	}
	PyRun_SimpleFile(fp, argv[1]);
	Py_Finalize();
	return 0;
}