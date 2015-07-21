// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� SERIAL_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// SERIAL_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef SERIAL_EXPORTS
#define SERIAL_API __declspec(dllexport)
#else
#define SERIAL_API __declspec(dllimport)
#endif

/*
// �����Ǵ� serial.dll ������
class SERIAL_API CSerial {

private:

public:
	CSerial(void);
	// TODO:  �ڴ�������ķ�����
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

