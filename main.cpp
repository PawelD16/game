#include <iostream>
#include <string>

#include "Player.h"

int main(int argc, char** argv) {

	if (argc != 5)
	{
		throw std::invalid_argument("Input correct agrs to console!");
	}

	const std::string map(argv[1]);
	const std::string status(argv[2]);
	const std::string orders(argv[3]);
	const std::string timeLimit(argv[4]);

	Player player(map, status, orders, std::stoi(timeLimit));
	player.takeATurn();

	return 0;
}
