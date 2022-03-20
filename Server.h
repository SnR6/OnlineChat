#pragma once

namespace Server {
	IpAddress ip = IpAddress::getLocalAddress(); // ip address

	UdpSocket sock; // socket
	unsigned short port; // port

	vector <User> Users; // clients

	void DownloadUsers() {
		ifstream in("Accounts.txt"); // file with user data
		string name, password;
		while (in >> name >> password)
		{
			User uuser;
			uuser.name = name;
			uuser.password = password;
			Users.push_back(uuser);
		}
		in.close();
	}

	void UploadUsers() {
		ofstream out("Accounts.txt"); // file with user data
		for (int i = 0; i < Users.size(); i++) {
			out << Users[i].name << ' ' << Users[i].password << endl;
		}
		out.close();
	}

	void NewMessage() { // wait new message
		while (true) {
			IpAddress rIp; // sender ip
			unsigned short port; // sender port
			Packet message; // message
			if (sock.receive(message, rIp, port) == Socket::Done) { // give message
				string type; // type command
				message >> type;
				if (type == "Connect") { // connect
					string name; // username
					string password; // password
					string logreg = "reg"; // regestration or login
					message >> name >> password;
					for (int i = 0; i < Users.size(); i++)
						if (Users[i].name == name)
							logreg = "log"; // if find this user -> login | else registration

					if (logreg == "reg") { // registation ->
						User nuser;
						nuser.name = name;
						nuser.password = password;
						nuser.ip = rIp;
						nuser.port = port;
						nuser.isActive = true;
						Users.push_back(nuser);
						cout << "Connections: Registration new User: " << name << ' ' << password << endl;
						UploadUsers();
					}
					else if (logreg == "log") { // login ->
						cout << "Connections: Login User: " << name << ' ' << password << endl;
						for (int i = 0; i < Users.size(); i++)
							if (Users[i].name == name)
								if (Users[i].password == password) {
									Users[i].ip = rIp;
									Users[i].port = port;
									Users[i].isActive = true;
								}
								else
									cout << "Error: Wrong password: " << name << ' ' << password << endl;
					}

				}
				else if (type == "Message") { // message
					string text, username; // message / sender nickname
					message >> text;

					bool find = false;
					for (int i = 0; i < Users.size(); i++) {
						if (Users[i].ip == rIp && Users[i].port == port) { // find sender
							username = Users[i].name;
							find = true;
						}
					}

					if (find) {
						//cout << username << " | " << text << endl;
						message.clear(); // clear this message
						message << username << text; // new message 
						for (int i = 0; i < Users.size(); i++) {
							if (Users[i].name != username && Users[i].isActive)
								sock.send(message, Users[i].ip, Users[i].port); // send a message to all users except the sender
						}
					}
				}
			}
		}
	}

	void Settings() { // set port
		port = sock.getLocalPort();
		sock.bind(port); // port 0 - error
		port = sock.getLocalPort();
		sock.bind(port); // normal port

		cout << ip.toString() << endl << port << endl; // ip / port
	}

	void Startup() {
		Settings();
		DownloadUsers();

		thread nc(NewMessage); // new thread to check message
		nc.detach();

		string command = "";
		while (command != "exit") { // close server
			cin >> command;
		}
		UploadUsers();
	}
};