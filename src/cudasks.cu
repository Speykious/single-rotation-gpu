#include <cudasks.cuh>

int h_in[LEN*LEN];

int* d_in;
int* d_out;

__device__ int getIndex(int x, int y, int w, int h) {
	if (x>=0) x %= w;	else x += w - ((-x)%w);
	if (y>=0) y %= h;	else y += h - ((-y)%h);
	return w*y + x;
}

__global__ void gameOfLife(int* d_in, int* d_out) {
	int x = threadIdx.x, y = blockIdx.x, w = blockDim.x, h = gridDim.x;

	int n00 = getIndex(x-1, y-1, w, h), n10 = getIndex(x  , y-1, w, h), n20 = getIndex(x+1, y-1, w, h),
		n01 = getIndex(x-1, y  , w, h), idx = getIndex(x  , y  , w, h), n21 = getIndex(x+1, y  , w, h),
		n02 = getIndex(x-1, y+1, w, h), n12 = getIndex(x  , y+1, w, h), n22 = getIndex(x+1, y+1, w, h);
	
	int lif = d_in[n00] + d_in[n10] + d_in[n20]
			+ d_in[n01] 			+ d_in[n21]
			+ d_in[n02] + d_in[n12] + d_in[n22];
	
	int cell = d_in[idx];

	if (cell>0) {
		if (lif==2 || lif==3)
			d_out[idx] = 1;
		else d_out[idx] = 0;
	} else {
		if (lif==3)
			d_out[idx] = 1;
		else d_out[idx] = 0;
	}

	//printf("(%d -> %d | l=%d)\n", cell, d_out[idx], lif);
}

__global__ void singleRotation(int* d_in, int* d_out, int paircut) {
	int x = 2*threadIdx.x+paircut,	y = 2*blockIdx.x+paircut,
		w = 2*blockDim.x,	h = 2*gridDim.x;

	int n00 = getIndex(x  , y  , w, h), n10 = getIndex(x+1, y  , w, h),
		n01 = getIndex(x  , y+1, w, h), n11 = getIndex(x+1, y+1, w, h);
	
	int lif = d_in[n00] + d_in[n10] + d_in[n01] + d_in[n11];

	//	00 10 | 01 00
	//	01 11 |	11 10
	if (lif==1) {
		d_out[n00] = d_in[n01];
		d_out[n10] = d_in[n00];
		d_out[n01] = d_in[n11];
		d_out[n11] = d_in[n10];
	}
}

int paircut = 0;
void triggerPaircut() {
	paircut = (paircut) ? 0 : 1;
}

void initArrays() {
	for (int j = BORDER; j<LEN-BORDER; j++) {
		for (int i = BORDER; i<LEN-BORDER; i++) {
			h_in[j*LEN+i] = (rand()%1000 > 800) ? 1 : 0;
		}
	}

	cudaMalloc(&d_in, B2LEN);
	cudaMalloc(&d_out, B2LEN);
}

void calculateLife(int N) {
	for (int i = 0; i<N; i++) {
		cudaMemcpy(d_in, h_in, B2LEN, cudaMemcpyHostToDevice);
		// gameOfLife<<<LEN, LEN>>>(d_in, d_out);
		singleRotation<<<HLN, HLN>>>(d_in, d_out, paircut);
		cudaDeviceSynchronize();
		cudaMemcpy(h_in, d_out, B2LEN, cudaMemcpyDeviceToHost);
		triggerPaircut();
	}

	auto err = cudaGetLastError();
	while (cudaSuccess != err) {
		printf("Error %d: %s\n", err, cudaGetErrorString(err));
		err = cudaGetLastError();
	}
	
	
}

void freeArrays() {
	cudaFree(d_in);
	cudaFree(d_out);
}

int* get_h_in() {
	return (int*)h_in;
}