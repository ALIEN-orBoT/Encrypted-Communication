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

//判断素数
unsigned int isPrime(unsigned int n);

//最大公约数
unsigned int gcd(unsigned int a, unsigned int b);

//扩展欧几里得
void extgcd(unsigned int a, unsigned int b, unsigned int& d, unsigned int& x, unsigned int& y);

//求逆元
unsigned int modularInverse(unsigned int a, unsigned int b);

//生成密钥对
void generateKeypairs(unsigned int& e, unsigned int& d, unsigned int& n);

// 将字符串转换为二进制数据流
std::string stringToBinary(const std::string& str);

// 将二进制数据流分成固定长度的块
std::vector<std::string> splitIntoBlocks(const std::string& binary, int block_size);

//快速幂
unsigned long long quick_pow(unsigned long long x, unsigned long long n, unsigned long long m);

// 公钥加密
string Encrypt(const string& plaintext, unsigned int e, unsigned int n);

// 私钥解密
string Decrypt(const string& ciphertext, unsigned int d, unsigned int n);