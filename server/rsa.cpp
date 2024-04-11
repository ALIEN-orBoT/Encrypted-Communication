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

const int block_size = 28;
const int n_bits = 28;

//判断素数
unsigned int isPrime(unsigned int n)
{
    unsigned int temp = (unsigned int)sqrt(n);
    for (unsigned int i = 2; i <= temp; i++)
    {
        if (n % i == 0) return 0;//n不是素数的话返回0
    }
    return 1;//n是素数的话返回1
}

//最大公约数
unsigned int gcd(unsigned int a, unsigned int b) {
    unsigned int t;
    if (a < b)
    {
        // 交换两个数，使大数放在a的位置上。
        t = a;
        a = b;
        b = t;
    }
    while (b != 0)
    {
        // 利用辗转相除法，直到b为0为止。
        t = a % b;
        a = b;
        b = t;
    }
    return a;
}

void extgcd(unsigned int a, unsigned int b, unsigned int& d, unsigned int& x, unsigned int& y)
{
    if (!b)
    {
        d = a;
        x = 1;
        y = 0;
    }
    else
    {
        extgcd(b, a % b, d, y, x);
        y -= x * (a / b);
    }
}

//求逆元
unsigned int modularInverse(unsigned int a, unsigned int b)  //获取(1/a)mod b的结果
{
    unsigned int d, x, y;
    extgcd(a, b, d, x, y);
    return d == 1 ? (x + b) % b : -1;

}

//生成密钥对
void generateKeypairs(unsigned int& e, unsigned int& d, unsigned int& n) {
    unsigned int p, q;
    unsigned int n1, phi_n;
    unsigned int e1, d1;

    do {
        do {
            p = rand() % 100 + 201;//20001-21000之间的素数，n=203034337，其位数为28，明文数据的最大长度应该为 block_size = (n_bits - 1) / 8 =3 bytes
        } while (p % 2 == 0);
    } while (!isPrime(p));

    for (q = p + 3; !isPrime(q); q++);

//    cout << "p:" << p << "-----q:" << q << endl;

    n1 = p * q;
//    cout << "n:" << n1 << endl;

    phi_n = (p - 1) * (q - 1);
//    cout << "phi_n:" << phi_n << endl;

    do {
        e1 = rand() % (phi_n - 2) + 2;// 1 < e < phi_n
    } while (gcd(e1, phi_n) != 1);

    d1 = modularInverse(e1, phi_n);

    e = e1;
    d = d1;
    n = n1;

//    cout << "e:" << e1 << "-----d:" << d1 << "-----n:" << n1 << endl;
}

// 将字符串转换为二进制数据流
std::string stringToBinary(const std::string& str) {
    std::string binary;
    for (char c : str) {
        binary += std::bitset<n_bits>(c).to_string();
    }
    return binary;
}

// 将二进制数据流分成固定长度的块
std::vector<std::string> splitIntoBlocks(const std::string& binary, int block_size) {
    std::vector<std::string> blocks;
    int numBlocks = binary.length() / block_size;
    for (int i = 0; i < numBlocks; i++) {
        std::string block = binary.substr(i * block_size, block_size);
        blocks.push_back(block);
    }
    return blocks;
}

//快速幂
unsigned long long quick_pow(unsigned long long x, unsigned long long n, unsigned long long m) {
    unsigned long long res = 1;
    while (n > 0) {
        if (n & 1)	res = res * x % m;
        x = x * x % m;
        n >>= 1;//相当于n=n/2.详情请参考位移运算符。
    }
    return res;
}

// 公钥加密
string Encrypt(const string& plaintext, unsigned int e, unsigned int n) {
    string binary = stringToBinary(plaintext); // 将明文转换为二进制数据流
    std::vector<std::string> blocks = splitIntoBlocks(binary, block_size); // 将二进制数据流分成固定长度的块

    string ciphertext = "";
    for (const string& block : blocks) {
        unsigned int m = bitset<block_size>(block).to_ulong(); // 将二进制块转换为整数

        unsigned int c = quick_pow(m, e, n); // 使用公钥加密

        string binary_c = bitset<block_size>(c).to_string(); // 将加密后的整数转换为二进制

        ciphertext += binary_c;
    }
    return ciphertext;
}

// 私钥解密
string Decrypt(const string& ciphertext, unsigned int d, unsigned int n) {
    vector<string> blocks = splitIntoBlocks(ciphertext, block_size); // 将密文分成固定长度的块

    string plaintext = "";
    for (const string& block : blocks) {
        unsigned int c = bitset<block_size>(block).to_ulong(); // 将二进制块转换为整数
        unsigned int m = quick_pow(c, d, n); // 使用私钥解密
        string binary_m = bitset <block_size>(m).to_string(); // 将解密后的整数转换为二进制
        char x = (char)bitset<n_bits>(binary_m).to_ulong(); // 将二进制转换为字符
        plaintext += x;
    }
    return plaintext;
}


/*
int main() {

    unsigned int e, d, n;
    generateKeypairs(e, d, n);
    cout << "公钥(e,n): " << e << "," << n << endl;
    cout << "私钥(d,n): " << d << "," << n << endl;

    string m = "";
    string c = "";
    cout << "输入要加密的内容:";
    cin >> m;

    cout << "plain:" << m << endl;

    c = Encrypt(m, e, n);
    cout << "Cipher(二进制):" << c << endl;

    m = Decrypt(c, d, n);
    cout << "plain-:" << m << endl;

    return 0;
}
*/