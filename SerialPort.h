#pragma once
#include <windows.h>

class CSerialPort
{

private:

	HANDLE      m_hComm; // Device handle 
	OVERLAPPED  m_ov;   // A structure that contains informations which are
						// used for asynchronous input and output operations
	TCHAR       m_lpszErrorMessage[256];
public:

	CSerialPort(VOID);
	CSerialPort(LPCSTR lpszPortNum);
	~CSerialPort() { Close(); }

	// For more definitions see <winbase.h>
	BOOL	Open(LPCSTR lpszPortNum = "com1",
		DWORD  dwBaudRate = CBR_9600,
		BYTE   byParity = NOPARITY,
		BYTE   byStopBits = ONESTOPBIT,
		BYTE   byByteSize = 8);
	VOID	Close(VOID);

	DWORD	WriteData(LPCVOID lpData, DWORD dwLen);
	DWORD	ReadData(LPVOID  lpDest, DWORD dwLen, DWORD dwMaxWait = 500);

	LPSTR	GetErrorMessage(VOID) { return m_lpszErrorMessage; }

private:

	VOID	ErrorToString(LPCSTR lpszMessage);
};

