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

//�ж�����
unsigned int isPrime(unsigned int n)
{
    unsigned int temp = (unsigned int)sqrt(n);
    for (unsigned int i = 2; i <= temp; i++)
    {
        if (n % i == 0) return 0;//n���������Ļ�����0
    }
    return 1;//n�������Ļ�����1
}

//���Լ��
unsigned int gcd(unsigned int a, unsigned int b) {
    unsigned int t;
    if (a < b)
    {
        // ������������ʹ��������a��λ���ϡ�
        t = a;
        a = b;
        b = t;
    }
    while (b != 0)
    {
        // ����շת�������ֱ��bΪ0Ϊֹ��
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

//����Ԫ
unsigned int modularInverse(unsigned int a, unsigned int b)  //��ȡ(1/a)mod b�Ľ��
{
    unsigned int d, x, y;
    extgcd(a, b, d, x, y);
    return d == 1 ? (x + b) % b : -1;

}

//������Կ��
void generateKeypairs(unsigned int& e, unsigned int& d, unsigned int& n) {
    unsigned int p, q;
    unsigned int n1, phi_n;
    unsigned int e1, d1;

    do {
        do {
            p = rand() % 100 + 201;//20001-21000֮���������n=203034337����λ��Ϊ28���������ݵ���󳤶�Ӧ��Ϊ block_size = (n_bits - 1) / 8 =3 bytes
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

// ���ַ���ת��Ϊ������������
std::string stringToBinary(const std::string& str) {
    std::string binary;
    for (char c : str) {
        binary += std::bitset<n_bits>(c).to_string();
    }
    return binary;
}

// ���������������ֳɹ̶����ȵĿ�
std::vector<std::string> splitIntoBlocks(const std::string& binary, int block_size) {
    std::vector<std::string> blocks;
    int numBlocks = binary.length() / block_size;
    for (int i = 0; i < numBlocks; i++) {
        std::string block = binary.substr(i * block_size, block_size);
        blocks.push_back(block);
    }
    return blocks;
}

//������
unsigned long long quick_pow(unsigned long long x, unsigned long long n, unsigned long long m) {
    unsigned long long res = 1;
    while (n > 0) {
        if (n & 1)	res = res * x % m;
        x = x * x % m;
        n >>= 1;//�൱��n=n/2.������ο�λ���������
    }
    return res;
}

// ��Կ����
string Encrypt(const string& plaintext, unsigned int e, unsigned int n) {
    string binary = stringToBinary(plaintext); // ������ת��Ϊ������������
    std::vector<std::string> blocks = splitIntoBlocks(binary, block_size); // ���������������ֳɹ̶����ȵĿ�

    string ciphertext = "";
    for (const string& block : blocks) {
        unsigned int m = bitset<block_size>(block).to_ulong(); // �������ƿ�ת��Ϊ����

        unsigned int c = quick_pow(m, e, n); // ʹ�ù�Կ����

        string binary_c = bitset<block_size>(c).to_string(); // �����ܺ������ת��Ϊ������

        ciphertext += binary_c;
    }
    return ciphertext;
}

// ˽Կ����
string Decrypt(const string& ciphertext, unsigned int d, unsigned int n) {
    vector<string> blocks = splitIntoBlocks(ciphertext, block_size); // �����ķֳɹ̶����ȵĿ�

    string plaintext = "";
    for (const string& block : blocks) {
        unsigned int c = bitset<block_size>(block).to_ulong(); // �������ƿ�ת��Ϊ����
        unsigned int m = quick_pow(c, d, n); // ʹ��˽Կ����
        string binary_m = bitset <block_size>(m).to_string(); // �����ܺ������ת��Ϊ������
        char x = (char)bitset<n_bits>(binary_m).to_ulong(); // ��������ת��Ϊ�ַ�
        plaintext += x;
    }
    return plaintext;
}


/*
int main() {

    unsigned int e, d, n;
    generateKeypairs(e, d, n);
    cout << "��Կ(e,n): " << e << "," << n << endl;
    cout << "˽Կ(d,n): " << d << "," << n << endl;

    string m = "";
    string c = "";
    cout << "����Ҫ���ܵ�����:";
    cin >> m;

    cout << "plain:" << m << endl;

    c = Encrypt(m, e, n);
    cout << "Cipher(������):" << c << endl;

    m = Decrypt(c, d, n);
    cout << "plain-:" << m << endl;

    return 0;
}
*/