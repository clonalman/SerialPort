#include "stdafx.h"
#include "SerialPort.h"



CSerialPort::CSerialPort(VOID)
{
	m_hComm = 0;
	m_lpszErrorMessage[0] = '\0';
	ZeroMemory(&m_ov, sizeof(m_ov));
}


CSerialPort::CSerialPort(LPCSTR lpszPortNum)
{
	CSerialPort::CSerialPort();
	CSerialPort::Open(lpszPortNum);
}

BOOL CSerialPort::Open(LPCSTR lpszPortNum,
	DWORD  dwBaudRate,
	BYTE   byParity,
	BYTE   byStopBits,
	BYTE   byByteSize)
  {
	DCB  dcb; // structure that defines the control setting for a serial communications device
	BOOL bSuccess;

	m_hComm = CreateFile(lpszPortNum,           // pointer to name of the file
		GENERIC_READ | GENERIC_WRITE, // access mode
		0,                     // comm devices must be opened w/exclusive-access 
		NULL,                  // no security attributs 
		OPEN_EXISTING,         // comm devices must use OPEN_EXISTING 
		FILE_FLAG_OVERLAPPED,  // overlapped I/O
		NULL);                 // hTemplate must be NULL for comm devices 
	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		// handle the error
		CSerialPort::ErrorToString("Open(): CreateFile() failed, invalid handle value");

		return FALSE;
	}
	//
	// Omit the call to SetupComm to use the default queue sizes.
	// Get the current configuration.
	//
	bSuccess = GetCommState(m_hComm, &dcb);
	if (!bSuccess)
	{
		// Handle the error.
		CSerialPort::ErrorToString("Open(): GetCommState() failed");
		CSerialPort::Close();

		return FALSE;
	}
	//
	// Fill in the DCB: baud=9600, 8 data bits, no parity, 1 stop bit are default parameters
	//
	dcb.BaudRate = dwBaudRate;
	dcb.ByteSize = byByteSize;
	dcb.Parity = byParity;
	dcb.StopBits = byStopBits;
	bSuccess = SetCommState(m_hComm, &dcb);
	if (!bSuccess)
	{
		// Handle the error. 
		CSerialPort::ErrorToString("Open(): SetCommState() failed");
		CSerialPort::Close();

		return FALSE;
	}
	return TRUE;
}

VOID CSerialPort::Close(VOID)
{
	if (m_hComm > 0)
	{
		CloseHandle(m_hComm);
	}
	m_hComm = 0;
}


DWORD CSerialPort::WriteData(
	LPCVOID lpData, 
	DWORD	dwLen)
{
	BOOL  bSuccess;
	DWORD written = 0;

	if (dwLen < 1)
		return (0);

	// create event for overlapped I/O
	m_ov.hEvent = CreateEvent(NULL,   // pointer to security attributes 
		FALSE,   // flag for manual-reset event 
		FALSE,  // flag for initial state 
		NULL);    // pointer to event-object name 

	if (m_ov.hEvent == INVALID_HANDLE_VALUE)
	{
		// Handle the error.
		CSerialPort::ErrorToString("WriteData(): CreateEvent() failed");
		return (-1);
	}

	bSuccess = WriteFile(m_hComm,   // handle to file to write to  
		lpData,    // pointer to data to write to file 
		dwLen,      // number of bytes to write 
		&written, // pointer to number of bytes written 
		&m_ov);   // pointer to structure needed for overlapped I/O

	if (bSuccess)
	{
		bSuccess = GetOverlappedResult(m_hComm, &m_ov, &written, TRUE);

		if (!bSuccess)
		{
			// Handle the error.
			CloseHandle(m_ov.hEvent);
			CSerialPort::ErrorToString("WriteData(): GetOverlappedResult() failed");
			return (-1);
		}
	}
	else if (dwLen != written)
	{
		// Handle the error.
		CloseHandle(m_ov.hEvent);
		CSerialPort::ErrorToString("WriteData(): WriteFile() failed");
		return (-1);
	}
	CloseHandle(m_ov.hEvent);
	return written;
}

DWORD CSerialPort::ReadData(
	LPVOID lpDest,
	DWORD  dwLen,
	DWORD  dwMaxWait)
{
	BOOL  bSuccess;
	DWORD result = 0,
		read = 0, // num read bytes
		mask = 0; // a 32-bit variable that receives a mask 
	// indicating the type of event that occurred

	if (dwLen < 1) return(0);

	// create event for overlapped I/O
	m_ov.hEvent = CreateEvent(NULL,   // pointer to security attributes 
		FALSE,   // flag for manual-reset event 
		FALSE,  // flag for initial state 
		NULL);    // pointer to event-object name 

	if (m_ov.hEvent == INVALID_HANDLE_VALUE)
	{
		// Handle the error.
		CSerialPort::ErrorToString("ReadData(): CreateEvent() failed");
		return(-1);
	}

	// Specify here the event to be enabled
	bSuccess = SetCommMask(m_hComm, EV_RXCHAR);
	if (!bSuccess)
	{
		// Handle the error.
		CloseHandle(m_ov.hEvent);
		CSerialPort::ErrorToString("ReadData(): SetCommMask() failed");
		return(-1);
	}

	// WaitForSingleObject
	bSuccess = WaitCommEvent(m_hComm, &mask, &m_ov);
	if (!bSuccess)
	{
		int err = GetLastError();

		if (err == ERROR_IO_PENDING)
		{
			result = WaitForSingleObject(m_ov.hEvent, dwMaxWait);  //wait dwMaxWait
			// milli seconds before returning
			if (result == WAIT_FAILED)
			{
				// Handle the error.
				CloseHandle(m_ov.hEvent);
				CSerialPort::ErrorToString("ReadData(): WaitForSingleObject() failed");
				return(-1);
			}
		}
	}

	// The specified event occured?
	if (mask & EV_RXCHAR)
	{
		bSuccess = ReadFile(m_hComm, // handle of file to read 
			lpDest,  // address of buffer that receives data 
			dwLen,    // number of bytes to read 
			&read,  // address of number of bytes read 
			&m_ov); // address of structure for data 

		if (bSuccess)
		{
			bSuccess = GetOverlappedResult(m_hComm, &m_ov, &read, TRUE);

			if (!bSuccess)
			{
				// Handle the error.
				CloseHandle(m_ov.hEvent);
				CSerialPort::ErrorToString("WriteData(): GetOverlappedResult() failed");
				return(-1);
			}
		}
		else
		{
			// Handle the error.
			CloseHandle(m_ov.hEvent);
			CSerialPort::ErrorToString("ReadData(): ReadFile() failed");
			return(-1);
		}
	}
	else
	{
		// Handle the error.
		CloseHandle(m_ov.hEvent);
		wsprintf(m_lpszErrorMessage, "Error ReadData(): No EV_RXCHAR occured\n");
		return(-1);
	}
	CloseHandle(m_ov.hEvent);
	return read;
}


VOID CSerialPort::ErrorToString(LPCSTR lpszMessage)
{
	LPVOID lpMessageBuffer;
	DWORD  error = GetLastError();

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,      // source and processing options
		NULL,                            // pointer to message source
		error,                           // requested message identifie
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // the user default language.
		(LPTSTR)&lpMessageBuffer,       // pointer to message buffer
		0,                               // maximum size of message buffer
		NULL);                           // address of array of message inserts 

	// and copy it in our error string
	wsprintf(m_lpszErrorMessage, "%s: (%d) %s\n", lpszMessage, error, lpMessageBuffer);

	LocalFree(lpMessageBuffer);
}

