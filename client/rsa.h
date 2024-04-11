#pragma once
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<cmath>
#include<string>
#include<cstring>
#include<bitset>
#include<vector>
#include <sstream>
using namespace std;

//�ж�����
unsigned int isPrime(unsigned int n);

//���Լ��
unsigned int gcd(unsigned int a, unsigned int b);

//��չŷ�����
void extgcd(unsigned int a, unsigned int b, unsigned int& d, unsigned int& x, unsigned int& y);

//����Ԫ
unsigned int modularInverse(unsigned int a, unsigned int b);

//������Կ��
void generateKeypairs(unsigned int& e, unsigned int& d, unsigned int& n);

// ���ַ���ת��Ϊ������������
std::string stringToBinary(const std::string& str);

// ���������������ֳɹ̶����ȵĿ�
std::vector<std::string> splitIntoBlocks(const std::string& binary, int block_size);

//������
unsigned long long quick_pow(unsigned long long x, unsigned long long n, unsigned long long m);

// ��Կ����
string Encrypt(const string& plaintext, unsigned int e, unsigned int n);

// ˽Կ����
string Decrypt(const string& ciphertext, unsigned int d, unsigned int n);