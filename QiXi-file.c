/**************************************************************************
 * Title:	A very interesting little encryption algorithm and its reverse
 * Author: 	iyzyi
 * WebSite: http://iyzyi.com 
 * Date:   	25 Aug 2020 (七夕快乐)
 **************************************************************************/

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdint.h> 

unsigned int encrypt1(unsigned int p){
	//下列算式出自2020强网杯逆向safe_m2m，这里只修改了变量名，其余均未改动。 
	return ((p ^ (unsigned int)(32 * p)) >> 17) ^ (32 * p) ^ ((((p ^ (unsigned int)(32 * p)) >> 17) ^ p ^ (32 * p)) << 13) ^ p;
}

unsigned int encrypt2(unsigned int p){
	//为了便于理解，这里给出等价形式，但是加密的时候不要选用此函数
	unsigned int x = p ^ (p << 5);
	unsigned int y = x ^ (x >> 17);
	unsigned int c = y ^ (y << 13);
	return c;
}

unsigned int getAnsOfPowerOfTwo(int a, int b){
	//求2的幂的和
	unsigned int ans = 0;
	int i;
	for (i = a; i < b; i++){
		ans += pow(2, i);
	}
	return ans;
}

unsigned int getBit(const unsigned int n, int i){
	//取某一位的bit 
	unsigned int t =  pow(2, i);
	return (t & n) >> i;
}

unsigned int rightShiftXor(const unsigned int n, int m){
	unsigned int r = n & getAnsOfPowerOfTwo(0, 32 - m);
	unsigned int l = n & getAnsOfPowerOfTwo(32 - m, 32);
	unsigned int t = l;
	int i, t_i;
	for (i = 31 - m; i >= 0; i--){
		t_i = getBit(t, i + m) ^ getBit(n, i);
		t += t_i << i;
	}
	return t;
}

unsigned int leftShiftXor(const unsigned int n, int m){
	unsigned int r = n & getAnsOfPowerOfTwo(0, m);
	unsigned int l = n & getAnsOfPowerOfTwo(m, 32);
	unsigned int t = r;
	int i, t_i;
	for (i = m; i < 32; i++){
		t_i = getBit(t, i - m) ^ getBit(n, i);
		t += t_i << i;
	}
	return t;
}

unsigned int decrypt(unsigned int c){
	unsigned int y = leftShiftXor(c, 13);
	unsigned int x = rightShiftXor(y, 17);
	unsigned int p = leftShiftXor(x, 5);
	return p;
}

int encryptFile(char *sourceFile){
	char targetFile[256];
	strcpy (targetFile, sourceFile);
	strcat (targetFile, ".encrypt");

	FILE *fpSource, *fpTarget;
	fpSource = fopen(sourceFile, "rb");
	fpTarget = fopen(targetFile, "w+b");
	if(fpSource==NULL){
        printf("文件[%s]打开失败\n", sourceFile);
        return 0;
    }
    if(fpTarget==NULL){
        printf("文件[%s]创建/打开失败\n", targetFile);
        return 0;
    }
    
	unsigned int data[2] = {0, 0}, readCount = 0, num = 0;
	uint8_t temp[2];
	while ((readCount = fread(temp, 1, 1, fpSource)) > 0){
		data[0] += temp[0] << 8 * num;
		num ++;
		if (num == 4){
			data[0] = encrypt1(data[0]);
			fwrite(data, 4, 1, fpTarget);

			num = 0;
			data[0] = 0;
		}
	}
	if (num != 0){
		data[0] = encrypt1(data[0]);
		fwrite(data, 4, 1, fpTarget);
	}
	fclose(fpSource);
	fclose(fpTarget);
}

int decryptFile(char *sourceFile){
	char targetFile[256];
	strcpy (targetFile, sourceFile);
	strcat (targetFile, ".decrypt");

	FILE *fpSource, *fpTarget;
	fpSource = fopen(sourceFile, "rb");
	fpTarget = fopen(targetFile, "w+b");
	if(fpSource == NULL){
        printf("文件[%s]打开失败\n", sourceFile);
        return 0;
    }
    if(fpTarget == NULL){
        printf("文件[%s]创建/打开失败\n", targetFile);
        return 0;
    }
    
	unsigned int data[2] = {0, 0}, temp[2] = {0, 0}, readCount = 0, num = 0;
	while ((readCount = fread(temp, 1, 1, fpSource)) > 0){
		data[0] += temp[0] << 8 * num;
		num ++;
		if (num == 4){
			data[0] = decrypt(data[0]);
			fwrite(data, 4, 1, fpTarget);

			num = 0;
			data[0] = 0;
		}
	}
	if (num != 0){
		data[0] = decrypt(data[0]);
		fwrite(data, 4, 1, fpTarget);
	} 
	fclose(fpSource);
	fclose(fpTarget);
}

int main(int argc, char *argv[]){
	//解密后与加密前的文件的md5可能不同,是因为最后4个字节可能有多余的填充字节0x00. 
	char plain[] = "D:\\桌面\\Seer-master.zip";
	encryptFile(plain);
	
	char cipher[] = "D:\\桌面\\Seer-master.zip.encrypt";
	decryptFile(cipher);
		
	return 0;
}
