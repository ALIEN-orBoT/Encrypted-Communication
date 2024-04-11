
#include<winsock.h>
#pragma comment(lib,"ws2_32.lib")
#include "rsa.h"
#include "des.h"

const int MaxSize = 1000;

void initialization() {
	//��ʼ���׽��ֿ�
	WORD w_req = MAKEWORD(2, 2);//�汾��
	WSADATA wsadata;
	int err;
	err = WSAStartup(w_req, &wsadata);
	if (err != 0) {
		//cout << "��ʼ���׽��ֿ�ʧ��!" << endl;
		cout << "Failed to initialize the socket." << endl;
	}
	//���汾��
	if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wHighVersion) != 2) {
		//cout << "�׽��ֿ�汾�Ų�����" << endl;
		cout << "Socket version mismatch!" << endl;
		WSACleanup();
	}
}

//��������ַ���
string rand_str(const int len)  /*����Ϊ�ַ����ĳ���*/
{
	/*��ʼ��*/
	string str;                 /*����������������ַ�����str*/
	char c;                     /*�����ַ�c����������������ɵ��ַ�*/
	int idx;                    /*����ѭ���ı���*/
	/*ѭ�����ַ��������������ɵ��ַ�*/
	for (idx = 0; idx < len; idx++)
	{
		c = rand() % 36; //a~z A~Z 0~9 һ��36���ַ�
		if (c < 10) {
			c += '0';
		}

		else {
			c -= 10;
			c += 'a';
		}
		str.push_back(c);       /*push_back()��string��β�庯���������������ַ�c*/
	}
	return str;                 /*�������ɵ�����ַ���*/
}

int main() {
	//���峤�ȱ���
	int send_len = 0;
	int recv_len = 0;
	//���巢�ͻ������ͽ��ܻ�����
	char send_buf[MaxSize];
	char recv_buf[MaxSize];
	//���������׽��֣����������׽���
	SOCKET s_server;
	//����˵�ַ�ͻ��˵�ַ
	SOCKADDR_IN server_addr;
	initialization();
	//���������Ϣ
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	server_addr.sin_port = htons(12341);
	//�����׽���
	s_server = socket(AF_INET, SOCK_STREAM, 0);
	if (connect(s_server, (SOCKADDR*)&server_addr, sizeof(SOCKADDR)) == SOCKET_ERROR) {
		//cout << "����������ʧ�ܣ�" << endl;
		cout << "Connection failed!" << endl;
		WSACleanup();
	}

	cout << "Successfully connected to server." << endl << endl;


	//���ܹ�Կ
	int pKeyE;
	int pKeyN;
	recv_len = recv(s_server, recv_buf, MaxSize, 0);
	cout << "Server(publicKey) e:" << recv_buf << endl;
	pKeyE = atoi(recv_buf);
	recv_len = recv(s_server, recv_buf, MaxSize, 0);
	cout << "Server(publicKey) n:" << recv_buf << endl;
	pKeyN = atoi(recv_buf);
	cout << "���յ���Կ(e,n)" << endl;
	
	//�������ͨ����ԿS
	string secret = rand_str(16);//(64λ��16���ַ�)
	cout << "����ͨ����ԿS��" << secret << endl;

	//ʹ�ù�Կ����S������
	cout << "����S..." << endl;
	string c = Encrypt(secret, pKeyE, pKeyN);
	strcpy_s(send_buf, c.c_str());
;	send_len = send(s_server, send_buf, MaxSize, 0);
	if (send_len < 0) {
		//cout << "����ʧ�ܣ�" << endl;
		cout << "Send failed!" << endl;
	}else{
		cout << "���ܺ��S�ѷ��͸�server" << endl<<endl;
	}

	cout << "---------------��Կ������ϣ���ʼͨ��--------------" << endl;

	string meg;
	//ʹ����ԿS��������
	while (1) {
		cout << "Please enter the message to send:";

		cin >> meg;

		if (meg == "quit" || meg == "exit") {
			strcpy_s(send_buf, meg.c_str());
			send(s_server, send_buf, MaxSize, 0);
			break;
		}

		//����ǰʹ��S����	
		string megEn = des_encrypt(meg, secret);	
		strcpy_s(send_buf, megEn.c_str());

		send_len = send(s_server, send_buf, MaxSize, 0);
		if (send_len < 0) {
			//cout << "����ʧ�ܣ�" << endl;
			cout << "Send failed!" << endl;
			break;
		}
		cout << "�Ѽ��ܲ�����" << endl;

		recv_len = recv(s_server, recv_buf, MaxSize, 0);
		if (recv_len < 0) {
			//cout << "����ʧ�ܣ�" << endl;
			cout << "Recieve failed!" << endl;
			break;
		}
		else {
			//���պ�ʹ��S����
			meg = recv_buf;
			cout << "Server:" << des_decrypt(meg) << endl;
		}
	}
	//�ر��׽���
	closesocket(s_server);
	//�ͷ�DLL��Դ
	WSACleanup();

	cout << endl;
	cout << "�ѶϿ����ӣ��ر��׽���" << endl;
	system("pause");

	return 0;
}
