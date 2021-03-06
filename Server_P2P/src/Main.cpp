#include <SFML\Network.hpp>
#include <iostream>
#include "Direction.h"
#include "Utils.hpp"

void main() {
	//ESTABLECER CONEXION
	vector<Direction> aPeers;
	sf::IpAddress ip = sf::IpAddress::getLocalAddress();
	sf::TcpListener listener;

	if (listener.listen(PORT) != sf::Socket::Done) {
		cout << "No se puede vincular al puerto 5000" << endl;
	}

	//Connect clients
	for (int i = 0; i < MAX_PLAYERS; i++) {
		sf::TcpSocket sock;
		listener.accept(sock);
		//Nickname
		char receivedText[MAX_LENGTH] = { "" };
		size_t receivedLength;
		sock.receive(receivedText, MAX_LENGTH, receivedLength);
		string newNick = "";
		newNick = receivedText;
		Direction d(newNick, sock.getRemoteAddress().toString(), sock.getRemotePort());
		cout << "Socket " << newNick << ": IP - " << d.ip << " PORT - " << d.port << endl;

		sf::Packet pack;
		pack << i;
		for (int j = 0; j < i; j++) {
			pack << aPeers[j].nick << aPeers[j].ip << aPeers[j].port;
		}
		sock.send(pack);
		aPeers.push_back(d);

		sock.disconnect();
	}

	listener.close();
}