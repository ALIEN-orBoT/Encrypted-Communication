
#include<winsock.h>
#pragma comment(lib,"ws2_32.lib")
#include "rsa.h"
#include "des.h"

const int MaxSize = 1000; //缓冲区最大长度

void initialization() {
	//初始化套接字库
	WORD w_req = MAKEWORD(2, 2);//版本号
	WSADATA wsadata;
	int err;
	err = WSAStartup(w_req, &wsadata);
	if (err != 0) {
		//cout << "初始化套接字库失败！" << endl;
		cout << "Failed to initialize the socket." << endl;
	}
	//检测版本号
	if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wHighVersion) != 2) {
		//cout << "套接字库版本不符！" << endl;
		cout << "Socket version mismatch!" << endl;
	}
}

int main() {
	//定义长度变量
	int send_len = 0;
	int recv_len = 0;
	int len = 0;
	//定义发送缓冲区和接受缓冲区
	char send_buf[MaxSize];
	char recv_buf[MaxSize];
	//定义服务端套接字，接受请求套接字
	SOCKET s_server;
	SOCKET s_accept;
	//服务端地址客户端地址
	SOCKADDR_IN server_addr;
	SOCKADDR_IN accept_addr;
	initialization();
	//填充服务端信息
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(12341);
	//创建套接字
	s_server = socket(AF_INET, SOCK_STREAM, 0);
	if (bind(s_server, (SOCKADDR*)&server_addr, sizeof(SOCKADDR)) == SOCKET_ERROR) {
		//cout << "套接字绑定失败！" << endl;
		cout << "Socket binding failed." << endl;
		WSACleanup();
	}
	//设置套接字为监听状态
	if (listen(s_server, SOMAXCONN) < 0) {
		//cout << "设置监听失败！" << endl;
		cout << "Listening failed." << endl;
		WSACleanup();
	}

	cout << "Listening..." << endl;

	//接受连接请求
	len = sizeof(SOCKADDR);
	s_accept = accept(s_server, (SOCKADDR*)&accept_addr, &len);
	if (s_accept == SOCKET_ERROR) {
		cout << "Connection failed!" << endl;
		WSACleanup();
		return 0;
	}
	cout << "Successfully connected to client." << endl << endl;

	unsigned int e, d, n;
	//生成密钥对
	generateKeypairs(e, d, n);
	cout << "generateKeypairs:e,d,n:" <<e << " " <<  d << " " << n << endl;
	//发送 e,n
	unsigned int pKeyE = e;
	unsigned int pKeyN = n;
	
	char key_char[MaxSize];

	_itoa_s(pKeyE, key_char, 10);
	send_len = send(s_accept, key_char, MaxSize, 0);

	_itoa_s(pKeyN, key_char, 10);
	send_len = send(s_accept, key_char, MaxSize, 0);
	cout << "已发送(e,n)" << endl;

	//接受来自client的用公钥加密后的S
	recv(s_accept, recv_buf, MaxSize, 0);
	cout << "收到来自client的通信密钥S(已加密)" << endl;

	//解密得到S
	string c = recv_buf;
	string secret = Decrypt(c, d, n);
	cout << "使用私钥(d,n)解密得到S:" << secret << endl<<endl;

	cout << "---------------密钥交换完毕，开始通信--------------" << endl;

	string secret2;
	secret2 = des_H(secret);
	des_generateKeys(secret2);
	
	string meg;
	string megEn;
	//接受数据
	while (1) {
		meg = "";
		megEn = "";

		recv_len = recv(s_accept, recv_buf, MaxSize, 0);

		if (recv_len < 0) {
			cout << "Recieve failed!" << endl;
			break;
		}
		else {
			megEn = recv_buf;
			if (megEn == "quit" || megEn == "exit") {
				cout << "client已断开" << endl;
				break;
			}
			//接收后使用S解密
			meg = des_decrypt(megEn);
			cout << "Client:" << meg << endl;
		}
		cout << "Please enter the message to send:";
		cin >> send_buf;

		//发送前使用S加密
		meg = send_buf;
//		string megEn = "";
		megEn = des_encrypt(meg, secret);

		strcpy_s(send_buf, megEn.c_str());
		send_len = send(s_accept, send_buf, MaxSize, 0);
		if (send_len < 0) {
			cout << "Send failed!" << endl;
			break;
		}
		cout << "已加密并发送" << endl;
	}

	//关闭套接字
	closesocket(s_server);
	closesocket(s_accept);
	//释放DLL资源
	WSACleanup();

	cout << endl;
	cout << "已断开连接，关闭套接字" << endl;
	system("pause");

	return 0;
}
