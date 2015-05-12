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



