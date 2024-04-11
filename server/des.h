
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


//�����û����� 64λ->64λ
string final_permutation(string s);

//P���û����� 32λ->32λ
string P_box(string s);

//S���û����� 48λ->32λ
string S_box(string s);

//������㺯��
string desXOR(string s1, string s2);

//������չ���� 32->48
string plaintext_righthalf_extended_permutation(string s);

//��Կѹ���û����� 56λ->48λ
string secret_key_compression_replacement(string s);

//��Կѭ�����ƺ��� 56λ->56λ
string secret_ket_left_move(int k, string s);

//��Կ��ʼ�û����� 64λ->56λ
string secret_key_initial_permutation(string s);

//���ĳ�ʼ�û����� 64λ->64λ
string plaintext_initial_permutation(string s);

//16����ת2���ƺ���
string des_H(string s);

//2����ת16���ƺ���
string des_G(string str);

//��װ����f
string des_f(string str1, string str2);

//����Կ���ɺ���
void des_generateKeys(string s);

// �����ַ���ת����0/1�ַ���
string des_StrToBitStr(string str);

//0/1�ַ���װ��Ϊ�ַ���ʽ���ַ���
string des_BitStrToStr(string bstr);

string chardeel(string& str1, string& str2);

//DES���ܺ��� 64λ->64λ
string des_encrypt(string str1, string str2);

//���ܺ���
string des_decrypt(string str);


