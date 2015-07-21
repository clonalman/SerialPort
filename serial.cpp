// serial.cpp : ���� DLL Ӧ�ó���ĵ���������
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
// ���ǵ���������һ��ʾ��
SERIAL_API int nserial=0;

// ���ǵ���������һ��ʾ����
SERIAL_API int fnserial(void)
{
	return 42;
}

// �����ѵ�����Ĺ��캯����
// �й��ඨ�����Ϣ������� serial.h
CSerial::CSerial()
{
	return;
}
*/

LPVOID WINAPI OpenComm(
	LPCSTR lpszPortNum, 
	DWORD  dwBaudRate,
	BYTE   byParity,
	BYTE   byStopBits,
	BYTE   byByteSize)
{
	CSerialPort *pSerial = new CSerialPort();
	if (pSerial->Open(lpszPortNum, dwBaudRate, byParity, byStopBits, byByteSize)){
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

DWORD WINAPI WriteComm(LPVOID lpComm, LPCSTR lpData, DWORD dwLen)
{
	if (lpComm != INVALID_HANDLE_VALUE){
		//CSerialPort *pSerial = (CSerialPort*)lpComm;
		CSerialPort *pSerial = static_cast<CSerialPort *>(lpComm);
		DWORD dwResult = pSerial->WriteData(lpData, dwLen);
		if (dwResult > 0 && dwResult < (DWORD)-1){
			pSerial->Flush();
			pSerial->Clear();
		}
		return dwResult;
	}
	return (0);
}

DWORD WINAPI ReadComm(LPVOID lpComm, LPSTR lpDest, DWORD dwLen, DWORD dwMaxWait)
{
	if (lpComm != INVALID_HANDLE_VALUE){
		//CSerialPort *pSerial = (CSerialPort*)lpComm;
		char *buffer = new char[dwLen];
		CSerialPort *pSerial = static_cast<CSerialPort *>(lpComm);
		DWORD dwResult = pSerial->ReadData(buffer, dwLen, dwMaxWait);
		if (dwResult > 0 && dwResult <= dwLen){
			//strncpy(lpDest, buffer, dwResult);
			strncpy_s(lpDest, dwLen, buffer, dwResult);
		}
		return dwResult;
	}
	return (0);
}



