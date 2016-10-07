#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<direct.h>
#include<vector>
#include<string>
#include<io.h>
#include<iostream>
#include<set>
#include<regex>
using namespace std;

// get file's MD5
string getMD5(string fname);

// find file of folder, not contains sub folders
vector<string> getFiles(string path){
	vector<string> files;
	long   hFile = 0;
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1){
		do{
			if ((fileinfo.attrib &  _A_SUBDIR));
			else files.push_back(p.assign(path).append("\\").append(fileinfo.name));
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
	return files;
}

// get width and height of JPEG file
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

// is a horizontal wall paper
int isWallPaper(string fname){
	FILE* f = fopen(fname.c_str(), "rb");
	int w, h, r;
	r = getJpgSize(f, &w, &h);
	fclose(f);
	if (r) r = w > h;
	return r;
}

// copy file
void copy_file(string src, string dst){
	FILE* in = fopen(src.c_str(), "rb");
	FILE* out = fopen(dst.c_str(), "wb");
	int c;
	while ((c = fgetc(in)) != EOF){
		fputc(c, out);
	}
	fclose(in);
	fclose(out);
}

int main(){
	string picDir, dstDir;
	char* userFolder = getenv("USERPROFILE");
	picDir = userFolder;
	dstDir = userFolder;
	picDir.append("/AppData/Local/Packages/Microsoft.Windows.ContentDeliveryManager_cw5n1h2txyewy/LocalState/Assets");
	dstDir.append("/Pictures/Saved Pictures");
	
	vector<string> fps = getFiles(picDir);
	vector<string> olds = getFiles(dstDir);
	set<string> md5s;

	regex r("(\\d+)(?=\\.jpg)");
	smatch sm;
	int pid = 0;
	for (int k = 0; k < olds.size(); k++){
		md5s.insert(getMD5(olds[k]));
		// find the next numbered name of new pictures 
		if (regex_search(olds[k], sm, r)){
			int id = stoi(sm.str());
			if (id>pid) pid = id;
		}
	}

	int add = 0;
	int pic = 0;
	for (int k = 0; k < fps.size(); k++){
		if (isWallPaper(fps[k])){
			pic++;
			string md5 = getMD5(fps[k]);
			// add new pictures identified by MD5 
			if (!md5s.count(md5)){
				add++;
				md5s.insert(md5);
				string out = dstDir;
				char s[100];
				sprintf(s, "/%03d.jpg", ++pid);
				out.append(s);
				copy_file(fps[k], out);
			}
		}
	}
	printf("find     %3d pics\n", pic);
	printf("add      %3d pics\n", add);
	printf("start at %03d.jpg\n", pid - add + 1);
	// open dst folder
	string cmd = "explorer \"";
	cmd.append(dstDir).append("\"");
	char cmds[256];
	strcpy(cmds, cmd.c_str());
	for (int k = 0; cmds[k] != '\0'; k++){
		if (cmds[k] == '/') cmds[k] = '\\';
	}
	system(cmds);

	system("pause");
}

/*********************************************************************
**                     Begin of RSA md5                             **
*********************************************************************/

/*********************************************************************
** Copyright (C) 1990, RSA Data Security, Inc. All rights reserved. **
**                                                                  **
** RSA Data Security, Inc. makes no representations concerning      **
** either the merchantability of this software or the suitability   **
** of this software for any particular purpose.  It is provided "as **
** is" without express or implied warranty of any kind.             **
**                                                                  **
** These notices must be retained in any copies of any part of this **
** documentation and/or software.                                   **
*********************************************************************/

/* typedef a 32 bit type */
typedef unsigned long int UINT4;

/* Data structure for MD5 (Message Digest) computation */
typedef struct {
	UINT4 i[2];                   /* number of _bits_ handled mod 2^64 */
	UINT4 buf[4];                                    /* scratch buffer */
	unsigned char in[64];                              /* input buffer */
	unsigned char digest[16];     /* actual digest after MD5Final call */
} MD5_CTX;

/* forward declaration */
static void Transform(UINT4 *buf, UINT4 *in);

static unsigned char PADDING[64] = {
	0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/* F, G and H are basic MD5 functions: selection, majority, parity */
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z))) 

/* ROTATE_LEFT rotates x left n bits */
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

/* FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4 */
/* Rotation is separate from addition to prevent recomputation */
#define FF(a, b, c, d, x, s, ac) \
{	(a) += F((b), (c), (d)) + (x)+(UINT4)(ac); \
	(a) = ROTATE_LEFT((a), (s)); \
	(a) += (b); \
}
#define GG(a, b, c, d, x, s, ac) \
{	(a) += G((b), (c), (d)) + (x)+(UINT4)(ac); \
	(a) = ROTATE_LEFT((a), (s)); \
	(a) += (b); \
	}
#define HH(a, b, c, d, x, s, ac) \
{	(a) += H((b), (c), (d)) + (x)+(UINT4)(ac); \
	(a) = ROTATE_LEFT((a), (s)); \
	(a) += (b); \
	}
