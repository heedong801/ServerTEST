// ConnectClientBase.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <conio.h>

#include <WinSock2.h>
#include <Windows.h> // 아래쪽에 놔야 합니다.
#include <WS2tcpip.h> // inet_pton 함수를 사용하기 위한 헤더
#include <mutex>

#pragma comment (lib, "ws2_32") // 서버통신용 구현이 들어있는 라이브러리

std::mutex Lock;

std::thread RecvThread;

void RecvFunc(SOCKET _SessionSocket) 
{
	while (true)
	{
		char Buffer[1024];

		int Result = recv(_SessionSocket, Buffer, sizeof(Buffer), 0);

		std::cout << Buffer << std::endl;
	}
}

int main()
{
	WSADATA wsa;

	if (0 != WSAStartup(MAKEWORD(2, 2), &wsa))
	{
		std::cout << "WSAStartup Error" << std::endl;
	}

	SOCKET SessionSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (INVALID_SOCKET == SessionSocket)
	{
		return 0;
	}

	std::cout << "IP주소를 입력해주세요" << std::endl;
	std::string Ip;
	std::string Port;
	std::cin >> Ip;

	if ("Q" == Ip
		|| "q" == Ip)
	{
		Ip = "127.0.0.1";
	}

	SOCKADDR_IN Add = { 0, };
	Add.sin_family = AF_INET;
	Add.sin_port = htons(30001);
	if (SOCKET_ERROR == inet_pton(AF_INET, Ip.c_str(), &Add.sin_addr))
	{
		return 0;
	}
	int Len = sizeof(SOCKADDR_IN);

	if (SOCKET_ERROR == connect(SessionSocket, (const sockaddr*)&Add, Len))
	{
		return 0;
	}

	RecvThread = std::thread(RecvFunc, SessionSocket);

	while (true)
	{
		std::string In;

		// Lock.lock();

		std::cin >> In;

		char Buffer[1024] = { 0 };

		memcpy_s(Buffer, sizeof(Buffer), In.c_str(), In.size());

		int Result = send(SessionSocket, Buffer, sizeof(Buffer), 0);

		// Lock.unlock();
	}



	// std::cout << "접속에 성공했습니다." << std::endl;

	_getch();

}
