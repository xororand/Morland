#include "mServer/Server.h"

int main() {
	IMGUI_CHECKVERSION();
	Server server;
	server.run(60);
	return 0;
}