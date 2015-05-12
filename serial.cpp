// serial.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "serial.h"
#include "SerialPort.h"
/*
#include <vector>
using namespace std;

vector<CSerialPort> vPorts;
*/
/*
// 这是导出变量的一个示例
SERIAL_API int nserial=0;

// 这是导出函数的一个示例。
SERIAL_API int fnserial(void)
{
	return 42;
}

// 这是已导出类的构造函数。
// 有关类定义的信息，请参阅 serial.h
CSerial::CSerial()
{
	return;
}
*/

LPVOID WINAPI OpenComm(LPCSTR lpszPortNum)
{
	CSerialPort *pSerial = new CSerialPort();
	if (pSerial->Open(lpszPortNum)){
		return pSerial;
	} else {
		delete pSerial;
		return INVALID_HANDLE_VALUE;
	}
}

void WINAPI CloseComm(LPVOID lpComm)
{
	if (lpComm != INVALID_HANDLE_VALUE){
		//CSerialPort *pSerial = (CSerialPort*)lpComm;
		CSerialPort *pSerial = static_cast<CSerialPort *>(lpComm);
		if (NULL != pSerial) {
			pSerial->Close();
			delete pSerial;
		}
	}
}

DWORD WINAPI WriteComm(LPVOID lpComm, LPCVOID lpData, DWORD dwLen)
{
	if (lpComm != INVALID_HANDLE_VALUE){
		//CSerialPort *pSerial = (CSerialPort*)lpComm;
		CSerialPort *pSerial = static_cast<CSerialPort *>(lpComm);
		return pSerial->WriteData(lpData, dwLen);
	}
	return (0);
}

DWORD WINAPI ReadComm(LPVOID lpComm, LPVOID lpDest, DWORD dwLen, DWORD dwMaxWait)
{
	if (lpComm != INVALID_HANDLE_VALUE){
		//CSerialPort *pSerial = (CSerialPort*)lpComm;
		CSerialPort *pSerial = static_cast<CSerialPort *>(lpComm);
		return pSerial->ReadData(lpDest, dwLen, dwMaxWait);
	}
	return (0);
}



