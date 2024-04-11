
#include<winsock.h>
#pragma comment(lib,"ws2_32.lib")
#include "rsa.h"
#include "des.h"

const int MaxSize = 1000;

void initialization() {
	//初始化套接字库
	WORD w_req = MAKEWORD(2, 2);//版本号
	WSADATA wsadata;
	int err;
	err = WSAStartup(w_req, &wsadata);
	if (err != 0) {
		//cout << "初始化套接字库失败!" << endl;
		cout << "Failed to initialize the socket." << endl;
	}
	//检测版本号
	if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wHighVersion) != 2) {
		//cout << "套接字库版本号不符！" << endl;
		cout << "Socket version mismatch!" << endl;
		WSACleanup();
	}
}

//随机生成字符串
string rand_str(const int len)  /*参数为字符串的长度*/
{
	/*初始化*/
	string str;                 /*声明用来保存随机字符串的str*/
	char c;                     /*声明字符c，用来保存随机生成的字符*/
	int idx;                    /*用来循环的变量*/
	/*循环向字符串中添加随机生成的字符*/
	for (idx = 0; idx < len; idx++)
	{
		c = rand() % 36; //a~z A~Z 0~9 一共36种字符
		if (c < 10) {
			c += '0';
		}

		else {
			c -= 10;
			c += 'a';
		}
		str.push_back(c);       /*push_back()是string类尾插函数。这里插入随机字符c*/
	}
	return str;                 /*返回生成的随机字符串*/
}

int main() {
	//定义长度变量
	int send_len = 0;
	int recv_len = 0;
	//定义发送缓冲区和接受缓冲区
	char send_buf[MaxSize];
	char recv_buf[MaxSize];
	//定义服务端套接字，接受请求套接字
	SOCKET s_server;
	//服务端地址客户端地址
	SOCKADDR_IN server_addr;
	initialization();
	//填充服务端信息
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	server_addr.sin_port = htons(12341);
	//创建套接字
	s_server = socket(AF_INET, SOCK_STREAM, 0);
	if (connect(s_server, (SOCKADDR*)&server_addr, sizeof(SOCKADDR)) == SOCKET_ERROR) {
		//cout << "服务器连接失败！" << endl;
		cout << "Connection failed!" << endl;
		WSACleanup();
	}

	cout << "Successfully connected to server." << endl << endl;


	//接受公钥
	int pKeyE;
	int pKeyN;
	recv_len = recv(s_server, recv_buf, MaxSize, 0);
	cout << "Server(publicKey) e:" << recv_buf << endl;
	pKeyE = atoi(recv_buf);
	recv_len = recv(s_server, recv_buf, MaxSize, 0);
	cout << "Server(publicKey) n:" << recv_buf << endl;
	pKeyN = atoi(recv_buf);
	cout << "已收到公钥(e,n)" << endl;
	
	//随机生成通信密钥S
	string secret = rand_str(16);//(64位，16个字符)
	cout << "生成通信密钥S：" << secret << endl;

	//使用公钥加密S并发送
	cout << "加密S..." << endl;
	string c = Encrypt(secret, pKeyE, pKeyN);
	strcpy_s(send_buf, c.c_str());
;	send_len = send(s_server, send_buf, MaxSize, 0);
	if (send_len < 0) {
		//cout << "发送失败！" << endl;
		cout << "Send failed!" << endl;
	}else{
		cout << "加密后的S已发送给server" << endl<<endl;
	}

	cout << "---------------密钥交换完毕，开始通信--------------" << endl;

	string meg;
	//使用密钥S发送数据
	while (1) {
		cout << "Please enter the message to send:";

		cin >> meg;

		if (meg == "quit" || meg == "exit") {
			strcpy_s(send_buf, meg.c_str());
			send(s_server, send_buf, MaxSize, 0);
			break;
		}

		//发送前使用S加密	
		string megEn = des_encrypt(meg, secret);	
		strcpy_s(send_buf, megEn.c_str());

		send_len = send(s_server, send_buf, MaxSize, 0);
		if (send_len < 0) {
			//cout << "发送失败！" << endl;
			cout << "Send failed!" << endl;
			break;
		}
		cout << "已加密并发送" << endl;

		recv_len = recv(s_server, recv_buf, MaxSize, 0);
		if (recv_len < 0) {
			//cout << "接受失败！" << endl;
			cout << "Recieve failed!" << endl;
			break;
		}
		else {
			//接收后使用S解密
			meg = recv_buf;
			cout << "Server:" << des_decrypt(meg) << endl;
		}
	}
	//关闭套接字
	closesocket(s_server);
	//释放DLL资源
	WSACleanup();

	cout << endl;
	cout << "已断开连接，关闭套接字" << endl;
	system("pause");

	return 0;
}