#define II(a, b, c, d, x, s, ac) \
{	(a) += I((b), (c), (d)) + (x)+(UINT4)(ac); \
	(a) = ROTATE_LEFT((a), (s)); \
	(a) += (b); \
	}

void MD5Init(MD5_CTX *mdContext){
	mdContext->i[0] = mdContext->i[1] = (UINT4)0;

	/* Load magic initialization constants.
	*/
	mdContext->buf[0] = (UINT4)0x67452301;
	mdContext->buf[1] = (UINT4)0xefcdab89;
	mdContext->buf[2] = (UINT4)0x98badcfe;
	mdContext->buf[3] = (UINT4)0x10325476;
}

void MD5Update(MD5_CTX *mdContext, unsigned char *inBuf, unsigned int inLen){
	UINT4 in[16];
	int mdi;
	unsigned int i, ii;

	/* compute number of bytes mod 64 */
	mdi = (int)((mdContext->i[0] >> 3) & 0x3F);

	/* update number of bits */
	if ((mdContext->i[0] + ((UINT4)inLen << 3)) < mdContext->i[0])
		mdContext->i[1]++;
	mdContext->i[0] += ((UINT4)inLen << 3);
	mdContext->i[1] += ((UINT4)inLen >> 29);

	while (inLen--) {
		/* add new character to buffer, increment mdi */
		mdContext->in[mdi++] = *inBuf++;

		/* transform if necessary */
		if (mdi == 0x40) {
			for (i = 0, ii = 0; i < 16; i++, ii += 4)
				in[i] = (((UINT4)mdContext->in[ii + 3]) << 24) |
				(((UINT4)mdContext->in[ii + 2]) << 16) |
				(((UINT4)mdContext->in[ii + 1]) << 8) |
				((UINT4)mdContext->in[ii]);
			Transform(mdContext->buf, in);
			mdi = 0;
		}
	}
}

void MD5Final(MD5_CTX *mdContext){
	UINT4 in[16];
	int mdi;
	unsigned int i, ii;
	unsigned int padLen;

	/* save number of bits */
	in[14] = mdContext->i[0];
	in[15] = mdContext->i[1];

	/* compute number of bytes mod 64 */
	mdi = (int)((mdContext->i[0] >> 3) & 0x3F);

	/* pad out to 56 mod 64 */
	padLen = (mdi < 56) ? (56 - mdi) : (120 - mdi);
	MD5Update(mdContext, PADDING, padLen);

	/* append length in bits and transform */
	for (i = 0, ii = 0; i < 14; i++, ii += 4)
		in[i] = (((UINT4)mdContext->in[ii + 3]) << 24) |
		(((UINT4)mdContext->in[ii + 2]) << 16) |
		(((UINT4)mdContext->in[ii + 1]) << 8) |
		((UINT4)mdContext->in[ii]);
	Transform(mdContext->buf, in);

	/* store buffer in digest */
	for (i = 0, ii = 0; i < 4; i++, ii += 4) {
		mdContext->digest[ii] = (unsigned char)(mdContext->buf[i] & 0xFF);
		mdContext->digest[ii + 1] =
			(unsigned char)((mdContext->buf[i] >> 8) & 0xFF);
		mdContext->digest[ii + 2] =
			(unsigned char)((mdContext->buf[i] >> 16) & 0xFF);
		mdContext->digest[ii + 3] =
			(unsigned char)((mdContext->buf[i] >> 24) & 0xFF);
	}
}

