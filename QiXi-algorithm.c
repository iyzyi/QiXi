/**************************************************************************
 * Title:	A very interesting little encryption algorithm and its reverse
 * Author: 	iyzyi
 * WebSite: http://iyzyi.com 
 * Date:   	25 Aug 2020 (��Ϧ����)
 **************************************************************************/

#include <stdio.h>
#include <math.h>

unsigned int encrypt1(unsigned int p){
	//������ʽ����2020ǿ��������safe_m2m��ֻ�޸��˱������������δ�Ķ��� 
	return ((p ^ (unsigned int)(32 * p)) >> 17) ^ (32 * p) ^ ((((p ^ (unsigned int)(32 * p)) >> 17) ^ p ^ (32 * p)) << 13) ^ p;
}

unsigned int encrypt2(unsigned int p){
	//Ϊ�˱�����⣬��������ȼ���ʽ�����Ǽ��ܵ�ʱ��Ҫѡ�ô˺���
	unsigned int x = p ^ (p << 5);
	unsigned int y = x ^ (x >> 17);
	unsigned int c = y ^ (y << 13);
	return c;
}

unsigned int getAnsOfPowerOfTwo(int a, int b){
	//��2���ݵĺ�
	unsigned int ans = 0;
	int i;
	for (i = a; i < b; i++){
		ans += pow(2, i);
	}
	return ans;
}

unsigned int getBit(const unsigned int n, int i){
	//ȡĳһλ��bit 
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

unsigned int decrypt(c){
	unsigned int y = leftShiftXor(c, 13);
	unsigned int x = rightShiftXor(y, 17);
	unsigned int p = leftShiftXor(x, 5);
	return p;
}

int main(){
	unsigned int p0 = 0xfedcba98;
	
	unsigned int c1 = encrypt1(p0);
	printf("encrypt1(0x%x) = 0x%x\n", p0, c1);
	unsigned int c2 = encrypt2(p0);
	printf("encrypt2(0x%x) = 0x%x\n", p0, c2);
	
	unsigned int c = c1;
	unsigned int p = decrypt(c);
	printf("\ndecrypt(0x%x) = 0x%x\n", c, p); 
	
	if (c1 == c2 && p0 == p){
		printf("\nTRUE");
	}
	return 0;
}
