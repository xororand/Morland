#include "mServer/Server.h"

int main() {
	IMGUI_CHECKVERSION();
	Server server(60);
	server.run();
	return 0;
}