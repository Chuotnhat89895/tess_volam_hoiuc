/*****************************************************************************************
//	�����ʷ����Core�Ľӿڷ�������
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-12-20
------------------------------------------------------------------------------------------
	��磨�����ϵͳ��ͨ���˽ӿڴ�Core��ȡ��Ϸ�������ݡ�
*****************************************************************************************/

#ifndef CORESERVERSHELL_H
#define CORESERVERSHELL_H

#include "CoreServerDataDef.h"

//=========================================================
// Core�ⲿ�ͻ���core�Ĳ����������������
//=========================================================
enum SERVER_SHELL_OPERATION_INDEX
{
	SSOI_LAUNCH = 1,   //��������
	SSOI_SHUTDOWN,	   //�رշ���
	SSOI_BROADCASTING, // boardcasting
	// uParam = (char*)pMessage
	// nParam = (int)nMsgLen

	SSOI_RELOAD_WELCOME_MSG, // reload the server's welcome msg

	SSOI_TONG_CREATE,	  // relay ��ᴴ���ɹ���֪ͨ core ������Ӧ�Ĵ���
	SSOI_TONG_REFUSE_ADD, // �ܾ�����������
	SSOI_TONG_ADD,		  // relay ����Ա���ӳɹ���֪ͨ core ������Ӧ�Ĵ���
};

//=========================================================
// Core�ⲿ�ͻ���core��ȡ��Ϸ���ݵ�������������������
//=========================================================
//����������������ز���uParam��nParam�����ע����δ�ἰ���򴫵ݶ�ֵ0��
//����ر�ָ������ֵ���壬��ɹ���ȡ���ݷ���1��δ�ɹ�����0��
enum GAMEDATA_INDEX
{
	SGDI_CHARACTER_NAME,
	// uParam = (char*) buffer to retrive character name
	// nParam = Client index
	// Return = bIsExistCharacter
	SGDI_LOADEDMAP_ID,
	SGDI_CHARACTER_ACCOUNT,
	SGDI_CHARACTER_ID,
	SGDI_CHARACTER_NETID,
	SGDI_CHARACTER_EXTPOINT,
	SGDI_CHARACTER_EXTPOINTCHANGED,
	// uParam = (char*) id buffer

	// �����Ὠ�����������������Ƿ����
	// uParam : struct STONG_SERVER_TO_CORE_APPLY_CREATE point
	// return : �����Ƿ����
	SGDI_TONG_APPLY_CREATE,
	SGDI_TONG_APPLY_CHANGE,

	// ���������
	// uParam : struct STONG_SERVER_TO_CORE_APPLY_ADD point
	SGDI_TONG_APPLY_ADD,

	// �жϼ����������Ƿ����
	// uParam : ����� char point �����ڽ��հ������
	// nParam : struct STONG_SERVER_TO_CORE_CHECK_ADD_CONDITION point
	SGDI_TONG_CHECK_ADD_CONDITION,

	// ��ð����Ϣ
	// uParam : ����� STONG_SERVER_TO_CORE_GET_INFO point
	SGDI_TONG_GET_INFO,

	// �ж��Ƿ�������Ȩ��
	// uParam : ����� TONG_APPLY_INSTATE_COMMAND point
	// nParam : PlayerIndex
	SGDI_TONG_INSTATE_POWER,

	// ��������������ݱ仯
	// uParam : ����� STONG_SERVER_TO_CORE_BE_INSTATED point
	SGDI_TONG_BE_INSTATED,

	// �ж��Ƿ�������Ȩ��
	// uParam : ����� TONG_APPLY_KICK_COMMAND point
	// nParam : PlayerIndex
	SGDI_TONG_KICK_POWER,

	// ���߳����
	// uParam : ����� STONG_SERVER_TO_CORE_BE_KICKED point
	SGDI_TONG_BE_KICKED,

	// �뿪����ж�
	// uParam : ����� TONG_APPLY_LEAVE_COMMAND point
	// nParam : PlayerIndex
	SGDI_TONG_LEAVE_POWER,

	// �뿪���
	// uParam : ����� STONG_SERVER_TO_CORE_LEAVE point
	SGDI_TONG_LEAVE,

	// �ܷ�λ�ж�
	// uParam : ����� TONG_APPLY_CHANGE_MASTER_COMMAND point
	// nParam : PlayerIndex
	SGDI_TONG_CHANGE_MASTER_POWER,

	// �ܷ���ܴ�λ�ж�
	// uParam : ����� STONG_SERVER_TO_CORE_CHECK_GET_MASTER_POWER point
	SGDI_TONG_GET_MASTER_POWER,

	// ��λ�������ݸı�
	// uParam : ����� STONG_SERVER_TO_CORE_CHANGE_AS point
	SGDI_TONG_CHANGE_AS,

	// ��������
	// uParam : ����� STONG_SERVER_TO_CORE_CHANGE_MASTER point
	SGDI_TONG_CHANGE_MASTER,

	// ��ð�����ַ���ת���ɵ� dword
	// nParam : PlayerIndex
	SGDI_TONG_GET_TONG_NAMEID,

	// ��½ʱ���ð����Ϣ
	// uParam : ����� STONG_SERVER_TO_CORE_LOGIN point
	SGDI_TONG_LOGIN,

