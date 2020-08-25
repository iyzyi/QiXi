'''
 * Title:	A very interesting little encryption algorithm and its reverse
 * Author: 	iyzyi
 * WebSite: 	http://iyzyi.com 
 * Date:   	25 Aug 2020 (七夕快乐)
 * Usage:   将QiXi-file或QiXi-algorithm中的encrypt1()、decrypt()替换成本程序生成的encrypt1()、decrypt()
'''

import random

class QiXi_builder:

    def __init__(self, num):
        self.num = num
        self.shift_symbol= []
        self.shift_param = []

        shift = ['<<', '>>']
        for i in range(self.num):
            fn = random.randint(0, 1)
            self.shift_symbol.append(shift[fn])

            n = random.randint(0+1, 31-1)
            self.shift_param.append(n)

        self.encyrpt1_builder()
        self.encrypt2_builder()
        self.decrypt_builder()



    def encyrpt1_builder(self):
        encyrpt1 = []
        for i in range(self.num):
            if i == 0:
                t = '(p ^ (p {} {}))'.format(self.shift_symbol[i], self.shift_param[i])
                encyrpt1.append(t)
            else:
                t = '({} ^ ({} {} {}))'.format(encyrpt1[i-1], encyrpt1[i-1], self.shift_symbol[i], self.shift_param[i])
                encyrpt1.append(t)

        template1 = '''unsigned int encrypt1(unsigned int p){
	return %s;
}''' % encyrpt1[self.num - 1]

        print(template1)
        return template1



    def encrypt2_builder(self):
        # 为了便于理解encrypt1，所以也生成这个等价的函数。但是加密的时候最好不要选用此函数。
        encrypt2 = ''
        for i in range(self.num):
            if i == 0:
                t = '	unsigned int t{} = p ^ (p {} {});\n'.format(i+1, self.shift_symbol[i], self.shift_param[i])
            elif i == self.num - 1:
                t = '	unsigned int c = t{} ^ (t{} {} {});\n'.format(i, i, self.shift_symbol[i], self.shift_param[i])
            else:
                t = '	unsigned int t{} = t{} ^ (t{} {} {});\n'.format(i+1, i, i, self.shift_symbol[i], self.shift_param[i])
            encrypt2 += t
        
        template2 = '''unsigned int encrypt2(unsigned int p){
%s	return c;
}''' % encrypt2

        print(template2)
        return template2



    def decrypt_builder(self):
        decrypt = ''
        for i in range(self.num-1, -1, -1):
            symbol = 'left' if self.shift_symbol[i] == '<<' else 'right'
            if i == 0:
                t = '	unsigned int p = {}ShiftXor(t{}, {});\n'.format(symbol, i+1, self.shift_param[i])
            elif i == self.num - 1:
                t = '	unsigned int t{} = {}ShiftXor(c, {});\n'.format(i, symbol, self.shift_param[i])
            else:
                t = '	unsigned int t{} = {}ShiftXor(t{}, {});\n'.format(i, symbol, i+1, self.shift_param[i])
            decrypt += t

        template3 = '''unsigned int decrypt(unsigned int c){
%s	return p;
}''' % decrypt

        print(template3)
        return template3



QiXi_builder(8)        #参数为嵌套次数