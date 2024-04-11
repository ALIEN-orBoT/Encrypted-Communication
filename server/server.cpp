
#include<winsock.h>
#pragma comment(lib,"ws2_32.lib")
#include "rsa.h"
#include "des.h"

const int MaxSize = 1000; //��������󳤶�

void initialization() {
	//��ʼ���׽��ֿ�
	WORD w_req = MAKEWORD(2, 2);//�汾��
	WSADATA wsadata;
	int err;
	err = WSAStartup(w_req, &wsadata);
	if (err != 0) {
		//cout << "��ʼ���׽��ֿ�ʧ�ܣ�" << endl;
		cout << "Failed to initialize the socket." << endl;
	}
	//���汾��
	if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wHighVersion) != 2) {
		//cout << "�׽��ֿ�汾������" << endl;
		cout << "Socket version mismatch!" << endl;
	}
}

int main() {
	//���峤�ȱ���
	int send_len = 0;
	int recv_len = 0;
	int len = 0;
	//���巢�ͻ������ͽ��ܻ�����
	char send_buf[MaxSize];
	char recv_buf[MaxSize];
	//���������׽��֣����������׽���
	SOCKET s_server;
	SOCKET s_accept;
	//����˵�ַ�ͻ��˵�ַ
	SOCKADDR_IN server_addr;
	SOCKADDR_IN accept_addr;
	initialization();
	//���������Ϣ
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(12341);
	//�����׽���
	s_server = socket(AF_INET, SOCK_STREAM, 0);
	if (bind(s_server, (SOCKADDR*)&server_addr, sizeof(SOCKADDR)) == SOCKET_ERROR) {
		//cout << "�׽��ְ�ʧ�ܣ�" << endl;
		cout << "Socket binding failed." << endl;
		WSACleanup();
	}
	//�����׽���Ϊ����״̬
	if (listen(s_server, SOMAXCONN) < 0) {
		//cout << "���ü���ʧ�ܣ�" << endl;
		cout << "Listening failed." << endl;
		WSACleanup();
	}

	cout << "Listening..." << endl;

	//������������
	len = sizeof(SOCKADDR);
	s_accept = accept(s_server, (SOCKADDR*)&accept_addr, &len);
	if (s_accept == SOCKET_ERROR) {
		cout << "Connection failed!" << endl;
		WSACleanup();
		return 0;
	}
	cout << "Successfully connected to client." << endl << endl;

	unsigned int e, d, n;
	//������Կ��
	generateKeypairs(e, d, n);
	cout << "generateKeypairs:e,d,n:" <<e << " " <<  d << " " << n << endl;
	//���� e,n
	unsigned int pKeyE = e;
	unsigned int pKeyN = n;
	
	char key_char[MaxSize];

	_itoa_s(pKeyE, key_char, 10);
	send_len = send(s_accept, key_char, MaxSize, 0);

	_itoa_s(pKeyN, key_char, 10);
	send_len = send(s_accept, key_char, MaxSize, 0);
	cout << "�ѷ���(e,n)" << endl;

	//��������client���ù�Կ���ܺ��S
	recv(s_accept, recv_buf, MaxSize, 0);
	cout << "�յ�����client��ͨ����ԿS(�Ѽ���)" << endl;

	//���ܵõ�S
	string c = recv_buf;
	string secret = Decrypt(c, d, n);
	cout << "ʹ��˽Կ(d,n)���ܵõ�S:" << secret << endl<<endl;

	cout << "---------------��Կ������ϣ���ʼͨ��--------------" << endl;

	string secret2;
	secret2 = des_H(secret);
	des_generateKeys(secret2);
	
	string meg;
	string megEn;
	//��������
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
				cout << "client�ѶϿ�" << endl;
				break;
			}
			//���պ�ʹ��S����
			meg = des_decrypt(megEn);
			cout << "Client:" << meg << endl;
		}
		cout << "Please enter the message to send:";
		cin >> send_buf;

		//����ǰʹ��S����
		meg = send_buf;
//		string megEn = "";
		megEn = des_encrypt(meg, secret);

		strcpy_s(send_buf, megEn.c_str());
		send_len = send(s_accept, send_buf, MaxSize, 0);
		if (send_len < 0) {
			cout << "Send failed!" << endl;
			break;
		}
		cout << "�Ѽ��ܲ�����" << endl;
	}

	//�ر��׽���
	closesocket(s_server);
	closesocket(s_accept);
	//�ͷ�DLL��Դ
	WSACleanup();

	cout << endl;
	cout << "�ѶϿ����ӣ��ر��׽���" << endl;
	system("pause");

	return 0;
}