	// ֪ͨcore����ĳ��ҵİ����Ϣ
	// nParam : player index
	SGDI_TONG_SEND_SELF_INFO,

	SGDI_CHARACTER_SEX,

	SGDI_TONG_CHANGE_TITLE_FEMALE,

	SGDI_TONG_CHANGE_TITLE_MALE,

	SGDI_TONG_CHANGE_TITLE,

	SGDI_TONG_DEPOSIT_MONEY,
	SGDI_TONG_TAKE_MONEY,

	SGDI_CHARACTER_CAMP,
};

#ifdef _STANDALONE
	class IClient;
#else
	struct IClient;
#endif

#ifndef _STANDALONE
	struct _declspec(novtable) iCoreServerShell
#else
	struct iCoreServerShell
#endif
{
	virtual int GetLoopRate() = 0;
	virtual void GetGuid(int nIndex, void *pGuid) = 0;
	virtual DWORD GetExchangeMap(int nIndex) = 0;
	virtual bool IsPlayerLoginTimeOut(int nIndex) = 0;
	virtual void RemovePlayerLoginTimeOut(int nIndex) = 0;
	virtual bool IsPlayerExchangingServer(int nIndex) = 0;
	virtual void ProcessClientMessage(int nIndex, const char *pChar, int nSize) = 0;
	virtual void ProcessNewClientMessage(IClient *pTransfer, DWORD dwFromIP, DWORD dwFromRelayID, int nPlayerIndex, const char *pChar, int nSize) = 0;
	virtual void SendNetMsgToTransfer(IClient *pClient) = 0;
	virtual void SendNetMsgToChat(IClient *pClient) = 0;
	virtual void SendNetMsgToTong(IClient *pClient) = 0;
	virtual void ProcessBroadcastMessage(const char *pChar, int nSize) = 0;
	virtual void ProcessExecuteMessage(const char *pChar, int nSize) = 0;
	virtual void ClientDisconnect(int nIndex) = 0;
	virtual void RemoveQuitingPlayer(int nIndex) = 0;
	virtual void *SavePlayerDataAtOnce(int nIndex) = 0;
	virtual bool IsCharacterQuiting(int nIndex) = 0;
	virtual bool CheckProtocolSize(const char *pChar, int nSize) = 0;
	virtual bool PlayerDbLoading(int nPlayerIndex, int bSyncEnd, int &nStep, unsigned int &nParam) = 0;

	virtual void ExSpcritStarPlayer(int nPlayerIndex) = 0;

	virtual void ExSpcritStopPlayer(int nPlayerIndex) = 0;

	virtual void KickAcountAndBlock(int nPlayerIndex, const char *szNameCpuIp) = 0;

	virtual int AttachPlayer(const unsigned long lnID, GUID *pGuid) = 0;
	virtual void GetPlayerIndexByGuid(GUID *pGuid, int *pnIndex, int *plnID) = 0;
	virtual void AddPlayerToWorld(int nIndex) = 0;
	virtual void *PreparePlayerForExchange(int nIndex) = 0;
	virtual void PreparePlayerForLoginFailed(int nIndex) = 0;
	virtual void RemovePlayerForExchange(int nIndex) = 0;
	virtual void RecoverPlayerExchange(int nIndex) = 0;
	virtual int AddCharacter(int nExtPoint, int nChangeExtPoint, void *pBuffer, GUID *pGuid) = 0;
	virtual int AddTempTaskValue(int nIndex, const char *pData) = 0;
	//����Ϸ���Ͳ���
	virtual int OperationRequest(unsigned int uOper, unsigned int uParam, int nParam) = 0;
	//��ȡ����״��
	virtual int GetConnectInfo(KCoreConnectInfo *pInfo) = 0;
	//����Ϸ�����ȡ����
	virtual int GetGameData(unsigned int uDataId, unsigned int uParam, int nParam) = 0;
	//�ճ����core���Ҫ���������򷵻�0�����򷵻ط�0ֵ
	virtual int Breathe() = 0;

	//�ͷŽӿڶ���
	virtual void Release() = 0;
	virtual void SetSaveStatus(int nIndex, UINT uStatus) = 0;
	virtual UINT GetSaveStatus(int nIndex) = 0;
	virtual BOOL GroupChat(IClient *pClient, DWORD FromIP, unsigned long FromRelayID, DWORD channid, BYTE tgtcls, DWORD tgtid, const void *pData, size_t size) = 0;
	virtual void SetLadder(void *pData, size_t uSize) = 0;
	virtual BOOL PayForSpeech(int nIndex, int nType) = 0;
	virtual BOOL PayForSpeech2(int nIndex, int nType, int nChanelId) = 0;

	virtual BOOL CheckCastChat(int nIndex, int nType) = 0;
	// UYTHAC
	virtual int IsCharacterLiXian(int nIndex) = 0;
	virtual int IsCheckLiXian(int nIndex) = 0;
	virtual void SetCharacterLiXian(int nIndex, int i) = 0;
	// END
};

#ifndef CORE_EXPORTS
	//��ȡiCoreShell�ӿ�ʵ����ָ��
	#ifndef __linxu
		extern "C"
	#endif
		iCoreServerShell *CoreGetServerShell();
	#endif
#endif
