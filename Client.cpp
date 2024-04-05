#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <string>
#include <Windows.h>
#pragma warning(disable: 4996)

using namespace std;

SOCKET Connection;

void ClearCin() {
	cin.clear();
	cin.ignore((numeric_limits<streamsize>::max)(), '\n');
}

void ClientHandler() {
	int msg_size;
	int check;
	while (true) {
		
		check = recv(Connection, (char*)&msg_size, sizeof(int), NULL);

		if (check > 0) {
			char* msg = new char[msg_size + 1];
			msg[msg_size] = '\0';
			recv(Connection, msg, msg_size, NULL);

			cout << msg;
			delete[] msg;
		}
		else if (check == -1) {
			cout << "\n!!!Соединение с сервером потеряно!!!";
			closesocket(Connection);
			WSACleanup();
			exit(0);
		}
		else exit(0);
	}
}

int main(int argc, char* argv[]) {
	
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	//WSAStartup
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		cout << "Ошибка" << endl;
		exit(1);
	}

	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;

	Connection = socket(AF_INET, SOCK_STREAM, NULL);

	//Изначальный заруск программы
	//while (true) {
	//	cout << "Подключиться к серверу, чтоб начать игру? [y/n]   ";
	//	char startProgram;
	//	cin >> startProgram;
	//	if (startProgram == 'y') {
	//		break;
	//	}
	//	else {
	//		cout << "Вы ввели NO или ошибочное выражение. Не выходить из программы? [y/n]   ";
	//		ClearCin();
	//		cin >> startProgram;
	//		if (startProgram != 'y') {
	//			return 0;
	//		}
	//	}
	//}
	
	//Выход из программы в случае превышение лимита ожидания
	int timer = 0;
	while (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) {
		if (timer == 0) cout << "Ошибка: не удалось подключиться к серверу.\n";
		timer += 1;
		Sleep(500);
		if (timer == 4) {
			cout << "!!! К серверу так и не удалось подключиться !!!\n"; 
			return 0;
		}
	}
	cout << "Соединение с сервером установлено!\n";

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, NULL, NULL, NULL);

	string msg1;
	while (true) {
		getline(cin, msg1);
		int msg_size = msg1.size();
		send(Connection, (char*)&msg_size, sizeof(int), NULL);
		send(Connection, msg1.c_str(), msg_size, NULL);
		Sleep(10);
	}


	system("pause");
	return 0;
}