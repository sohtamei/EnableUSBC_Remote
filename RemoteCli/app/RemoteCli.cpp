// "get live view with http and ptz(for c#)" sample
#include <chrono>
#include <cinttypes>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <future>
#include <iostream>
#include <mutex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

#if !defined(__APPLE__)
  #if defined(USE_EXPERIMENTAL_FS) // for jetson
    #include <experimental/filesystem>
    namespace fs = std::experimental::filesystem;
  #else
    #include <filesystem>
    namespace fs = std::filesystem;
  #endif
#endif

#if defined(__APPLE__) || defined(__linux__)
  #include <unistd.h>
#endif

// macro for multibyte character
#if defined(_WIN32) || defined(_WIN64)
  using CrString = std::wstring;
  #define CRSTR(s) L ## s
  #define CrCout std::wcerr
  #define DELIMITER CRSTR("\\")
#else
  using CrString = std::string;
  #define CRSTR(s) s
  #define CrCout std::cerr
  #define DELIMITER CRSTR("/")
#endif


#include "CRSDK/CrDeviceProperty.h"
#include "CRSDK/CameraRemote_SDK.h"
#include "CRSDK/IDeviceCallback.h"
#include "CrDebugString.h"   // use CrDebugString.cpp
#include "RemoteCli.h"

#define PrintError(msg, err) { fprintf(stderr, "Error in %s(%d):" msg ",%s\n", __FUNCTION__, __LINE__, (err ? CrErrorString(err).c_str():"")); }
#define GotoError(msg, err) { PrintError(msg, err); goto Error; }

std::vector<std::string> _split(std::string inputLine, char delimiter)
{
    std::vector<std::string> strArray;
    if (inputLine.empty()) return strArray;

    std::string tmp;
    std::stringstream ss{inputLine};
    while (getline(ss, tmp, delimiter)) {
        strArray.push_back(tmp);
    }
    return strArray;
}


class CameraDevice : public SCRSDK::IDeviceCallback
{
public:
	int64_t  m_device_handle = 0;
	bool  m_connected = false;
	CrString m_modelId;

	std::mutex m_eventPromiseMutex;
	uint32_t m_setDPCode = 0;
	std::promise<void>* m_eventPromise = nullptr;
	void setEventPromise(std::promise<void>* dp)
	{
	    std::lock_guard<std::mutex> lock(m_eventPromiseMutex);
	    m_eventPromise = dp;
	}

	SCRSDK::CrError getDeviceProperty(uint32_t code, SCRSDK::CrDeviceProperty* devProp)
	{
	    std::int32_t nprop = 0;
	    SCRSDK::CrDeviceProperty* prop_list = nullptr;
	    SCRSDK::CrError err = SCRSDK::GetSelectDeviceProperties(m_device_handle, 1, &code, &prop_list, &nprop);
	    if(err) GotoError("", err);
	    if(prop_list && nprop >= 1) {
	        *devProp = prop_list[0];
	    }
	Error:
	    if(prop_list) SCRSDK::ReleaseDeviceProperties(m_device_handle, prop_list);
	    return err;
	}

	SCRSDK::CrError setDeviceProperty(uint32_t code, uint64_t data, bool blocking=true)
	{
	    int result = SCRSDK::CrError_Generic_Unknown;
	    SCRSDK::CrError err = 0;
	    std::promise<void> eventPromise;
	    std::future<void> eventFuture = eventPromise.get_future();
	    std::future_status status;

	    SCRSDK::CrDeviceProperty devProp;

	    err = getDeviceProperty(code, &devProp);
	    if(err) GotoError("", err);
	    if (blocking && devProp.GetCurrentValue() == data) {
	        std::cerr << "skipped\n";
	        return 0;
	    }

	    if(blocking) {
	        std::lock_guard<std::mutex> lock(m_eventPromiseMutex);
	        m_setDPCode = code;
	        m_eventPromise = &eventPromise;
	    }

	    devProp.SetCurrentValue(data);
	    err = SCRSDK::SetDeviceProperty(m_device_handle, &devProp);
	    if(err) GotoError("", err);

	    if(!blocking) return 0;

	    status = eventFuture.wait_for(std::chrono::milliseconds(3000));
	    if(status != std::future_status::ready) GotoError("timeout", 0);

	    try{
	        eventFuture.get();
	    } catch(const std::exception&) GotoError("", 0);
	    std::cerr << "OK\n";

	    result = 0;
	Error:
	    setEventPromise(nullptr);
	    return result;
	}

