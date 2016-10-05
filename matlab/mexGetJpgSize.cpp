#include<stdio.h>
#include<mex.h>
int getJpgSize(FILE* f, int*w, int*h){
	if (!f) return 0;// is exist
	if (!(fgetc(f) == 0xFF && fgetc(f) == 0xD8)) return 0;// is JPEG file
    // find FFC0
	int p = 0, c, finded = 0;
	while ((c = fgetc(f)) != EOF){
		if (p == 0xFF && c == 0xC0) {
			finded = 1;
			break;
		}
		p = c;
	}
	if (!finded) return 0;
    // read height and width
	fseek(f, 3, SEEK_CUR);
	*h = (fgetc(f) << 8) | fgetc(f);
	*w = (fgetc(f) << 8) | fgetc(f);
	return 1;
}
/*
 * sz = mexGetJpgSize(path)
 * input:
 *  path    path of image file
 * output:
 *  sz      return [height,width] of jpeg file
 *          return [] if file not exists or is not jpeg file
 */
void mexFunction(int nlhs, mxArray* plhs[], int nrhs, mxArray* prhs[]){
	char* fname = mxArrayToString(prhs[0]);
	FILE* f = fopen(fname, "rb");
	int w, h;
	int b = getJpgSize(f, &w, &h);
	if (b){
		plhs[0] = mxCreateDoubleMatrix(1, 2, mxREAL);
		double* p = mxGetPr(plhs[0]);
		p[0] = h;
		p[1] = w;	
	}
	else{
		plhs[0] = mxCreateDoubleMatrix(0, 0, mxREAL);
	}
	fclose(f);
}