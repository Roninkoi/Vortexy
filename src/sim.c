#include "sim.h"

void s_init(struct Sim *s)
{
	s->running = 1;

	s->usegpu = 1;

	s->time = 0.0f;
	s->dt = 1.0f;

	// intialize random
	srand(timeNow());

#if RENDER_ENABLED == true
	r_init(&s->renderer, &s->running);
#endif
}

// main loop
void s_run(struct Sim *s)
{
	while (s->running) {
		s_tick(s);

#if RENDER_ENABLED == true
		if (s->rendered)
			r_render(&s->renderer);
#endif
	}
}

void s_tick(struct Sim *s)
{
	++s->ticks;

	s->time += s->dt; // increment time

	Field test;

	//ppmLoader(&test, "");

	p_fieldInit(&test, 128);

	test.f[0][0] = p_vec4(0.0f, 0.0f, 0.0f, 0.0f);

	const int size = 1024;

	int *A = (int *) malloc(sizeof(int) * size);
	int *B = (int *) malloc(sizeof(int) * size);

	for (int i = 0; i < size; ++i) {
		A[i] = i;
		B[i] = size - i;
	}

	FILE *fp;
	char *src;
	size_t src_size;

	fp = fopen("src/kernel/add.cl", "r");

	if (!fp) {
		exit(1);
	}

	src = (char *) malloc(SRC_MAX);
	src_size = fread(src, 1, SRC_MAX, fp);

	fclose(fp);

	cl_platform_id platform_id = NULL;
	cl_device_id device_id = NULL;
	cl_uint num_devices;
	cl_uint num_platforms;

	cl_uint ret = clGetPlatformIDs(1, &platform_id, &num_platforms);

	ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, &num_devices);

	cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);

	cl_command_queue queue = clCreateCommandQueue(context, device_id, 0, &ret);

	// memory buffers for a, b, c
	cl_mem a_mem = clCreateBuffer(context, CL_MEM_READ_ONLY, size * sizeof(int), NULL, &ret);
	cl_mem b_mem = clCreateBuffer(context, CL_MEM_READ_ONLY, size * sizeof(int), NULL, &ret);
	cl_mem c_mem = clCreateBuffer(context, CL_MEM_READ_ONLY, size * sizeof(int), NULL, &ret);

	// copy a, b to buffers
	ret = clEnqueueWriteBuffer(queue, a_mem, CL_TRUE, 0, size * sizeof(int), A, 0, NULL, NULL);
	ret = clEnqueueWriteBuffer(queue, b_mem, CL_TRUE, 0, size * sizeof(int), B, 0, NULL, NULL);

	// program from source
	cl_program program = clCreateProgramWithSource(context, 1, (const char **) &src, (const size_t *) &src_size, &ret);

	ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);

	// compute kernel
	cl_kernel kernel = clCreateKernel(program, "vector_add", &ret);

	// args
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *) &a_mem);
	ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *) &b_mem);
	ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *) &c_mem);

	size_t global_item_size = size;
	size_t local_item_size = 64; // number of work items

	ret = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL, NULL);

	int *C = (int *) malloc(sizeof(int) * size);

	ret = clEnqueueReadBuffer(queue, c_mem, CL_TRUE, 0, size * sizeof(int), C, 0, NULL, NULL);

	for (int i = 0; i < size; i++)
		printf("%d + %d = %d\n", A[i], B[i], C[i]);

	ret = clFlush(queue);
	ret = clFinish(queue);
	ret = clReleaseKernel(kernel);
	ret = clReleaseProgram(program);
	ret = clReleaseMemObject(a_mem);
	ret = clReleaseMemObject(b_mem);
	ret = clReleaseMemObject(c_mem);
	ret = clReleaseCommandQueue(queue);
	ret = clReleaseContext(context);

	free(A);
	free(B);
	free(C);
}