    void OnConnected(SCRSDK::DeviceConnectionVersioin version)
    {
        CrCout << "Connected to " << m_modelId << "\n";
        m_connected = true;
        std::lock_guard<std::mutex> lock(m_eventPromiseMutex);
        if(m_eventPromise) {
            m_eventPromise->set_value();
            m_eventPromise = nullptr;
        }
    }

    void OnError(CrInt32u error)
    {
        fprintf(stderr, "Connection error:%s\n", CrErrorString(error).c_str());
        std::lock_guard<std::mutex> lock(m_eventPromiseMutex);
        if(m_eventPromise) {
            m_eventPromise->set_exception(std::make_exception_ptr(std::runtime_error("error")));
            m_eventPromise = nullptr;
        }
    }

    void OnDisconnected(CrInt32u error)
    {
        CrCout << "Disconnected from " << m_modelId << "\n";
        m_connected = false;
        std::lock_guard<std::mutex> lock(m_eventPromiseMutex);
        if(m_eventPromise) {
            m_eventPromise->set_value();
            m_eventPromise = nullptr;
        }
    }

    void OnCompleteDownload(CrChar* filename, CrInt32u type )
    {
        CrCout << "OnCompleteDownload:" << filename << "\n";
    }

    void OnNotifyContentsTransfer(CrInt32u notify, SCRSDK::CrContentHandle contentHandle, CrChar* filename)
    {
        std::cerr << "OnNotifyContentsTransfer.\n";
    }

    void OnWarning(CrInt32u warning)
    {
        if (warning == SCRSDK::CrWarning_Connect_Reconnecting) {
            CrCout << "Reconnecting to " << m_modelId << "\n";
            return;
        }
        std::cerr << "OnWarning:" << CrErrorString(warning) << "\n";
    }

    void OnWarningExt(CrInt32u warning, CrInt32 param1, CrInt32 param2, CrInt32 param3) {}
    void OnLvPropertyChanged() {}
    void OnLvPropertyChangedCodes(CrInt32u num, CrInt32u* codes) {}
    void OnPropertyChanged() {}
    void OnPropertyChangedCodes(CrInt32u num, CrInt32u* codes)
    {
        //std::cerr << "OnPropertyChangedCodes:\n";
        for(uint32_t i = 0; i < num; ++i) {
            std::lock_guard<std::mutex> lock(m_eventPromiseMutex);
            if(m_setDPCode && m_setDPCode == codes[i]) {
                m_setDPCode = 0;
                if(m_eventPromise) {
                    m_eventPromise->set_value();
                    m_eventPromise = nullptr;
                }
            }
        }
    }
    
    void OnNotifyMonitorUpdated(CrInt32u type, CrInt32u frameNo) {}

	CrString _getModelId(const SCRSDK::ICrCameraObjectInfo* objInfo)
	{
	    CrString id;
	    if (CrString(objInfo->GetConnectionTypeName()) == CRSTR("IP")) {
	        id = CrString(objInfo->GetMACAddressChar());
	    } else {
	        id = CrString((CrChar*)objInfo->GetId());
	    }
	    return CrString(objInfo->GetModel()).append(CRSTR(" (")).append(id).append(CRSTR(")"));
	}

