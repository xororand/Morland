#include "mServer/Server.h"

int main() {
	IMGUI_CHECKVERSION();
	Server server;
	server.run();
	return 0;
}