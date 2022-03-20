#pragma once

struct User
{
	string name;
	string password;

	IpAddress ip;
	unsigned short port;

	bool isActive = 0;
};