    SCRSDK::CrError connect(void)
    {
	    SCRSDK::CrError err = SCRSDK::CrError_None;
	    SCRSDK::ICrEnumCameraObjectInfo* enumCameraObjectInfo = nullptr;
	    SCRSDK::ICrCameraObjectInfo* objInfo = nullptr;

	  #if defined(__APPLE__)
	    #define MAC_MAX_PATH 255
	    char pathBuf[MAC_MAX_PATH] = {0};
	    if(NULL == getcwd(pathBuf, sizeof(pathBuf) - 1)) return 1;
	    CrString path = pathBuf;
	  #else
	    CrString path = fs::current_path().native();
	  #endif

	    // enumeration
	    {
	        uint32_t count = 0;
	        uint32_t index = 0;

	        err = SCRSDK::EnumCameraObjects(&enumCameraObjectInfo, 3/*timeInSec*/);
	        if(err || !enumCameraObjectInfo) GotoError("no camera", err);

	        count = enumCameraObjectInfo->GetCount();
            for (index = 0; index < count; ++index) {
	        	objInfo = (SCRSDK::ICrCameraObjectInfo*)enumCameraObjectInfo->GetCameraObjectInfo(index);
			    if (CrString(objInfo->GetConnectionTypeName()) == CRSTR("USB")) {
					break;
				}
            }
			if(index >= count) GotoError("no camera", err);

	        m_modelId = _getModelId(objInfo);
	    }

	    // connect
	    {
	        std::promise<void> eventPromise;
	        std::future<void> eventFuture = eventPromise.get_future();

	        setEventPromise(&eventPromise);
	        err = SCRSDK::Connect(objInfo, this, &m_device_handle,
	            SCRSDK::CrSdkControlMode_Remote,
	            SCRSDK::CrReconnecting_ON,
	            NULL, NULL, NULL, 0);
	        if(err) GotoError("", err);

	    //  std::future_status status = eventFuture.wait_for(std::chrono::milliseconds(3000));
	    //  if(status != std::future_status::ready) GotoError("timeout",0);
	        try{
	            eventFuture.get();
	        } catch(const std::exception&) GotoError("", 0);
	    }

	    // set work directory
	    {
	        CrCout << "path=" << path.data() << "\n";
	        err = SCRSDK::SetSaveInfo(m_device_handle, const_cast<CrChar*>(path.data()), const_cast<CrChar*>(CRSTR("DSC")), -1/*startNo*/);
	        if(err) GotoError("", err);
	    }

	    if(enumCameraObjectInfo) enumCameraObjectInfo->Release();
	    return 0;
	Error:
	    if(enumCameraObjectInfo) enumCameraObjectInfo->Release();
	    disconnect();
    	return -1;
	}

    SCRSDK::CrError disconnect(void)
    {
	    if(m_connected) {
	        std::promise<void> eventPromise;
	        std::future<void> eventFuture = eventPromise.get_future();
	        setEventPromise(&eventPromise);
	        SCRSDK::Disconnect(m_device_handle);
	        std::future_status status = eventFuture.wait_for(std::chrono::milliseconds(3000));
	        if(status != std::future_status::ready) PrintError("timeout",0);
	    }
	    if(m_device_handle) SCRSDK::ReleaseDevice(m_device_handle);

	    m_connected = false;
	    m_device_handle = 0;
	    fprintf(stderr, "xxx\n");
	    return 0;
	}
};

CameraDevice cameraDevice[1];

int RemoteCli_init(void)
{
    bool boolRet = SCRSDK::Init();
    if(!boolRet) GotoError("", 0);
    return 0;
Error:
	return -1;
}

int RemoteCli_Release(void)
{
    SCRSDK::Release();
    return 0;
}

int RemoteCli_connect(void)
{
	return cameraDevice[0].connect();
}

int RemoteCli_disconnect(void)
{
	return cameraDevice[0].disconnect();
}

int setDeviceProperty(char* code, int64_t data, bool blocking=true)
{
    int32_t codeInt = CrDevicePropertyCode(code);
    if(codeInt < 0) {
        PrintError("unknown DP",0);
        return SCRSDK::CrError_Generic_Unknown;
    }
    return cameraDevice[0].setDeviceProperty(codeInt, (uint64_t)data, blocking);
}

int64_t getDeviceProperty(char* code)
{
    int32_t codeInt = CrDevicePropertyCode(code);
    if(codeInt < 0) {
        PrintError("unknown DP",0);
        return 0;
    }

    int64_t data = 0;
    SCRSDK::CrError err = 0;
    SCRSDK::CrDeviceProperty devProp;
    err = cameraDevice[0].getDeviceProperty(codeInt, &devProp);
    if(err) GotoError("", err);
    data = devProp.GetCurrentValue();
Error:
    return data;
}

int sendCommand(char* inputLine)
{
    SCRSDK::CrError err = 0;
    int64_t data = 0;
    int32_t code = 0;
    std::vector<std::string> args = _split(inputLine, ' ');
    if(args.size() < 2) GotoError("invalid", 0);

    code = CrCommandIdCode(args[0]);
    if(code < 0) return -1;
    try{ data = stoi(args[1]); } catch(const std::exception&) GotoError("", 0);

    err = SCRSDK::SendCommand(cameraDevice[0].m_device_handle, code, (SCRSDK::CrCommandParam)data);
    if(err) GotoError("", err);

    if(args.size() >= 3) {
	    std::this_thread::sleep_for(std::chrono::milliseconds(50));
		try{ data = stoi(args[2]); } catch(const std::exception&) GotoError("", 0);

	    err = SCRSDK::SendCommand(cameraDevice[0].m_device_handle, code, (SCRSDK::CrCommandParam)data);
	    if(err) GotoError("", err);
    }
Error:
    return err;
}
