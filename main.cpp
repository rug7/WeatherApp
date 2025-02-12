#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#endif

#include "httplib.h"
#include "WeatherApp.h"

int main() {
    WeatherApp app;
    app.Run();
    return 0;
}