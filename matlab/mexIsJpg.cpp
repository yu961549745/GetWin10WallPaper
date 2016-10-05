#include<stdio.h>
#include<mex.h>
// JPEG start with FFD8
int isJpg(char* fname){
	FILE* f = fopen(fname, "rb");
	if (!f) return 0;
	int b = fgetc(f) == 0xFF && fgetc(f) == 0xD8;
	fclose(f);
	return b;
}
/*
 * b = mexIsJpg(path)
 * input:
 *  path    path of image file
 * output:
 *  b       is this a JPEG file
 */
void mexFunction(int nlhs, mxArray* plhs[], int nrhs, mxArray* prhs[]){
	plhs[0] = mxCreateDoubleScalar(isJpg(mxArrayToString(prhs[0])));
}