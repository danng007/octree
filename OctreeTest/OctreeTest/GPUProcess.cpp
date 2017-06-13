#include "GPUProcess.h"
GPUProcess::GPUProcess()
{
}
GPUProcess::~GPUProcess()
{
}
void GPUProcess::getFileValues(vector<vector<float>> valuesIn, int rowsIn, int rowSizeIn) {
	fileValues = valuesIn;
	fileRows = rowsIn;
	rowSize = rowSizeIn;
	fileSize = fileRows * rowSize;
	outputRowSize = rowSize + 11;//box parameters:3 levels index: 8 
	resultSize = fileRows * outputRowSize;
	resizeFileValue();
}
void GPUProcess::resizeFileValue() {
	fileValue1D = (float *)malloc(fileSize * sizeof(float));
	for (int i = 0; i < fileRows; i++)
	{
		for (int j = 0; j < rowSize; j++)
		{
			fileValue1D[i*rowSize + j] = fileValues[i][j];
		}
	}
	result1D = (float *)malloc(resultSize * sizeof(float));
}
int GPUProcess::convertToString(const char *filename, std::string& s)
{
	size_t size;
	char* str;
	std::fstream f(filename, (std::fstream::in | std::fstream::binary));
	if (f.is_open())
	{
		size_t fileSize;
		f.seekg(0, std::fstream::end);
		size = fileSize = (size_t)f.tellg();
		f.seekg(0, std::fstream::beg);
		str = new char[size + 1];
		if (!str)
		{
			f.close();
			return NULL;
		}
		f.read(str, fileSize);
		f.close();
		str[size] = '\0';
		s = str;
		delete[] str;
		return 0;
	}
	printf("Error: Failed to open file %s\n", filename);
	return 1;
}
void GPUProcess::outputResultToFile() {
	ofstream outFile;
	outFile.open("D:/LaserImaging/dataFiles/GPUtest/resultOut.txt");
	for (int i = 0; i < fileRows; i++)
	{
		for (int j = 0; j < outputRowSize; j++) {
			outFile << result1D[i*outputRowSize + j] << " ";
		}
		outFile << "\n";
	}
}

