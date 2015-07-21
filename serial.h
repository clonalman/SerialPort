// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 SERIAL_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// SERIAL_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef SERIAL_EXPORTS
#define SERIAL_API __declspec(dllexport)
#else
#define SERIAL_API __declspec(dllimport)
#endif

/*
// 此类是从 serial.dll 导出的
class SERIAL_API CSerial {

private:

public:
	CSerial(void);
	// TODO:  在此添加您的方法。
};


extern SERIAL_API int _stdcall nserial;

SERIAL_API int fnserial(void);
*/

extern "C" 
{
	SERIAL_API LPVOID WINAPI OpenComm(LPCSTR lpszPortNum,
		DWORD  dwBaudRate = CBR_9600,
		BYTE   byParity = NOPARITY,
		BYTE   byStopBits = ONESTOPBIT,
		BYTE   byByteSize = 8);

	SERIAL_API void WINAPI CloseComm(LPVOID lpComm);

	SERIAL_API DWORD WINAPI WriteComm(LPVOID lpComm, LPCSTR lpData, DWORD dwLen);
	SERIAL_API DWORD WINAPI ReadComm(LPVOID lpComm, LPSTR lpDest, DWORD dwLen, DWORD dwMaxWait);
}

