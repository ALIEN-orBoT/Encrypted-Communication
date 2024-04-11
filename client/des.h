
#pragma once
#include <string>
#include <bitset>
#include <iostream>
#include <algorithm>
using namespace std;

extern string desKeys[20];

extern int T1[8][8];
extern int T2[8][7];
extern int T3[16];
extern int T4[8][6];
extern int T5[8][6];
extern int S[8][4][16];
extern int P[4][8];
extern int T6[8][8];


//最终置换函数 64位->64位
string final_permutation(string s);

//P盒置换函数 32位->32位
string P_box(string s);

//S盒置换函数 48位->32位
string S_box(string s);

//异或运算函数
string desXOR(string s1, string s2);

//数据扩展函数 32->48
string plaintext_righthalf_extended_permutation(string s);

//密钥压缩置换函数 56位->48位
string secret_key_compression_replacement(string s);

//密钥循环左移函数 56位->56位
string secret_ket_left_move(int k, string s);

//密钥初始置换函数 64位->56位
string secret_key_initial_permutation(string s);

//明文初始置换函数 64位->64位
string plaintext_initial_permutation(string s);

//16进制转2进制函数
string des_H(string s);

//2进制转16进制函数
string des_G(string str);

//封装函数f
string des_f(string str1, string str2);

//子密钥生成函数
void des_generateKeys(string s);

// 明文字符串转换成0/1字符串
string des_StrToBitStr(string str);

//0/1字符串装换为字符形式的字符串
string des_BitStrToStr(string bstr);

string chardeel(string& str1, string& str2);

//DES加密函数 64位->64位
string des_encrypt(string str1, string str2);

//解密函数
string des_decrypt(string str);