void GPUProcess::run() {
	cl_uint status;
	cl_platform_id platform;

	//Initial platform 
	status = clGetPlatformIDs(1, &platform, NULL);
	cl_device_id device;
	//Initial GPU Device
	clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
	//Initial context
	cl_context context = clCreateContext(NULL, 1, &device, NULL, NULL, NULL);
	//Initial Command Queue
	cl_command_queue commandQueue = clCreateCommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, NULL);
	if (commandQueue == NULL)
		perror("Failed to create commandQueue for device 0.");

	//Initial OpenCL memory objects
	cl_mem memObjects[2] = { 0, 0 };
	memObjects[0] = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(float) * fileSize, fileValue1D, NULL);
	memObjects[1] = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(float) * resultSize, result1D, NULL);
	if (memObjects[0] == NULL || memObjects[1] == NULL)
		perror("Error in clCreateBuffer.\n");

	// Load the kernel source code into the array source_str
	const char * filename = "processValueKernel.cl";
	string sourceStr;
	status = convertToString(filename, sourceStr);
	if (status)
		cout << status << " !!!!!!!!" << endl;
	const char * source = sourceStr.c_str();
	size_t sourceSize[] = { strlen(source) };
	printf("kernel loading done\n");
	//Initial Program object
	cl_program program = clCreateProgramWithSource(context, 1, &source, sourceSize, NULL);

	//Edit Program object
	status = clBuildProgram(program, 1, &device, NULL, NULL, NULL);
	if (status)
		cout << status << " !!!!!!!!" << endl;
	if (status != 0)
	{
		printf("clBuild failed:%d\n", status);
		char tbuf[0x10000];
		clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0x10000, tbuf,
			NULL);
		printf("\n%s\n", tbuf);
		//return −1;
	}
	//Initial Kernel
	cl_kernel kernel = clCreateKernel(program, "processValue", NULL);

	//Pass Kernel arguments
	cl_int clnum = 5;
	status = clSetKernelArg(kernel, 0, sizeof(int), &fileRows);
	if (status)
		cout << "Arguments 1 Wrong" << endl;
	status = clSetKernelArg(kernel, 1, sizeof(int), &rowSize);
	if (status)
		cout << "Arguments 2 Wrong" << endl;
	status = clSetKernelArg(kernel, 2, sizeof(cl_mem), &memObjects[0]);
	if (status)
		cout << "Arguments 3 Wrong" << endl;
	status = clSetKernelArg(kernel, 3, sizeof(cl_mem), &memObjects[1]);
	if (status)
		cout << "Arguments 4 Wrong" << endl;
	status = clSetKernelArg(kernel, 4, sizeof(int), &outputRowSize);
	if (status)
		cout << "Arguments 4 Wrong" << endl;

	//Run kernel
	size_t global[2];
	cl_event prof_event;
	cl_ulong ev_start_time = (cl_ulong)0;
	cl_ulong ev_end_time = (cl_ulong)0;
	double rum_time;
	global[0] = (size_t)fileRows;
	global[1] = (size_t)rowSize;
	status = clEnqueueNDRangeKernel(commandQueue, kernel, 2, NULL,
		global, NULL, 0, NULL, &prof_event);
	if (status)
		cout << "Error in Run Kernel" << endl;
	clFinish(commandQueue);

	//Count Running Time
	//status = clGetEventProfilingInfo(prof_event, CL_PROFILING_COMMAND_QUEUED,
	//	sizeof(cl_ulong), &ev_start_time, NULL);
	//status = clGetEventProfilingInfo(prof_event, CL_PROFILING_COMMAND_END,
	//	sizeof(cl_ulong), &ev_end_time, NULL);
	//if (status)
	//	perror("Error in time count\n");
	//rum_time = (double)(ev_end_time - ev_start_time);
	//cout << "Processing time:" << rum_time << endl;

	//Read data to CPU
	status = clEnqueueReadBuffer(commandQueue, memObjects[1], CL_TRUE, 0,
		sizeof(float)* resultSize, result1D, 0, NULL, NULL);
	if (status)
		perror("Error in saving result to CPU\n");


	outputResultToFile();

	free(fileValue1D);
	free(result1D);
	clReleaseMemObject(memObjects[1]);
	clReleaseMemObject(memObjects[0]);
	clReleaseProgram(program);
	clReleaseCommandQueue(commandQueue);
	clReleaseContext(context);



	//const int LIST_SIZE = 1024;
	//int *A = (int*)malloc(sizeof(int)*LIST_SIZE);
	//int *B = (int*)malloc(sizeof(int)*LIST_SIZE);
	//for (int i = 0; i < LIST_SIZE; i++) {
	//	A[i] = i;
	//	B[i] = LIST_SIZE - i;
	//}

	//FILE *fp;
	//char *source_str;
	//size_t source_size;

	//fp = fopen("vector_add_kernel.cl", "r");
	//if (!fp) {
	//	fprintf(stderr, "Failed to load kernel.\n");
	//	exit(1);
	//}
	//source_str = (char*)malloc(MAX_SOURCE_SIZE);
	//source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
	//fclose(fp);
	//printf("kernel loading done\n");

	//cl_device_id device_id = NULL;
	//cl_uint ret_num_devices;
	//cl_uint ret_num_platforms;


	//cl_int ret = clGetPlatformIDs(0, NULL, &ret_num_platforms);
	//cl_platform_id *platforms = NULL;
	//platforms = (cl_platform_id*)malloc(ret_num_platforms * sizeof(cl_platform_id));

	//ret = clGetPlatformIDs(ret_num_platforms, platforms, NULL);
	//printf("ret at %d is %d\n", __LINE__, ret);

	//ret = clGetDeviceIDs(platforms[1], CL_DEVICE_TYPE_ALL, 1,
	//	&device_id, &ret_num_devices);
	//printf("ret at %d is %d\n", __LINE__, ret);
	//// Create an OpenCL context
	//cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
	//printf("ret at %d is %d\n", __LINE__, ret);

	//// Create a command queue
	//cl_command_queue command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
	//printf("ret at %d is %d\n", __LINE__, ret);

	//// Create memory buffers on the device for each vector 
	//cl_mem a_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY,
	//	LIST_SIZE * sizeof(int), NULL, &ret);
	//cl_mem b_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY,
	//	LIST_SIZE * sizeof(int), NULL, &ret);
	//cl_mem c_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
	//	LIST_SIZE * sizeof(int), NULL, &ret);

	//// Copy the lists A and B to their respective memory buffers
	//ret = clEnqueueWriteBuffer(command_queue, a_mem_obj, CL_TRUE, 0,
	//	LIST_SIZE * sizeof(int), A, 0, NULL, NULL);
	//printf("ret at %d is %d\n", __LINE__, ret);

	//ret = clEnqueueWriteBuffer(command_queue, b_mem_obj, CL_TRUE, 0,
	//	LIST_SIZE * sizeof(int), B, 0, NULL, NULL);
	//printf("ret at %d is %d\n", __LINE__, ret);

	//printf("before building\n");
	//// Create a program from the kernel source
	//cl_program program = clCreateProgramWithSource(context, 1,
	//	(const char **)&source_str, (const size_t *)&source_size, &ret);
	//printf("ret at %d is %d\n", __LINE__, ret);

	//// Build the program
	//ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
	//printf("ret at %d is %d\n", __LINE__, ret);

	//printf("after building\n");
	//// Create the OpenCL kernel
	//cl_kernel kernel = clCreateKernel(program, "vector_add", &ret);
	//printf("ret at %d is %d\n", __LINE__, ret);

	//// Set the arguments of the kernel
	//ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&a_mem_obj);
	//printf("ret at %d is %d\n", __LINE__, ret);

	//ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&b_mem_obj);
	//printf("ret at %d is %d\n", __LINE__, ret);

	//ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&c_mem_obj);
	//printf("ret at %d is %d\n", __LINE__, ret);

	////added this to fix garbage output problem
	////ret = clSetKernelArg(kernel, 3, sizeof(int), &LIST_SIZE);

	//printf("before execution\n");
	//// Execute the OpenCL kernel on the list
	//size_t global_item_size = LIST_SIZE; // Process the entire lists
	//size_t local_item_size = 64; // Divide work items into groups of 64
	//ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL,
	//	&global_item_size, &local_item_size, 0, NULL, NULL);
	//printf("after execution\n");
	//// Read the memory buffer C on the device to the local variable C
	//int *C = (int*)malloc(sizeof(int)*LIST_SIZE);
	//ret = clEnqueueReadBuffer(command_queue, c_mem_obj, CL_TRUE, 0,
	//	(LIST_SIZE) * sizeof(int), C, 0, NULL, NULL);
	//printf("after copying\n");
	//// Display the result to the screen
	//for (int i = 0; i < LIST_SIZE; i++)
	//	printf("%d + %d = %d\n", A[i], B[i], C[i]);

	//// Clean up
	//ret = clFlush(command_queue);
	//ret = clFinish(command_queue);
	//ret = clReleaseKernel(kernel);
	//ret = clReleaseProgram(program);
	//ret = clReleaseMemObject(a_mem_obj);
	//ret = clReleaseMemObject(b_mem_obj);
	//ret = clReleaseMemObject(c_mem_obj);
	//ret = clReleaseCommandQueue(command_queue);
	//ret = clReleaseContext(context);
	//free(A);
	//free(B);
	//free(C);


	// Get platform and device information

}
