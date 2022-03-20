//#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>

using namespace std;
using namespace sf;

#include "User.h"
#include "Client.h"
#include "Server.h"

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	char ct = 's';
	cout << "s / c \n"; // s - server / c - client
	cin >> ct;

	if (ct == 's') {
		Server::Startup();
	}
	else if (ct == 'c') {
		Client::Startup();
	}
}