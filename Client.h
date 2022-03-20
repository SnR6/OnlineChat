#pragma once

namespace Client {
	IpAddress sendIP;
	UdpSocket sock;
	unsigned short port;
	unsigned short SPort;

	void CheckMessage() {
		while (true) {
			Packet message;

			string text;
			getline(cin, text);

			message << "Message" << text;

			sock.send(message, sendIP, SPort);
		}
	}

	void Connect() {
		port = sock.getLocalPort();
		sock.bind(port);
		port = sock.getLocalPort();
		sock.bind(port);

		cout << "Enter server IP: ";
		cin >> sendIP;

		cout << "Enter server port: ";
		cin >> SPort;

		Packet message;
		string name;
		string password;

		cout << "Enter nickname: ";
		getline(cin, name);
		getline(cin, name);

		cout << "Enter password: ";
		getline(cin, password);

		message << "Connect" << name << password;
		sock.send(message, sendIP, SPort);
	}

	void Startup() {
		Connect();

		thread sender(CheckMessage);
		sender.detach();

		while (true) {
			Packet message;
			IpAddress tIP;
			unsigned short tPr;

			if (sock.receive(message, tIP, tPr) == Socket::Done) {
				string text;
				string username;

				message >> username >> text;
				cout << username << " | " << text << endl;
			}
		}
	}
};