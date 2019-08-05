#pragma once
#ifndef _SOCKETMANAGER_H_
#define _SOCKETMANAGER_H_

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/serialization/vector.hpp>
using namespace boost::iostreams;

#define BUFFER_SIZE 512
#define DEFAULT_PORT "27015"

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>

#pragma comment (lib, "Ws2_32.lib")

class playerInfo
{

public:
	friend class boost::serialization::access;

	playerInfo(int _playerId, int _mouseX, int _mouseY, bool* _mouseInput, int* _keyInput)
		:playerId(_playerId), mouseX(_mouseX), mouseY(_mouseY)
	{
		for (int i = 0; i < sizeof(_mouseInput); i++)
			mouseInput[i] = _mouseInput[i];
		for (int i = 0; i < sizeof(keyInput) / sizeof(int); i++)
			keyInput[i] = _keyInput[i];
	}

	playerInfo() {
	}

	~playerInfo() {

	}
	int playerId;
	int mouseX;
	int mouseY;

	bool mouseInput[3];
	int keyInput[10];

	template<class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar& playerId;
		ar& mouseX;
		ar& mouseY;
		ar& mouseInput;
		ar& keyInput;
	}
};


class socketManager
{
public:
	socketManager();
	~socketManager();
	int Initialize();
	void Frame();
	void Shutdown();

	bool CheckNewConnection();

	void PushToClients();

	int receiveMessage(SOCKET ConnectSocket);
	int sendMessage(SOCKET ConnectSocket);
private:
	void CloseClientSockets(std::vector<int>);


	char sendBuffer[BUFFER_SIZE];
	char recvBuffer[BUFFER_SIZE];
	std::vector<int> delimiterIndex;
	//Last is NULL waiting for new connection
	std::vector<SOCKET> ClientSocket;
	SOCKET ListenSocket;
	playerInfo pInfo;
	int count;
};

#endif