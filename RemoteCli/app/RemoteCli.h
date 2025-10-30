#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <memory>
#define RemoteCli_API __declspec(dllexport)

extern "C" __declspec(dllexport)
int RemoteCli_init(void);

extern "C" __declspec(dllexport)
int RemoteCli_Release(void);

extern "C" __declspec(dllexport)
int RemoteCli_connect(void);

extern "C" __declspec(dllexport)
int RemoteCli_disconnect(void);

extern "C" __declspec(dllexport)
int setDeviceProperty(char* code, int64_t data, bool blocking);

extern "C" __declspec(dllexport)
int64_t getDeviceProperty(char* code);

extern "C" __declspec(dllexport)
int sendCommand(char* inputLine);