/* Basic MD5 step. Transform buf based on in.
*/
static void Transform(UINT4 *buf, UINT4 *in){
	UINT4 a = buf[0], b = buf[1], c = buf[2], d = buf[3];

	/* Round 1 */
#define S11 7
#define S12 12
#define S13 17
#define S14 22
	FF(a, b, c, d, in[0], S11, 3614090360); 	/* 1 */
	FF(d, a, b, c, in[1], S12, 3905402710); 	/* 2 */
	FF(c, d, a, b, in[2], S13, 606105819); 	/* 3 */
	FF(b, c, d, a, in[3], S14, 3250441966); 	/* 4 */
	FF(a, b, c, d, in[4], S11, 4118548399); 	/* 5 */
	FF(d, a, b, c, in[5], S12, 1200080426); 	/* 6 */
	FF(c, d, a, b, in[6], S13, 2821735955); 	/* 7 */
	FF(b, c, d, a, in[7], S14, 4249261313); 	/* 8 */
	FF(a, b, c, d, in[8], S11, 1770035416); 	/* 9 */
	FF(d, a, b, c, in[9], S12, 2336552879); 	/* 10 */
	FF(c, d, a, b, in[10], S13, 4294925233); 	/* 11 */
	FF(b, c, d, a, in[11], S14, 2304563134); 	/* 12 */
	FF(a, b, c, d, in[12], S11, 1804603682); 	/* 13 */
	FF(d, a, b, c, in[13], S12, 4254626195); 	/* 14 */
	FF(c, d, a, b, in[14], S13, 2792965006); 	/* 15 */
	FF(b, c, d, a, in[15], S14, 1236535329); 	/* 16 */

	/* Round 2 */
#define S21 5
#define S22 9
#define S23 14
#define S24 20
	GG(a, b, c, d, in[1], S21, 4129170786); 	/* 17 */
	GG(d, a, b, c, in[6], S22, 3225465664); 	/* 18 */
	GG(c, d, a, b, in[11], S23, 643717713); 	/* 19 */
	GG(b, c, d, a, in[0], S24, 3921069994); 	/* 20 */
	GG(a, b, c, d, in[5], S21, 3593408605); 	/* 21 */
	GG(d, a, b, c, in[10], S22, 38016083); 	/* 22 */
	GG(c, d, a, b, in[15], S23, 3634488961); 	/* 23 */
	GG(b, c, d, a, in[4], S24, 3889429448);	/* 24 */
	GG(a, b, c, d, in[9], S21, 568446438); 	/* 25 */
	GG(d, a, b, c, in[14], S22, 3275163606);	/* 26 */
	GG(c, d, a, b, in[3], S23, 4107603335); 	/* 27 */
	GG(b, c, d, a, in[8], S24, 1163531501); 	/* 28 */
	GG(a, b, c, d, in[13], S21, 2850285829); 	/* 29 */
	GG(d, a, b, c, in[2], S22, 4243563512); 	/* 30 */
	GG(c, d, a, b, in[7], S23, 1735328473); 	/* 31 */
	GG(b, c, d, a, in[12], S24, 2368359562); 	/* 32 */

	/* Round 3 */
#define S31 4
#define S32 11
#define S33 16
#define S34 23
	HH(a, b, c, d, in[5], S31, 4294588738); 	/* 33 */
	HH(d, a, b, c, in[8], S32, 2272392833); 	/* 34 */
	HH(c, d, a, b, in[11], S33, 1839030562); 	/* 35 */
	HH(b, c, d, a, in[14], S34, 4259657740); 	/* 36 */
	HH(a, b, c, d, in[1], S31, 2763975236); 	/* 37 */
	HH(d, a, b, c, in[4], S32, 1272893353); 	/* 38 */
	HH(c, d, a, b, in[7], S33, 4139469664); 	/* 39 */
	HH(b, c, d, a, in[10], S34, 3200236656); 	/* 40 */
	HH(a, b, c, d, in[13], S31, 681279174); 	/* 41 */
	HH(d, a, b, c, in[0], S32, 3936430074); 	/* 42 */
	HH(c, d, a, b, in[3], S33, 3572445317); 	/* 43 */
	HH(b, c, d, a, in[6], S34, 76029189); 	/* 44 */
	HH(a, b, c, d, in[9], S31, 3654602809); 	/* 45 */
	HH(d, a, b, c, in[12], S32, 3873151461); 	/* 46 */
	HH(c, d, a, b, in[15], S33, 530742520); 	/* 47 */
	HH(b, c, d, a, in[2], S34, 3299628645); 	/* 48 */

	/* Round 4 */
#define S41 6
#define S42 10
#define S43 15
#define S44 21
	II(a, b, c, d, in[0], S41, 4096336452); 	/* 49 */
	II(d, a, b, c, in[7], S42, 1126891415); 	/* 50 */
	II(c, d, a, b, in[14], S43, 2878612391); 	/* 51 */
	II(b, c, d, a, in[5], S44, 4237533241); 	/* 52 */
	II(a, b, c, d, in[12], S41, 1700485571); 	/* 53 */
	II(d, a, b, c, in[3], S42, 2399980690); 	/* 54 */
	II(c, d, a, b, in[10], S43, 4293915773); 	/* 55 */
	II(b, c, d, a, in[1], S44, 2240044497); 	/* 56 */
	II(a, b, c, d, in[8], S41, 1873313359); 	/* 57 */
	II(d, a, b, c, in[15], S42, 4264355552); 	/* 58 */
	II(c, d, a, b, in[6], S43, 2734768916); 	/* 59 */
	II(b, c, d, a, in[13], S44, 1309151649); 	/* 60 */
	II(a, b, c, d, in[4], S41, 4149444226); 	/* 61 */
	II(d, a, b, c, in[11], S42, 3174756917); 	/* 62 */
	II(c, d, a, b, in[2], S43, 718787259); 	/* 63 */
	II(b, c, d, a, in[9], S44, 3951481745); 	/* 64 */

	buf[0] += a;
	buf[1] += b;
	buf[2] += c;
	buf[3] += d;
}
/*********************************************************************
**                       End of RSA md5                             **
*********************************************************************/

string getMD5(string fname){
	FILE *inFile = fopen(fname.c_str(), "rb");
	MD5_CTX mdContext;
	int bytes;
	unsigned char data[1024];

	MD5Init(&mdContext);
	while ((bytes = fread(data, 1, 1024, inFile)) != 0)
		MD5Update(&mdContext, data, bytes);
	MD5Final(&mdContext);
	fclose(inFile);
	char str[33];
	for (int k = 0; k < 16; k++){
		sprintf(str + 2 * k, "%02X", mdContext.digest[k]);
	}
	str[32] = '\0';
	string s(str);
	return s;
}