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

	CSerialPort(void);
	CSerialPort(LPCSTR lpszPortNum);
	~CSerialPort() { Close(); }

	// For more definitions see <winbase.h>
	bool	Open(LPCSTR lpszPortNum = "com1",
		DWORD  dwBaudRate = CBR_9600,
		BYTE   byParity = NOPARITY,
		BYTE   byStopBits = ONESTOPBIT,
		BYTE   byByteSize = 8);
	void	Close(void);
	void	Clear(void);
	void	Flush(void);

	DWORD	WriteData(LPCVOID lpData, DWORD dwLen);
	DWORD	ReadData(LPVOID  lpDest, DWORD dwLen, DWORD dwMaxWait = 500);

	LPSTR	GetErrorMessage(void) { return m_lpszErrorMessage; }
private:

	void	ErrorToString(LPCSTR lpszMessage);
};

