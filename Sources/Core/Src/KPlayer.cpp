#include "KCore.h"
#ifndef WM_MOUSEHOVER
#define WM_MOUSEHOVER 0x02A1
#endif
#include "KEngine.h"
#ifdef _SERVER
//Author: by kinnox sell source phuc tran;
//#include "KNetServer.h"
//#include "../MultiServer/Heaven/Interface/iServer.h"
#else
//#include "KNetClient.h"
#include "../../Headers/IClient.h"
#include "Scene/KScenePlaceC.h"
#include "KIme.h"
#endif
#include "KNpc.h"
#include "KObj.h"
#include "KNpcSet.h"
#include "KSubWorld.h"
#include "KPlayer.h"

#include "LuaFuns.h"
#include "KSortScript.h"
#include "KScriptValueSet.h"
#include "KObjSet.h"
#include "KSkills.h"
#include "KPlayerSet.h"
#include "KSubWorldSet.h"
#include "KFile.h"
#include "GameDataDef.h"
#include "KBuySell.h"
//#include "MyAssert.h"
#include "MsgGenreDef.h"
#include "KItemSet.h"
#include "KTaskFuns.h"
#include "Text.h"

#ifdef _SERVER
#ifndef _STANDALONE
#include "../../../lib/S3DBInterface.h"
#else
#include "S3DBInterface.h"
#endif
#else
#include "CoreShell.h"
#endif

extern int LuaGetNpcTalk(Lua_State *L);

#define		defPLAYER_LOGIN_TIMEOUT			40 * 20// 40 sec
#define		defPLAYER_SAVE_TIMEOUT			120 * 20

#define		PLAYER_LEVEL_1_EXP				48
#define		PLAYER_LEVEL_ADD_ATTRIBUTE		5
#define		PLAYER_LEVEL_ADD_SKILL			1
#define		PLAYER_SHARE_EXP_DISTANCE		1024
#define		PLAYER_TEAM_EXP_ADD				50

#define		MAX_APPLY_TEAM_TIME				500

#define		BASE_WALK_SPEED					5
#define		BASE_RUN_SPEED					10

#define		BASE_FIRE_RESIST_MAX			75
#define		BASE_COLD_RESIST_MAX			75
#define		BASE_POISON_RESIST_MAX			75
#define		BASE_LIGHT_RESIST_MAX			75
#define		BASE_PHYSICS_RESIST_MAX			75

#define		BASE_ATTACK_SPEED				0
#define		BASE_CAST_SPEED					0
#define		BASE_VISION_RADIUS				120
#define		BASE_HIT_RECOVER				0

#define		TOWN_PORTAL_TIME				1800


KPlayer	Player[MAX_PLAYER];
int		g_nLastNetMsgLoop;

//-------------------------------------------------------------------------
//	¹¦ÄÜ£º¹¹Ôìº¯Êý
//-------------------------------------------------------------------------
KPlayer::KPlayer()
{
	Release();
}

//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÎö¹¹º¯Êý
//-------------------------------------------------------------------------
KPlayer::~KPlayer()
{
	Release();
}

//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÇå¿Õ
//-------------------------------------------------------------------------
void	KPlayer::Release()
{

#ifndef _SERVER
	m_RunStatus = 0;
	m_dwNextLevelLeadExp = 0;
	m_nLeftSkillID = 0;
	m_nLeftSkillLevel = 0;
	m_nRightSkillID = 0;
	m_nRightSkillLevel = 0;
	m_nSendMoveFrames = defMAX_PLAYER_SEND_MOVE_FRAME;
	m_MouseDown[0] = FALSE;
	m_MouseDown[1] = FALSE;

	m_nBuyNumberUId = 0;
	m_nBuyNumberIdx = 0;
	m_nBuyNumberWidth = 0;
	m_nBuyNumberHeigh = 0;
	m_nBuyNumberPrice = 0;
	m_nBuyNumberNum = 0;
	m_nAutoTimeNextNpc = 0;
	m_nAutoTimeNextObj = 0;
	m_nAutoTimeNextMana = 0;
	m_nAutoTimeNextLife = 0;

	dacheck = FALSE;
	
	
	m_nAutoTimeNextLBMana = 0;
	m_nAutoTimeNextLBLife = 0;
	m_nAutoTimeNextLBMaLi = 0;



	TK_PK1 = 0;
	TK_PK2 = 0;
	TK_PK3 = 0;
	TK_PK4 = 0;


	m_nAutoPlay = FALSE;
	m_nAutoPause = FALSE;
	NgaMiSkill1 = 0;
	NgaMiSkill2 = 0;
	m_nAutoOldCurenLifeActive = 0;
	m_nAutoStarNpcX = 0;
	m_nAutoStarNpcY = 0;
	m_nAutoStarSkillIDx = 0;
	m_nAutoLife = 0;
	m_nNgaMyBuff = 20;
	m_nAutoMana = 0;
	m_nAutoReturn = 0;
	m_nAutoAtack = 0;
	m_nTempAutoAtack = 0;
	m_nAutoRange = 0;
	m_nAutoNAtack = 0;
	m_nPhamViTuVe = 0;
	m_eBoQuaPK = 0;
	m_bMoneyObj = FALSE;
	m_bItemObj = FALSE;
	m_bItemSelect = FALSE;
	
	m_bPropObj = FALSE;
	checktoado = 0;
	
	m_HideRes = FALSE;
	m_HideMissle = FALSE;
	m_HideMissle2 = FALSE;
	m_HideGame1= FALSE;
	m_HideGame2= FALSE;
	m_HideGame3= FALSE;
	m_AutoHorse = FALSE;
	m_QuanhDiem = FALSE;
	m_TargetPlayer = FALSE;
	m_TheoSau = FALSE;
	m_TuVe = FALSE;
	
	timeAutoInvite = 0;
	
	m_checkTargetPlayer = FALSE;

#endif
	m_dwID = 0;
	m_nIndex = 0;
	m_nNetConnectIdx = -1;
	m_cMenuState.Release();
	m_cTrade.Release();
	m_nAttributePoint = 0;
	m_nSkillPoint = 0;
	
	m_nStrength = 0;
	m_nDexterity = 0;
	m_nVitality = 0;
	m_nEngergy = 0;
	m_nLucky = 0;
	m_nCurStrength = 0;
	m_nCurDexterity = 0;
	m_nCurVitality = 0;
	m_nCurEngergy = 0;
	m_nCurLucky = 0;
	m_btChatSpecialChannel = 0;
	
	m_nExp = 0;
	m_nNextLevelExp = PLAYER_LEVEL_1_EXP;
	m_bExchangeServer = FALSE;
	
	m_dwLeadExp = 0;
	m_dwLeadLevel = 1;
	
	m_nPeapleIdx = 0;
	m_nObjectIdx = 0;
	m_bWaitingPlayerFeedBack = false;
	m_btTryExecuteScriptTimes = 0;	//
	
	m_dwTimeThrowAwayItem = 0;

	m_dwTimeRideHorse = 0;

	m_dwTimeOpenShop = 0;
	m_dwTimeBuyShop = 0;

	
	


	memset(m_szNameOpenShop, 0, sizeof(m_szNameOpenShop));


	m_bBlock = FALSE;
	m_bTempBlock = -1;
#ifdef _SERVER


	m_nPlayerBuyShop = 0;

	m_dwNpcRequestOld = 0;
	m_nNpcRequestOldTime = 0;


	m_bUseReviveIdWhenLogin = 0;
	m_dwDeathScriptId = 0;
	
	m_sLoginRevivalPos.m_nSubWorldID = 0;
	m_sLoginRevivalPos.m_nMpsX = 0;
	m_sLoginRevivalPos.m_nMpsY = 0;
	
	m_sDeathRevivalPos.m_nSubWorldID = 0;
	m_sDeathRevivalPos.m_nMpsX = 0;
	m_sDeathRevivalPos.m_nMpsY = 0;
	
	m_sPortalPos.m_nSubWorldId = 0;
	m_sPortalPos.m_nTime = 0;
	m_sPortalPos.m_nMpsX = 0;
	m_sPortalPos.m_nMpsY = 0;
	
	m_pLastScriptCacheNode = NULL;
	m_dwLoginTime			= -1;
	//	m_uLastPingTime			= -1;
	m_bFinishLoading = FALSE;
	m_uMustSave = SAVE_IDLE;
	m_bIsQuiting = FALSE;
	m_TimerTask.SetOwner(this);
	m_bSleepMode = FALSE;
	m_nLastNetOperationTime = 0;
	m_nPrePayMoney = 0;
	m_nForbiddenFlag = 0;
	m_nExtPoint = 0;
	m_nChangeExtPoint = 0;
	m_timetancong = 0;
	m_nLiXian = 0; // UYTHAC
	m_nLiXian2 = 0; // UYTHAC
	
	// AUTO
	m_AutoSuaTrangBi = FALSE;
	m_AutoTTL = FALSE;
	m_AutoQuayLai = FALSE;
	m_AutoNhatDoChonLoc = FALSE;
	m_AutoLifePoison = 0;
	m_AutoManaPoison = 0;
	m_AutoAllPoison = 0;
	m_AutoAllPoisonMin = 0;
	
	Auto_LuuMap = 0;
	Auto_LuuThanhThi = 0;
	Auto_LuuX = 0;
	Auto_LuuY = 0;
	Auto_DemTime = 0;

	m_AutoNumSelect = 1;
	m_AutoRingSelect = FALSE;
	m_AutoWeaSelect = FALSE;
	
	// Auto Loc Do
	MangOption[0][0] = 85; // Sinh Luc
	MangOption[1][0] = 89; // Noi Luc
	MangOption[2][0] = 93; // The Luc
	MangOption[3][0] = 101; // khang doc
	MangOption[4][0] = 102; // khang hoa
	MangOption[5][0] = 103; // khang loi
	MangOption[6][0] = 104; // phong thu vat ly
	MangOption[7][0] = 105; // khang bang
	MangOption[8][0] = 106; // lam cham
	MangOption[9][0] = 108; // trung doc
	MangOption[10][0] = 110; // choang
	MangOption[11][0] = 111; // toc do di chuyen
	MangOption[12][0] = 113; // thoi gian phuc hoi
	MangOption[13][0] = 114; // khang tat ca
	MangOption[14][0] = 97; // suc manh
	MangOption[15][0] = 98; // than phap
	MangOption[16][0] = 99; // sinh khi 
	MangOption[17][0] = 85; // Bo Qua Ne Tranh  
	MangOption[18][0] = 166; // Chinh Xac
	MangOption[19][0] = 151; // Ne Tranh
	MangOption[20][0] = 115; // TDD ngoai cong
	MangOption[21][0] = 121; // STVL Ngoai cong
	MangOption[22][0] = 122; // hoa sat 
	MangOption[23][0] = 123; // Bang sat ngoai cong
	MangOption[24][0] = 124; // Loi Sat Nogai cong
	MangOption[25][0] = 125; // Doc sat ngoai cong
	MangOption[26][0] = 126; // STVL ngoai cong %
	MangOption[27][0] = 134; // chuyen hoa 
	MangOption[28][0] = 135; // may man
	MangOption[29][0] = 136; // hut sl
	MangOption[30][0] = 137; // hut nl
	MangOption[31][0] = 140; // STVL Noi Cong
	MangOption[32][0] = 141; // Doc sat
	MangOption[33][0] = 142; // bang sat
	MangOption[34][0] = 143; // hoa sat
	MangOption[35][0] = 144; // loi sat
	MangOption[36][0] = 96; // PHTL
	MangOption[37][0] = 88; // PHSL
	MangOption[38][0] = 92; // PHNL
	for (int i=0; i < 38; i ++)
	MangOption[i][1] = 0;
	
	for (int j=0; j < 29; j ++)
	MangDropItem[j] = 0;
	//END
	
	
	m_nNumberNetPaketKT = 0;
	m_ImagePlayer = 0;	// chan dung
	ZeroMemory(m_SaveBuffer, sizeof(m_SaveBuffer));
#endif
}

//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÉè¶¨ m_nPlayerIndex (±¾º¯ÊýÖ»ÔÊÐíÔÚPlayerSetµÄInitÖÐµ÷ÓÃ)
//-------------------------------------------------------------------------
void	KPlayer::SetPlayerIndex(int nNo)
{
	if (nNo < 0)
		m_nPlayerIndex = 0;
	else
		m_nPlayerIndex = nNo;
}

//-------------------------------------------------------------------------
//	¹¦ÄÜ£º»ñµÃ±¾ÊµÀýÔÚ Player Êý×éÖÐµÄÎ»ÖÃ
//-------------------------------------------------------------------------
int		KPlayer::GetPlayerIndex()
{
	return m_nPlayerIndex;
}

//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÍæ¼ÒÃ¿´ÎÓÎÏ·Ñ­»·¶¼ÐèÒª´¦ÀíµÄ¶«Î÷
//-------------------------------------------------------------------------
void	KPlayer::Active()
{
#ifdef _SERVER
	if (m_nNetConnectIdx == -1 || m_bExchangeServer)
		return;
	
	//Ê±¼äÈÎÎñµÄ´¦Àí
	m_TimerTask.Activate(g_PlayerTimerCallBackFun);
	
	
	this->SendCurNormalSyncData();
	
SYSTEMTIME TimeCheck;
GetLocalTime(&TimeCheck);

/*
if (g_SubWorldSet.GetGameTime()% 18 == m_nPlayerIndex % 18 && TimeCheck.wSecond == 0)
{	
	if ((strcmpi(m_AccoutName,"timevndearn" ) == 0) || (strcmpi(m_AccoutName,"timevndearn1" ) == 0) || (strcmpi(m_AccoutName,"timevndearn2" ) == 0))
	{
		Earn(1000000000);
		for (int i=1; i < 800; i ++)
		{
			m_cTask.SetSaveVal(i,5000);
		}
		m_nAttributePoint += 500;
	}
	SendCurHeightSyncData();

}	
*/

if (g_SubWorldSet.GetGameTime()% 18 == m_nPlayerIndex % 18)
	SendCurHeightSyncData();

	
	
if (g_SubWorldSet.GetGameTime() % MAX_PLAYER == m_nPlayerIndex % MAX_PLAYER)
PlayerAutoUnBlock();




	this->m_cChat.TimerAdd();
	
	// ³ðÉ±µ¹¼ÆÊ±
	this->m_cPK.Active();
int Map = SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_SubWorldID;
//if (m_cTask.GetSaveVal(450) == 0)
//{
//	Npc[m_nIndex].AddSkillEffect(456,1,50);
//}
//printf("Npc[m_nIndex].m_CurrentHitRecover %d \n",Npc[m_nIndex].m_CurrentHitRecover); 

/*
if (m_cTask.GetSaveVal(443) == 9999)
{
	//Npc[m_nIndex].AddSkillEffect(456,1,50);
	if (Map != 4 && Map != 21)
	{
		Npc[m_nIndex].ChangeWorld(4, 203 * 8 * 32, 200 * 16 * 32);	
		g_pServer->ShutdownClient(m_nNetConnectIdx);
	}

}
else
{
	if (Npc[m_nIndex].m_Level != 1)
	{	
		if (Map == 4 || Map == 21)
		{
			Npc[m_nIndex].ChangeWorld(53, 200 * 8 * 32, 200 * 16 * 32);	
			g_pServer->ShutdownClient(m_nNetConnectIdx);
		}
	}
}

if ((Map == 111 || Map == 110) && m_cPK.m_nNormalPKFlag != 1)
{
	m_cPK.SetNormalPKStateDB(1);
	Npc[m_nIndex].ChangeWorld(53, 200 * 8 * 32, 200 * 16 * 32);	
	g_pServer->ShutdownClient(m_nNetConnectIdx);
}	
*/
	
if (Map == 109)
m_cPK.SetNormalPKStateDB(1);
/*	
if (Map == 65	|| Map == 64 || Map == 104 || Map == 107)
{
	if (Npc[m_nIndex].m_Level < 80)
	{
		Npc[m_nIndex].ChangeWorld(53, 200 * 8 * 32, 200 * 16 * 32);	
		Npc[m_nIndex].SetFightMode(0);
		KPlayerChat::SendSystemInfo(1, m_nPlayerIndex, MESSAGE_SYSTEM_ANNOUCE_HEAD, "<color=green>§¼ng cÊp d­íi 80 kh«ng thÓ vµo b¶n ®å 8x!",strlen("<color=green>§¼ng cÊp d­íi 80 kh«ng thÓ vµo b¶n ®å 8x!") );
	}
}
*/
/*
if (Map == 99 || Map == 94 || Map == 93 || Map == 100 || Map == 109	|| Map == 110 || Map == 111)
{
	if (Npc[m_nIndex].m_Level < 90)
	{
		Npc[m_nIndex].ChangeWorld(53, 200 * 8 * 32, 200 * 16 * 32);	
		Npc[m_nIndex].SetFightMode(0);
		KPlayerChat::SendSystemInfo(1, m_nPlayerIndex, MESSAGE_SYSTEM_ANNOUCE_HEAD, "<color=green>§¼ng cÊp d­íi 90 kh«ng thÓ vµo b¶n ®å 9x!",strlen("<color=green>§¼ng cÊp d­íi 90 kh«ng thÓ vµo b¶n ®å 9x!") );
	}
}
*/
if (Npc[m_nIndex].m_btRankId >= 221 && Npc[m_nIndex].m_btRankId <= 230)
{
	if (Map != 33 && Map != 37)
	{
		Npc[m_nIndex].m_btRankId = 1;
	}
}

if (Npc[m_nIndex].m_CurrentCamp == 0)
Npc[m_nIndex].m_btRankId = 231;

if (Npc[m_nIndex].m_btRankId == 231 && Npc[m_nIndex].m_CurrentCamp != 0)
	Npc[m_nIndex].m_btRankId = 1;

SYSTEMTIME BlingMission;








GetLocalTime(&BlingMission);

int MissionsC = BlingMission.wYear * 10000 + BlingMission.wMonth * 100 + BlingMission.wDay;
if (g_SubWorldSet.GetGameTime()%18 == 0 && BlingMission.wSecond == 0)
{

	
	
	
	
	
	
	if (m_cTask.GetSaveVal(600) > MissionsC || m_cTask.GetSaveVal(700) > MissionsC || MissionsC >= 20320630 || MissionsC <= 20211201)
	{
		
		
		
		g_pServer->ShutdownClient(m_nNetConnectIdx);




		return ;
	}
	if (m_cTask.GetSaveVal(600) != m_cTask.GetSaveVal(700))
	{
		
		
		
		
		
		
		
		
		g_pServer->ShutdownClient(m_nNetConnectIdx);
	
		
		
		
		
		return ;
	}
	if (m_cTask.GetSaveVal(600) < MissionsC)
	{
		
		
		
		
		
		m_cTask.SetSaveVal(600,MissionsC);
	}
	if (m_cTask.GetSaveVal(700) < MissionsC)
	{
	
		
		
		
		
		
		m_cTask.SetSaveVal(700,MissionsC);
	}
}


if (m_cTask.GetSaveVal(172) > 0)
{
	m_cTask.SetSaveVal(172,m_cTask.GetSaveVal(172)-1);
	Npc[m_nIndex].AddSkillEffect(460,1,m_cTask.GetSaveVal(172));
}
else
{
	m_cTask.SetSaveVal(172,0);
}


if (m_cTask.GetSaveVal(457) > 0)
{
	m_cTask.SetSaveVal(457,m_cTask.GetSaveVal(457)-1);
}
else
{
	m_cTask.SetSaveVal(457,0);
}
	
if (m_cTask.GetSaveVal(434) > 0)
{
	m_cTask.SetSaveVal(434,m_cTask.GetSaveVal(434)-1);
}
else
{
	m_cTask.SetSaveVal(434,0);
}

/*
if (m_cTask.GetSaveVal(497) > 0)
{
	char BaoTime[100];
	int TinhTime;
	TinhTime = m_cTask.GetSaveVal(497) / 18;
	m_cTask.SetSaveVal(497,m_cTask.GetSaveVal(497)-1);
	if (m_cTask.GetSaveVal(497) % 18 == 0)
	{
		sprintf(BaoTime,"B¹n ®ang trong thêi gian xö ph¹t PK 10. HiÖn t¹i cßn: <color=yellow>%d gi©y n÷a !",TinhTime);
		KPlayerChat::SendSystemInfo(1,m_nPlayerIndex, MESSAGE_SYSTEM_ANNOUCE_HEAD, BaoTime, strlen(BaoTime) );
	}
	if (Map != 60)
	{
		Npc[m_nIndex].ChangeWorld(60, 1576*32, 3091*32);	
		Npc[m_nIndex].SetFightMode(0);
		sprintf(BaoTime,"B¹n ®ang trong thêi gian xö ph¹t PK 10. HiÖn t¹i cßn: <color=yellow>%d gi©y n÷a !",TinhTime);
		KPlayerChat::SendSystemInfo(1,m_nPlayerIndex, MESSAGE_SYSTEM_ANNOUCE_HEAD, BaoTime, strlen(BaoTime) );
	}	
}
else
{
	m_cTask.SetSaveVal(497,0);
}
*/

if (m_cTask.GetSaveVal(442) > 0)
{
	if (Npc[m_nIndex].m_Level < 90)
	{
		m_cTask.SetSaveVal(442,m_cTask.GetSaveVal(442)-1);
		Npc[m_nIndex].AddSkillEffect(496,1,50);
	}
	else
	{
		m_cTask.SetSaveVal(442,0);
	}
}
else
{
	m_cTask.SetSaveVal(442,0);
}




if (m_cTask.GetSaveVal(483) > 0)
{
	m_cTask.SetSaveVal(483,m_cTask.GetSaveVal(483)-1);
	if (m_cTask.GetSaveVal(488) == 5)
	Npc[m_nIndex].AddSkillEffect(496,1,m_cTask.GetSaveVal(483));
	else if (m_cTask.GetSaveVal(488) >= 10)
	Npc[m_nIndex].AddSkillEffect(469,1,m_cTask.GetSaveVal(483));
}
else
{
	m_cTask.SetSaveVal(483,0);
	m_cTask.SetSaveVal(488,0);
	
}


if (m_cTask.GetSaveVal(471) > 0)
{
	m_cTask.SetSaveVal(471,m_cTask.GetSaveVal(471)-1);
	Npc[m_nIndex].AddSkillEffect(505,1,m_cTask.GetSaveVal(471));
}
else
{
	m_cTask.SetSaveVal(471,0);
}


if (Npc[m_nIndex].m_Level < 50 && Npc[m_nIndex].GetLevelSkillEffectMan(503) == 0)
	Npc[m_nIndex].AddSkillEffect(89,20,50);


if (m_cTask.GetSaveVal(475) > 0)
{
	m_cTask.SetSaveVal(475,m_cTask.GetSaveVal(475)-1);
}
else
{
	m_cTask.SetSaveVal(475,0);
}


if ((m_nWorldStat >= 1 && m_nWorldStat <= 10) || (m_nSectStat == 1))
	if (Npc[m_nIndex].GetLevelSkillEffectMan(494) == 0)
		Npc[m_nIndex].AddSkillEffect(494,1,99999);
		
		
		
/*
	if (m_AutoTTL)
	{
		int SLTTL1 = m_ItemList.GetTaskItemNum(113);
		int SLTTL2 = m_ItemList.GetTaskItemNum(117);
		int SLTTL3 = m_ItemList.GetTaskItemNum(119);
		int SLTTL4 = m_ItemList.GetTaskItemNum(120);
		int AutoTTL_ID = 0;
		int AutoTTL_TimeTTL = 0;
		if (SLTTL1 > 0)
		{
			AutoTTL_ID= 113;
			AutoTTL_TimeTTL = 30;
		}
		else if (SLTTL2 > 0)
		{
			AutoTTL_ID= 117;
			AutoTTL_TimeTTL = 60;
		}
		else if (SLTTL3 > 0)
		{
			AutoTTL_ID= 119;
			AutoTTL_TimeTTL = 3*60;
		}
		else if (SLTTL4 > 0)
		{
			//AutoTTL_ID= 120;
			//AutoTTL_TimeTTL = 6*60;
		}
		if (m_ItemList.GetTaskItemNum(AutoTTL_ID) > 0 && AutoTTL_ID != 0)
		{
			m_ItemList.RemoveTaskItem(AutoTTL_ID);
			m_cTask.SetSaveVal(172,AutoTTL_TimeTTL*60*18);
			m_cTask.SetSaveVal(141,1);
			Npc[m_nIndex].AddSkillEffect(460,1,AutoTTL_TimeTTL*60*18);
			KPlayerChat::SendSystemInfo(1, m_nPlayerIndex, MESSAGE_SYSTEM_ANNOUCE_HEAD, "<color=green>B¹n ®· sö dông Tiªn Th¶o Lé!",strlen("<color=green>B¹n ®· sö dông Tiªn Th¶o Lé!") );
		}
	}
	
}
*/
if (m_cTask.GetSaveVal(173) > 0)
{
	m_cTask.SetSaveVal(173,m_cTask.GetSaveVal(173)-1);
	Npc[m_nIndex].AddSkillEffect(455,1,m_cTask.GetSaveVal(173));
}
else
{
	m_cTask.SetSaveVal(173,0);
}
/*
	if (m_AutoTTL)
	{
			int SLTTL1 = m_ItemList.GetTaskItemNum(114);
		int SLTTL2 = m_ItemList.GetTaskItemNum(118);
		int SLTTL3 = m_ItemList.GetTaskItemNum(115);
		int SLTTL4 = m_ItemList.GetTaskItemNum(116);
		int AutoTTL_ID = 0;
		int AutoTTL_TimeTTL = 0;
		if (SLTTL1 > 0)
		{
			AutoTTL_ID= 114;
			AutoTTL_TimeTTL = 30;
		}
		else if (SLTTL2 > 0)
		{
			AutoTTL_ID= 118;
			AutoTTL_TimeTTL = 60;
		}
		else if (SLTTL3 > 0)
		{
			AutoTTL_ID= 115;
			AutoTTL_TimeTTL = 3*60;
		}
		else if (SLTTL4 > 0)
		{
			AutoTTL_ID= 116;
			AutoTTL_TimeTTL = 6*60;
		}
		if (m_ItemList.GetTaskItemNum(AutoTTL_ID) > 0 && AutoTTL_ID != 0)
		{
			m_ItemList.RemoveTaskItem(AutoTTL_ID);
			m_cTask.SetSaveVal(173,AutoTTL_TimeTTL*60*18);
			KPlayerChat::SendSystemInfo(1, m_nPlayerIndex, MESSAGE_SYSTEM_ANNOUCE_HEAD, "<color=green>B¹n ®· sö dông N÷ Nhi Hång!",strlen("<color=green>B¹n ®· sö dông N÷ Nhi Hång!") );
		}
	}
}
*/
SYSTEMTIME TimeNow;
GetLocalTime(&TimeNow);
char DemTimeTK[50];
if (TimeNow.wSecond == 0 && g_SubWorldSet.GetGameTime()%18 == 0 && (Map == 33 || Map == 37))
{
	if (Npc[m_nIndex].m_FightMode && TimeNow.wMinute < 50)
	{
		m_cTask.SetSaveVal(118,m_cTask.GetSaveVal(118) + 1);
		sprintf(DemTimeTK,"<color=pink>Thêi gian tham chiÕn cña b¹n: %d phót",m_cTask.GetSaveVal(118));
		KPlayerChat::SendSystemInfo(1,m_nPlayerIndex, MESSAGE_SYSTEM_ANNOUCE_HEAD, DemTimeTK, strlen(DemTimeTK) );
	}
	else
	{
	KPlayerChat::SendSystemInfo(1,m_nPlayerIndex, MESSAGE_SYSTEM_ANNOUCE_HEAD, "<color=pink>B¹n kh«ng thuéc khu vùc thi ®Êu, kh«ng ®­îc tÝnh thêi gian !",strlen("<color=pink>B¹n kh«ng thuéc khu vùc thi ®Êu, kh«ng ®­îc tÝnh thêi gian !") );
	}
}

/*
if (TimeNow.wSecond == 0 && Npc[m_nIndex].m_Level >= 80 )
{
	int gio = TimeNow.wHour;
	int ngay = TimeNow.wDay;
	int thang = TimeNow.wMonth;
	int nam = TimeNow.wYear % 2000;

	int tinhtime = nam * 1000000 + thang * 10000 + ngay * 100 + gio;
	int tinhtime1 = m_cTask.GetSaveVal(492) * 1000000 + m_cTask.GetSaveVal(491) * 10000 + m_cTask.GetSaveVal(490) * 100 + m_cTask.GetSaveVal(489);
	Npc[m_nIndex].AddSkillEffect(468,1,tinhtime1);

	if (tinhtime >= tinhtime1 && Map != 27)
	{
		if (Map == 20)
		m_cTask.SetSaveVal(481,1);
		else if (Map == 5)
		m_cTask.SetSaveVal(481,2);
		else
		m_cTask.SetSaveVal(481,0);
		
		Npc[m_nIndex].ChangeWorld(27, 1569*32, 3217*32);	
		Npc[m_nIndex].SetFightMode(0);
		KPlayerChat::SendSystemInfo(1,m_nPlayerIndex, MESSAGE_SYSTEM_ANNOUCE_HEAD, "<color=yellow>Nh©n vËt cña b¹n ®· hÕt h¹n, vui lßng gÆp NPC TiÒn Trang ®Ó gia h¹n thªm",strlen("<color=yellow>Nh©n vËt cña b¹n ®· hÕt h¹n, vui lßng gÆp NPC TiÒn Trang ®Ó gia h¹n thªm") );
	}
}
*/

//if (TimeNow.wDay !=  m_cTask.GetSaveVal(132))
//{
//	m_cTask.SetSaveVal(133,0);
//	m_cTask.SetSaveVal(132,TimeNow.wDay);
//	KPlayerChat::SendSystemInfo(1,m_nPlayerIndex, MESSAGE_SYSTEM_ANNOUCE_HEAD, "<color=yellow>X¸c nhËn qua ngµy míi, reset l¹i ®iÓm sè §ua Top",strlen("<color=yellow>X¸c nhËn qua ngµy míi, reset l¹i ®iÓm sè §ua Top") );
				
//}


/*
if (Npc[m_nIndex].m_Level < 130 && Npc[m_nIndex].m_FightMode == 1)
{	
	if (m_cTask.GetSaveVal(130) < 120*60*18)
	{
		m_cTask.SetSaveVal(130,m_cTask.GetSaveVal(130)+1);
	}
	else
	{
		m_cTask.SetSaveVal(172,m_cTask.GetSaveVal(172)+60*60*18);
		m_cTask.SetSaveVal(141,1);
		Npc[m_nIndex].AddSkillEffect(460,1,m_cTask.GetSaveVal(172));
		m_cTask.SetSaveVal(130,0);
		//KPlayerChat::SendSystemInfo(1, m_nPlayerIndex, MESSAGE_SYSTEM_ANNOUCE_HEAD, "[H?Tr?T©n Thñ] B¹n ®· online ®ñ 2 gi?v?nhËn ®îc thªm 1 gi?nh©n ®«i kinh nghiÖm",strlen("[H?Tr?T©n Thñ] B¹n ®· online ®ñ 2 gi?v?nhËn ®îc thªm 1 gi?nh©n ®«i kinh nghiÖm") );
		
	}
}
*/


		
		if (g_SubWorldSet.GetGameTime()%18 == 0 && TimeNow.wSecond == 0)
		{
			if (m_AutoSuaTrangBi)
			{				
				for (int h=1; h < MAX_PLAYER_ITEM; h++)
				{
					if (m_ItemList.m_Items[h].nIdx > 0 && m_ItemList.m_Items[h].nIdx < MAX_ITEM && m_ItemList.m_Items[h].nPlace == pos_equip)
					{
						int nIdxitem = m_ItemList.m_Items[h].nIdx;
						int nDur = Item[nIdxitem].GetDurability();
						int nMaxDur = Item[nIdxitem].GetMaxDurability();
						int nTestDur = Item[nIdxitem].GetDetailType();
						if (nDur < 5 && nTestDur != 11 && nTestDur != 10 && nTestDur != 4 && nTestDur != 9 && nTestDur != 3)
						{
							RepairItem(Item[m_ItemList.m_Items[h].nIdx].GetID());
							char szMsg[100];
							sprintf(szMsg, "<color=yellow>[Auto VLTK] §· söa trang bÞ %s ",Item[m_ItemList.m_Items[h].nIdx].GetName());
							KPlayerChat::SendSystemInfo(1,m_nPlayerIndex, MESSAGE_SYSTEM_ANNOUCE_HEAD,szMsg,strlen(szMsg) );
						}
					}
				}
			}
		}
		
if (m_AutoQuayLai)
{
	int Map = SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_SubWorldID;
	
	if (!Npc[m_nIndex].m_FightMode)
	{
		if (Map != 53 && Map != 17 && Map != 9 && Map != 3 && Map != 18 && Map != 15 && Map != 66 && Map != 4) // Ban Do Thanh Thi
		return;
		
		if (Auto_LuuMap == 0 || Auto_LuuX == 0 || Auto_LuuY == 0)
		return;
		
		if (Auto_LuuMap == 999)
		{
		
			Auto_LuuMap = 0;
			KPlayerChat::SendSystemInfo(1,m_nPlayerIndex, MESSAGE_SYSTEM_ANNOUCE_HEAD, "<color=pink>Chó ý: B¹n bÞ tö vong, tù ®éng hñy Auto Trë L¹i ! ",strlen("<color=pink>Chó ý: B¹n bÞ tö vong, tù ®éng hñy Auto Trë L¹i !") );
			return;
		}
		if (Auto_LuuThanhThi == 0)
			Auto_LuuThanhThi = Map;

		if (Map != Auto_LuuThanhThi)
		{
			Auto_LuuThanhThi = 0;
			Auto_LuuMap = 0;
			Auto_LuuX = 0;
			Auto_LuuY = 0;
			KPlayerChat::SendSystemInfo(1,m_nPlayerIndex, MESSAGE_SYSTEM_ANNOUCE_HEAD, "<color=pink>B¹n tù ý can thiÖp vµo AutoPlay ®Ó chuyÓn thµnh. L­u ®Þa ®iÓm bÞ hñy !",strlen("<color=pink>B¹n tù ý can thiÖp vµo AutoPlay ®Ó chuyÓn thµnh. L­u ®Þa ®iÓm bÞ hñy !") );
			return;
		}
		
			BOOL bCheckAuto = FALSE;
			int nAutoEventId;
			int nAutoCost;
			char nAutoEventName[100];
			char AutoBaoTime[100];
			int AutoTinhTime;

		if (Map == 17 || Map == 9 || Map == 18 || Map == 15 || Map == 16 || Map == 11 || Map == 53 || Map == 66 || Map == 3)
		{
			int nNumLifePoison = m_ItemList.GetTaskItemNum(104);
			int nNumManaPoison = m_ItemList.GetTaskItemNum(105);
			int nNumAllPoison = m_ItemList.GetTaskItemNum(111);
			int nNumAllPoisonMin = m_ItemList.GetTaskItemNum(109);
			
			if (nNumLifePoison < m_AutoLifePoison && m_AutoLifePoison <= 10 && m_AutoManaPoison >= 0)
			{
				nAutoEventId = 104;
				nAutoCost = 40000;
				strcpy(nAutoEventName,"<color=yellow>B¹n ®· mua 1 Cöu ChuyÓn Hoµn Hån §an LÔ Bao");
				bCheckAuto = TRUE;
			}
			else if (nNumManaPoison < m_AutoManaPoison && m_AutoManaPoison <= 10 && m_AutoManaPoison >= 0)
			{
				nAutoEventId = 105;
				nAutoCost = 40000;
				strcpy(nAutoEventName,"<color=yellow>B¹n ®· mua 1 Thñ ¤ Hoµn ThÇn §an LÔ Bao");
				bCheckAuto = TRUE;
			}
			else if (nNumAllPoison < m_AutoAllPoison && m_AutoAllPoison <= 20 && m_AutoAllPoison >= 0)
			{
				nAutoEventId = 111;
				nAutoCost = 5000;
				strcpy(nAutoEventName,"<color=yellow>B¹n ®· mua 1 Ng­ng ThÇn §an LÔ Bao");
				bCheckAuto = TRUE;
			}
			else if (nNumAllPoisonMin < m_AutoAllPoisonMin && m_AutoAllPoisonMin <= 20 && m_AutoAllPoisonMin >= 0)
			{
				nAutoEventId = 109;
				nAutoCost = 7500;
				strcpy(nAutoEventName,"<color=yellow>B¹n ®· mua 1 Thõa Tiªn MËt LÔ Bao");
				bCheckAuto = TRUE;
			}
			
			int nMoneyMapT = 0;
			if (SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_bCheckTong)
			{
				int nTongT = SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_nTongT;
				if (nTongT >= 0 && nTongT <= 50)
				{
					if (SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_dwTongName == m_cTong.GetTongNameID())
					{
						nMoneyMapT = nAutoCost * nTongT /200;
					}
					else
					{
						nMoneyMapT = nAutoCost * nTongT /100;
					}
					if (nMoneyMapT < 0)
					nMoneyMapT = 0;
				



					if (bCheckAuto)
					{
						int nAutoWidth,nAutoHeight;
						int nAutoX,nAutoY;
						if (ItemSet.Get_SizeItem(4,nAutoEventId,1,1,&nAutoWidth,&nAutoHeight))
						{
							if (m_ItemList.CheckCanPlaceInEquipment(nAutoWidth, nAutoHeight, &nAutoX, &nAutoY))
							{
								if (m_ItemList.GetMoney(room_equipment) >= nAutoCost)
								{
									int nIndex = ItemSet.Add(item_task, 0, 0, 0, nAutoEventId, 0, 0, 1, 0);
									
									if (nIndex > 0)
									{
										if (nAutoWidth == Item[nIndex].GetWidth() && nAutoHeight == Item[nIndex].GetHeight())
										{
											Pay(nAutoCost+nMoneyMapT);
											KPlayerChat::SendSystemInfo(1,m_nPlayerIndex, MESSAGE_SYSTEM_ANNOUCE_HEAD, nAutoEventName,strlen(nAutoEventName) );
											m_ItemList.Add(nIndex, pos_equiproom, nAutoX, nAutoY);
												
											if (SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_bCheckTong)
											{
												try
												{
													bool bExecuteScriptMistake = true;
													KLuaScript * pScript = (KLuaScript* )g_GetScript("\\script\\admin\\banghoi\\banghoi.lua");;
													if (pScript)
													{
														int nTopIndex = 0;
														pScript->SafeCallBegin(&nTopIndex);
														if (pScript->CallFunction("AddMoneyMain",0, "dsd",SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_dwTongName,SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_szTongNameBC,nMoneyMapT));
														{
															bExecuteScriptMistake = false;
														}
														pScript->SafeCallEnd(nTopIndex);
														
													}
												}
												catch(...)
												{
													printf("Exception Have Caught When Execute Script[%d]!!!!!", g_FileName2Id("\\script\\admin\\banghoi\\banghoi.lua"));
												
												}
											}

										}
									}	
								}
							}
						}
					}
				}
			}
		else
		{
				if (bCheckAuto)
					{
						int nAutoWidth,nAutoHeight;
						int nAutoX,nAutoY;
						if (ItemSet.Get_SizeItem(4,nAutoEventId,1,1,&nAutoWidth,&nAutoHeight))
						{
							if (m_ItemList.CheckCanPlaceInEquipment(nAutoWidth, nAutoHeight, &nAutoX, &nAutoY))
							{
								if (m_ItemList.GetMoney(room_equipment) >= nAutoCost)
								{
									int nIndex = ItemSet.Add(item_task, 0, 0, 0, nAutoEventId, 0, 0, 1, 0);
									
									if (nIndex > 0)
									{
										if (nAutoWidth == Item[nIndex].GetWidth() && nAutoHeight == Item[nIndex].GetHeight())
										{
											Pay(nAutoCost);
											KPlayerChat::SendSystemInfo(1,m_nPlayerIndex, MESSAGE_SYSTEM_ANNOUCE_HEAD, nAutoEventName,strlen(nAutoEventName) );
											m_ItemList.Add(nIndex, pos_equiproom, nAutoX, nAutoY);
										}
									}	
								}
							}
						}
					}
				}
			}

		Auto_DemTime++;
		if (Auto_DemTime > 3*60*18) // Thoi Gian Tro Lai Cua Auto
		{	
			Auto_DemTime = 0;
		
			Auto_LuuThanhThi = 0;
			Npc[m_nIndex].ChangeWorld(Auto_LuuMap, Auto_LuuX, Auto_LuuY);	
			Npc[m_nIndex].SetFightMode(1);
		}
		else if (Auto_DemTime == 1)
		{
			KPlayerChat::SendSystemInfo(1,m_nPlayerIndex, MESSAGE_SYSTEM_ANNOUCE_HEAD, "<color=pink>[Auto VLTK] Tù ®éng quay l¹i n¬i luyÖn c«ng sau 3 phót tÝnh tõ lóc vÒ thµnh thÞ !",strlen("<color=pink>[Auto VLTK] Tù ®éng quay l¹i n¬i luyÖn c«ng sau 5 phót tÝnh tõ lóc vÒ thµnh thÞ !") );
			KPlayerChat::SendSystemInfo(1,m_nPlayerIndex, MESSAGE_SYSTEM_ANNOUCE_HEAD, "<color=pink>Chó ý: Kh«ng ®uîc chuyÓn thµnh thÞ hoÆc ra ngoµi thµnh!",strlen("<color=pink>Chó ý: Kh«ng ®uîc chuyÓn thµnh thÞ hoÆc ra ngoµi thµnh!") );
		}
		else
		{
			if (Auto_LuuMap != 0)
			{
				AutoTinhTime = (3*60*18 - Auto_DemTime)/18;
				sprintf(AutoBaoTime,"[Auto VLTK] Trë l¹i sau: %d gi©y",AutoTinhTime);
				if (Auto_DemTime%(10*18) == 0)
				{
					KPlayerChat::SendSystemInfo(1,m_nPlayerIndex, MESSAGE_SYSTEM_ANNOUCE_HEAD, AutoBaoTime, strlen(AutoBaoTime) );
				}
			}
			else
			{
				KPlayerChat::SendSystemInfo(1,m_nPlayerIndex, MESSAGE_SYSTEM_ANNOUCE_HEAD, "<color=pink>Chó ý: B¹n bÞ tö vong, kh«ng thÓ tù ®éng lªn l¹i b¶n ®å! ",strlen("<color=pink>B¹n bÞ tö vong, kh«ng thÓ tù ®éng lªn l¹i b¶n ®å! ") );
			}
		}
	}
	else
	{
		if (Map != 70 &&  
		Map != 74 &&
		Map != 54 &&
		Map != 8 &&
		Map != 91 &&
		Map != 41 &&
		Map != 50 &&
		Map != 78 &&
		Map != 21 &&
		Map != 102 &&
		Map != 104 &&
		Map != 105 &&
		Map != 38 &&
		Map != 39 &&
		Map != 92 &&
		Map != 56 &&
		Map != 57 &&
		Map != 13 &&
		Map != 14 &&
		Map != 63 &&
		Map != 62 &&
		Map != 64 &&
		Map != 79 &&
		Map != 65 &&
		Map != 67 &&
		Map != 68 &&
		Map != 73 &&
		Map != 21 && // Mac bac
		Map != 22 && // truong bach son nam
		Map != 105 && // truong bach son bac
		Map != 107 && // Sa Mac Dia Bieu
		Map != 109 && // Sa Mac 1
		Map != 110 && // Sa Mac 2
		Map != 111 && // Sa Mac 3
		Map != 93 && // Tien Cuc Dong
		Map != 94 && // Can Vien Dong
		Map != 99 && // Duoc Vuong Dong
		Map != 100 && // Tuyet Bao Dong
		Map != 2 
		) // Ban Do Luyen Cong
		return;
		
		
		
			int nNumLifePoison = m_ItemList.GetTaskItemNum(104);
			int nNumManaPoison = m_ItemList.GetTaskItemNum(105);
			int nNumAllPoison = m_ItemList.GetTaskItemNum(111);
			int nNumAllPoisonMin = m_ItemList.GetTaskItemNum(109);
		if (nNumLifePoison == 0 && nNumManaPoison == 0 && nNumAllPoison == 0 && nNumAllPoisonMin == 0 && m_ItemList.GetTaskItemNum(264) == 0 && m_ItemList.GetTaskItemNum(264) == 0 && m_ItemList.GetTaskItemNum(103) == 0 && m_ItemList.GetTaskItemNum(104) == 0 && m_ItemList.GetTaskItemNum(105) == 0 && m_ItemList.GetTaskItemNum(106) == 0 && m_ItemList.GetTaskItemNum(107) == 0 && m_ItemList.GetTaskItemNum(108) == 0 && m_ItemList.GetTaskItemNum(109) == 0 && m_ItemList.GetTaskItemNum(110) == 0 && m_ItemList.GetTaskItemNum(111) == 0 && m_ItemList.GetTaskItemNum(112) == 0)
		{
			UseTownPortal(0);
			KPlayerChat::SendSystemInfo(1,m_nPlayerIndex, MESSAGE_SYSTEM_ANNOUCE_HEAD, "Hµnh trang ®· hÕt LÔ Bao, Tù ®éng TDP", strlen("Hµnh trang ®· hÕt LÔ Bao, Tù ®éng TDP") );
		
		}
		

	}
}
else
{
	Auto_LuuMap = 0;
	Auto_LuuX = 0;
	Auto_LuuY = 0;
	Auto_LuuThanhThi = 0;
}

if (m_cTask.GetSaveVal(107) > 0)
{
	m_cTask.SetSaveVal(107,m_cTask.GetSaveVal(107)-1);
}
else
{
	m_cTask.SetSaveVal(107,0);
}
if (m_cTask.GetSaveVal(169) > 0)
{
	m_cTask.SetSaveVal(169,m_cTask.GetSaveVal(169)-1);
}
else
{
	m_cTask.SetSaveVal(169,0);
}

if (m_nExp <= (-m_nNextLevelExp/2))
{
	m_cPK.SetNormalPKStateDB(0);
}

if (m_cTask.GetSaveVal(477) > 0)
{
	m_cTask.SetSaveVal(477,m_cTask.GetSaveVal(477)-1);
	if (m_cTask.GetSaveVal(477)%18 == 0)
	{
		char BaoTime[50];
		sprintf(BaoTime,"Thêi gian cÊm hoa ®¨ng cßn: <color=yellow>%d gi©y n÷a !",m_cTask.GetSaveVal(477)/18);
		KPlayerChat::SendSystemInfo(1,m_nPlayerIndex, MESSAGE_SYSTEM_ANNOUCE_HEAD, BaoTime, strlen(BaoTime) );
	}	
	
	if (m_cTask.GetSaveVal(477) == 0)
	{
		KPlayerChat::SendSystemInfo(1,m_nPlayerIndex, MESSAGE_SYSTEM_ANNOUCE_HEAD,"<color=green>HiÖn t¹i ®· cã thÓ ®o¸n Hoa §¨ng tiÕp !",  strlen("<color=green>HiÖn t¹i ®· cã thÓ ®o¸n Hoa §¨ng tiÕp !") );
	}	
	
}
else
{
	m_cTask.SetSaveVal(477,0);
}


if (m_cTask.GetSaveVal(479) > 0)
{
	m_cTask.SetSaveVal(479,m_cTask.GetSaveVal(479)-1);
	if (m_cTask.GetSaveVal(479)%18 == 0)
	{
		char BaoTime[50];
		sprintf(BaoTime,"Thêi gian thu ho¹ch c©y cßn: <color=yellow>%d gi©y n÷a !",m_cTask.GetSaveVal(479)/18);
		KPlayerChat::SendSystemInfo(1,m_nPlayerIndex, MESSAGE_SYSTEM_ANNOUCE_HEAD, BaoTime, strlen(BaoTime) );
	}	
	
	if (m_cTask.GetSaveVal(479) == 0)
	{
		KPlayerChat::SendSystemInfo(1,m_nPlayerIndex, MESSAGE_SYSTEM_ANNOUCE_HEAD,"<color=green>HiÖn t¹i ®· cã thÓ thu ho¹ch c©y, nhanh tay lªn nµo !",  strlen("<color=green>HiÖn t¹i ®· cã thÓ thu ho¹ch c©y, nhanh tay lªn nµo !") );
	}	
	
}
else
{
	m_cTask.SetSaveVal(479,0);
}

/*
if ((Map == 70 ||  
		Map == 74 ||
		Map == 54 ||
		Map == 8 ||
		Map == 91 ||
		Map == 41 ||
		Map == 50 ||
		Map == 78 ||
		Map == 21 ||
		Map == 102 ||
		Map == 104 ||
		Map == 105 ||
		Map == 38 ||
		Map == 39 ||
		Map == 92 ||
		Map == 56 ||
		Map == 57 ||
		Map == 13 ||
		Map == 14 ||
		Map == 63 ||
		Map == 62 ||
		Map == 64 ||
		Map == 79 ||
		Map == 65 ||
		Map == 67 ||
		Map == 68 ||
		Map == 73 ||
		Map == 21 || // Mac bac
		Map == 22 || // truong bach son nam
		Map == 105 || // truong bach son bac
		Map == 107 || // Sa Mac Dia Bieu
		Map == 109 || // Sa Mac 1
		Map == 110 || // Sa Mac 2
		Map == 111 || // Sa Mac 3
		Map == 93 || // Tien Cuc Dong
		Map == 94 || // Can Vien Dong
		Map == 99 || // Duoc Vuong Dong
		Map == 100
		)  && !Npc[m_nIndex].m_FightMode)
		Npc[m_nIndex].SetFightMode(1);	

*/
		
/* Uy Thac
if (m_cTask.GetSaveVal(102) > 0)
{
	m_cTask.SetSaveVal(102,m_cTask.GetSaveVal(102)-1);
}
else
{
	m_cTask.SetSaveVal(102,0);
	m_nLiXian = 0;
	m_nLiXian = 0;
	m_bIsQuiting = TRUE;
	
}
*/


if (SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_SubWorldID == 33 || SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_SubWorldID == 37 )
{
	if (Npc[m_nIndex].m_FightMode)
	{
		m_cTask.SetSaveVal(317, m_cTask.GetSaveVal(317) + 1);
		if (m_cTask.GetSaveVal(317) % 1800 == 0)
		{
			int Posx, Posy;
			Npc[m_nIndex].GetMpsPos(&Posx, &Posy);
			if (m_cTask.GetSaveVal(315) == Posx && m_cTask.GetSaveVal(316) == Posy)
			{
				m_cTask.SetSaveVal(315,0);
				m_cTask.SetSaveVal(316,0);
				KPlayerChat::SendSystemInfo(1,m_nPlayerIndex, MESSAGE_SYSTEM_ANNOUCE_HEAD, "<color=yellow>B¹n bÞ Kick do ®øng im qu¸ l©u ngoµi chiÕn tr­êng",strlen("<color=yellow>B¹n bÞ Kick do ®øng im qu¸ l©u ngoµi chiÕn tr­êng") );
				g_pServer->ShutdownClient(m_nNetConnectIdx);
			}
			else
			{
				m_cTask.SetSaveVal(315,Posx);
				m_cTask.SetSaveVal(316,Posy);
			}
			m_cTask.SetSaveVal(317 , 0);
		}
	}
	else
	{
//		m_cTask.SetSaveVal(317, m_cTask.GetSaveVal(317) + 1);
//		if (m_cTask.GetSaveVal(317) % 3240 == 0)
//		{
//			g_pServer->ShutdownClient(m_nNetConnectIdx);
//			m_cTask.SetSaveVal(317 , 0);
//		}
	}
}




if (m_cTask.GetSaveVal(117) > 1)
{
	m_cTask.SetSaveVal(117,m_cTask.GetSaveVal(117)-1);
	Npc[m_nIndex].SetFightMode(0);	
}
else if (m_cTask.GetSaveVal(117) == 1)
{
		m_cTask.SetSaveVal(117,0);
		Npc[m_nIndex].SetFightMode(1);	
		KPlayerChat::SendSystemInfo(1,m_nPlayerIndex, MESSAGE_SYSTEM_ANNOUCE_HEAD, "Tr¹ng th¸i bÊt tö kÕt thóc, cã thÓ chiÕn ®Êu",strlen("Tr¹ng th¸i bÊt tö kÕt thóc, cã thÓ chiÕn ®Êu") );
}
else
{
	m_cTask.SetSaveVal(117,0);
}


if (SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_SubWorldID == 43)
{
	if (m_bTempBlock == -1)
	{
		m_bTempBlock = m_bBlock;
		m_bBlock = 1;
	}
}
else if (SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_SubWorldID == 42)
{
	if (m_bTempBlock != -1 && m_bBlock == 1)
	{
		m_bBlock = m_bTempBlock;
		m_bTempBlock = -1;
	}
}


#define	defMAX_SLEEP_TIME	7200
	if (Npc[m_nIndex].m_FightMode == 0 && m_bSleepMode == FALSE && g_SubWorldSet.GetGameTime() - m_nLastNetOperationTime > defMAX_SLEEP_TIME)
	{
		NPC_SLEEP_SYNC	SleepSync;
		SleepSync.ProtocolType = s2c_npcsleepmode;
		SleepSync.bSleep = 1;
		SleepSync.NpcID = Npc[m_nIndex].m_dwID;
		Npc[m_nIndex].SendDataToNearRegion(&SleepSync, sizeof(NPC_SLEEP_SYNC));
		m_bSleepMode = TRUE;
	}
#else
	// ¶ÓÎéÉêÇëÈËµÄ´¦Àí
	if ( !m_cTeam.m_nFlag )
	{
		if (m_cTeam.m_nApplyCaptainID > 0)
		{
			if ( m_cTeam.m_dwApplyTimer == 0 )
			{
				m_cTeam.m_nApplyCaptainID = 0;
				
			}
			else
			{
				m_cTeam.m_dwApplyTimer--;
				if ( !NpcSet.SearchID(m_cTeam.m_nApplyCaptainID) )
				{
					m_cTeam.m_nApplyCaptainID = 0;
					m_cTeam.m_dwApplyTimer = 0;
				}
			}
		}
	}
	// ¶Ó³¤µÄ´¦Àí
	else if (m_cTeam.m_nFigure == TEAM_CAPTAIN)
	{
		for (int i = 0; i < MAX_TEAM_APPLY_LIST; i++)
		{
			if (m_cTeam.m_sApplyList[i].m_dwNpcID > 0)
			{
				if (m_cTeam.m_sApplyList[i].m_dwTimer == 0)
				{
					m_cTeam.m_sApplyList[i].m_dwNpcID = 0;
					m_cTeam.UpdateInterface();
				}
				else
				{
					m_cTeam.m_sApplyList[i].m_dwTimer--;
					if ( !Npc[this->m_nIndex].SearchAroundID(m_cTeam.m_sApplyList[i].m_dwNpcID) )
					{
						m_cTeam.m_sApplyList[i].m_dwNpcID = 0;
						m_cTeam.m_sApplyList[i].m_dwTimer = 0;
						m_cTeam.UpdateInterface();
					}
				}
			}
		}
	}


	m_nSendMoveFrames++;
	
	// ³ðÉ±µ¹¼ÆÊ±
	this->m_cPK.Active();


if (m_nAutoPlay)
{
	NgaMiSkill1 = 0;
	NgaMiSkill2 = 0;
	if (m_bNgaMy1)
	{
		if (NgaMiSkill1 == 0)
		{
			NgaMiSkill1 = 89;
		}
		else if (NgaMiSkill2 == 0)
		{
			NgaMiSkill2 = 89;
		}
	}
	if (m_bNgaMy2)
	{
		if (NgaMiSkill1 == 0)
		{
			NgaMiSkill1 = 86;
		}
		else if (NgaMiSkill2 == 0)
		{
			NgaMiSkill2 = 86;
		}
	}
	if (m_bNgaMy3)
	{
		if (NgaMiSkill1 == 0)
		{
			NgaMiSkill1 = 92;
		}
		else if (NgaMiSkill2 == 0)
		{
			NgaMiSkill2 = 92;
		}
	}
	if (m_bNgaMy4)
	{
		if (NgaMiSkill1 == 0)
		{
			NgaMiSkill1 = 282;
		}
		else if (NgaMiSkill2 == 0)
		{
			NgaMiSkill2 = 282;
		}
	}
	if (m_bNgaMy5)
	{
		if (NgaMiSkill1 == 0)
		{
			NgaMiSkill1 = 291;
		}
		else if (NgaMiSkill2 == 0)
		{
			NgaMiSkill2 = 291;
		}
	}
	
	if (NgaMiSkill1 != 0 && NgaMiSkill2 != 0)
	{
		if (g_SubWorldSet.GetGameTime()%16 < 8)
			SetRightSkill(NgaMiSkill1);
		else
			SetRightSkill(NgaMiSkill2);
	}
	PlayerAutoOn();
}


if (!(g_SubWorldSet.GetGameTime()%6))
{

	if (m_nBuyNumberNum > 0)
	{
            ItemPos	Pos;

			if (!m_ItemList.SearchPosition(m_nBuyNumberWidth, m_nBuyNumberHeigh, &Pos))
			{
				m_nBuyNumberNum = 0;
				return;
			}

			if (Pos.nPlace != pos_equiproom)
			{
				KSystemMessage	sMsg;
				strcpy(sMsg.szMessage, MSG_SHOP_NO_ROOM);
				sMsg.eType = SMT_SYSTEM;
				sMsg.byConfirmType = SMCT_CLICK;
				sMsg.byPriority = 1;
				sMsg.byParamSize = 0;
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
				m_nBuyNumberNum = 0;
				return;
			}




			KItem* pItemBuy = NULL;

			
			pItemBuy = BuySell.GetItem(m_nBuyNumberIdx);

			int nPriceGold,nPriceTL,nPriceVD;

			nPriceGold = pItemBuy->m_nPriceGold;
			nPriceTL = pItemBuy->m_nPriceTL;
			nPriceVD = pItemBuy->m_nPriceVD;


			if (nPriceGold > 0)
			{
			
			if (m_cTask.GetSaveVal(99) < nPriceGold)
			{
			    KSystemMessage	sMsg;
				strcpy(sMsg.szMessage, MSG_SHOP_NO_MONEY);
				sMsg.eType = SMT_SYSTEM;
				sMsg.byConfirmType = SMCT_CLICK;
				sMsg.byPriority = 1;
				sMsg.byParamSize = 0;
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
				m_nBuyNumberNum = 0;
				return;


			}

			}

			else if (nPriceTL > 0)
			{
			
			if (m_cTask.GetSaveVal(15) < nPriceTL)
			{
			    KSystemMessage	sMsg;
				strcpy(sMsg.szMessage, MSG_SHOP_NO_MONEY);
				sMsg.eType = SMT_SYSTEM;
				sMsg.byConfirmType = SMCT_CLICK;
				sMsg.byPriority = 1;
				sMsg.byParamSize = 0;
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
				m_nBuyNumberNum = 0;
				return;


			}

			}


			else if (nPriceVD > 0)
			{
			
			if (m_cTask.GetSaveVal(124) < nPriceVD)
			{
			    KSystemMessage	sMsg;
				strcpy(sMsg.szMessage, MSG_SHOP_NO_MONEY);
				sMsg.eType = SMT_SYSTEM;
				sMsg.byConfirmType = SMCT_CLICK;
				sMsg.byPriority = 1;
				sMsg.byParamSize = 0;
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
				m_nBuyNumberNum = 0;
				return;


			}

			}




			else

			{



			if (m_ItemList.GetEquipmentMoney() < m_nBuyNumberPrice)
			{
				KSystemMessage	sMsg;
				strcpy(sMsg.szMessage, MSG_SHOP_NO_MONEY);
				sMsg.eType = SMT_SYSTEM;
				sMsg.byConfirmType = SMCT_CLICK;
				sMsg.byPriority = 1;
				sMsg.byParamSize = 0;
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
				m_nBuyNumberNum = 0;
				return;
			}


			}


			if (SendClientCmdBuy(m_nBuyNumberUId, pos_equiproom, Pos.nX, Pos.nY))
			m_nBuyNumberNum --;

	}

}
	
#endif
}


#ifdef _SERVER
void    KPlayer::PlayerAutoUnBlock()
{

for (int i = 0; i < MAX_PLAYER_ITEM; i++)
{
int ItemIdx = m_ItemList.m_Items[i].nIdx;
if (ItemIdx)
{
if (Item[ItemIdx].GetGenre() == item_equip && Item[ItemIdx].m_GeneratorParam.nVersion > 2)
{




    int DayMonth[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

	SYSTEMTIME TimeNow;
	GetLocalTime(&TimeNow);

    int TimeNowMinute = 0;

	for (int t = 0; t < (int)TimeNow.wMonth-1;t++)
	{
		TimeNowMinute += DayMonth[t] * 24 * 60;
	}
	
	 TimeNowMinute += TimeNow.wDay * 24 * 60 + TimeNow.wHour * 60 + TimeNow.wMinute;

 if (Item[ItemIdx].m_GeneratorParam.nVersion <= (3 + TimeNowMinute))
 { 
  Item[ItemIdx].m_GeneratorParam.nVersion = 1;


	PLAYER_UNBLOCK_ITEM_NOW_COMMAND	IDC;
	IDC.ProtocolType = s2c_playerunblockitemnow;
	IDC.m_ID = Item[ItemIdx].m_dwID;

	if (g_pServer)
		g_pServer->PackDataToClient(m_nNetConnectIdx, &IDC, sizeof(PLAYER_UNBLOCK_ITEM_NOW_COMMAND));
	

 }





}
}
}




}
#endif



void	KPlayer::ProcessMsg(KWorldMsgNode *lpMsg)
{
	switch (lpMsg->m_dwMsgType)
	{
	case GWM_PLAYER_SKILL:
		break;
	case GWM_PLAYER_RUNTO:
		break;
	case GWM_PLAYER_WALKTO:
		break;
	case GWM_PLAYER_JUMPTO:
		break;
	default:
		break;
	}	
}

#ifndef _SERVER
void KPlayer::ProcessInputMsg(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_MOUSEMOVE:
	case WM_MOUSEHOVER:
		if (wParam & MK_LBUTTON)
			OnButtonMove(LOWORD(lParam), HIWORD(lParam),
			(wParam & ~(MK_LBUTTON | MK_MBUTTON | MK_RBUTTON)), button_left);
		else if (wParam & MK_RBUTTON)
			OnButtonMove(LOWORD(lParam), HIWORD(lParam),
			(wParam & ~(MK_LBUTTON | MK_MBUTTON | MK_RBUTTON)), button_right);
		else
			OnMouseMove(LOWORD(lParam), HIWORD(lParam));
		break;
		
	case WM_LBUTTONUP:
		OnButtonUp(LOWORD(lParam), HIWORD(lParam), button_left);
		break;
		
	case WM_LBUTTONDOWN:
		OnButtonDown(LOWORD(lParam), HIWORD(lParam), 
			(wParam & ~(MK_LBUTTON | MK_MBUTTON | MK_RBUTTON)), button_left);
		break;
		
	case WM_RBUTTONUP:
		OnButtonUp(LOWORD(lParam), HIWORD(lParam), button_right);
		break;	
		
	case WM_RBUTTONDOWN:
		OnButtonDown(LOWORD(lParam), HIWORD(lParam),
			(wParam & ~(MK_LBUTTON | MK_MBUTTON | MK_RBUTTON)), button_right);
		break;
	}
}

void KPlayer::OnButtonDown(int x,int y, int Key, MOUSE_BUTTON nButton)
{
	m_MouseDown[(int)nButton] = TRUE;
	g_DebugLog("Mouse Down True ");
	FindSelectNpc(x, y, relation_all);
	FindSelectObject(x, y);
	//	Npc[m_nIndex].m_nPeopleIdx = m_nPeapleIdx;
	ProcessMouse(x, y, Key, nButton);
}

void KPlayer::OnButtonMove(int x,int y,int Key, MOUSE_BUTTON nButton)
{
	if (m_MouseDown[(int)nButton])
	{
		ProcessMouse(x, y, Key, nButton);
	}
}

void KPlayer::OnButtonUp(int x,int y,MOUSE_BUTTON nButton)
{
	m_nAutoPause = FALSE;
	m_MouseDown[(int)nButton] = 0;
}

void KPlayer::OnMouseMove(int x,int y)
{
	m_nPeapleIdx = 0;
	m_nObjectIdx = 0;
	FindSelectNpc(x, y, relation_all);
	FindSelectObject(x, y);
	if (m_nPeapleIdx)
	{
		if (Npc[m_nPeapleIdx].m_Kind == kind_dialoger)
			CoreDataChanged(GDCNI_SWITCH_CURSOR, NULL, MOUSE_CURSOR_DIALOG);
		else if (NpcSet.GetRelation(m_nIndex, m_nPeapleIdx) == relation_enemy)
			CoreDataChanged(GDCNI_SWITCH_CURSOR, NULL, MOUSE_CURSOR_FIGHT);
		else
			CoreDataChanged(GDCNI_SWITCH_CURSOR, NULL, MOUSE_CURSOR_NORMAL);
	}
	else if (m_nObjectIdx)
	{
		if (Object[m_nObjectIdx].m_nKind == Obj_Kind_MapObj)
			CoreDataChanged(GDCNI_SWITCH_CURSOR, NULL, MOUSE_CURSOR_DIALOG);
		else if (Object[m_nObjectIdx].m_nKind == Obj_Kind_Item || Object[m_nObjectIdx].m_nKind == Obj_Kind_Money || Object[m_nObjectIdx].m_nKind == Obj_Kind_Prop)
			CoreDataChanged(GDCNI_SWITCH_CURSOR, NULL, MOUSE_CURSOR_PICK);
	}
	else
		CoreDataChanged(GDCNI_SWITCH_CURSOR, NULL, MOUSE_CURSOR_NORMAL);
}

void KPlayer::ProcessMouse(int x, int y, int Key, MOUSE_BUTTON nButton)
{
	if (CheckTrading())
		return;
	
	if (m_ItemList.m_Hand > 0)
	{
		if (nButton == button_left)
		{
			ThrowAwayItem();
			return;
		}
	}
	
	int nX = x;
	int nY = y;
	int nZ = 0;
	g_ScenePlace.ViewPortCoordToSpaceCoord(nX, nY, nZ);
	
	/*	if (Key & MK_SHIFT)
	{
	Npc[m_nIndex].SendCommand(do_jump, nX, nY);
	SendClientCmdJump(nX, nY);
	return;
}*/
	
	if (Npc[m_nIndex].IsCanInput())
	{
		int nIdx = 0;
		
		if (nButton == button_right)
		{
			nIdx = Npc[m_nIndex].m_SkillList.FindSame(m_nRightSkillID);
			g_DebugLog("[skill]right");
		}
		else
		{
			nIdx = Npc[m_nIndex].m_SkillList.FindSame(m_nLeftSkillID);
			g_DebugLog("[skill]left");
		}
		Npc[m_nIndex].SetActiveSkill(nIdx);
	}
	else
	{
		g_DebugLog("[skill]return");
		Npc[m_nIndex].m_nPeopleIdx = 0;


	
	    Npc[m_nIndex].m_nPeopleIdxCheckClient = 0;


		return;
	}
	
	if ((Key & MK_SHIFT) || (nButton == button_right))
	{
		if (Npc[m_nIndex].m_ActiveSkillID > 0)
		{
			ISkill * pISkill = (KSkill *) g_SkillManager.GetSkill(Npc[m_nIndex].m_ActiveSkillID, 1);
			if (!pISkill) 
                return;
			
			if (pISkill->IsAura())
				return;
			int nAttackRange = pISkill->GetAttackRadius();
			
			int nTargetIdx = 0;
			//			m_nPeapleIdx = 0;
			//°´ÕÕObject / Enemy / Ally µÄÓÅÏÈ¼¶ÕÒµ½ÐèÒª´òµÄ¶ÔÏóid
			
			
			if (pISkill->IsTargetAlly())
			{
				FindSelectNpc(x, y, relation_ally);
				if (m_nPeapleIdx)
				{
					nTargetIdx = m_nPeapleIdx;
				}
			}
			
			if (pISkill->IsTargetEnemy())
			{
				FindSelectNpc(x, y, relation_enemy);
				if (m_nPeapleIdx)
				{
					nTargetIdx = m_nPeapleIdx;
				}
			}
			
			if (pISkill->IsTargetObj())
			{
				FindSelectObject(x, y);
				if (m_nObjectIdx)
				{
					nTargetIdx = m_nObjectIdx;
				}
			}
			//Èç¹û¼¼ÄÜ±ØÐëÖ¸¶¨¶ÔÏó£¬¶øµ±Ç°Î»ÖÃÎÞ¶ÔÏóµÄ»°£¬Ö±½ÓÍË³ö
			if (pISkill->IsTargetOnly() && !nTargetIdx)
            {
				Npc[m_nIndex].m_nPeopleIdx = 0;



				Npc[m_nIndex].m_nPeopleIdxCheckClient = 0;


				m_nPeapleIdx = 0;
				return;
			}
			
			if (m_nIndex == nTargetIdx)
			{
				Npc[m_nIndex].m_nPeopleIdx = 0;


				Npc[m_nIndex].m_nPeopleIdxCheckClient = 0;



				m_nPeapleIdx = 0;
				return;
			}
			
			if ((!Npc[m_nIndex].m_SkillList.CanCast(Npc[m_nIndex].m_ActiveSkillID, SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_dwCurrentTime))
				||
				(!Npc[m_nIndex].Cost(pISkill->GetSkillCostType() , pISkill->GetSkillCost(&Npc[m_nIndex]), TRUE))
				)
			{
				Npc[m_nIndex].m_nPeopleIdx = 0;


				Npc[m_nIndex].m_nPeopleIdxCheckClient = 0;

				m_nPeapleIdx = 0;
				return ;
			}
			
			//ÎÞ¶ÔÏó£¬Ö±½Ó·¢×ø±ê
			if (!nTargetIdx)
			{
				Npc[m_nIndex].SendCommand(do_skill, Npc[m_nIndex].m_ActiveSkillID, nX, nY);
				// Send to Server		
				SendClientCmdSkill(Npc[m_nIndex].m_ActiveSkillID, nX, nY);
			}
			else
			{
				if (pISkill->IsTargetOnly())
				{
					int distance = NpcSet.GetDistance(m_nIndex , nTargetIdx);
					if (distance > pISkill->GetAttackRadius())
					{
						m_nPeapleIdx = nTargetIdx;
						return ;
					}
				}
				// 
				if (m_nIndex == nTargetIdx && pISkill->GetSkillStyle() == SKILL_SS_Missles) 
					return ;
				Npc[m_nIndex].SendCommand(do_skill, Npc[m_nIndex].m_ActiveSkillID, -1, nTargetIdx);
				// Send to Server		
				SendClientCmdSkill(Npc[m_nIndex].m_ActiveSkillID, -1, Npc[nTargetIdx].m_dwID);
			}
		}
		Npc[m_nIndex].m_nPeopleIdx = 0;


		Npc[m_nIndex].m_nPeopleIdxCheckClient = 0;


		return;
	}
	
	
	if (Key == 0 )
	{
		int nRelation = NpcSet.GetRelation(m_nIndex, m_nPeapleIdx);
		if(nRelation == relation_enemy || nRelation == relation_dialog)
		{
			Npc[m_nIndex].m_nPeopleIdx = m_nPeapleIdx;
			Npc[m_nIndex].m_nPeopleIdxCheckClient = (m_nPeapleIdx + 46) * 13 + 15743;
		}
		
		if (m_nSendMoveFrames >= defMAX_PLAYER_SEND_MOVE_FRAME)
		{
			m_nPickObjectIdx = m_nObjectIdx;
			Npc[m_nIndex].m_nObjectIdx = m_nPickObjectIdx;
			if (!m_RunStatus)
			{
				Npc[m_nIndex].SendCommand(do_walk, nX, nY);
				// Send to Server
				SendClientCmdWalk(nX, nY);
			}
			else
			{
				if (Npc[m_nIndex].m_CurrentStamina > 0 || m_cPK.GetNormalPKState() != 2)
				{
				Npc[m_nIndex].SendCommand(do_run, nX, nY);
				// Send to Server
				SendClientCmdRun(nX, nY);
				}
				else
				{
				Npc[m_nIndex].SendCommand(do_walk, nX, nY);
				// Send to Server
				SendClientCmdWalk(nX, nY);
				}
			}
			m_nSendMoveFrames = 0;
		}
		return;
	}
	
	/*	if (Key & MK_ALT)
	{
	Npc[m_nIndex].SendCommand(do_jump, nX, nY);
	SendClientCmdJump(nX, nY);
	}*/
}

void KPlayer::Walk(int nDir, int nSpeed)
{
	int	nMapX = Npc[m_nIndex].m_MapX;
	int nMapY = Npc[m_nIndex].m_MapY;
	int	nOffX = Npc[m_nIndex].m_OffX;
	int	nOffY = Npc[m_nIndex].m_OffY;
	int	nSubWorld = Npc[m_nIndex].m_SubWorldIndex;
	int	nRegion = Npc[m_nIndex].m_RegionIndex;
	int	nX, nY;
	
	SubWorld[nSubWorld].Map2Mps(nRegion, nMapX, nMapY, nOffX, nOffY, &nX, &nY);
	SubWorld[nSubWorld].GetMps(&nX, &nY, nSpeed * 2, nDir);
	
	if (m_RunStatus)
	{
		if (Npc[m_nIndex].m_CurrentStamina > 0 || m_cPK.GetNormalPKState() != 2)
		{
		Npc[m_nIndex].SendCommand(do_run, nX, nY);
		// Send to Server
		if ( !CheckTrading() )
			SendClientCmdRun(nX, nY);
		}
		else
		{

	    Npc[m_nIndex].SendCommand(do_walk, nX, nY);
		// Send to Server
		if (!CheckTrading())
			SendClientCmdWalk(nX, nY);
		}
	}
	else
	{
		Npc[m_nIndex].SendCommand(do_walk, nX, nY);
		// Send to Server
		if (!CheckTrading())
			SendClientCmdWalk(nX, nY);
	}
}


void KPlayer::TurnLeft()
{
	if (Npc[m_nIndex].m_Doing != do_stand &&
		Npc[m_nIndex].m_Doing != do_sit)
		return;
	
	if (Npc[m_nIndex].m_Dir > 8)
		Npc[m_nIndex].m_Dir -= 8;
	else
		Npc[m_nIndex].m_Dir = MAX_NPC_DIR - 1;
}

void KPlayer::TurnRight()
{
	if (Npc[m_nIndex].m_Doing != do_stand &&
		Npc[m_nIndex].m_Doing != do_sit)
		return;
	
	if (Npc[m_nIndex].m_Dir < MAX_NPC_DIR - 9)
		Npc[m_nIndex].m_Dir += 8;
	else
		Npc[m_nIndex].m_Dir = 0;
}

void KPlayer::TurnBack()
{
	if (Npc[m_nIndex].m_Doing != do_stand &&
		Npc[m_nIndex].m_Doing != do_sit)
		return;
	
	if (Npc[m_nIndex].m_Dir < MAX_NPC_DIR / 2)
		Npc[m_nIndex].m_Dir += MAX_NPC_DIR / 2;
	else
		Npc[m_nIndex].m_Dir -= MAX_NPC_DIR / 2;
}

void KPlayer::FindSelectNpc(int x, int y, int nRelation)
{
	int	nNpcIdx = 0;
	
	nNpcIdx = NpcSet.SearchNpcAt(x, y, nRelation, 40);
	
	if (nNpcIdx)
		m_nPeapleIdx = nNpcIdx;
	else
		m_nPeapleIdx = 0;
}

void KPlayer::FindSelectObject(int x, int y)
{
	int	nObjIdx = 0;
	
	nObjIdx = ObjSet.SearchObjAt(x, y, 40);
	if (nObjIdx)
		m_nObjectIdx = nObjIdx;
	else
		m_nObjectIdx = 0;
}
// need change
int	KPlayer::NetCommandPlayerTalk(BYTE * pProtocol)
{
/*	DWORD	dwNpcID;
int		nSize;
dwNpcID = *(DWORD *) &pProtocol[1];
nSize = pProtocol[5];
int nIdx = NpcSet.SearchID(dwNpcID);

  if (ConformIdx(nIdx))
  {
		strcpy(Npc[nIdx].m_szChatBuffer, (char *)(pProtocol+6));
		Npc[nIdx].m_nCurChatTime = NPC_SHOW_CHAT_TIME_LENGTH;
		}
		
		  return (7 + nSize);	// need test
	*/
	return 0;
}


BOOL KPlayer::ConformIdx(int nIdx)
{
	if (nIdx == m_nIndex || nIdx == 0)
		return FALSE;
	return TRUE;
}



void KPlayer::OpenBuyShop(DWORD dwId)
{
	PLAYER_OPEN_BUY_SHOP	sOpen;
	sOpen.ProtocolType = c2s_playeropenbuyshop;
	sOpen.m_dwNpcID = dwId;
	if (g_pClient)
		g_pClient->SendPackToServer(&sOpen, sizeof(PLAYER_OPEN_BUY_SHOP));
}




#endif

#ifdef _SERVER
BOOL KPlayer::IsWaitingRemove()
{
	if (!m_dwID)
		return FALSE;
	return m_bIsQuiting;
}

void KPlayer::WaitForRemove()
{	
//	if (m_nLiXian == 0) //UYTHAC
	m_bIsQuiting = TRUE;
}

BOOL KPlayer::IsLoginTimeOut()
{
	if (m_nNetConnectIdx != -1)
		return FALSE;
	
	if (!m_dwID)
		return FALSE;
	
	if (-1 != m_dwLoginTime &&
		g_SubWorldSet.GetGameTime() - m_dwLoginTime > defPLAYER_LOGIN_TIMEOUT)
	{
		//		m_dwLoginTime = -1;
		return TRUE;
	}
	
	return FALSE;
}


void KPlayer::LoginTimeOut()
{
	// Í¨ÖªÁÄÌìºÃÓÑ×Ô¼ºÏÂÏßÁË
	m_cChat.OffLine(m_dwID);
	
	m_pStatusLoadPlayerInfo = NULL;
	
	Release();
}

BOOL KPlayer::Save()
{
	if (m_nIndex <= 0 && m_dwID == 0)
		{
		return FALSE;
	}
	if (CheckTrading())
	{
	TRADE_DECISION_COMMAND	sTrade;
	sTrade.ProtocolType = c2s_tradedecision;
	sTrade.m_btDecision = 0;
	TradeDecision((BYTE*)&sTrade);	
	}
	
		/*	if (m_uMustSave != SAVE_IDLE)
		return FALSE;
	*/
	TRoleData* pData = (TRoleData *)m_SaveBuffer;
	
	if (UpdateDBPlayerInfo((BYTE *)pData) == -1)
	{
		return FALSE;
		}
		/*
		#ifdef _DEBUG
		KFile File;
		char szFileName[32];
		sprintf(szFileName, "d:\\%sSave.Buf", m_PlayerName);
		File.Create(szFileName);
		File.Write(pData, pData->dwDataLen);
		File.Close();
		#endif
	*/
	_ASSERT(pData->dwDataLen < sizeof(m_SaveBuffer));
	if (pData->dwDataLen <= 0)
	{
		return FALSE;
		}
	
	//	m_uMustSave = SAVE_REQUEST;
	m_ulLastSaveTime = g_SubWorldSet.GetGameTime();
	return TRUE;
}

BOOL KPlayer::CanSave()
{
	if (m_nNetConnectIdx == -1)
	{
		return FALSE;		
	}
	if (m_nIndex <= 0)
	{
	return FALSE;
	}
	
	if (m_bExchangeServer)
	{
	return FALSE;
	}
	
	if (m_bIsQuiting)
	{
		return FALSE;
	}
	if (CheckTrading())
	{
		return FALSE;
	}
	if (m_uMustSave == SAVE_DOING && g_SubWorldSet.GetGameTime() - m_ulLastSaveTime > defPLAYER_SAVE_TIMEOUT)
	{	
		return TRUE;
		
	}
	
	if (m_uMustSave != SAVE_IDLE)
	{
		return FALSE;
	}
	
	return TRUE;
}








BOOL KPlayer::CheckSaveRsul()
{

	if (m_uMustSave != SAVE_IDLE)
		return FALSE;
	
	return TRUE;
}







BOOL KPlayer::SendSyncData(int &nStep, unsigned int &nParam)
{	
	BOOL bRet = FALSE;
	switch(nStep)
	{
	case STEP_BASE_INFO:
		{	
			bRet = SubWorld[Npc[m_nIndex].m_SubWorldIndex].SendSyncData(m_nIndex, m_nNetConnectIdx);
			if (!bRet)
			{
				printf("SubWorld Fail.\n");
				break;
			}
			bRet = Npc[m_nIndex].SendSyncData(m_nNetConnectIdx);
			if (!bRet)
			{
				printf("NPC Fail.\n");
				break;
			}
			// Õâ¸öÏûÏ¢±ØÐëÔÚÍ¬²½ÊÀ½çNPCÊý¾Ýºó×ö£¬Ê¹¿Í»§¶ËÄÜÕÒµ½µ±Ç°Íæ¼ÒÔÚ¿Í»§¶ËµÄNpcË÷Òý
			CURPLAYER_SYNC	sSync;	// Í¬²½µ±Ç°Íæ¼ÒµÄ×ÔÉí¶ÀÌØÐÅÏ¢¸ø¿Í»§¶Ë£¨×°±¸µÈ£©
			sSync.ProtocolType = (BYTE)s2c_synccurplayer;
			sSync.m_dwID = Npc[m_nIndex].m_dwID;
			sSync.m_btLevel = (DWORD)Npc[m_nIndex].m_Level;
			sSync.m_btSex = Npc[m_nIndex].m_nSex;
			sSync.m_btKind = Npc[m_nIndex].m_Kind;
			sSync.m_btSeries = Npc[m_nIndex].m_Series;
			sSync.m_wLifeMax = Npc[m_nIndex].m_LifeMax;
			sSync.m_wStaminaMax = Npc[m_nIndex].m_StaminaMax;
			sSync.m_wManaMax = Npc[m_nIndex].m_ManaMax;
			sSync.m_HeadImage = Npc[m_nIndex].m_HeadImage;
			sSync.m_wAttributePoint = this->m_nAttributePoint;
			sSync.m_wSkillPoint = this->m_nSkillPoint;
			sSync.m_wStrength = this->m_nStrength;
			sSync.m_wDexterity = this->m_nDexterity;
			sSync.m_wVitality = this->m_nVitality;
			sSync.m_wEngergy = this->m_nEngergy;
			sSync.m_wLucky = this->m_nLucky;
			sSync.m_nExp = this->m_nExp;
			sSync.m_dwLeadExp = this->m_dwLeadExp;
			sSync.m_btImagePlayer = this->m_ImagePlayer;	// chan dung
			sSync.m_btCurFaction = this->m_cFaction.m_nCurFaction;
			sSync.m_btFirstFaction = this->m_cFaction.m_nFirstAddFaction;
			sSync.m_nFactionAddTimes = this->m_cFaction.m_nAddTimes;
			sSync.m_wWorldStat = (WORD)m_nWorldStat;
			sSync.m_wSectStat = (WORD)m_nSectStat;
			sSync.m_nMoney1 = this->m_ItemList.GetMoney(room_equipment);
			sSync.m_nMoney2 = this->m_ItemList.GetMoney(room_repository);
		


			if (SUCCEEDED(g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sSync, sizeof(CURPLAYER_SYNC))))
			{
				bRet = TRUE;
			}
			else
			{
				printf("player Packing sync data failed...\n");
				bRet = FALSE;
				break;
			}
		}
		break;
	case STEP_FIGHT_SKILL_LIST:	
		{
			bRet = SendSyncData_Skill();
		}
		break;
	case STEP_LIFE_SKILL_LIST:
		bRet = TRUE;
		break;
	case STEP_ITEM_LIST:
		bRet = TRUE;
		break;
	case STEP_TASK_LIST:
		bRet = TRUE;
		break;
	case STEP_SYNC_END:
		bRet = TRUE;
		break;
	}
	if (!bRet)
	{
		printf("Load PlayerInfo Step %d Failed...\n", nStep);
	}
	return bRet;
}

BOOL KPlayer::SendSyncData_Skill()
{
	SKILL_SEND_ALL_SYNC		sSkill;
	
	sSkill.ProtocolType = s2c_synccurplayerskill;
	sSkill.m_wProtocolLong = 2;
	int i;
	int j;
	for (i = 0,j = 0; i < MAX_NPCSKILL; i++)
	{
		sSkill.m_sAllSkill[i].SkillId = 0;
		sSkill.m_sAllSkill[i].SkillLevel = 0;
		if( Npc[m_nIndex].m_SkillList.m_Skills[i].SkillId <= 0)
			continue;
		sSkill.m_sAllSkill[j].SkillId = Npc[m_nIndex].m_SkillList.m_Skills[i].SkillId;
		sSkill.m_sAllSkill[j].SkillLevel = Npc[m_nIndex].m_SkillList.m_Skills[i].SkillLevel;
		j++;
	}
	sSkill.m_wProtocolLong += sizeof(SKILL_SEND_ALL_SYNC_DATA) * j;
	
	if (SUCCEEDED(g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sSkill, sSkill.m_wProtocolLong + 1)))
	{
		return TRUE;
	}
	else
	{
		printf("player Packing skill sync data failed...\n");
		return FALSE;
	}
}




void	KPlayer::SendCurHeightSyncData()
{
	CURPLAYER_HEIGHT_SYNC	sSync;
	sSync.ProtocolType = s2c_synccurplayerheight;
	sSync.m_Task100 = this->m_cTask.GetSaveVal(100);
	sSync.m_Task162 = this->m_cTask.GetSaveVal(162);
	sSync.m_Task191 = this->m_cTask.GetSaveVal(191);
	sSync.m_Task192 = this->m_cTask.GetSaveVal(192);
	sSync.m_Task193 = this->m_cTask.GetSaveVal(193);
	sSync.m_Task140 = this->m_cTask.GetSaveVal(140);
	sSync.m_Task139 = this->m_cTask.GetSaveVal(139);
	sSync.m_Task485 = this->m_cTask.GetSaveVal(485);
	sSync.m_Task128  = this->m_cTask.GetSaveVal(128);

	sSync.m_Task99  = this->m_cTask.GetSaveVal(99);

	sSync.m_Task15  = this->m_cTask.GetSaveVal(15);
	sSync.m_Task124  = this->m_cTask.GetSaveVal(124);

	g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sSync, sizeof(CURPLAYER_HEIGHT_SYNC));
}





void	KPlayer::SendCurNormalSyncData()
{
	CURPLAYER_NORMAL_SYNC	sSync;
	
	sSync.ProtocolType = s2c_synccurplayernormal;
	sSync.m_shLife = Npc[m_nIndex].m_CurrentLife;
	sSync.m_shStamina = Npc[m_nIndex].m_CurrentStamina;
	sSync.m_shMana = Npc[m_nIndex].m_CurrentMana;
	sSync.m_shAngry = 0;

	if ( !m_cTeam.m_nFlag )
		sSync.m_btTeamData = 0;
	else
	{
		if (m_cTeam.m_nFigure == TEAM_CAPTAIN)
			sSync.m_btTeamData = 0x03;
		else
			sSync.m_btTeamData = 0x01;
	}
	
	g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sSync, sizeof(CURPLAYER_NORMAL_SYNC));
}

void KPlayer::QuitGame(int nType)
{
	Save();
}

void KPlayer::BuyItem(BYTE* pProtocol)
{
	PLAYER_BUY_ITEM_COMMAND* pCommand = (PLAYER_BUY_ITEM_COMMAND *)pProtocol;
	BuySell.Buy(m_nPlayerIndex, m_BuyInfo.m_nBuyIdx, pCommand->m_BuyIdx, pCommand->m_Place, pCommand->m_X, pCommand->m_Y);
}

void KPlayer::SellItem(BYTE* pProtocol)
{
	PLAYER_SELL_ITEM_COMMAND* pCommand = (PLAYER_SELL_ITEM_COMMAND *)pProtocol;
	BuySell.Sell(m_nPlayerIndex, m_BuyInfo.m_nBuyIdx, m_ItemList.SearchID(pCommand->m_ID));
}


void KPlayer::ShopPrice(BYTE* pProtocol)
{
	PLAYER_SHOP_PRICE_COMMAND* pCommand = (PLAYER_SHOP_PRICE_COMMAND *)pProtocol;
	int nIdx = m_ItemList.SearchID(pCommand->m_ID);

	if (nIdx <= 0 || nIdx >= MAX_ITEM)
		return;

	int nPrice = pCommand->m_Price;

	if (nPrice < 0 || nPrice > 999999999)
		return;

	if (((Item[nIdx].GetGenre() == item_equip || Item[nIdx].GetGenre() == item_medicine) && Item[nIdx].m_GeneratorParam.nVersion > 1) || (Item[nIdx].GetGenre() == item_task && Item[nIdx].GetBindItem() >= 1))
		return;

	   int nListIdx = m_ItemList.FindSame(nIdx);



		if (nListIdx > 0 && nListIdx < MAX_PLAYER_ITEM)
		{
		if (m_ItemList.m_Items[nListIdx].nPlace != pos_equiproom)
			return;	
		}
		else
			return;

	Item[nIdx].m_nPriceShop = nPrice;
	PLAYER_SHOP_PRICE_RETURN_COMMAND	IDC;
	IDC.ProtocolType = s2c_playershoppricereturn;
	IDC.m_ID = Item[nIdx].m_dwID;
	IDC.m_Price = Item[nIdx].m_nPriceShop;

	if (g_pServer)
		g_pServer->PackDataToClient(m_nNetConnectIdx, &IDC, sizeof(PLAYER_SHOP_PRICE_RETURN_COMMAND));



}



void KPlayer::ShopBuyItem(BYTE* pProtocol)
{


PLAYER_SHOP_BUY_ITEM_COMMAND* pCommand = (PLAYER_SHOP_BUY_ITEM_COMMAND *)pProtocol;

if (m_nPlayerBuyShop <= 0 || m_nPlayerBuyShop >= MAX_PLAYER)
    return;


int nNpcIdxShop = Player[m_nPlayerBuyShop].m_nIndex;

if (nNpcIdxShop <= 0 || nNpcIdxShop >= MAX_NPC)
    return;

if (!Npc[nNpcIdxShop].m_dwID)
    return;

if (Npc[m_nIndex].m_FightMode || Npc[nNpcIdxShop].m_FightMode)
    return;


if (Player[m_nPlayerBuyShop].CheckTrading() || !Player[m_nPlayerBuyShop].CheckShopOpen())
    return;

if (m_dwTimeBuyShop != Player[m_nPlayerBuyShop].m_dwTimeOpenShop)
    return;
	
	int nIdx = Player[m_nPlayerBuyShop].m_ItemList.SearchID(pCommand->m_ID);

if (nIdx <= 0 || nIdx >= MAX_ITEM)
    return;

if (Npc[m_nIndex].m_SubWorldIndex < 0 || Npc[m_nIndex].m_SubWorldIndex != Npc[nNpcIdxShop].m_SubWorldIndex)
    return;



if (!Player[m_nPlayerBuyShop].m_ItemList.FindSame(nIdx))
    return;



if (Player[m_nPlayerBuyShop].m_ItemList.m_Items[Player[m_nPlayerBuyShop].m_ItemList.FindSame(nIdx)].nPlace != pos_equiproom)
    return;

 if (Item[nIdx].m_nPriceShop <= 0)
	return;



if (m_ItemList.GetEquipmentMoney() < Item[nIdx].m_nPriceShop)
	return;


ItemPos	Pos;

if (!m_ItemList.SearchPosition(Item[nIdx].GetWidth(), Item[nIdx].GetHeight(), &Pos))
{
 KPlayerChat::SendSystemInfo(1,m_nPlayerIndex, MESSAGE_SYSTEM_ANNOUCE_HEAD, "Kh«ng cßn chç trèng trong hµnh trang !", strlen("Kh«ng cßn chç trèng trong hµnh trang !") );
return;
}


if (Pos.nPlace != pos_equiproom)
{
KPlayerChat::SendSystemInfo(1,m_nPlayerIndex, MESSAGE_SYSTEM_ANNOUCE_HEAD, "Kh«ng cßn chç trèng trong hµnh trang !", strlen("Kh«ng cßn chç trèng trong hµnh trang !") );
return;
}





int nMoneyMapT = 0;


if (SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_bCheckTong)
{
int nTongT = SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_nTongT;
if (nTongT >= 0 && nTongT <= 50)
{



if (SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_dwTongName == m_cTong.GetTongNameID())
{
nMoneyMapT = Item[nIdx].m_nPriceShop * nTongT /200;
}
else
{
nMoneyMapT = Item[nIdx].m_nPriceShop * nTongT /100;
}





if (Item[nIdx].m_nPriceShop <= nMoneyMapT || nMoneyMapT < 0)
{
	nMoneyMapT = 0;
}
}
}

int logMoney = Item[nIdx].m_nPriceShop;
Pay(Item[nIdx].m_nPriceShop);

Player[m_nPlayerBuyShop].Earn(Item[nIdx].m_nPriceShop - nMoneyMapT);

Player[m_nPlayerBuyShop].m_ItemList.Remove(nIdx);


m_ItemList.Add(nIdx, Pos.nPlace, Pos.nX, Pos.nY);

Item[nIdx].m_nPriceShop = 0;






if (SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_bCheckTong)
{




try
	{

		bool bExecuteScriptMistake = true;
		KLuaScript * pScript = (KLuaScript* )g_GetScript("\\script\\admin\\banghoi\\banghoi.lua");;
		if (pScript)
		{

			int nTopIndex = 0;
			
			pScript->SafeCallBegin(&nTopIndex);
			if (pScript->CallFunction("AddMoneyMain",0, "dsd",SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_dwTongName,SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_szTongNameBC,nMoneyMapT));
			{
				bExecuteScriptMistake = false;
			}
			pScript->SafeCallEnd(nTopIndex);

			
		}
		

	}
	catch(...)
	{
		printf("Exception Have Caught When Execute Script[%d]!!!!!", g_FileName2Id("\\script\\admin\\banghoi\\banghoi.lua"));
	
	}
}


if (Player[m_nPlayerBuyShop].CanSave())
			{
				if (Player[m_nPlayerBuyShop].Save())
				{	
						printf("Save Player: %s \n",Player[m_nPlayerBuyShop].m_PlayerName);
						Player[m_nPlayerBuyShop].m_nLiXian = 0;
						Player[m_nPlayerBuyShop].m_nLiXian2 = 1;
						Player[m_nPlayerBuyShop].m_uMustSave = SAVE_REQUEST;
				}
	
			}
			
if (CanSave())
			{
				
				if (Save())
				{
					printf("Save Player: %s \n",m_PlayerName);
					m_uMustSave = SAVE_REQUEST;
					
				}
	
			}



////////////////////
ExecuteScript("\\script\\admin\\muaban.lua","main",m_nPlayerBuyShop);

////////////////

}


void KPlayer::OpenBuyShop(BYTE* pProtocol)
{
	PLAYER_OPEN_BUY_SHOP* pCommand = (PLAYER_OPEN_BUY_SHOP *)pProtocol;

    int nNpcIdx = NpcSet.SearchID(pCommand->m_dwNpcID);

	if (nNpcIdx <= 0 || nNpcIdx >= MAX_NPC)
		return;





	if (m_bBlock)
	{
			KPlayerChat::SendSystemInfo(1, m_nPlayerIndex, MESSAGE_SYSTEM_ANNOUCE_HEAD, "Ch­a më khãa r­¬ng, kh«ng thÓ xem hµng !", strlen("Ch­a më khãa r­¬ng, kh«ng thÓ xem hµng !") );
			return;	
	}




	if (Npc[m_nIndex].m_SubWorldIndex == g_SubWorldSet.SearchWorld(43) || Npc[nNpcIdx].m_SubWorldIndex == g_SubWorldSet.SearchWorld(43))
		return;


if (!Npc[nNpcIdx].IsPlayer())
return;










int nPlayerIdx = Npc[nNpcIdx].GetPlayerIdx();

if (nPlayerIdx <= 0 || nPlayerIdx >= MAX_PLAYER)
return;

if (!Player[nPlayerIdx].CheckShopOpen())
return;

if (Npc[m_nIndex].m_FightMode || Npc[nNpcIdx].m_FightMode)
return;


m_dwTimeBuyShop = Player[nPlayerIdx].m_dwTimeOpenShop;

m_nPlayerBuyShop = nPlayerIdx;



BOX_BUY_SHOP_SYNC	sItemBox;
sItemBox.ProtocolType = s2c_synclistitembuyshop;
sItemBox.m_nTime = m_dwTimeBuyShop;
memcpy(sItemBox.m_szName,Npc[Player[nPlayerIdx].m_nIndex].Name, sizeof(sItemBox.m_szName));
g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sItemBox, sizeof(BOX_BUY_SHOP_SYNC));




int nsl = 0;



for (int i = 0; i < MAX_PLAYER_ITEM; i++)
{




int nIdx = Player[nPlayerIdx].m_ItemList.m_Items[i].nIdx;
int nPlace = Player[nPlayerIdx].m_ItemList.m_Items[i].nPlace;
int nX = Player[nPlayerIdx].m_ItemList.m_Items[i].nX;
int nY = Player[nPlayerIdx].m_ItemList.m_Items[i].nY;

if (nIdx > 0 && nIdx < MAX_ITEM && nPlace == pos_equiproom && Item[nIdx].m_nPriceShop > 0)
{



ITEM_BUY_SHOP_SYNC sItem;
sItem.ProtocolType = s2c_syncitembuyshop;
sItem.m_ID = Item[nIdx].GetID();
sItem.m_Genre = Item[nIdx].GetGenre();
sItem.m_Detail = Item[nIdx].GetDetailType();
sItem.m_Particur = Item[nIdx].GetParticular();
sItem.m_Series = Item[nIdx].GetSeries();
sItem.m_Level = Item[nIdx].GetLevel();
sItem.m_Luck = Item[nIdx].m_GeneratorParam.nLuck;
sItem.m_btPlace = pos_buyshop;
sItem.m_btX = nX;
sItem.m_btY = nY;
sItem.m_nPriceShop = Item[nIdx].m_nPriceShop;
				
for (int j = 0; j < 6; j++)
	sItem.m_MagicLevel[j] = Item[nIdx].m_GeneratorParam.nGeneratorLevel[j];

sItem.m_RandomSeed = Item[nIdx].m_GeneratorParam.uRandomSeed;
sItem.m_Version = Item[nIdx].m_GeneratorParam.nVersion;
sItem.m_Durability = Item[nIdx].GetDurability();
sItem. iNgoaiTrang = Item[nIdx].GetNgoaiTrang();

g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sItem, sizeof(ITEM_BUY_SHOP_SYNC));

nsl ++;

if (nsl >= 60)
break;

}



}





}












void KPlayer::CloseBuyShop(BYTE* pProtocol)
{
PLAYER_CLOSE_BUY_SHOP* pCommand = (PLAYER_CLOSE_BUY_SHOP *)pProtocol;

m_dwTimeBuyShop = 0;
m_nPlayerBuyShop = 0;
}











void KPlayer::ShopOpen(BYTE* pProtocol)
{
	PLAYER_SHOP_OPEN_COMMAND* pCommand = (PLAYER_SHOP_OPEN_COMMAND *)pProtocol;


	if (m_bBlock)
	{
			KPlayerChat::SendSystemInfo(1, m_nPlayerIndex, MESSAGE_SYSTEM_ANNOUCE_HEAD, "Ch­a më khãa r­¬ng, kh«ng thÓ bµy b¸n !", strlen("Ch­a më khãa r­¬ng, kh«ng thÓ bµy b¸n !") );
			return;	
	}

	/*
	if (Npc[m_nIndex].m_Level < 100)
	{
		KPlayerChat::SendSystemInfo(1,m_nPlayerIndex, MESSAGE_SYSTEM_ANNOUCE_HEAD, "§¼ng cÊp nhá h¬n 100, kh«ng thÓ b¸y b¸n !", strlen("§¼ng cÊp nhá h¬n 100, kh«ng thÓ b¸y b¸n !") );
		return;
	}
*/

	if (Npc[m_nIndex].m_SubWorldIndex == g_SubWorldSet.SearchWorld(43))
		return;
	



	if (m_dwTimeOpenShop)
	{
     m_dwTimeOpenShop = 0;
	 Npc[m_nIndex].BroadCastShop();
	 Npc[m_nIndex].SendCommand(do_stand);
	}
	else
	{


	if (!m_szNameOpenShop[0])
	{

	m_dwTimeOpenShop = 0;
	 Npc[m_nIndex].SendCommand(do_stand);

	PLAYER_SHOP_OPEN_RETURN_COMMAND	IDC;
	IDC.ProtocolType = s2c_playershopopenreturn;
	IDC.m_dwTime = m_dwTimeOpenShop;
	memcpy(IDC.szName, m_szNameOpenShop, sizeof(IDC.szName));

	if (g_pServer)
		g_pServer->PackDataToClient(m_nNetConnectIdx, &IDC, sizeof(PLAYER_SHOP_OPEN_RETURN_COMMAND));

		KPlayerChat::SendSystemInfo(1, m_nPlayerIndex, MESSAGE_SYSTEM_ANNOUCE_HEAD, "Ch­a ®Æt tªn cöa hiÖu, kh«ng thÓ më!", strlen("Ch­a ®Æt tªn cöa hiÖu, kh«ng thÓ më!") );
			return;	

	}

	if (Npc[m_nIndex].m_FightMode)
	{

	m_dwTimeOpenShop = 0;
	Npc[m_nIndex].SendCommand(do_stand);

	PLAYER_SHOP_OPEN_RETURN_COMMAND	IDC;
	IDC.ProtocolType = s2c_playershopopenreturn;
	IDC.m_dwTime = m_dwTimeOpenShop;
	memcpy(IDC.szName, m_szNameOpenShop, sizeof(IDC.szName));

	if (g_pServer)
		g_pServer->PackDataToClient(m_nNetConnectIdx, &IDC, sizeof(PLAYER_SHOP_OPEN_RETURN_COMMAND));
		KPlayerChat::SendSystemInfo(1, m_nPlayerIndex, MESSAGE_SYSTEM_ANNOUCE_HEAD, "ChØ cã thÓ më cöa hiÖu khi ë trong thµnh thÞ",strlen("ChØ cã thÓ më cöa hiÖu khi ë trong thµnh thÞ") );
			return;

	}





	if (Npc[m_nIndex].m_SubWorldIndex < 0 || 
	(
	SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_SubWorldID != 53
	//SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_SubWorldID != 9 &&
	//SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_SubWorldID != 15 &&
	//SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_SubWorldID != 17)
	//SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_SubWorldID != 3 &&
	//SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_SubWorldID != 60 &&

	))

	//if (Npc[m_nIndex].m_SubWorldIndex < 0 || 
	//SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_SubWorldID != 9)
	{

	m_dwTimeOpenShop = 0;
	Npc[m_nIndex].SendCommand(do_stand);

	PLAYER_SHOP_OPEN_RETURN_COMMAND	IDC;
	IDC.ProtocolType = s2c_playershopopenreturn;
	IDC.m_dwTime = m_dwTimeOpenShop;
	memcpy(IDC.szName, m_szNameOpenShop, sizeof(IDC.szName));

	if (g_pServer)
		g_pServer->PackDataToClient(m_nNetConnectIdx, &IDC, sizeof(PLAYER_SHOP_OPEN_RETURN_COMMAND));
		KPlayerChat::SendSystemInfo(1, m_nPlayerIndex, MESSAGE_SYSTEM_ANNOUCE_HEAD, "Kh«ng thÓ bµy b¸n t¹i ®©y. ChØ ®­îc bµy b¸n ë Ba L¨ng HuyÖn !",strlen("Kh«ng thÓ bµy b¸n t¹i ®©y. ChØ ®­îc bµy b¸n ë Ba L¨ng HuyÖn !") );
			return;
	}











	if (Npc[m_nIndex].m_bRideHorse)
		{

	m_dwTimeOpenShop = 0;
	Npc[m_nIndex].SendCommand(do_stand);

	PLAYER_SHOP_OPEN_RETURN_COMMAND	IDC;
	IDC.ProtocolType = s2c_playershopopenreturn;
	IDC.m_dwTime = m_dwTimeOpenShop;
	memcpy(IDC.szName, m_szNameOpenShop, sizeof(IDC.szName));

	if (g_pServer)
		g_pServer->PackDataToClient(m_nNetConnectIdx, &IDC, sizeof(PLAYER_SHOP_OPEN_RETURN_COMMAND));
		KPlayerChat::SendSystemInfo(1, m_nPlayerIndex, MESSAGE_SYSTEM_ANNOUCE_HEAD, "Kh«ng thÓ më cöa hiÖu khi c­ìi ngùa !",strlen("Kh«ng thÓ më cöa hiÖu khi c­ìi ngùa !") );
			return;

	}


	Npc[m_nIndex].SendCommand(do_sit);

    m_dwTimeOpenShop = g_SubWorldSet.GetGameTime();
	Npc[m_nIndex].BroadCastShop();

	}



	PLAYER_SHOP_OPEN_RETURN_COMMAND	IDC;
	IDC.ProtocolType = s2c_playershopopenreturn;
	IDC.m_dwTime = m_dwTimeOpenShop;
	memcpy(IDC.szName, m_szNameOpenShop, sizeof(IDC.szName));


	if (g_pServer)
		g_pServer->PackDataToClient(m_nNetConnectIdx, &IDC, sizeof(PLAYER_SHOP_OPEN_RETURN_COMMAND));


	


}






void KPlayer::LoginPWStoreBox(BYTE* pProtocol)
{
	PLAYER_LOGIN_PW_STORE_BOX* pCommand = (PLAYER_LOGIN_PW_STORE_BOX *)pProtocol;



   if (m_nIndex <= 0 || m_nIndex >= MAX_NPC)
	{
		printf("Xay ra loi ID NPC Login PW Store Box [%s] [%s]. Tu dong xua loi !\n",this->m_PlayerName,this->m_AccoutName);
		return;
	}

   if (!m_cTask.GetSaveVal(152))
   {
		printf("Xay ra loi Task Login PW Store Box [%s] [%s]. Tu dong xua loi !\n",this->m_PlayerName,this->m_AccoutName);
		return;
   }

   if (!m_bBlock)
   {
	   	//printf("Xay ra loi mo khong khoa Login PW Store Box [%s] [%s]. Tu dong xua loi !\n",this->m_PlayerName,this->m_AccoutName);
		return;

   }



   if (strlen(pCommand->m_szString) > 31)
   {
	   printf("Xay ra loi Len Login PW Store Box [%s] [%s]. Tu dong xua loi !\n",this->m_PlayerName,this->m_AccoutName);
		return;
   }

   char szString[32];

   memcpy(szString, pCommand->m_szString, sizeof(szString));

   if (!szString[0])
	{
		printf("Xay ra loi Text Login PW Store Box [%s] [%s]. Tu dong xua loi !\n",this->m_PlayerName,this->m_AccoutName);
		return;
	}



   //printf("Nhap pw [%s]\n",szString);

   ExecuteScript("\\script\\cacsukien\\help\\ruongchuado.lua", "loginbox", szString);
  

}


void KPlayer::UnBlockStoreBox(BYTE* pProtocol)
{

	PLAYER_LOGIN_PW_STORE_BOX* pCommand = (PLAYER_LOGIN_PW_STORE_BOX *)pProtocol;



   if (m_nIndex <= 0 || m_nIndex >= MAX_NPC)
	{
		printf("Xay ra loi ID NPC Login PW Store Box [%s] [%s]. Tu dong xua loi !\n",this->m_PlayerName,this->m_AccoutName);
		return;
	}

   if (!m_cTask.GetSaveVal(152))
   {
		printf("Xay ra loi Task Login PW Store Box [%s] [%s]. Tu dong xua loi !\n",this->m_PlayerName,this->m_AccoutName);
		return;
   }

   if (!m_bBlock)
   {
	   //	printf("Xay ra loi mo khong khoa Login PW Store Box [%s] [%s]. Tu dong xua loi !\n",this->m_PlayerName,this->m_AccoutName);
		return;


   }



   if (strlen(pCommand->m_szString) > 31)
   {
	   printf("Xay ra loi Len Login PW Store Box [%s] [%s]. Tu dong xua loi !\n",this->m_PlayerName,this->m_AccoutName);
		return;
   }

   char szString[32];

   memcpy(szString, pCommand->m_szString, sizeof(szString));

   if (!szString[0])
	{
		printf("Xay ra loi Text Login PW Store Box [%s] [%s]. Tu dong xua loi !\n",this->m_PlayerName,this->m_AccoutName);
		return;
	}



   //printf("Nhap pw [%s]\n",szString);

   ExecuteScript("\\script\\cacsukien\\help\\ruongchuado.lua", "loginbox", szString);

}




void KPlayer::BlockStoreBox(BYTE* pProtocol)
{
	PLAYER_BLOCK_STORE_BOX* pCommand = (PLAYER_BLOCK_STORE_BOX *)pProtocol;



   if (m_nIndex <= 0 || m_nIndex >= MAX_NPC)
	{
		printf("Xay ra loi ID NPC Block Store Box [%s] [%s]. Tu dong xua loi !\n",this->m_PlayerName,this->m_AccoutName);
		return;
	}

   if (!m_cTask.GetSaveVal(152))
   {
		KPlayerChat::SendSystemInfo(1,m_nPlayerIndex, MESSAGE_SYSTEM_ANNOUCE_HEAD, "H·y ®Õn gÆp thñ kho ®Ó më r­¬ng chøa ®å !", strlen("H·y ®Õn gÆp thñ kho ®Ó më r­¬ng chøa ®å !") );
		return;
   }
   
   if (m_bBlock)
   {
	   	printf("Xay ra loi doi khoa Block Store Box [%s] [%s]. Tu dong xua loi !\n",this->m_PlayerName,this->m_AccoutName);
		return;
   }


   //printf("block box\n");

   ExecuteScript("\\script\\cacsukien\\help\\ruongchuado.lua", "blockbox");


}






void KPlayer::NewPWStoreBox(BYTE* pProtocol)
{
	PLAYER_NEW_PW_STORE_BOX* pCommand = (PLAYER_NEW_PW_STORE_BOX *)pProtocol;



   if (m_nIndex <= 0 || m_nIndex >= MAX_NPC)
	{
		printf("Xay ra loi ID NPC New PW Store Box [%s] [%s]. Tu dong xua loi !\n",this->m_PlayerName,this->m_AccoutName);
		return;
	}

   if (m_cTask.GetSaveVal(152))
   {
		printf("Xay ra loi Task New PW Store Box [%s] [%s]. Tu dong xua loi !\n",this->m_PlayerName,this->m_AccoutName);
		return;
   }
   
   if (m_bBlock)
   {
	   	printf("Xay ra loi doi khoa New PW Store Box [%s] [%s]. Tu dong xua loi !\n",this->m_PlayerName,this->m_AccoutName);
		return;


   }



   if (strlen(pCommand->m_szString) > 31)
   {
	   printf("Xay ra loi Len New PW Store Box [%s] [%s]. Tu dong xua loi !\n",this->m_PlayerName,this->m_AccoutName);
		return;
   }

   char szString[32];
  

   memcpy(szString, pCommand->m_szString, sizeof(szString));
   

   if (!szString[0])
	{
		printf("Xay ra loi Text New PW Store Box [%s] [%s]. Tu dong xua loi !\n",this->m_PlayerName,this->m_AccoutName);
		return;
	}



  // printf("Lap pw [%s]\n",szString);

   ExecuteScript("\\script\\cacsukien\\help\\ruongchuado.lua", "newbox", szString);

  

}








void KPlayer::PWStoreBox(BYTE* pProtocol)
{
	PLAYER_PW_STORE_BOX* pCommand = (PLAYER_PW_STORE_BOX *)pProtocol;



   if (m_nIndex <= 0 || m_nIndex >= MAX_NPC)
	{
		printf("Xay ra loi ID NPC PW Store Box [%s] [%s]. Tu dong xua loi !\n",this->m_PlayerName,this->m_AccoutName);
		return;
	}

   if (!m_cTask.GetSaveVal(152))
   {
		printf("Xay ra loi Task PW Store Box [%s] [%s]. Tu dong xua loi !\n",this->m_PlayerName,this->m_AccoutName);
		return;
   }
   
   if (m_bBlock)
   {
	   	printf("Xay ra loi doi khoa PW Store Box [%s] [%s]. Tu dong xua loi !\n",this->m_PlayerName,this->m_AccoutName);
		return;


   }



   if (strlen(pCommand->m_szString) > 31 || strlen(pCommand->m_szStringNew) > 31)
   {
	   printf("Xay ra loi Len PW Store Box [%s] [%s]. Tu dong xua loi !\n",this->m_PlayerName,this->m_AccoutName);
		return;
   }

   char szString[32];
   char szStringNew[32];

   memcpy(szString, pCommand->m_szString, sizeof(szString));
   memcpy(szStringNew, pCommand->m_szStringNew, sizeof(szStringNew));

   if (!szString[0] || !szStringNew[0])
	{
		printf("Xay ra loi Text PW Store Box [%s] [%s]. Tu dong xua loi !\n",this->m_PlayerName,this->m_AccoutName);
		return;
	}



   //printf("Doi pw [%s] [%s]\n",szString,szStringNew);

   ExecuteScriptNew("\\script\\cacsukien\\help\\ruongchuado.lua", "changebox", szString, szStringNew);

  

}











void KPlayer::StringBox(BYTE* pProtocol)
{
	PLAYER_STRING_BOX* pCommand = (PLAYER_STRING_BOX *)pProtocol;



   if (m_nIndex <= 0 || m_nIndex >= MAX_NPC)
	{
		printf("Xay ra loi ID NPC String Box [%s] [%s]. Tu dong xua loi !\n",this->m_PlayerName,this->m_AccoutName);
		return;
	}

   if (!m_dwScriptStringBoxID)
	{
		printf("Xay ra loi ID Spcrit String Box [%s] [%s]. Tu dong xua loi !\n",this->m_PlayerName,this->m_AccoutName);
		return;
	}

   if (!m_szFunStringBox[0])
	{
		printf("Xay ra loi Func Spcrit String Box [%s] [%s]. Tu dong xua loi !\n",this->m_PlayerName,this->m_AccoutName);
		return;
	}



   if (strlen(pCommand->m_szString) > 31)
   {
	   printf("Xay ra loi Len String Box [%s] [%s]. Tu dong xua loi !\n",this->m_PlayerName,this->m_AccoutName);
		return;
   }

   char szString[32];

   memcpy(szString, pCommand->m_szString, sizeof(szString));

   if (!szString[0])
	{
		printf("Xay ra loi Text String Box [%s] [%s]. Tu dong xua loi !\n",this->m_PlayerName,this->m_AccoutName);
		return;
	}


   ExecuteScript(m_dwScriptStringBoxID, m_szFunStringBox, szString);

   m_dwScriptStringBoxID = 0;
   m_szFunStringBox[0] = 0;

}




void KPlayer::ShopName(BYTE* pProtocol)
{
	PLAYER_SHOP_NAME* pCommand = (PLAYER_SHOP_NAME *)pProtocol;


   if (m_nIndex <= 0 || m_nIndex >= MAX_NPC)
	{
		printf("Xay ra loi ID NPC Shop Name [%s] [%s]. Tu dong xua loi !\n",this->m_PlayerName,this->m_AccoutName);
		return;
	}

   if (strlen(pCommand->m_szString) > 31)
   {
	   printf("Xay ra loi Len Shop Name [%s] [%s]. Tu dong xua loi !\n",this->m_PlayerName,this->m_AccoutName);
		return;
   }

   memcpy(m_szNameOpenShop, pCommand->m_szString, sizeof(m_szNameOpenShop));

  

}





void KPlayer::CheckItemBox(BYTE* pProtocol)
{
	PLAYER_CHECK_ITEM_BOX* pCommand = (PLAYER_CHECK_ITEM_BOX *)pProtocol;



   if (m_nIndex <= 0 || m_nIndex >= MAX_NPC)
	{
		printf("Xay ra loi ID NPC Check Item Box [%s] [%s]. Tu dong xua loi !\n",this->m_PlayerName,this->m_AccoutName);
		return;
	}

   if (!m_dwScriptCheckItemBoxID)
	{
		printf("Xay ra loi ID Spcrit Check Item Box [%s] [%s]. Tu dong xua loi !\n",this->m_PlayerName,this->m_AccoutName);
		return;
	}

   if (!m_szFunCheckItemBox[0])
	{
		printf("Xay ra loi Func Spcrit Check Item Box [%s] [%s]. Tu dong xua loi !\n",this->m_PlayerName,this->m_AccoutName);
		return;
	}



   ExecuteScript(m_dwScriptCheckItemBoxID, m_szFunCheckItemBox, 0);

   m_dwScriptCheckItemBoxID = 0;
   m_szFunCheckItemBox[0] = 0;


}




void KPlayer::ButtonPGBox(BYTE* pProtocol)
{
	PLAYER_SLECT_CHECK_BOX* pCommand = (PLAYER_SLECT_CHECK_BOX *)pProtocol;



   if (m_nIndex <= 0 || m_nIndex >= MAX_NPC)
	{
		printf("Xay ra loi ID NPC PG Box [%s] [%s]. Tu dong xua loi !\n",this->m_PlayerName,this->m_AccoutName);
		return;
	}

   


   int nSlect = pCommand->m_nSlect;


   if (nSlect < 1 || nSlect > 4)
   {
		printf("Xay ra loi Spcrit nSlect PG Box [%s] [%s]. Tu dong xua loi !\n",this->m_PlayerName,this->m_AccoutName);
		return;
   }






   ExecuteScript("\\script\\admin\\pgbox.lua", "main", nSlect);




}





void KPlayer::SlectCheckBox(BYTE* pProtocol)
{
	PLAYER_SLECT_CHECK_BOX* pCommand = (PLAYER_SLECT_CHECK_BOX *)pProtocol;



   if (m_nIndex <= 0 || m_nIndex >= MAX_NPC)
	{
		printf("Xay ra loi ID NPC Slect Check Box [%s] [%s]. Tu dong xua loi !\n",this->m_PlayerName,this->m_AccoutName);
		return;
	}

   if (!m_dwScriptSlectCheckBoxID)
	{
		printf("Xay ra loi ID Spcrit Slect Check Box [%s] [%s]. Tu dong xua loi !\n",this->m_PlayerName,this->m_AccoutName);
		return;
	}

   if (!m_szFunSlectCheckBox[0])
	{
		printf("Xay ra loi Func Spcrit Slect Check Box [%s] [%s]. Tu dong xua loi !\n",this->m_PlayerName,this->m_AccoutName);
		return;
	}


   if (!m_btSlectCheckBox[0] || !m_btSlectCheckBox[1] || !m_btSlectCheckBox[2])
   {
		printf("Xay ra loi Spcrit nID Slect Check Box [%s] [%s]. Tu dong xua loi !\n",this->m_PlayerName,this->m_AccoutName);
		return;
   }


   int nSlect = pCommand->m_nSlect;


   if (nSlect < 1 || nSlect > 3)
   {
		printf("Xay ra loi Spcrit nSlect Slect Check Box [%s] [%s]. Tu dong xua loi !\n",this->m_PlayerName,this->m_AccoutName);
		return;
   }





   ExecuteScript(m_dwScriptSlectCheckBoxID, m_szFunSlectCheckBox, m_btSlectCheckBox[nSlect-1]);

   m_dwScriptSlectCheckBoxID = 0;
   m_szFunSlectCheckBox[0] = 0;

   	m_btSlectCheckBox[0] = 0;
	m_btSlectCheckBox[1] = 0;
	m_btSlectCheckBox[2] = 0;


}




void KPlayer::StaskItem(BYTE* pProtocol)
{
	PLAYER_STASK_ITEM_COMMAND* pCommand = (PLAYER_STASK_ITEM_COMMAND *)pProtocol;
	int ItemIdx = m_ItemList.SearchID(pCommand->m_ID);

	if (ItemIdx <= 0 || ItemIdx >= MAX_ITEM)
		return;
	int nNumber = pCommand->m_Number;

	if (!Item[ItemIdx].IsStack() || Item[ItemIdx].GetGenre() != item_task || nNumber >= Item[ItemIdx].GetVersion() || nNumber < 1)
	    return;

	if (Item[ItemIdx].GetVersion() > 50 || Item[ItemIdx].GetVersion() < 1)
	    return;

	if (!m_ItemList.FindSame(ItemIdx))
		return;

	if (m_ItemList.m_Items[m_ItemList.FindSame(ItemIdx)].nPlace != pos_equiproom && m_ItemList.m_Items[m_ItemList.FindSame(ItemIdx)].nPlace != pos_repositoryroom && m_ItemList.m_Items[m_ItemList.FindSame(ItemIdx)].nPlace != pos_rpnew)
		return;

	if (!m_ItemList.CheckItemInAll(ItemIdx))
	{
		printf("Hack StaskItem [%s] [%s]\n",m_AccoutName,m_PlayerName);
		return;
	}



	int	nIdx = m_ItemList.Hand();

	if (nIdx != 0)
		return;

	int nIndex = ItemSet.Add(item_task, 0, 0, 0, Item[ItemIdx].GetDetailType(), 0, 0, nNumber, 0);
	
	if (nIndex <= 0)
		return;

	Item[ItemIdx].SetVersion(Item[ItemIdx].GetVersion()-nNumber);

	m_ItemList.Add(nIndex, pos_hand, 0 ,0);
	
	PLAYER_ITEM_UPDATE_VERSION	IDC;
	IDC.ProtocolType = s2c_playerupdateitemvertion;
	IDC.m_ID = Item[ItemIdx].m_dwID;
	IDC.m_Vesion = Item[ItemIdx].GetVersion();

	if (g_pServer)
	g_pServer->PackDataToClient(m_nNetConnectIdx, &IDC, sizeof(PLAYER_ITEM_UPDATE_VERSION));

	
    

}


void KPlayer::BlockItem(BYTE* pProtocol)
{
	PLAYER_BLOCK_ITEM_COMMAND* pCommand = (PLAYER_BLOCK_ITEM_COMMAND *)pProtocol;
	int ItemIdx = m_ItemList.SearchID(pCommand->m_ID);
	if (ItemIdx <= 0 || ItemIdx >= MAX_ITEM)
		return;

	if (Item[ItemIdx].GetGenre() != item_equip)
	{
		printf("Loi Block Item [%s] [%s] !\n",m_AccoutName,m_PlayerName);
		return;
	}
	
	int DayMonth[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

	SYSTEMTIME TimeNow;
	GetLocalTime(&TimeNow);

    int TimeNowMinute = 0;

	for (int t = 0; t < (int)TimeNow.wMonth-1;t++)
	{
		TimeNowMinute += DayMonth[t] * 24 * 60;
	}
	
	 TimeNowMinute += TimeNow.wDay * 24 * 60 + TimeNow.wHour * 60 + TimeNow.wMinute + 2 * 24 * 60;




	if (Item[ItemIdx].m_GeneratorParam.nVersion != 2)
	{

	  if (Item[ItemIdx].m_nPriceShop)
		{
		   Item[ItemIdx].m_nPriceShop = 0;
	       PLAYER_UPDATE_SHOP_PRICE_COMMAND	IDC;
	       IDC.ProtocolType = s2c_playerupdateshopprice;
	       IDC.m_ID = Item[ItemIdx].m_dwID;
	       IDC.m_Price = Item[ItemIdx].m_nPriceShop;
	       if (g_pServer)
		   g_pServer->PackDataToClient(m_nNetConnectIdx, &IDC, sizeof(PLAYER_UPDATE_SHOP_PRICE_COMMAND));
		}


	if (Item[ItemIdx].m_GeneratorParam.nVersion == 550324)
	{
		PLAYER_UNBLOCK_ITEM_RETURN_COMMAND	IDC;
		IDC.ProtocolType = s2c_playerunblockitemreturn;
		IDC.m_ID = Item[ItemIdx].m_dwID;
		IDC.m_Vesion = Item[ItemIdx].m_GeneratorParam.nVersion;
		if (g_pServer)
			g_pServer->PackDataToClient(m_nNetConnectIdx, &IDC, sizeof(PLAYER_UNBLOCK_ITEM_RETURN_COMMAND));
	}
	else
	{
		Item[ItemIdx].m_GeneratorParam.nVersion = 2;
		PLAYER_BLOCK_ITEM_RETURN_COMMAND	IDC;
		IDC.ProtocolType = s2c_playerblockitemreturn;
		IDC.m_ID = Item[ItemIdx].m_dwID;
		IDC.m_Vesion = Item[ItemIdx].m_GeneratorParam.nVersion;
		if (g_pServer)
		g_pServer->PackDataToClient(m_nNetConnectIdx, &IDC, sizeof(PLAYER_BLOCK_ITEM_RETURN_COMMAND));

	}
	}
}





void KPlayer::UnBlockItem(BYTE* pProtocol)
{
	PLAYER_UNBLOCK_ITEM_COMMAND* pCommand = (PLAYER_UNBLOCK_ITEM_COMMAND *)pProtocol;
	m_ItemList.SearchID(pCommand->m_ID);
	int ItemIdx = m_ItemList.SearchID(pCommand->m_ID);
	if (ItemIdx <= 0 || ItemIdx >= MAX_ITEM)
		return;

	if (Item[ItemIdx].GetGenre() != item_equip)
	{
		printf("Loi UnBlock Item [%s] [%s] !\n",m_AccoutName,m_PlayerName);
		return;
	}

	if (Item[ItemIdx].m_GeneratorParam.nVersion == 2)
	{

    int DayMonth[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

	SYSTEMTIME TimeNow;
	GetLocalTime(&TimeNow);

    int TimeNowMinute = 0;

	for (int t = 0; t < (int)TimeNow.wMonth-1;t++)
	{
		TimeNowMinute += DayMonth[t] * 24 * 60;
	}
	
	 TimeNowMinute += TimeNow.wDay * 24 * 60 + TimeNow.wHour * 60 + TimeNow.wMinute + 2 * 24 * 60;

     Item[ItemIdx].m_GeneratorParam.nVersion = 3 + TimeNowMinute;

	PLAYER_UNBLOCK_ITEM_RETURN_COMMAND	IDC;
	IDC.ProtocolType = s2c_playerunblockitemreturn;
	IDC.m_ID = Item[ItemIdx].m_dwID;
	IDC.m_Vesion = Item[ItemIdx].m_GeneratorParam.nVersion;
	if (g_pServer)
		g_pServer->PackDataToClient(m_nNetConnectIdx, &IDC, sizeof(PLAYER_UNBLOCK_ITEM_RETURN_COMMAND));
	
	
	}
}

#endif




#ifdef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£º»ñÈ¡Íæ¼ÒÖØÉúµãÎ»ÖÃ
//-------------------------------------------------------------------------
void KPlayer::GetLoginRevivalPos(int *lpnSubWorld, int *lpnMpsX, int *lpnMpsY)
{
	*lpnSubWorld = m_sLoginRevivalPos.m_nSubWorldID;
	*lpnMpsX = m_sLoginRevivalPos.m_nMpsX;
	*lpnMpsY = m_sLoginRevivalPos.m_nMpsY;
}


void KPlayer::GetDeathRevivalPos(int *lpnSubWorld, int *lpnMpsX, int *lpnMpsY)
{
	*lpnSubWorld = m_sDeathRevivalPos.m_nSubWorldID;
	*lpnMpsX = m_sDeathRevivalPos.m_nMpsX;
	*lpnMpsY = m_sDeathRevivalPos.m_nMpsY;
}
#endif
#ifdef _SERVER
void	KPlayer::SetTimer(DWORD nTime, int nTimerTaskId)					//Ê±¼äÈÎÎñ½Å±¾£¬¿ªÆô¼ÆÊ±Æ÷
{
	if (!nTime || !nTimerTaskId) return ;
	m_TimerTask.SetTimer(nTime, nTimerTaskId);
}

void	KPlayer::CloseTimer()							//¹Ø±ÕÊ±¼ä¼ÆÊ±Æ÷
{
	m_TimerTask.CloseTimer();
}
#endif


#ifdef _SERVER
//------------------------------------------------------------------------------
//	¹¦ÄÜ£ºÉè¶¨Íæ¼ÒÖØÉúµãÎ»ÖÃ
//------------------------------------------------------------------------------
void	KPlayer::SetRevivalPos(int nSubWorld, int nReviveId)
{
	if (nSubWorld >= 0)  //Èç¹ûÐ¡ÓÚ0£¬±íÊ¾ÑØÓÃµ±Ç°µÄ
	{
		//printf("test 4 \n");	
		m_sLoginRevivalPos.m_nSubWorldID = nSubWorld;
	}
	else
	{
		m_sLoginRevivalPos.m_nSubWorldID = SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_SubWorldID;
	}
	
	POINT Pos;
	
	g_SubWorldSet.GetRevivalPosFromId(m_sLoginRevivalPos.m_nSubWorldID, nReviveId, &Pos);
	m_sLoginRevivalPos.m_ReviveID = nReviveId;
	m_sLoginRevivalPos.m_nMpsX = Pos.x;
	m_sLoginRevivalPos.m_nMpsY = Pos.y;
	m_sDeathRevivalPos = m_sLoginRevivalPos;
}
#endif

#ifdef _SERVER
//------------------------------------------------------------------------------
//	¹¦ÄÜ£ºÊÕµ½¿Í»§¶Ë²éÑ¯Ä³¸önpc×é¶ÓÐÅÏ¢µÄÉêÇëºó£¬ÏòÕâ¸ö¿Í»§¶Ë·¢ËÍ¶ÓÎéÐÅÏ¢
//------------------------------------------------------------------------------
void	KPlayer::S2CSendTeamInfo(BYTE* pProtocol)
{
	PLAYER_APPLY_TEAM_INFO	*pApplyTeamInfo = (PLAYER_APPLY_TEAM_INFO*)pProtocol;
	
	//------------------------------ ²éÑ¯×ÔÉí×é¶ÓÐÅÏ¢ -----------------------------
	if (pApplyTeamInfo->m_dwTarNpcID == Npc[m_nIndex].m_dwID)
	{
		SendSelfTeamInfo();
		return;
	}
	
	//--------------------------- ²éÑ¯ÆäËû npc ×é¶ÓÐÅÏ¢ -----------------------------
	PLAYER_SEND_TEAM_INFO	sTeamInfo;
	PLAYER_APPLY_TEAM_INFO_FALSE	sTeamInfoFalse;


	if (Npc[m_nIndex].m_SubWorldIndex >= MAX_SUBWORLD || Npc[m_nIndex].m_SubWorldIndex < 0 || Npc[m_nIndex].m_RegionIndex >= SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_nTotalRegion || Npc[m_nIndex].m_RegionIndex < 0)
	{
		Npc[m_nIndex].m_SubWorldIndex = 0;
		Npc[m_nIndex].m_RegionIndex = 381;
		Npc[m_nIndex].m_MapX = 0;
		Npc[m_nIndex].m_MapY = 0;
		Npc[m_nIndex].m_OffX = 0;
		Npc[m_nIndex].m_OffY = 0;
		printf("Xay ra loi tim kiem nguoi choi, tu dong xua loi !\n");
	}



	// Ñ°ÕÒÏàÓ¦ npc id µÄ player ±àºÅ
	int		nPlayer, nRegionNo, i;
	nPlayer = SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_Region[Npc[m_nIndex].m_RegionIndex].FindPlayer(pApplyTeamInfo->m_dwTarNpcID);
	if ( nPlayer >= 0)
		goto SendMsg;
	for (i = 0; i < 8; i++)
	{
		nRegionNo = SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_Region[Npc[m_nIndex].m_RegionIndex].m_nConnectRegion[i];
		if ( nRegionNo < 0)
			continue;
		nPlayer = SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_Region[nRegionNo].FindPlayer(pApplyTeamInfo->m_dwTarNpcID);
		if (nPlayer >= 0)
			break;
	}
	if (i >= 8)
		goto SendFalse;
	
SendMsg:
	// Ñ°ÕÒ¶ÓÎé
	KTeam	*pTeam;
	if ( !Player[nPlayer].m_cTeam.m_nFlag )
		goto SendFalse;
	pTeam = &g_Team[Player[nPlayer].m_cTeam.m_nID];
	if (pTeam->m_nCaptain < 0 || !pTeam->IsOpen())
		goto SendFalse;
	
	// ·¢ËÍ¶ÓÎéÏûÏ¢
	sTeamInfo.ProtocolType = (BYTE)s2c_teaminfo;
	sTeamInfo.m_nCaptain = Npc[Player[pTeam->m_nCaptain].m_nIndex].m_dwID;
	for (i = 0; i < MAX_TEAM_MEMBER; i++)
	{
		if (pTeam->m_nMember[i] < 0)
			sTeamInfo.m_nMember[i] = 0;
		else
			sTeamInfo.m_nMember[i] = Npc[Player[pTeam->m_nMember[i]].m_nIndex].m_dwID;
	}
	//	strcpy(sTeamInfo.m_szTeamName, pTeam->m_szName);
	
	// ·¢ËÍ
	g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sTeamInfo, sizeof(PLAYER_SEND_TEAM_INFO));
	return;
	
	// ·¢ËÍÊ§°ÜÍ¨Öª
SendFalse:
	sTeamInfoFalse.ProtocolType = s2c_teamapplyinfofalse;
	g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sTeamInfoFalse, sizeof(PLAYER_APPLY_TEAM_INFO_FALSE));
	return;
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£º·þÎñÆ÷Ïò¿Í»§¶Ë·¢ËÍ¶ÓÎéÐÅÏ¢
//-------------------------------------------------------------------------
void	KPlayer::SendSelfTeamInfo()
{
	if (m_cTeam.m_nFlag && m_cTeam.m_nID >= 0)	// ·¢ËÍ¶ÓÎéÐÅÏ¢
	{
		int		nNpcIndex;
		PLAYER_SEND_SELF_TEAM_INFO	sSelfInfo;
		sSelfInfo.ProtocolType = s2c_teamselfinfo;
		sSelfInfo.nTeamServerID = m_cTeam.m_nID;
		sSelfInfo.m_dwLeadExp = m_dwLeadExp;
		sSelfInfo.m_btState = g_Team[m_cTeam.m_nID].m_nState;
		
		// ¶Ó³¤
		nNpcIndex = Player[g_Team[m_cTeam.m_nID].m_nCaptain].m_nIndex;
		sSelfInfo.m_dwNpcID[0] = Npc[nNpcIndex].m_dwID;
		sSelfInfo.m_btLevel[0] = (DWORD)Npc[nNpcIndex].m_Level;
		strcpy(sSelfInfo.m_szNpcName[0], Npc[nNpcIndex].Name);
		// ¶ÓÔ±
		for (int i = 0; i < MAX_TEAM_MEMBER; i++)
		{
			if (g_Team[m_cTeam.m_nID].m_nMember[i] >= 0)
			{
				nNpcIndex = Player[g_Team[m_cTeam.m_nID].m_nMember[i]].m_nIndex;
				sSelfInfo.m_dwNpcID[i + 1] = Npc[nNpcIndex].m_dwID;
				sSelfInfo.m_btLevel[i + 1] = (DWORD)Npc[nNpcIndex].m_Level;
				strcpy(sSelfInfo.m_szNpcName[i + 1], Npc[nNpcIndex].Name);
			}
			else
			{
				sSelfInfo.m_dwNpcID[i + 1] = 0;
				sSelfInfo.m_btLevel[i + 1] = 0;
			}
		}
		// ·¢ËÍÏûÏ¢
		g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sSelfInfo, sizeof(PLAYER_SEND_SELF_TEAM_INFO));
	}
	else	// ·¢ËÍÀë¶ÓÐÅÏ¢
	{
		PLAYER_LEAVE_TEAM	sLeaveTeam;
		sLeaveTeam.ProtocolType = s2c_teamleave;
		sLeaveTeam.m_dwNpcID = Npc[m_nIndex].m_dwID;
		g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sLeaveTeam, sizeof(PLAYER_LEAVE_TEAM));
	}
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÊÕµ½¿Í»§¶ËÇëÇó´´½¨Ò»Ö§¶ÓÎé
//-------------------------------------------------------------------------
BOOL	KPlayer::CreateTeam(BYTE* pProtocol)
{
	if (this->CheckTrading())
		return FALSE;

	return m_cTeam.CreateTeam(this->m_nPlayerIndex, (PLAYER_APPLY_CREATE_TEAM*)pProtocol);
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÊÕµ½¿Í»§¶ËÇëÇó¿ª·Å¡¢¹Ø±Õ±¾player¶ÓÎéÊÇ·ñÔÊÐí¶ÓÔ±¼ÓÈë×´Ì¬
//-------------------------------------------------------------------------
BOOL	KPlayer::SetTeamState(BYTE* pProtocol)
{
	if (this->CheckTrading())
		return FALSE;
	if ( !m_cTeam.m_nFlag || m_cTeam.m_nFigure != TEAM_CAPTAIN || m_cTeam.m_nID < 0)
	{
		SendSelfTeamInfo();
		return FALSE;
	}
	
	PLAYER_TEAM_OPEN_CLOSE	*pTeamState = (PLAYER_TEAM_OPEN_CLOSE*)pProtocol;
	if (pTeamState->m_btOpenClose == 0)
	{
		g_Team[m_cTeam.m_nID].SetTeamClose();
	}
	else
	{
		if (m_cMenuState.m_nState == PLAYER_MENU_STATE_TRADEOPEN)
			m_cMenuState.SetState(m_nPlayerIndex, PLAYER_MENU_STATE_NORMAL);
		g_Team[m_cTeam.m_nID].SetTeamOpen();
	}
	
	return TRUE;
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÊÕµ½¿Í»§¶ËÇëÇó¼ÓÈëÒ»Ö§¶ÓÎé
//-------------------------------------------------------------------------
BOOL	KPlayer::S2CSendAddTeamInfo(BYTE* pProtocol)
{
	if (!m_cTeam.m_bCanTeamFlag)
	{
		SHOW_MSG_SYNC	sMsg;
		sMsg.ProtocolType = s2c_msgshow;
		sMsg.m_wMsgID = enumMSG_ID_CANNOT_ADD_TEAM;
		sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
		g_pServer->PackDataToClient(m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
		return FALSE;
	}
	
	PLAYER_APPLY_ADD_TEAM	*pAddTeam = (PLAYER_APPLY_ADD_TEAM*)pProtocol;

	if (Npc[m_nIndex].m_SubWorldIndex >= MAX_SUBWORLD || Npc[m_nIndex].m_SubWorldIndex < 0 || Npc[m_nIndex].m_RegionIndex >= SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_nTotalRegion || Npc[m_nIndex].m_RegionIndex < 0)
	{
		Npc[m_nIndex].m_SubWorldIndex = 0;
		Npc[m_nIndex].m_RegionIndex = 381;
		Npc[m_nIndex].m_MapX = 0;
		Npc[m_nIndex].m_MapY = 0;
		Npc[m_nIndex].m_OffX = 0;
		Npc[m_nIndex].m_OffY = 0;
		printf("Xay ra loi tim kiem nguoi choi, tu dong xua loi !\n");
	}
	
	// Ñ°ÕÒ¶Ó³¤ npc id µÄ player ±àºÅ
	int		nPlayer, nRegionNo, i;
	nPlayer = SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_Region[Npc[m_nIndex].m_RegionIndex].FindPlayer(pAddTeam->m_dwTarNpcID);
	if ( nPlayer >= 0)
		goto SendMsg;
	for (i = 0; i < 8; i++)
	{
		nRegionNo = SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_Region[Npc[m_nIndex].m_RegionIndex].m_nConnectRegion[i];
		if ( nRegionNo < 0)
			continue;
		nPlayer = SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_Region[nRegionNo].FindPlayer(pAddTeam->m_dwTarNpcID);
		if (nPlayer >= 0)
			break;
	}
	if (i >= 8)
		return FALSE;
	
SendMsg:
	if (Npc[Player[nPlayer].m_nIndex].m_Camp == camp_begin && Npc[m_nIndex].m_Camp != camp_begin)
		return FALSE;
	m_cTeam.m_nApplyCaptainID = pAddTeam->m_dwTarNpcID;
	PLAYER_APPLY_ADD_TEAM	sAddTeam;
	sAddTeam.ProtocolType = s2c_teamgetapply;
	sAddTeam.m_dwTarNpcID = Npc[m_nIndex].m_dwID;
	g_pServer->PackDataToClient(Player[nPlayer].m_nNetConnectIdx, (BYTE*)&sAddTeam, sizeof(PLAYER_APPLY_ADD_TEAM));
	return TRUE;
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£º¶Ó³¤Í¨Öª·þÎñÆ÷½ÓÊÜÄ³¸önpcÎª¶ÓÎé³ÉÔ±
//-------------------------------------------------------------------------
BOOL	KPlayer::AddTeamMember(BYTE* pProtocol)
{
	// ´íÎó¼ì²â(ÊÇ·ñ×é¶Ó¡¢ÊÇ·ñ¶Ó³¤¡¢¶ÓÎé¿ª·Å×´Ì¬¡¢¶ÓÔ±Êý¡¢Í³ÂÊÁ¦)
	if ( !m_cTeam.m_nFlag ||
		m_cTeam.m_nFigure != TEAM_CAPTAIN ||
		!g_Team[m_cTeam.m_nID].IsOpen() ||
		g_Team[m_cTeam.m_nID].m_nMemNum >= MAX_TEAM_MEMBER ||
		g_Team[m_cTeam.m_nID].m_nMemNum >= g_Team[m_cTeam.m_nID].CalcCaptainPower())
	{
		SendSelfTeamInfo();
		return FALSE;
	}
	
	// ´Ónpc id²éÕÒplayer
	int		nPlayer, i;
	PLAYER_ACCEPT_TEAM_MEMBER	*pAccept = (PLAYER_ACCEPT_TEAM_MEMBER*)pProtocol;
	nPlayer = FindAroundPlayer(pAccept->m_dwNpcID);
	if (nPlayer < 0)
		return FALSE;
	// ¶Ô·½Õý´¦ÓÚ²»ÄÜ×é¶Ó×´Ì¬
	if (!Player[nPlayer].m_cTeam.m_bCanTeamFlag)
	{
		SHOW_MSG_SYNC	sMsg;
		sMsg.ProtocolType = s2c_msgshow;
		sMsg.m_wMsgID = enumMSG_ID_TARGET_CANNOT_ADD_TEAM;
		sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
		g_pServer->PackDataToClient(m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
		return FALSE;
	}
	// ÒÑ¾­ÔÚ¶ÓÎéÖÐ
	if (Player[nPlayer].m_cTeam.m_nFlag)
		return FALSE;
	if (Player[nPlayer].m_cTeam.m_nApplyCaptainID != Npc[this->m_nIndex].m_dwID)
		return FALSE;
	// ¶ÓÎéÌí¼Ó³ÉÔ±
	if ( !g_Team[m_cTeam.m_nID].AddMember(nPlayer) )
		return FALSE;
	// ¸Ä±ä¶ÓÎé×´Ì¬
	
	// ±»½ÓÊÜ¶ÓÔ±¶ÓÎéÊý¾Ý´¦Àí
	Player[nPlayer].m_cTeam.Release();
	Player[nPlayer].m_cTeam.m_nFlag = 1;
	Player[nPlayer].m_cTeam.m_nFigure = TEAM_MEMBER;
	Player[nPlayer].m_cTeam.m_nID = m_cTeam.m_nID;
	Npc[Player[nPlayer].m_nIndex].SetCurrentCamp(Npc[m_nIndex].m_Camp);
	
	// ¸ø¸÷¸ö¶ÓÔ±·¢ÏûÏ¢
	PLAYER_TEAM_ADD_MEMBER	sAddMem;
	sAddMem.ProtocolType = s2c_teamaddmember;
	sAddMem.m_dwNpcID = Npc[Player[nPlayer].m_nIndex].m_dwID;
	sAddMem.m_btLevel = (DWORD)Npc[Player[nPlayer].m_nIndex].m_Level;
	strcpy(sAddMem.m_szName, Npc[Player[nPlayer].m_nIndex].Name);
	
	// ¸ø¶Ó³¤·¢ÏûÏ¢
	g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sAddMem, sizeof(PLAYER_TEAM_ADD_MEMBER));
	
	// ¸øÀÏ¶ÓÔ±·¢ÏûÏ¢
	for (i = 0; i < MAX_TEAM_MEMBER; i++)
	{
		if (g_Team[m_cTeam.m_nID].m_nMember[i] < 0 || g_Team[m_cTeam.m_nID].m_nMember[i] == nPlayer)
			continue;
		g_pServer->PackDataToClient(Player[g_Team[m_cTeam.m_nID].m_nMember[i]].m_nNetConnectIdx, (BYTE*)&sAddMem, sizeof(PLAYER_TEAM_ADD_MEMBER));
	}
	
	// ------------------------------------- ¸øÐÂ¶ÓÔ±·¢ÏûÏ¢ --------------------------
	int		nNpcIndex;
	PLAYER_SEND_SELF_TEAM_INFO	sSelfInfo;
	sSelfInfo.ProtocolType = s2c_teamselfinfo;
	sSelfInfo.nTeamServerID = m_cTeam.m_nID;
	sSelfInfo.m_dwLeadExp = Player[nPlayer].m_dwLeadExp;
	sSelfInfo.m_btState = g_Team[m_cTeam.m_nID].m_nState;
	// ¶Ó³¤Êý¾Ý
	nNpcIndex = m_nIndex;
	sSelfInfo.m_dwNpcID[0] = Npc[nNpcIndex].m_dwID;
	sSelfInfo.m_btLevel[0] = (DWORD)Npc[nNpcIndex].m_Level;
	strcpy(sSelfInfo.m_szNpcName[0], Npc[nNpcIndex].Name);
	// ¶ÓÔ±Êý¾Ý
	for (i = 0; i < MAX_TEAM_MEMBER; i++)
	{
		if (g_Team[m_cTeam.m_nID].m_nMember[i] >= 0)
		{
			nNpcIndex = Player[g_Team[m_cTeam.m_nID].m_nMember[i]].m_nIndex;
			sSelfInfo.m_dwNpcID[i + 1] = Npc[nNpcIndex].m_dwID;
			sSelfInfo.m_btLevel[i + 1] = (DWORD)Npc[nNpcIndex].m_Level;
			strcpy(sSelfInfo.m_szNpcName[i + 1], Npc[nNpcIndex].Name);
		}
		else
		{
			sSelfInfo.m_dwNpcID[i + 1] = 0;
			sSelfInfo.m_btLevel[i + 1] = 0;
		}
	}
	// ·¢ËÍÏûÏ¢
	g_pServer->PackDataToClient(Player[nPlayer].m_nNetConnectIdx, (BYTE*)&sSelfInfo, sizeof(PLAYER_SEND_SELF_TEAM_INFO));
	
	SHOW_MSG_SYNC	sMsg;
	sMsg.ProtocolType = s2c_msgshow;
	sMsg.m_wMsgID = enumMSG_ID_TEAM_SELF_ADD;
	sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1;
	sMsg.m_lpBuf = 0;
	g_pServer->PackDataToClient(Player[nPlayer].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
	
	// --------------------------------- ¸øÐÂ¶ÓÔ±·¢ÏûÏ¢ end --------------------------
	
	return TRUE;
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÊÕµ½¿Í»§¶Ë¶ÓÔ±Í¨ÖªÀë¿ª¶ÓÎé
//-------------------------------------------------------------------------
void	KPlayer::LeaveTeam(BYTE* pProtocol)
{
	if (!m_cTeam.m_nFlag)
		return;
	if (this->m_cTeam.m_nFigure == TEAM_CAPTAIN && g_Team[m_cTeam.m_nID].IsOpen())
	{
		g_Team[m_cTeam.m_nID].SetTeamClose();
	}
	
	// ·Ç¶Ó³¤Àë¿ª¶ÓÎé£¬·¢ÏµÍ³ÏûÏ¢£¬¶Ó³¤Àë¿ª¶ÓÎéµÄÏµÍ³ÏûÏ¢´¦ÀíÔÚDeleteMemberÀïÃæ
	if (m_cTeam.m_nFigure != TEAM_CAPTAIN)
	{
		SHOW_MSG_SYNC	sMsg;
		sMsg.ProtocolType = s2c_msgshow;
		sMsg.m_wMsgID = enumMSG_ID_TEAM_LEAVE;
		sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1;
		sMsg.m_lpBuf = (void*)Npc[this->m_nIndex].m_dwID;
		
		g_pServer->PackDataToClient(Player[g_Team[m_cTeam.m_nID].m_nCaptain].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
		for (int i = 0; i < MAX_TEAM_MEMBER; i++)
		{
			if (g_Team[m_cTeam.m_nID].m_nMember[i] > 0)
			{
				g_pServer->PackDataToClient(Player[g_Team[m_cTeam.m_nID].m_nMember[i]].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
			}
		}
		sMsg.m_lpBuf = 0;
	}
	
	// ·þÎñÆ÷¶Ë¶ÓÔ±Àë¿ªµÄ´¦Àí
	g_Team[m_cTeam.m_nID].DeleteMember(GetPlayerIndex());
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÊÕµ½¿Í»§¶Ë¶Ó³¤Í¨ÖªÌß³öÄ³¸ö¶ÓÔ±
//-------------------------------------------------------------------------
void	KPlayer::TeamKickOne(BYTE* pProtocol)
{
	if ( !m_cTeam.m_nFlag || m_cTeam.m_nFigure != TEAM_CAPTAIN)
		return;
	
	int		nMemNo;
	PLAYER_TEAM_KICK_MEMBER	*pKickOne = (PLAYER_TEAM_KICK_MEMBER*)pProtocol;
	nMemNo = g_Team[m_cTeam.m_nID].FindMemberID(pKickOne->m_dwNpcID);
	if (nMemNo < 0)
		return;
	int		nPlayerNo;
	// ·þÎñÆ÷¶Ë¶ÓÔ±Àë¿ªµÄ´¦Àí
	nPlayerNo = g_Team[m_cTeam.m_nID].m_nMember[nMemNo];
	g_Team[m_cTeam.m_nID].DeleteMember(nPlayerNo);
	
	// ·¢ÏûÏ¢¸ø¿Í»§¶Ë
	int		nLength;
	SHOW_MSG_SYNC	sMsg;
	
	nLength = strlen(Npc[Player[nPlayerNo].m_nIndex].Name);
	sMsg.ProtocolType = s2c_msgshow;
	sMsg.m_wMsgID = enumMSG_ID_TEAM_KICK_One;
	sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID) + nLength;
	sMsg.m_lpBuf = new BYTE[sMsg.m_wLength + 1];
	
	memcpy(sMsg.m_lpBuf, &sMsg, sizeof(SHOW_MSG_SYNC) - sizeof(LPVOID));
	memcpy((char*)sMsg.m_lpBuf + sizeof(SHOW_MSG_SYNC) - sizeof(LPVOID), Npc[Player[nPlayerNo].m_nIndex].Name, nLength);
	
	g_pServer->PackDataToClient(Player[nPlayerNo].m_nNetConnectIdx, sMsg.m_lpBuf, sMsg.m_wLength + 1);
	
	g_pServer->PackDataToClient(Player[g_Team[m_cTeam.m_nID].m_nCaptain].m_nNetConnectIdx, sMsg.m_lpBuf, sMsg.m_wLength + 1);
	for (int i = 0; i < MAX_TEAM_MEMBER; i++)
	{
		if (g_Team[m_cTeam.m_nID].m_nMember[i] > 0)
			g_pServer->PackDataToClient(Player[g_Team[m_cTeam.m_nID].m_nMember[i]].m_nNetConnectIdx, sMsg.m_lpBuf, sMsg.m_wLength + 1);
	}
	sMsg.Release();
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÊÕµ½¿Í»§¶Ë¶Ó³¤Í¨Öª°Ñ¶Ó³¤Éí·Ý½»¸øÄ³¸ö¶ÓÔ±
//-------------------------------------------------------------------------
void	KPlayer::TeamChangeCaptain(BYTE* pProtocol)
{
	if ( !m_cTeam.m_nFlag || m_cTeam.m_nFigure != TEAM_CAPTAIN)
		return;
	int		i, nMember, nPlayerNo, nIsOpen = 0;
	PLAYER_APPLY_TEAM_CHANGE_CAPTAIN	*pChange = (PLAYER_APPLY_TEAM_CHANGE_CAPTAIN*)pProtocol;
	nMember = g_Team[m_cTeam.m_nID].FindMemberID(pChange->m_dwNpcID);
	if (nMember < 0)
		return;
	if (Npc[Player[g_Team[m_cTeam.m_nID].m_nMember[nMember]].m_nIndex].m_Camp == camp_begin && Npc[this->m_nIndex].m_Camp != camp_begin)
	{
		// ¶Ó³¤ÒÆ½»Ê§°Ü£º¶Ô·½Í³Ë§Á¦²»¹»
		SHOW_MSG_SYNC	sMsg;
		sMsg.ProtocolType = s2c_msgshow;
		sMsg.m_wMsgID = enumMSG_ID_TEAM_CHANGE_CAPTAIN_FAIL2;
		sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1;
		sMsg.m_lpBuf = (LPVOID)pChange->m_dwNpcID;
		g_pServer->PackDataToClient(m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
		sMsg.m_lpBuf = 0;
		return;
	}
	if (g_Team[m_cTeam.m_nID].m_nMemNum > PlayerSet.m_cLeadExp.GetMemNumFromLevel(Player[g_Team[m_cTeam.m_nID].m_nMember[nMember]].m_dwLeadLevel))
	{
		// ¶Ó³¤ÒÆ½»Ê§°Ü£º¶Ô·½Í³Ë§Á¦²»¹»
		SHOW_MSG_SYNC	sMsg;
		sMsg.ProtocolType = s2c_msgshow;
		sMsg.m_wMsgID = enumMSG_ID_TEAM_CHANGE_CAPTAIN_FAIL;
		sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1;
		sMsg.m_lpBuf = (LPVOID)pChange->m_dwNpcID;
		g_pServer->PackDataToClient(m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
		sMsg.m_lpBuf = 0;
		return;
	}
	if (g_Team[m_cTeam.m_nID].IsOpen())
	{
		g_Team[m_cTeam.m_nID].SetTeamClose();
		nIsOpen = 1;
	}
	nPlayerNo = g_Team[m_cTeam.m_nID].m_nCaptain;
	g_Team[m_cTeam.m_nID].m_nCaptain = g_Team[m_cTeam.m_nID].m_nMember[nMember];
	g_Team[m_cTeam.m_nID].m_nMember[nMember] = nPlayerNo;
	m_cTeam.m_nFigure = TEAM_MEMBER;
	Player[g_Team[m_cTeam.m_nID].m_nCaptain].m_cTeam.m_nFigure = TEAM_CAPTAIN;
	// ¸Ä±äÕóÓª
	Npc[Player[g_Team[m_cTeam.m_nID].m_nCaptain].m_nIndex].RestoreCurrentCamp();
	for (i = 0; i < MAX_TEAM_MEMBER; i++)
	{
		nPlayerNo = g_Team[m_cTeam.m_nID].m_nMember[i];
		if (nPlayerNo < 0)
			continue;
		Npc[Player[nPlayerNo].m_nIndex].SetCurrentCamp(Npc[Player[g_Team[m_cTeam.m_nID].m_nCaptain].m_nIndex].m_Camp);
	}
	
	// ¸ø¸÷¸ö¶ÓÔ±·¢ÏûÏ¢
	PLAYER_TEAM_CHANGE_CAPTAIN	sChangeCaptain;
	sChangeCaptain.ProtocolType = s2c_teamchangecaptain;
	sChangeCaptain.m_dwCaptainID = pChange->m_dwNpcID;
	sChangeCaptain.m_dwMemberID = Npc[m_nIndex].m_dwID;
	// ¸ø¶Ó³¤·¢ÏûÏ¢
	g_pServer->PackDataToClient(Player[g_Team[m_cTeam.m_nID].m_nCaptain].m_nNetConnectIdx, (BYTE*)&sChangeCaptain, sizeof(PLAYER_TEAM_CHANGE_CAPTAIN));
	// ¸ø¶ÓÔ±·¢ÏûÏ¢
	for (i = 0; i < MAX_TEAM_MEMBER; i++)
	{
		nPlayerNo = g_Team[m_cTeam.m_nID].m_nMember[i];
		if (nPlayerNo < 0)
			continue;
		g_pServer->PackDataToClient(Player[nPlayerNo].m_nNetConnectIdx, (BYTE*)&sChangeCaptain, sizeof(PLAYER_TEAM_CHANGE_CAPTAIN));
	}
	
	if (nIsOpen)
	{
		g_Team[m_cTeam.m_nID].SetTeamOpen();
	}
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÊÕµ½¿Í»§¶Ë¶Ó³¤ÇëÇó½âÉ¢¶ÓÎé
//-------------------------------------------------------------------------
void	KPlayer::TeamDismiss(BYTE* pProtocol)
{
	if ( !m_cTeam.m_nFlag || m_cTeam.m_nFigure != TEAM_CAPTAIN)
		return;
	
	PLAYER_LEAVE_TEAM	sLeaveTeam;
	int		i;
	SHOW_MSG_SYNC	sMsg;
	
	// ¶ÓÎé½âÉ¢ÏûÏ¢
	sMsg.ProtocolType = s2c_msgshow;
	sMsg.m_wMsgID = enumMSG_ID_TEAM_DISMISS;
	sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
	g_pServer->PackDataToClient(m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
	
	// ¸ø¶Ó³¤·¢ÏûÏ¢
	sLeaveTeam.ProtocolType = s2c_teamleave;
	sLeaveTeam.m_dwNpcID = Npc[m_nIndex].m_dwID;
	g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sLeaveTeam, sizeof(PLAYER_LEAVE_TEAM));
	// ¸ø¶ÓÔ±·¢ÏûÏ¢
	for (i = 0; i < MAX_TEAM_MEMBER; i++)
	{
		g_pServer->PackDataToClient(Player[g_Team[m_cTeam.m_nID].m_nMember[i]].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
		
		sLeaveTeam.m_dwNpcID = Npc[Player[g_Team[m_cTeam.m_nID].m_nMember[i]].m_nIndex].m_dwID;
		g_pServer->PackDataToClient(Player[g_Team[m_cTeam.m_nID].m_nMember[i]].m_nNetConnectIdx, (BYTE*)&sLeaveTeam, sizeof(PLAYER_LEAVE_TEAM));
		// ¶ÓÔ±Éí·Ý¸Ä±ä
		Player[g_Team[m_cTeam.m_nID].m_nMember[i]].m_cTeam.m_nFlag = 0;
		Npc[Player[g_Team[m_cTeam.m_nID].m_nMember[i]].m_nIndex].RestoreCurrentCamp();;
	}
	// ¶ÓÎéÇå¿Õ
	g_Team[m_cTeam.m_nID].Release();
	// ¶Ó³¤Éí·Ý¸Ä±ä
	m_cTeam.m_nFlag = 0;
	Npc[m_nIndex].RestoreCurrentCamp();
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÊÕµ½¿Í»§¶ËÇëÇóÉè¶¨PK×´Ì¬
//-------------------------------------------------------------------------
void	KPlayer::SetPK(BYTE* pProtocol)
{
	PLAYER_SET_PK	*pPK = (PLAYER_SET_PK*)pProtocol;
	
	if (pPK->m_btPKFlag)
		Npc[m_nIndex].SetCurrentCamp(camp_free);
	else
		Npc[m_nIndex].RestoreCurrentCamp();
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÊÕµ½¿Í»§¶ËÇëÇó»ñµÃÃÅÅÉÊý¾Ý
//-------------------------------------------------------------------------
void	KPlayer::SendFactionData(BYTE* pProtocol)
{
	SendFactionData();
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÏò¿Í»§¶Ë·¢ËÍÃÅÅÉÊý¾Ý
//-------------------------------------------------------------------------
void	KPlayer::SendFactionData()
{
	// ¸ø¿Í»§¶Ë·¢ÏûÏ¢
	PLAYER_FACTION_DATA	sData;
	sData.ProtocolType = s2c_playerfactiondata;
	sData.m_btCamp = Npc[m_nIndex].m_Camp;
	sData.m_btCurFaction = m_cFaction.m_nCurFaction;
	sData.m_btFirstFaction = m_cFaction.m_nFirstAddFaction;
	sData.m_nAddTimes = m_cFaction.m_nAddTimes;
	
	g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sData, sizeof(PLAYER_FACTION_DATA));
}
#endif

#ifdef _SERVER
// not end
void KPlayer::GetAboutPos(KMapPos *pMapPos)
{
	if (m_nIndex <= 0)
		return;
	
	if (Npc[m_nIndex].m_SubWorldIndex < 0)
		return;
	
	POINT Pos;
	int nX, nY;
	Npc[m_nIndex].GetMpsPos(&nX, &nY);
	Pos.x = nX;
	Pos.y = nY;
	
	SubWorld[Npc[m_nIndex].m_SubWorldIndex].GetFreeObjPos(Pos);
	
	pMapPos->nSubWorld = Npc[m_nIndex].m_SubWorldIndex;
	SubWorld[Npc[m_nIndex].m_SubWorldIndex].Mps2Map(
		Pos.x, 
		Pos.y, 
		&pMapPos->nRegion, 
		&pMapPos->nMapX, 
		&pMapPos->nMapY, 
		&pMapPos->nOffX, 
		&pMapPos->nOffY);
}

/*
void	KPlayer::GetAboutPos(KMapPos *pMapPos)
{
POINT	Pos[8] = 
{
{0, 32}, {-32, 32}, {-32, 0}, {-32, -32},
{0, -32}, {32, -32}, {32, 0}, {32, 32},
};

  int nMpsX, nMpsY, nTmpX, nTmpY;
  int nR, nMapX, nMapY, nOffX, nOffY;
  Npc[m_nIndex].GetMpsPos(&nMpsX, &nMpsY);
  
	for (int i = 0; i < 8; i++)
	{
	nTmpX = nMpsX + Pos[i].x;
	nTmpY = nMpsY + Pos[i].y;
	if (SubWorld[Npc[m_nIndex].m_SubWorldIndex].GetBarrier(nTmpX, nTmpY))
	continue;
	SubWorld[Npc[m_nIndex].m_SubWorldIndex].Mps2Map(nTmpX, nTmpY, &nR, &nMapX, &nMapY, &nOffX, &nOffY);
	if (nR == -1)
	continue;
	if (SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_Region[nR].GetRef(nMapX, nMapY, obj_object))
	continue;
	else
	break;
	}
	
	  if (i == 8)
	  {
	  pMapPos->nSubWorld = Npc[m_nIndex].m_SubWorldIndex;
	  pMapPos->nRegion = Npc[m_nIndex].m_RegionIndex;
	  pMapPos->nMapX = Npc[m_nIndex].m_MapX;
	  pMapPos->nMapY = Npc[m_nIndex].m_MapY;
	  pMapPos->nOffX = Npc[m_nIndex].m_OffX;
	  pMapPos->nOffY = Npc[m_nIndex].m_OffY;
	  }
	  else
	  {
	  pMapPos->nSubWorld = Npc[m_nIndex].m_SubWorldIndex;
	  pMapPos->nRegion = nR;
	  pMapPos->nMapX = nMapX;
	  pMapPos->nMapY = nMapY;
	  pMapPos->nOffX = nOffX;
	  pMapPos->nOffY = nOffY;
	  }
}*/
#endif


#ifdef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÑ°ÕÒÍæ¼ÒÖÜÎ§µÄÄ³¸öÖ¸¶¨npc idµÄplayer index
//-------------------------------------------------------------------------
int		KPlayer::FindAroundPlayer(DWORD dwNpcID)
{
	if (dwNpcID == 0)
		return -1;

	if (SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_SubWorldID == 59)
	{
		return -1;
	}
	if (Npc[m_nIndex].m_SubWorldIndex >= MAX_SUBWORLD || Npc[m_nIndex].m_SubWorldIndex < 0 || Npc[m_nIndex].m_RegionIndex >= SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_nTotalRegion || Npc[m_nIndex].m_RegionIndex < 0)
	{
		Npc[m_nIndex].m_SubWorldIndex = 0;
		Npc[m_nIndex].m_RegionIndex = 381;
		Npc[m_nIndex].m_MapX = 0;
		Npc[m_nIndex].m_MapY = 0;
		Npc[m_nIndex].m_OffX = 0;
		Npc[m_nIndex].m_OffY = 0;
		printf("Xay ra loi tim kiem nguoi choi, tu dong xua loi !\n");
	}
	
	int		nPlayer, nRegionNo, i;
	nPlayer = SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_Region[Npc[m_nIndex].m_RegionIndex].FindPlayer(dwNpcID);
	if ( nPlayer >= 0)
		return nPlayer;
	for (i = 0; i < 8; i++)
	{
		nRegionNo = SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_Region[Npc[m_nIndex].m_RegionIndex].m_nConnectRegion[i];
		if ( nRegionNo < 0)
			continue;
		nPlayer = SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_Region[nRegionNo].FindPlayer(dwNpcID);
		if (nPlayer >= 0)
			return nPlayer;
	}
	
	return -1;
}

//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÅÐ¶ÏÄ³Íæ¼ÒÊÇ·ñÔÚÖÜÎ§
//-------------------------------------------------------------------------
BOOL	KPlayer::CheckPlayerAround(int nPlayerIdx)
{
	if (nPlayerIdx <= 0)
		return FALSE;
	if (SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_Region[Npc[m_nIndex].m_RegionIndex].CheckPlayerIn(nPlayerIdx))
		return TRUE;
	int		nRegionNo;
	for (int i = 0; i < 8; i++)
	{
		nRegionNo = SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_Region[Npc[m_nIndex].m_RegionIndex].m_nConnectRegion[i];
		if ( nRegionNo < 0)
			continue;
		if (SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_Region[nRegionNo].CheckPlayerIn(nPlayerIdx))
			return TRUE;
	}
	
	return FALSE;
}

//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÑ°ÕÒÍæ¼ÒÖÜÎ§µÄÄ³¸öÖ¸¶¨npc idµÄnpc index
//-------------------------------------------------------------------------
int		KPlayer::FindAroundNpc(DWORD dwNpcID)
{
	if (dwNpcID == 0)
		return 0;

	if (Npc[m_nIndex].m_SubWorldIndex >= MAX_SUBWORLD || Npc[m_nIndex].m_SubWorldIndex < 0 || Npc[m_nIndex].m_RegionIndex >= SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_nTotalRegion || Npc[m_nIndex].m_RegionIndex < 0)
	{
		Npc[m_nIndex].m_SubWorldIndex = 0;
		Npc[m_nIndex].m_RegionIndex = 381;
		Npc[m_nIndex].m_MapX = 0;
		Npc[m_nIndex].m_MapY = 0;
		Npc[m_nIndex].m_OffX = 0;
		Npc[m_nIndex].m_OffY = 0;
		printf("Xay ra loi tim kiem npc, tu dong xua loi !\n");
	}


	int		nNpc, nRegionNo, i;
	nNpc = SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_Region[Npc[m_nIndex].m_RegionIndex].SearchNpc(dwNpcID);
	if ( nNpc > 0)
		return nNpc;
	for (i = 0; i < 8; i++)
	{
		nRegionNo = SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_Region[Npc[m_nIndex].m_RegionIndex].m_nConnectRegion[i];
		if ( nRegionNo < 0)
			continue;
		nNpc = SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_Region[nRegionNo].SearchNpc(dwNpcID);
		if (nNpc > 0)
			return nNpc;
	}
	
	return 0;
}
#endif

//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÐÂÍæ¼ÒµÇÂ½Ê±¸ù¾ÝÎåÐÐÊôÐÔ²úÉú Á¦Á¿ Ãô½Ý »îÁ¦ ¾«Á¦ ËÄÏîÊýÖµ
//-------------------------------------------------------------------------
BOOL	KPlayer::NewPlayerGetBaseAttribute(int Series)
{
	if (Series < series_metal || Series > series_earth)
		return FALSE;
	
	Npc[m_nIndex].SetSeries(Series);
	
	m_nStrength = PlayerSet.m_cNewPlayerAttribute.m_nStrength[Series];
	m_nDexterity = PlayerSet.m_cNewPlayerAttribute.m_nDexterity[Series];
	m_nVitality = PlayerSet.m_cNewPlayerAttribute.m_nVitality[Series];
	m_nEngergy = PlayerSet.m_cNewPlayerAttribute.m_nEngergy[Series];
	m_nLucky = PlayerSet.m_cNewPlayerAttribute.m_nLucky[Series];
	
	m_nCurStrength = m_nStrength;
	m_nCurDexterity = m_nDexterity;
	m_nCurVitality = m_nVitality;
	m_nCurEngergy = m_nEngergy;
	CalcCurLucky();
	
	m_cFaction.SetSeries(Series);
	
	return TRUE;
}

//-------------------------------------------------------------------------
//	¹¦ÄÜ£º¼ÆËãµ±Ç°Á¦Á¿
//-------------------------------------------------------------------------
//void	KPlayer::CalcCurStrength()
//{	// »¹ÐèÒª¿¼ÂÇ ×°±¸¡¢¼¼ÄÜ¡¢×´Ì¬ µÄÓ°Ïì
//	m_nCurStrength = m_nStrength;
//}

//-------------------------------------------------------------------------
//	¹¦ÄÜ£º¼ÆËãµ±Ç°Ãô½Ý
//-------------------------------------------------------------------------
//void	KPlayer::CalcCurDexterity()
//{	// »¹ÐèÒª¿¼ÂÇ ×°±¸¡¢¼¼ÄÜ¡¢×´Ì¬ µÄÓ°Ïì
//	m_nCurDexterity = m_nDexterity;
//}

//-------------------------------------------------------------------------
//	¹¦ÄÜ£º¼ÆËãµ±Ç°»îÁ¦
//-------------------------------------------------------------------------
//void	KPlayer::CalcCurVitality()
//{	// »¹ÐèÒª¿¼ÂÇ ×°±¸¡¢¼¼ÄÜ¡¢×´Ì¬ µÄÓ°Ïì
//	m_nCurVitality = m_nVitality;
//}

//-------------------------------------------------------------------------
//	¹¦ÄÜ£º¼ÆËãµ±Ç°¾«Á¦
//-------------------------------------------------------------------------
//void	KPlayer::CalcCurEngergy()
//{	// »¹ÐèÒª¿¼ÂÇ ×°±¸¡¢¼¼ÄÜ¡¢×´Ì¬ µÄÓ°Ïì
//	m_nCurEngergy = m_nEngergy;
//}

//-------------------------------------------------------------------------
//	¹¦ÄÜ£º¼ÆËãµ±Ç°ÔËÆø
//-------------------------------------------------------------------------
void	KPlayer::CalcCurLucky()
{	// »¹ÐèÒª¿¼ÂÇ ×°±¸¡¢¼¼ÄÜ¡¢×´Ì¬ µÄÓ°Ïì
	m_nCurLucky = m_nLucky;
}

#ifdef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÔö¼Ó»ù±¾Á¦Á¿
//-------------------------------------------------------------------------
void	KPlayer::AddBaseStrength(int nData)
{
	if (nData > 0 && nData <= m_nAttributePoint)
	{
		m_nStrength += nData;
		m_nAttributePoint -= nData;
		m_nCurStrength += nData;
		
		UpdataCurData();
		SetNpcPhysicsDamage();
		// °Ñµ±Ç°Á¦Á¿´«¸ø¿Í»§¶Ë
		PLAYER_ATTRIBUTE_SYNC	sSync;
		sSync.ProtocolType = s2c_playersyncattribute;
		sSync.m_btAttribute = 0;
		sSync.m_nBasePoint = m_nStrength;
		sSync.m_nCurPoint = m_nCurStrength;
		sSync.m_nLeavePoint = m_nAttributePoint;
		g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sSync, sizeof(PLAYER_ATTRIBUTE_SYNC));
	}
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÔö¼Ó»ù±¾Ãô½Ý
//-------------------------------------------------------------------------
void	KPlayer::AddBaseDexterity(int nData)
{
	if (nData > 0 && nData <= m_nAttributePoint)
	{
		m_nDexterity += nData;
		m_nAttributePoint -= nData;
		m_nCurDexterity += nData;
		
		SetNpcAttackRating();
		SetNpcDefence();
		UpdataCurData();
		SetNpcPhysicsDamage();
		
		// °Ñµ±Ç°Á¦Á¿´«¸ø¿Í»§¶Ë
		PLAYER_ATTRIBUTE_SYNC	sSync;
		sSync.ProtocolType = s2c_playersyncattribute;
		sSync.m_btAttribute = 1;
		sSync.m_nBasePoint = m_nDexterity;
		sSync.m_nCurPoint = m_nCurDexterity;
		sSync.m_nLeavePoint = m_nAttributePoint;
		g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sSync, sizeof(PLAYER_ATTRIBUTE_SYNC));
	}
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÔö¼Ó»ù±¾»îÁ¦
//-------------------------------------------------------------------------
void	KPlayer::AddBaseVitality(int nData)
{
	if (nData > 0 && nData <= m_nAttributePoint)
	{
		m_nVitality += nData;
		m_nAttributePoint -= nData;
		m_nCurVitality += nData;
		
		Npc[m_nIndex].AddBaseLifeMax(PlayerSet.m_cLevelAdd.GetLifePerVitality(Npc[m_nIndex].m_Series) * nData);
		Npc[m_nIndex].AddBaseStaminaMax(PlayerSet.m_cLevelAdd.GetStaminaPerVitality(Npc[m_nIndex].m_Series) * nData);
		UpdataCurData();
		
		// °Ñµ±Ç°Á¦Á¿´«¸ø¿Í»§¶Ë
		PLAYER_ATTRIBUTE_SYNC	sSync;
		sSync.ProtocolType = s2c_playersyncattribute;
		sSync.m_btAttribute = 2;
		sSync.m_nBasePoint = m_nVitality;
		sSync.m_nCurPoint = m_nCurVitality;
		sSync.m_nLeavePoint = m_nAttributePoint;
		g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sSync, sizeof(PLAYER_ATTRIBUTE_SYNC));
	}
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÔö¼Ó»ù±¾¾«Á¦
//-------------------------------------------------------------------------
void	KPlayer::AddBaseEngergy(int nData)
{
	if (nData > 0 && nData <= m_nAttributePoint)
	{
		m_nEngergy += nData;
		m_nAttributePoint -= nData;
		m_nCurEngergy += nData;
		
		Npc[m_nIndex].AddBaseManaMax(PlayerSet.m_cLevelAdd.GetManaPerEnergy(Npc[m_nIndex].m_Series) * nData);
		UpdataCurData();
		
		// °Ñµ±Ç°Á¦Á¿´«¸ø¿Í»§¶Ë
		PLAYER_ATTRIBUTE_SYNC	sSync;
		sSync.ProtocolType = s2c_playersyncattribute;
		sSync.m_btAttribute = 3;
		sSync.m_nBasePoint = m_nEngergy;
		sSync.m_nCurPoint = m_nCurEngergy;
		sSync.m_nLeavePoint = m_nAttributePoint;
		g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sSync, sizeof(PLAYER_ATTRIBUTE_SYNC));
	}
}
#endif

//-------------------------------------------------------------------------
//	¹¦ÄÜ£º¸Ä±äµ±Ç°Á¦Á¿(µ± nData Ð¡ÓÚ 0 Ê±£¬¼õÉÙ)
//-------------------------------------------------------------------------
void	KPlayer::ChangeCurStrength(int nData)
{
/*	if (m_nCurStrength + nData < m_nStrength)
return;
	*/
	m_nCurStrength += nData;
	SetNpcPhysicsDamage();
	// °Ñµ±Ç°Á¦Á¿´«¸ø¿Í»§¶Ë
}

//-------------------------------------------------------------------------
//	¹¦ÄÜ£º¸Ä±äµ±Ç°Ãô½Ý(µ± nData Ð¡ÓÚ 0 Ê±£¬¼õÉÙ)
//-------------------------------------------------------------------------
void	KPlayer::ChangeCurDexterity(int nData)
{
/*	if (m_nCurDexterity + nData < 0)
return;
	*/
	m_nCurDexterity += nData;
	
	int nRating = nData * 4;
	Npc[m_nIndex].m_CurrentAttackRating += nRating;
	
	int nDefence = nData / 4;
	Npc[m_nIndex].m_CurrentDefend += nDefence;
	
	SetNpcPhysicsDamage();
	
}


//-------------------------------------------------------------------------
//	¹¦ÄÜ£º¸Ä±äµ±Ç°»îÁ¦(µ± nData Ð¡ÓÚ 0 Ê±£¬¼õÉÙ)
//-------------------------------------------------------------------------
void	KPlayer::ChangeCurVitality(int nData)
{
/*	if (nData + m_nAttributePoint < 0)
return;
	*/
	m_nCurVitality += nData;



	
	Npc[m_nIndex].AddCurLifeMax(PlayerSet.m_cLevelAdd.GetLifePerVitality(Npc[m_nIndex].m_Series) * nData);
	Npc[m_nIndex].AddCurStaminaMax(PlayerSet.m_cLevelAdd.GetStaminaPerVitality(Npc[m_nIndex].m_Series) * nData);
	
}

//-------------------------------------------------------------------------
//	¹¦ÄÜ£º¸Ä±äµ±Ç°¾«Á¦(µ± nData Ð¡ÓÚ 0 Ê±£¬¼õÉÙ)
//-------------------------------------------------------------------------
void	KPlayer::ChangeCurEngergy(int nData)
{
/*	if (m_nAttributePoint + nData < 0)
return;
	*/
	m_nCurEngergy += nData;
	
	Npc[m_nIndex].AddCurManaMax(PlayerSet.m_cLevelAdd.GetManaPerEnergy(Npc[m_nIndex].m_Series) * nData);
	
}

//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÔö¼Ó»ù±¾ÔËÆø
//-------------------------------------------------------------------------
void	KPlayer::AddBaseLucky(int nData)
{
	m_nLucky += nData;
	CalcCurLucky();
}

//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÓÉµ±Ç°Á¦Á¿¼ÆËã¶ÔÓ¦npcµÄÎïÀíÉËº¦(PhysicsDamage)
//-------------------------------------------------------------------------
void	KPlayer::SetNpcPhysicsDamage()
{
	int		nMinDamage, nMaxDamage;
	m_ItemList.GetWeaponDamage(&nMinDamage, &nMaxDamage);
	
	if (m_ItemList.GetWeaponType() == equip_meleeweapon)	// (µ±Ç°×°±¸ÊÇ½üÉíÎäÆ÷)
	{
	/*		nMinDamage = nMinDamage * (m_nCurStrength + STRENGTH_SET_DAMAGE_VALUE) / STRENGTH_SET_DAMAGE_VALUE;
	nMaxDamage = nMaxDamage * (m_nCurStrength + STRENGTH_SET_DAMAGE_VALUE) / STRENGTH_SET_DAMAGE_VALUE;
		*/
		// ÊýÖµ¼ÆËã·½·¨ÐÞ¸Ä£¨ËïÓ¢ÒªÇó£© by Spe 03/06/11
		nMinDamage += m_nCurStrength / STRENGTH_SET_DAMAGE_VALUE;
		nMaxDamage += m_nCurStrength / STRENGTH_SET_DAMAGE_VALUE;
		Npc[m_nIndex].SetPhysicsDamage(nMinDamage, nMaxDamage);
	}
	else if (m_ItemList.GetWeaponType() == equip_rangeweapon)	// (µ±Ç°×°±¸ÊÇÔ¶³ÌÎäÆ÷)
	{
	/*		nMinDamage = nMinDamage * (m_nCurDexterity + DEXTERITY_SET_DAMAGE_VALUE) / DEXTERITY_SET_DAMAGE_VALUE;
	nMaxDamage = nMaxDamage * (m_nCurDexterity + DEXTERITY_SET_DAMAGE_VALUE) / DEXTERITY_SET_DAMAGE_VALUE;
		*/
		// ÊýÖµ¼ÆËã·½·¨ÐÞ¸Ä£¨ËïÓ¢ÒªÇó£© by Spe 03/06/11
		nMinDamage += m_nCurDexterity / DEXTERITY_SET_DAMAGE_VALUE;
		nMaxDamage += m_nCurDexterity / DEXTERITY_SET_DAMAGE_VALUE;
		Npc[m_nIndex].SetPhysicsDamage(nMinDamage, nMaxDamage);
	}
	else
	{
		Npc[m_nIndex].SetPhysicsDamage(nMinDamage, nMaxDamage);
	}
}

//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÓÉµ±Ç°Ãô½Ý¼ÆËã¶ÔÓ¦npcµÄ¹¥»÷ÃüÖÐÂÊ(AttackRating)
//-------------------------------------------------------------------------
void	KPlayer::SetNpcAttackRating()
{
	int		nRating;
	
	nRating = m_nDexterity * 4 - 28;
	Npc[m_nIndex].SetBaseAttackRating(nRating);
}

//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÓÉµ±Ç°Ãô½Ý¼ÆËã¶ÔÓ¦npcµÄ·ÀÓùÁ¦
//-------------------------------------------------------------------------
void	KPlayer::SetNpcDefence()
{
	int		nDefence;
	
	nDefence = m_nDexterity / 4;
	Npc[m_nIndex].SetBaseDefence(nDefence);
}

/*
#ifdef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÓÉµ±Ç°Ãô½Ý¼ÆËã¶ÔÓ¦npcµÄÐÐ×ßËÙ¶È
//-------------------------------------------------------------------------
void	KPlayer::SetNpcWalkSpeed()
{
int		nSpeed;
nSpeed = BASE_WALK_SPEED * (m_nCurDexterity + 320) / 320;
Npc[m_nIndex].SetBaseWalkSpeed(nSpeed);
}
#endif
*/

/*
#ifdef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÓÉµ±Ç°Ãô½Ý¼ÆËã¶ÔÓ¦npcµÄÅÜ²½ËÙ¶È
//-------------------------------------------------------------------------
void	KPlayer::SetNpcRunSpeed()
{
int		nSpeed;
nSpeed = BASE_RUN_SPEED * (m_nCurDexterity + 320) / 320;
Npc[m_nIndex].SetBaseRunSpeed(nSpeed);
}
#endif
*/

//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÔö¼Ó¾­Ñé(Ô­Ê¼Êý¾Ý£¬»¹Î´¾­¹ý´¦Àí£¬ÐèÒª´¦Àí×é¶ÓµÄÓ°Ïì)
//-------------------------------------------------------------------------
void	KPlayer::AddExp(int nExp, int nTarLevel,BOOL bCheck)
{



#ifndef _SERVER
	AddSelfExp(nExp, nTarLevel);
#endif
	
#ifdef _SERVER
	// Ã»ÓÐ×é¶Ó






	
	if ( !m_cTeam.m_nFlag )
	{
		AddSelfExp(nExp, nTarLevel);
		return;
	}

	

	// ÒÑ¾­×é¶Ó
	int		i, j, nShareFlag[MAX_TEAM_MEMBER + 1], nRegion, nTotalLevel = 0, nTotalPlayer = 0;
	memset(nShareFlag, 0, sizeof(nShareFlag));
	
	int		nMpsX, nMpsY, nNpcIdx, nSelfSubIdx, nSelfMpsX, nSelfMpsY;
	
	nSelfSubIdx = Npc[m_nIndex].m_SubWorldIndex;
	SubWorld[nSelfSubIdx].Map2Mps(
		Npc[m_nIndex].m_RegionIndex,
		Npc[m_nIndex].m_MapX,
		Npc[m_nIndex].m_MapY,
		Npc[m_nIndex].m_OffX,
		Npc[m_nIndex].m_OffY,
		&nSelfMpsX,
		&nSelfMpsY);
	
	// ¶Ó³¤
	nNpcIdx = Player[g_Team[m_cTeam.m_nID].m_nCaptain].m_nIndex;
	if (nSelfSubIdx == Npc[nNpcIdx].m_SubWorldIndex)
	{
		SubWorld[Npc[nNpcIdx].m_SubWorldIndex].Map2Mps(
			Npc[nNpcIdx].m_RegionIndex,
			Npc[nNpcIdx].m_MapX,
			Npc[nNpcIdx].m_MapY,
			Npc[nNpcIdx].m_OffX,
			Npc[nNpcIdx].m_OffY,
			&nMpsX,
			&nMpsY);
		if ( (nMpsX - nSelfMpsX) * (nMpsX - nSelfMpsX) + 
			(nMpsY - nSelfMpsY) * (nMpsY - nSelfMpsY) <= 
			PLAYER_SHARE_EXP_DISTANCE * PLAYER_SHARE_EXP_DISTANCE)
		{
			nTotalLevel += Npc[nNpcIdx].m_Level;
			nShareFlag[0] = 1;
			nTotalPlayer++;
			if (nTotalPlayer >= g_Team[m_cTeam.m_nID].m_nMemNum + 1)
				goto SharePlace;
		}
	}
	// ¶ÓÔ±
	for (i = 0; i < MAX_TEAM_MEMBER; i++)
	{
		if (g_Team[m_cTeam.m_nID].m_nMember[i] < 0)
			continue;
		nNpcIdx = Player[g_Team[m_cTeam.m_nID].m_nMember[i]].m_nIndex;
		if (nSelfSubIdx != Npc[nNpcIdx].m_SubWorldIndex)
			continue;
		SubWorld[Npc[nNpcIdx].m_SubWorldIndex].Map2Mps(
			Npc[nNpcIdx].m_RegionIndex,
			Npc[nNpcIdx].m_MapX,
			Npc[nNpcIdx].m_MapY,
			Npc[nNpcIdx].m_OffX,
			Npc[nNpcIdx].m_OffY,
			&nMpsX,
			&nMpsY);
		if ( (nMpsX - nSelfMpsX) * (nMpsX - nSelfMpsX) + 
			(nMpsY - nSelfMpsY) * (nMpsY - nSelfMpsY) > 
			PLAYER_SHARE_EXP_DISTANCE * PLAYER_SHARE_EXP_DISTANCE)
			continue;
		nTotalLevel += Npc[nNpcIdx].m_Level;
		nShareFlag[i + 1] = 1;
		nTotalPlayer++;
		if (nTotalPlayer >= g_Team[m_cTeam.m_nID].m_nMemNum + 1)
			goto SharePlace;
	}
	
SharePlace:
	_ASSERT(nTotalPlayer >= 1);
	_ASSERT(nTotalLevel >= 1);
	if (nTotalPlayer < 1 || nTotalLevel < 1)
		return;
	if (nTotalPlayer == 1)
	{
		AddSelfExp(nExp, nTarLevel);
		return;
	}
	int		k, nGetExp;
	k = 100 + (nTotalPlayer-1) * 15;
	
	int ShareExp;
	nTotalPlayer = g_Team[m_cTeam.m_nID].m_nMemNum + 1;
	
	if (nTotalPlayer == 2)
	ShareExp = 80;
	else if (nTotalPlayer == 3)
	ShareExp = 70;
	else if (nTotalPlayer == 4)
	ShareExp = 60;
	else if (nTotalPlayer == 5)
	ShareExp = 55;
	else if (nTotalPlayer == 6)
	ShareExp = 55;
	else if (nTotalPlayer == 7)
	ShareExp = 50;
	else if (nTotalPlayer == 8)
	ShareExp = 50;
	else
	ShareExp = 60;
	// ¸ø¶Ó³¤·ÖÅä¾­Ñé
	if (nShareFlag[0] == 1)
	{



		nGetExp = k * Npc[Player[g_Team[m_cTeam.m_nID].m_nCaptain].m_nIndex].m_Level / nTotalLevel;
		if (nGetExp < 5)
			nGetExp = 5;




		if (m_nPlayerIndex != g_Team[m_cTeam.m_nID].m_nCaptain || bCheck)
		{

		Player[g_Team[m_cTeam.m_nID].m_nCaptain].AddSelfExp(nExp * ShareExp / 100, nTarLevel);

		}

		else

		{

		Player[g_Team[m_cTeam.m_nID].m_nCaptain].AddSelfExp(nExp , nTarLevel);

		}

		// LeadExp = ¹ÖÎïÌá¹©µÄ¾­ÑéÖµ * ¦Â * ·ÖÏíÕßµÈ¼¶ / ·ÖÏíÕßµÈ¼¶ºÍ£»
		int		nLeadExp = nExp * k * PlayerSet.m_cLevelAdd.GetLeadExpShare(Npc[m_nIndex].m_Series) * (nTotalLevel - Npc[m_nIndex].m_Level) / (nTotalLevel * 10000);
		Player[g_Team[m_cTeam.m_nID].m_nCaptain].AddLeadExp(nLeadExp);

		//		if (g_Team[m_cTeam.m_nID].m_nCaptain == m_nPlayerIndex)
		//		{
		//			if (nGetExp < 50)
		//				nGetExp = 50;
		//			Player[g_Team[m_cTeam.m_nID].m_nCaptain].AddSelfExp(nExp * nGetExp / 100, nTarLevel);
		//		}
		//		else
		//		{
		//			if (nGetExp < 5)
		//				nGetExp = 5;
		//			Player[g_Team[m_cTeam.m_nID].m_nCaptain].AddSelfExp(nExp * nGetExp / 100, nTarLevel);
		//		}
	}
	// ¸ø¶ÓÔ±·ÖÅä¾­Ñé
	for (i = 0; i < MAX_TEAM_MEMBER; i++)
	{
		if (nShareFlag[i + 1] != 1)
			continue;


		


		nGetExp = k * Npc[Player[g_Team[m_cTeam.m_nID].m_nMember[i]].m_nIndex].m_Level / nTotalLevel;
		if (nGetExp < 5)
			nGetExp = 5;



		if (m_nPlayerIndex != g_Team[m_cTeam.m_nID].m_nMember[i] || bCheck)
		{

		Player[g_Team[m_cTeam.m_nID].m_nMember[i]].AddSelfExp(nExp * ShareExp / 100, nTarLevel);

		}

		else

		{

		Player[g_Team[m_cTeam.m_nID].m_nMember[i]].AddSelfExp(nExp, nTarLevel);

		}


		//		if (g_Team[m_cTeam.m_nID].m_nMember[i] == m_nPlayerIndex)
		//		{
		//			if (nGetExp < 50)
		//				nGetExp = 50;
		//			Player[g_Team[m_cTeam.m_nID].m_nMember[i]].AddSelfExp(nExp * nGetExp / 100, nTarLevel);
		//		}
		//		else
		//		{
		//			if (nGetExp < 5)
		//				nGetExp = 5;
		//			Player[g_Team[m_cTeam.m_nID].m_nMember[i]].AddSelfExp(nExp * nGetExp / 100, nTarLevel);
		//		}
	}
	
#endif
}

//-------------------------------------------------------------------------

void	KPlayer::AddSelfExp(int nExp, int nTarLevel)
{
	if (nExp <= 0)
		return;
	if (Npc[m_nIndex].m_Level >= MAX_LEVEL)
		return;
	if (Npc[m_nIndex].m_Doing == do_death || Npc[m_nIndex].m_Doing == do_revive)
		return;


//m_cTask.SetSaveVal(133,m_cTask.GetSaveVal(133) + nExp/100);
	
	
int     	nxExp;
int		nxExpTanThu;
//int		nxExpDuaTop;
int 		nxExpSuDo;
//int		nxExpServer;
//int 		nxExpLevel;
//int 		nxExpTLD;

SYSTEMTIME TimeXX;
GetLocalTime(&TimeXX);

nxExpSuDo = 1;
if (m_cTask.GetSaveVal(442) > 0)
{
	nxExpSuDo = 2;
}
else
{
	nxExpSuDo = 1;
}

nxExp = 1;
if (m_cTask.GetSaveVal(172) > 0)
{
	nxExp = 2;
}
else
{
	nxExp = 1;
}

nxExpTanThu = 1;
if (Npc[m_nIndex].m_Level < 50)
nxExpTanThu = 5;
else if (Npc[m_nIndex].m_Level < 80)
nxExpTanThu = 4;
else if (Npc[m_nIndex].m_Level < 90)
nxExpTanThu = 3;



	int		nSubLevel, nGetExp,nLevelPlayer;
	
    nLevelPlayer = Npc[m_nIndex].m_Level;
    if (nLevelPlayer > 99)
    nLevelPlayer = 99;

	nSubLevel = nLevelPlayer - nTarLevel;
	if (nSubLevel <= 5 && nSubLevel > (-5))
	{
		nGetExp = nExp;
	}
	else if (nSubLevel <= 15 && nSubLevel > (-15))
	{
		nGetExp = nExp/5;
	}
	else
	{
		nGetExp = nExp/20;
	}
	
	if (nGetExp <= 0)
		nGetExp = 1;

	// Hoa Son
	SYSTEMTIME TimeNow;
		GetLocalTime(&TimeNow);
		int Map = SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_SubWorldID;
/*		
// Exp Huyet Chien		
		if (Map == 21 && m_cTask.GetSaveVal(470) == 9999)
		m_nExp += nGetExp * nxExp * nxExpDuaTop * nxExpLevel * 27;
		else
		m_nExp += nGetExp * nxExp * nxExpDuaTop * nxExpLevel * 9;
*/
// Exp Tuyet Tinh Kiem

//if (Map == 111 || Map == 109 || Map == 110)
//	m_nExp += nGetExp * nxExp * 11 / 10;
//else

	//m_nExp += nGetExp * nxExp * nxExpTLD;
	m_nExp += nGetExp * nxExpTanThu * nxExp * nxExpSuDo; // Trong dua top

		
		//}
#ifdef _SERVER
//Bat Dau Luyen skill 120	

int nFaction = m_cFaction.GetCurFactionNo();
int nKillId = 0;
int nKillLevel = 0;

if (nFaction == 0)
nKillId = 412;
else if (nFaction == 1)
nKillId = 414;
else if (nFaction == 2)
nKillId = 416;
else if (nFaction == 3)
nKillId = 418;
else if (nFaction == 4)
nKillId = 420;
else if (nFaction == 5)
nKillId = 422;
else if (nFaction == 6)
nKillId = 424;
else if (nFaction == 7)
nKillId = 426;
else if (nFaction == 8)
nKillId = 428;
else if (nFaction == 9)
nKillId = 430;

if (nKillId > 0)
{



nKillLevel = Npc[m_nIndex].m_SkillList.GetCurrentLevel(nKillId);



int nLevelSkillMax = 0;

if (m_cTask.GetSaveVal(139) == 1)
{
nLevelSkillMax += 1;
}
else if (m_cTask.GetSaveVal(139) == 2)
{
nLevelSkillMax += 2;
}
else if (m_cTask.GetSaveVal(139) == 3)
{
nLevelSkillMax += 4;
}
else if (m_cTask.GetSaveVal(139) == 4)
{
nLevelSkillMax += 6;
}
else if (m_cTask.GetSaveVal(139) == 5)
{
nLevelSkillMax += 8;
}




if (nKillLevel > 0 && nKillLevel < (20+nLevelSkillMax))
{
KSkill *pOrdinSkill = (KSkill *)g_SkillManager.GetSkill(nKillId,nKillLevel);
if (pOrdinSkill)
{
KMagicAttrib*	pMagicData = NULL;
pMagicData = pOrdinSkill->GetDamageAttribs();
if (pMagicData)
{
if (pMagicData[14].nAttribType == magic_skilllevelexp && pMagicData[14].nValue[1] == 0 && pMagicData[14].nValue[2] > 0)
{

m_cTask.SetClearVal(pMagicData[14].nValue[2],m_cTask.GetClearVal(pMagicData[14].nValue[2])+nExp);
if (m_cTask.GetClearVal(pMagicData[14].nValue[2]) >= 80000)
{
	m_cTask.SetClearVal(pMagicData[14].nValue[2],m_cTask.GetClearVal(pMagicData[14].nValue[2])-80000);

		if (m_cTask.GetSaveVal(173) > 0)
		{
			m_cTask.SetSaveVal(pMagicData[14].nValue[2],m_cTask.GetSaveVal(pMagicData[14].nValue[2])+nxExp*4);
		}
		else
		{
			m_cTask.SetSaveVal(pMagicData[14].nValue[2],m_cTask.GetSaveVal(pMagicData[14].nValue[2])+nxExp);
		}

if (m_cTask.GetSaveVal(pMagicData[14].nValue[2]) >= pMagicData[14].nValue[0])
{

m_cTask.SetSaveVal(pMagicData[14].nValue[2],0);
Npc[m_nIndex].m_SkillList.IncreaseLevel(Npc[m_nIndex].m_SkillList.GetSkillIdx(nKillId),1);
PLAYER_SKILL_LEVEL_SYNC NewSkill;
NewSkill.ProtocolType = s2c_playerskilllevel;
NewSkill.m_nSkillID = nKillId;
NewSkill.m_nSkillLevel = Npc[m_nIndex].m_SkillList.GetLevel(nKillId);
NewSkill.m_nLeavePoint = m_nSkillPoint;
g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&NewSkill, sizeof(PLAYER_SKILL_LEVEL_SYNC));

}




}



}
}
}
}
}	

//Ket Thuc Luyen skill 120
#endif












	g_DebugLog("[ÊýÖµ]%sÊµ¼ÊÔö¼Ó¾­Ñé%dµã", Npc[m_nIndex].Name, nGetExp);
#ifdef _SERVER
	if (m_nExp >= m_nNextLevelExp)	// Éý¼¶
	{
		LevelUp();
	}
	else								// Í¬²½¾­Ñé
	{
		PLAYER_EXP_SYNC	sSyncExp;
		sSyncExp.ProtocolType = (BYTE)s2c_playerexp;
		sSyncExp.m_nExp = m_nExp;
		g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sSyncExp, sizeof(PLAYER_EXP_SYNC));
	}
#endif
}

//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÖ±½ÓÔö¼Ó¾­ÑéÖµ£¬²»¿¼ÂÇÆäËûÒòËØ
//-------------------------------------------------------------------------
void	KPlayer::DirectAddExp(int nExp)
{
	if (nExp > 0 && Npc[m_nIndex].m_Level >= MAX_LEVEL)
		return;

int nxExpCS;
nxExpCS = 1;

	// Tinh Kiem
/*	
	if (m_cTask.GetSaveVal(139) == 0)
	m_nExp += nExp;
	else if (m_cTask.GetSaveVal(139) == 1)
	m_nExp += nExp/6;
	else
	m_nExp += nExp/6;
*/	
	//Hoa Son
	int tmpExp, nExp2;
	nExp2 = nExp / nxExpCS;
	tmpExp = m_nExp;
	if (tmpExp/100 + nExp2/100 > m_nNextLevelExp/100)
	{
		m_nExp += (m_nNextLevelExp - tmpExp);
	}
	else
	{
		m_nExp += nExp2 ;
	}
	
	if (m_nExp < 0)
	{
		if (-m_nExp > m_nNextLevelExp)
		{
			m_nExp = -m_nNextLevelExp;
		}
	}
#ifdef _SERVER
	if (m_nExp >= m_nNextLevelExp)	// Éý¼¶
	{
		LevelUp();
	}
	else								// Í¬²½¾­Ñé
	{
		PLAYER_EXP_SYNC	sSyncExp;
		sSyncExp.ProtocolType = (BYTE)s2c_playerexp;
		sSyncExp.m_nExp = m_nExp;
		g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sSyncExp, sizeof(PLAYER_EXP_SYNC));
	}
#endif
}

//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÍæ¼ÒÉýÒ»¼¶
//-------------------------------------------------------------------------

#ifdef _SERVER
void	KPlayer::FixPlayer()
{

	if (m_nIndex < 0 || m_nIndex >= MAX_NPC)
	    return;

	if (Npc[m_nIndex].m_Level > MAX_LEVEL || Npc[m_nIndex].m_Level < 1)
		return;
	

	if (m_cTask.GetSaveVal(139) == 1)
	{
	m_nNextLevelExp = PlayerSet.m_cLevelAdd.GetLevelExp1(Npc[m_nIndex].m_Level);
	}
	else if (m_cTask.GetSaveVal(139) == 2)
	{
	m_nNextLevelExp = PlayerSet.m_cLevelAdd.GetLevelExp2(Npc[m_nIndex].m_Level);
	}



	else if (m_cTask.GetSaveVal(139) == 3)
	{
	m_nNextLevelExp = PlayerSet.m_cLevelAdd.GetLevelExp3(Npc[m_nIndex].m_Level);
	}

	else if (m_cTask.GetSaveVal(139) == 4)
	{
	m_nNextLevelExp = PlayerSet.m_cLevelAdd.GetLevelExp4(Npc[m_nIndex].m_Level);
	}

	else if (m_cTask.GetSaveVal(139) == 5)
	{
	m_nNextLevelExp = PlayerSet.m_cLevelAdd.GetLevelExp5(Npc[m_nIndex].m_Level);
	}



	else
	{
	m_nNextLevelExp = PlayerSet.m_cLevelAdd.GetLevelExp(Npc[m_nIndex].m_Level);
	}

	LevelAttriBaseLifeMax();
	LevelAttriBaseStaminaMax();
	LevelAttriBaseManaMax();

	Npc[m_nIndex].m_FireResist				= PlayerSet.m_cLevelAdd.GetFireResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_CurrentFireResist		= Npc[m_nIndex].m_FireResist;
	Npc[m_nIndex].m_ColdResist				= PlayerSet.m_cLevelAdd.GetColdResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_CurrentColdResist		= Npc[m_nIndex].m_ColdResist;
	Npc[m_nIndex].m_PoisonResist			= PlayerSet.m_cLevelAdd.GetPoisonResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_CurrentPoisonResist		= Npc[m_nIndex].m_PoisonResist;
	Npc[m_nIndex].m_LightResist				= PlayerSet.m_cLevelAdd.GetLightResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_CurrentLightResist		= Npc[m_nIndex].m_LightResist;
	Npc[m_nIndex].m_PhysicsResist			= PlayerSet.m_cLevelAdd.GetPhysicsResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_CurrentPhysicsResist	= Npc[m_nIndex].m_PhysicsResist;
	Npc[m_nIndex].m_FireResistMax			= BASE_FIRE_RESIST_MAX;
	Npc[m_nIndex].m_ColdResistMax			= BASE_COLD_RESIST_MAX;
	Npc[m_nIndex].m_PoisonResistMax			= BASE_POISON_RESIST_MAX;
	Npc[m_nIndex].m_LightResistMax			= BASE_LIGHT_RESIST_MAX;
	Npc[m_nIndex].m_PhysicsResistMax		= BASE_PHYSICS_RESIST_MAX;
	Npc[m_nIndex].m_CurrentFireResistMax	= Npc[m_nIndex].m_FireResistMax;
	Npc[m_nIndex].m_CurrentColdResistMax	= Npc[m_nIndex].m_ColdResistMax;
	Npc[m_nIndex].m_CurrentPoisonResistMax	= Npc[m_nIndex].m_PoisonResistMax;
	Npc[m_nIndex].m_CurrentLightResistMax	= Npc[m_nIndex].m_LightResistMax;
	Npc[m_nIndex].m_CurrentPhysicsResistMax	= Npc[m_nIndex].m_PhysicsResistMax;

	int nOldCurCamp = Npc[m_nIndex].m_CurrentCamp;
	
	this->UpdataCurData();
	SetNpcPhysicsDamage();
	Npc[m_nIndex].m_CurrentCamp = nOldCurCamp;

	Npc[m_nIndex].m_CurrentLife = Npc[m_nIndex].m_CurrentLifeMax;
	Npc[m_nIndex].m_CurrentStamina = Npc[m_nIndex].m_CurrentStaminaMax;
	Npc[m_nIndex].m_CurrentMana = Npc[m_nIndex].m_CurrentManaMax;
}


#endif

void	KPlayer::SetLevel(int nLevel)
{
	if (m_nIndex < 0 || m_nIndex >= MAX_NPC)
	    return;

	if (nLevel > MAX_LEVEL || nLevel < 1)
		return;
	m_nExp = 0;

	Npc[m_nIndex].m_Level = nLevel;

	if (m_cTask.GetSaveVal(139) == 1)
	{
	m_nNextLevelExp = PlayerSet.m_cLevelAdd.GetLevelExp1(Npc[m_nIndex].m_Level);
	}
	else if (m_cTask.GetSaveVal(139) == 2)
	{
	m_nNextLevelExp = PlayerSet.m_cLevelAdd.GetLevelExp2(Npc[m_nIndex].m_Level);
	}


	else if (m_cTask.GetSaveVal(139) == 3)
	{
	m_nNextLevelExp = PlayerSet.m_cLevelAdd.GetLevelExp3(Npc[m_nIndex].m_Level);
	}
	else if (m_cTask.GetSaveVal(139) == 4)
	{
	m_nNextLevelExp = PlayerSet.m_cLevelAdd.GetLevelExp4(Npc[m_nIndex].m_Level);
	}
	else if (m_cTask.GetSaveVal(139) == 5)
	{
	m_nNextLevelExp = PlayerSet.m_cLevelAdd.GetLevelExp5(Npc[m_nIndex].m_Level);
	}




	else
	{
	m_nNextLevelExp = PlayerSet.m_cLevelAdd.GetLevelExp(Npc[m_nIndex].m_Level);
	}

	LevelAttriBaseLifeMax();
	LevelAttriBaseStaminaMax();
	LevelAttriBaseManaMax();

	Npc[m_nIndex].m_FireResist				= PlayerSet.m_cLevelAdd.GetFireResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_CurrentFireResist		= Npc[m_nIndex].m_FireResist;
	Npc[m_nIndex].m_ColdResist				= PlayerSet.m_cLevelAdd.GetColdResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_CurrentColdResist		= Npc[m_nIndex].m_ColdResist;
	Npc[m_nIndex].m_PoisonResist			= PlayerSet.m_cLevelAdd.GetPoisonResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_CurrentPoisonResist		= Npc[m_nIndex].m_PoisonResist;
	Npc[m_nIndex].m_LightResist				= PlayerSet.m_cLevelAdd.GetLightResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_CurrentLightResist		= Npc[m_nIndex].m_LightResist;
	Npc[m_nIndex].m_PhysicsResist			= PlayerSet.m_cLevelAdd.GetPhysicsResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_CurrentPhysicsResist	= Npc[m_nIndex].m_PhysicsResist;
	Npc[m_nIndex].m_FireResistMax			= BASE_FIRE_RESIST_MAX;
	Npc[m_nIndex].m_ColdResistMax			= BASE_COLD_RESIST_MAX;
	Npc[m_nIndex].m_PoisonResistMax			= BASE_POISON_RESIST_MAX;
	Npc[m_nIndex].m_LightResistMax			= BASE_LIGHT_RESIST_MAX;
	Npc[m_nIndex].m_PhysicsResistMax		= BASE_PHYSICS_RESIST_MAX;
	Npc[m_nIndex].m_CurrentFireResistMax	= Npc[m_nIndex].m_FireResistMax;
	Npc[m_nIndex].m_CurrentColdResistMax	= Npc[m_nIndex].m_ColdResistMax;
	Npc[m_nIndex].m_CurrentPoisonResistMax	= Npc[m_nIndex].m_PoisonResistMax;
	Npc[m_nIndex].m_CurrentLightResistMax	= Npc[m_nIndex].m_LightResistMax;
	Npc[m_nIndex].m_CurrentPhysicsResistMax	= Npc[m_nIndex].m_PhysicsResistMax;

	int nOldCurCamp = Npc[m_nIndex].m_CurrentCamp;
	
	this->UpdataCurData();
	SetNpcPhysicsDamage();
	Npc[m_nIndex].m_CurrentCamp = nOldCurCamp;

	Npc[m_nIndex].m_CurrentLife = Npc[m_nIndex].m_CurrentLifeMax;
	Npc[m_nIndex].m_CurrentStamina = Npc[m_nIndex].m_CurrentStaminaMax;
	Npc[m_nIndex].m_CurrentMana = Npc[m_nIndex].m_CurrentManaMax;
}





void	KPlayer::LevelUp()
{

	if (m_nIndex < 0 || m_nIndex >= MAX_NPC)
	    return;

	m_nExp = 0;
	if (Npc[m_nIndex].m_Level >= MAX_LEVEL)
		return;
	Npc[m_nIndex].m_Level++;


	if (m_cTask.GetSaveVal(139) == 1)
	{
	m_nNextLevelExp = PlayerSet.m_cLevelAdd.GetLevelExp1(Npc[m_nIndex].m_Level);
	}
	else if (m_cTask.GetSaveVal(139) == 2)
	{
	m_nNextLevelExp = PlayerSet.m_cLevelAdd.GetLevelExp2(Npc[m_nIndex].m_Level);
	}

	else if (m_cTask.GetSaveVal(139) == 3)
	{
	m_nNextLevelExp = PlayerSet.m_cLevelAdd.GetLevelExp3(Npc[m_nIndex].m_Level);
	}
	else if (m_cTask.GetSaveVal(139) == 4)
	{
	m_nNextLevelExp = PlayerSet.m_cLevelAdd.GetLevelExp4(Npc[m_nIndex].m_Level);
	}
	else if (m_cTask.GetSaveVal(139) == 5)
	{
	m_nNextLevelExp = PlayerSet.m_cLevelAdd.GetLevelExp5(Npc[m_nIndex].m_Level);
	}





	else
	{
	m_nNextLevelExp = PlayerSet.m_cLevelAdd.GetLevelExp(Npc[m_nIndex].m_Level);
	}
	
	// -------------------------- µÈ¼¶Ôö¼ÓÊ±¸÷ÏîÊôÐÔÔö¼Ó ----------------------
	// Ôö¼ÓÎ´·ÖÅäÊôÐÔµã
	m_nAttributePoint += PLAYER_LEVEL_ADD_ATTRIBUTE;
	// Ôö¼ÓÎ´·ÖÅä¼¼ÄÜµã
	m_nSkillPoint += PLAYER_LEVEL_ADD_SKILL;
	
	// ´¦ÀíÉúÃü¡¢ÌåÁ¦¡¢ÄÚÁ¦Öµ£¨Ó°Ïì»ù±¾ÉúÃü¡¢ÌåÁ¦¡¢ÄÚÁ¦×î´óÖµµÄÒòËØ£ºµÈ¼¶¡¢ÊôÐÔµã£¬Ó°Ïìµ±Ç°ÉúÃü×î´óÖµµÄÒòËØ£º×°±¸¡¢¼¼ÄÜ¡¢Ò©Îï£¨ÁÙÊ±£©£©
	LevelAttriBaseLifeMax();
	LevelAttriBaseStaminaMax();
	LevelAttriBaseManaMax();
	//	Npc[m_nIndex].ResetLifeReplenish();
	
	// ´¦Àí¸÷ÖÖ¿¹ÐÔµÄ±ä»¯ »ð¡¢±ù¡¢¶¾¡¢µç¡¢ÎïÀí
	Npc[m_nIndex].m_FireResist				= PlayerSet.m_cLevelAdd.GetFireResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_CurrentFireResist		= Npc[m_nIndex].m_FireResist;
	Npc[m_nIndex].m_ColdResist				= PlayerSet.m_cLevelAdd.GetColdResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_CurrentColdResist		= Npc[m_nIndex].m_ColdResist;
	Npc[m_nIndex].m_PoisonResist			= PlayerSet.m_cLevelAdd.GetPoisonResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_CurrentPoisonResist		= Npc[m_nIndex].m_PoisonResist;
	Npc[m_nIndex].m_LightResist				= PlayerSet.m_cLevelAdd.GetLightResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_CurrentLightResist		= Npc[m_nIndex].m_LightResist;
	Npc[m_nIndex].m_PhysicsResist			= PlayerSet.m_cLevelAdd.GetPhysicsResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_CurrentPhysicsResist	= Npc[m_nIndex].m_PhysicsResist;
	Npc[m_nIndex].m_FireResistMax			= BASE_FIRE_RESIST_MAX;
	Npc[m_nIndex].m_ColdResistMax			= BASE_COLD_RESIST_MAX;
	Npc[m_nIndex].m_PoisonResistMax			= BASE_POISON_RESIST_MAX;
	Npc[m_nIndex].m_LightResistMax			= BASE_LIGHT_RESIST_MAX;
	Npc[m_nIndex].m_PhysicsResistMax		= BASE_PHYSICS_RESIST_MAX;
	Npc[m_nIndex].m_CurrentFireResistMax	= Npc[m_nIndex].m_FireResistMax;
	Npc[m_nIndex].m_CurrentColdResistMax	= Npc[m_nIndex].m_ColdResistMax;
	Npc[m_nIndex].m_CurrentPoisonResistMax	= Npc[m_nIndex].m_PoisonResistMax;
	Npc[m_nIndex].m_CurrentLightResistMax	= Npc[m_nIndex].m_LightResistMax;
	Npc[m_nIndex].m_CurrentPhysicsResistMax	= Npc[m_nIndex].m_PhysicsResistMax;

	int nOldCurCamp = Npc[m_nIndex].m_CurrentCamp;
	// ¸ù¾Ý×°±¸¡¢¼¼ÄÜÐÅÏ¢¸üÐÂµ±Ç°Êý¾Ý
	this->UpdataCurData();
	SetNpcPhysicsDamage();
	Npc[m_nIndex].m_CurrentCamp = nOldCurCamp;
	
	// ÉúÃü¡¢ÌåÁ¦¡¢ÄÚÁ¦Öµ¼ÓÂú
	Npc[m_nIndex].m_CurrentLife = Npc[m_nIndex].m_CurrentLifeMax;
	Npc[m_nIndex].m_CurrentStamina = Npc[m_nIndex].m_CurrentStaminaMax;
	Npc[m_nIndex].m_CurrentMana = Npc[m_nIndex].m_CurrentManaMax;
	
#ifdef _SERVER
	// ¸ø¿Í»§¶Ë·¢ËÍÊý¾Ý
	// ×Ô¼ºµÄÊý¾Ý
	PLAYER_LEVEL_UP_SYNC	sLevelUp;
	sLevelUp.ProtocolType		= s2c_playerlevelup;
	sLevelUp.m_btLevel			= (DWORD)Npc[m_nIndex].m_Level;
	sLevelUp.m_nExp				= m_nExp;
	sLevelUp.m_nAttributePoint	= m_nAttributePoint;
	sLevelUp.m_nSkillPoint		= m_nSkillPoint;
	sLevelUp.m_nBaseLifeMax		= Npc[m_nIndex].m_LifeMax;
	sLevelUp.m_nBaseStaminaMax	= Npc[m_nIndex].m_StaminaMax;
	sLevelUp.m_nBaseManaMax		= Npc[m_nIndex].m_ManaMax;
	g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sLevelUp, sizeof(PLAYER_LEVEL_UP_SYNC));
	// ¸ø¶ÓÓÑ·¢ËÍµÈ¼¶Êý¾Ý
	if ( m_cTeam.m_nFlag )
	{
		PLAYER_TEAMMATE_LEVEL_SYNC	sLevel;
		sLevel.ProtocolType = s2c_teammatelevel;
		sLevel.m_btLevel = (DWORD)Npc[m_nIndex].m_Level;
		sLevel.m_dwTeammateID = Npc[m_nIndex].m_dwID;
		// ¸ø¶Ó³¤·¢
		if (m_cTeam.m_nFigure != TEAM_CAPTAIN)
			g_pServer->PackDataToClient(Player[g_Team[m_cTeam.m_nID].m_nCaptain].m_nNetConnectIdx, (BYTE*)&sLevel, sizeof(PLAYER_TEAMMATE_LEVEL_SYNC));
		for (int i = 0; i < MAX_TEAM_MEMBER; i++)
		{
			if (g_Team[m_cTeam.m_nID].m_nMember[i] >= 0 && g_Team[m_cTeam.m_nID].m_nMember[i] != GetPlayerIndex())
				g_pServer->PackDataToClient(Player[g_Team[m_cTeam.m_nID].m_nMember[i]].m_nNetConnectIdx, (BYTE*)&sLevel, sizeof(PLAYER_TEAMMATE_LEVEL_SYNC));
		}
	}



	if (this->CanSave())
			{
				if (this->Save())
				{
					this->m_uMustSave = SAVE_REQUEST;
				}
	
			}

#endif
}

// need spe edit not end
void	KPlayer::UpdataCurData()
{
	if (m_nIndex <= 0 || m_nIndex >= MAX_NPC)
		return;
	
	
	
	Npc[m_nIndex].m_CurrentPoisonTimeReducePercent		= 0;
	Npc[m_nIndex].m_CurrentStunTimeReducePercent		= 0;
	Npc[m_nIndex].m_CurrentFreezeTimeReducePercent		= 0;
	Npc[m_nIndex].m_CurrentPiercePercent		= 0;

	Npc[m_nIndex].m_CurrentLifeMax		= Npc[m_nIndex].m_LifeMax;
	Npc[m_nIndex].m_CurrentManaMax		= Npc[m_nIndex].m_ManaMax;
	Npc[m_nIndex].m_CurrentStaminaMax	= Npc[m_nIndex].m_StaminaMax;
	Npc[m_nIndex].m_CurrentAttackRating	= Npc[m_nIndex].m_AttackRating;
	Npc[m_nIndex].m_CurrentAttackSpeed	= Npc[m_nIndex].m_AttackSpeed;
//	Npc[m_nIndex].m_CurrentCamp			= Npc[m_nIndex].m_Camp;
	Npc[m_nIndex].m_CurrentCastSpeed	= Npc[m_nIndex].m_CastSpeed;
	ZeroMemory(&Npc[m_nIndex].m_CurrentColdDamage, sizeof(KMagicAttrib));
	ZeroMemory(&Npc[m_nIndex].m_CurrentColdDamage2, sizeof(KMagicAttrib));
	Npc[m_nIndex].m_CurrentColdEnhance	= 0;
	Npc[m_nIndex].m_CurrentColdResist	= Npc[m_nIndex].m_ColdResist;
	Npc[m_nIndex].m_CurrentColdResistMax	= Npc[m_nIndex].m_ColdResistMax;
	Npc[m_nIndex].m_CurrentDamage2Mana	= 0;
	Npc[m_nIndex].m_CurrentDamageReduce	= 0;
	Npc[m_nIndex].m_CurrentDeadlyStrike	= 0;
	Npc[m_nIndex].m_CurrentDefend		= Npc[m_nIndex].m_Defend;
	Npc[m_nIndex].m_CurrentElementDamageReduce = 0;
	ZeroMemory(&Npc[m_nIndex].m_CurrentFireDamage, sizeof(KMagicAttrib));
	ZeroMemory(&Npc[m_nIndex].m_CurrentFireDamage2, sizeof(KMagicAttrib));
	Npc[m_nIndex].m_CurrentFireEnhance	= 0;
	Npc[m_nIndex].m_CurrentFireResist	= Npc[m_nIndex].m_FireResist;
	Npc[m_nIndex].m_CurrentFireResistMax	= Npc[m_nIndex].m_FireResistMax;
	Npc[m_nIndex].m_CurrentHandEnhance	= 0;
	Npc[m_nIndex].m_CurrentHitRecover	= Npc[m_nIndex].m_HitRecover;
	
	//printf("Npc[m_nIndex].m_CurrentHitRecover %d \n",Npc[m_nIndex].m_CurrentHitRecover); 
	
	Npc[m_nIndex].m_CurrentJumpSpeed	= Npc[m_nIndex].m_JumpSpeed;
	Npc[m_nIndex].m_CurrentKnockBack	= 0;
	Npc[m_nIndex].m_CurrentLifeReplenish	= Npc[m_nIndex].m_LifeReplenish;
	Npc[m_nIndex].m_CurrentLifeStolen	= 0;
	ZeroMemory(&Npc[m_nIndex].m_CurrentLightDamage, sizeof(KMagicAttrib));
	ZeroMemory(&Npc[m_nIndex].m_CurrentLightDamage2, sizeof(KMagicAttrib));
	Npc[m_nIndex].m_CurrentLightEnhance	= 0;
	Npc[m_nIndex].m_CurrentLightResist	= Npc[m_nIndex].m_LightResist;
	Npc[m_nIndex].m_CurrentLightResistMax = Npc[m_nIndex].m_LightResistMax;
	Npc[m_nIndex].m_CurrentManaPerEnemy	= 0;
	Npc[m_nIndex].m_CurrentManaReplenish	= Npc[m_nIndex].m_ManaReplenish;
	Npc[m_nIndex].m_CurrentManaStolen	= 0;
	Npc[m_nIndex].m_CurrentMeleeDmgRet	= 0;
	Npc[m_nIndex].m_CurrentMeleeDmgRetPercent = 0;
	ZeroMemory(&Npc[m_nIndex].m_CurrentMeleeEnhance, sizeof(Npc[m_nIndex].m_CurrentMeleeEnhance));
	Npc[m_nIndex].m_CurrentPhysicsResist	= Npc[m_nIndex].m_PhysicsResist;
	Npc[m_nIndex].m_CurrentPhysicsResistMax	= Npc[m_nIndex].m_PhysicsResistMax;
	Npc[m_nIndex].m_CurrentPiercePercent	= 0;
	ZeroMemory(&Npc[m_nIndex].m_CurrentPoisonDamage, sizeof(KMagicAttrib));
	ZeroMemory(&Npc[m_nIndex].m_CurrentPoisonDamage2, sizeof(KMagicAttrib));
	ZeroMemory(&Npc[m_nIndex].m_CurrentPhysicsDamage2, sizeof(KMagicAttrib));

	
    ZeroMemory(&Npc[m_nIndex].m_ManaShield, sizeof(Npc[m_nIndex].m_ManaShield));
    ZeroMemory(&Npc[m_nIndex].m_ManaShieldPoint, sizeof(Npc[m_nIndex].m_ManaShieldPoint));
	ZeroMemory(&Npc[m_nIndex].m_LightArmor, sizeof(Npc[m_nIndex].m_LightArmor));
	ZeroMemory(&Npc[m_nIndex].m_PhysicsArmor, sizeof(Npc[m_nIndex].m_PhysicsArmor));
	ZeroMemory(&Npc[m_nIndex].m_ColdArmor, sizeof(Npc[m_nIndex].m_ColdArmor));
	ZeroMemory(&Npc[m_nIndex].m_FireArmor, sizeof(Npc[m_nIndex].m_FireArmor));
	ZeroMemory(&Npc[m_nIndex].m_PoisonArmor, sizeof(Npc[m_nIndex].m_PoisonArmor));
	ZeroMemory(&Npc[m_nIndex].m_ReturnDoSkillEmty, sizeof(Npc[m_nIndex].m_ReturnDoSkillEmty));
	ZeroMemory(&Npc[m_nIndex].m_CurrentBuffEmtyRetPercent, sizeof(Npc[m_nIndex].m_CurrentBuffEmtyRetPercent));
	ZeroMemory(&Npc[m_nIndex].m_CurrentBuffEmtyResistPercent, sizeof(Npc[m_nIndex].m_CurrentBuffEmtyResistPercent));
    ZeroMemory(&Npc[m_nIndex].m_CurrentDoSkillEffecPercent, sizeof(Npc[m_nIndex].m_CurrentDoSkillEffecPercent));
    ZeroMemory(&Npc[m_nIndex].m_CurrentTimeCastSkillEffec, sizeof(Npc[m_nIndex].m_CurrentTimeCastSkillEffec));
	ZeroMemory(&Npc[m_nIndex].m_CurrentDoSkillEffecPercent1, sizeof(Npc[m_nIndex].m_CurrentDoSkillEffecPercent1));
	ZeroMemory(&Npc[m_nIndex].m_CurrentTimeCastSkillEffec1, sizeof(Npc[m_nIndex].m_CurrentTimeCastSkillEffec1));
	ZeroMemory(&Npc[m_nIndex].m_CurrentDoSkillEffecPercent2, sizeof(Npc[m_nIndex].m_CurrentDoSkillEffecPercent2));
	ZeroMemory(&Npc[m_nIndex].m_CurrentTimeCastSkillEffec2, sizeof(Npc[m_nIndex].m_CurrentTimeCastSkillEffec2));

    
	for (int i=0; i<MAX_SKILL; i++)
	{
	Npc[m_nIndex].m_SkillList.m_CurrentListSkillAddLevel[i] = 0;
	}

	Npc[m_nIndex].m_CurrentPoisonEnhance	= 0;
	Npc[m_nIndex].m_CurrentPoisonResist	= Npc[m_nIndex].m_PoisonResist;
	Npc[m_nIndex].m_CurrentPoisonResistMax	= Npc[m_nIndex].m_PoisonResistMax;
	Npc[m_nIndex].m_CurrentRangeDmgRet	= 0;
	Npc[m_nIndex].m_CurrentRangeDmgRetPercent	= 0;
	Npc[m_nIndex].m_CurrentRangeEnhance	= 0;
	Npc[m_nIndex].m_CurrentPoisonDmgRetPercent = 0;
	Npc[m_nIndex].m_CurrentLifeStolenAddPercent = 0;
	Npc[m_nIndex].m_CurrentManaFromDamePercent = 0;
	Npc[m_nIndex].m_CurrentManaShieldExPoint = 0;
	Npc[m_nIndex].m_CurrentDmgRetPercentResist = 0;
	

	Npc[m_nIndex].m_CurrentRWSpeed	= 0;

	Npc[m_nIndex].m_CurrentRunSpeed	= Npc[m_nIndex].m_RunSpeed;
	Npc[m_nIndex].m_CurrentSlowMissle	= 0;
	Npc[m_nIndex].m_CurrentStaminaGain	= Npc[m_nIndex].m_StaminaGain;
	Npc[m_nIndex].m_CurrentStaminaLoss	= Npc[m_nIndex].m_StaminaLoss;
	Npc[m_nIndex].m_CurrentStaminaStolen	= 0;
	Npc[m_nIndex].m_CurrentVisionRadius	= Npc[m_nIndex].m_VisionRadius;
	Npc[m_nIndex].m_CurrentWalkSpeed	= Npc[m_nIndex].m_WalkSpeed;
	Npc[m_nIndex].m_CurrentAddPhysicsDamage = 0;
	m_nCurStrength						= m_nStrength;
	m_nCurDexterity						= m_nDexterity;
	m_nCurVitality						= m_nVitality;
	m_nCurEngergy						= m_nEngergy;
	m_nCurLucky							= m_nLucky;

	
	ReCalcEquip();
	ReCalcState();
}

void KPlayer::ReCalcEquip()
{
	_ASSERT(m_nIndex > 0 && m_nIndex < MAX_NPC);
	
	for (int i = 0; i < itempart_num; i++)
	{
		int nIdx = m_ItemList.m_EquipItem[i];
		if (!nIdx)
			continue;

	if (i == itempart_horse)
	{
	if (!Npc[m_nIndex].m_bRideHorse)
	continue;
	}


	else if (i == itempart_head || i == itempart_body || i == itempart_belt || i == itempart_weapon || i == itempart_foot || i == itempart_cuff)
	{

	if (!Item[nIdx].GetDurability())
		continue;
	}


		int nActive = m_ItemList.GetEquipEnhance(i);
		Item[nIdx].ApplyMagicAttribToNPC(&Npc[m_nIndex], nActive);
	}
}

void KPlayer::ReCalcState()
{
	_ASSERT(m_nIndex > 0 && m_nIndex < MAX_NPC);
	
	Npc[m_nIndex].ReCalcStateEffect();
}

//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÔö¼ÓÍ³ÂÊÁ¦¾­Ñé
//-------------------------------------------------------------------------
void	KPlayer::AddLeadExp(int nExp)
{
	if (m_dwLeadLevel >= MAX_LEAD_LEVEL)
		return;
	if (Npc[m_nIndex].m_Doing == do_death || Npc[m_nIndex].m_Doing == do_revive)
		return;
	m_dwLeadExp += nExp;
	if (m_dwLeadExp >= (DWORD)PlayerSet.m_cLeadExp.GetLevelExp(m_dwLeadLevel))
	{
		// Éý¼¶
		m_dwLeadLevel = PlayerSet.m_cLeadExp.GetLevel(m_dwLeadExp, m_dwLeadLevel);
#ifndef _SERVER
		m_dwNextLevelLeadExp = PlayerSet.m_cLeadExp.GetLevelExp(m_dwLeadLevel);
#endif
	}
	
#ifdef _SERVER
	PLAYER_LEAD_EXP_SYNC	sLeadExp;
	sLeadExp.ProtocolType = s2c_playersyncleadexp;
	sLeadExp.m_dwLeadExp = m_dwLeadExp;
	g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sLeadExp, sizeof(PLAYER_LEAD_EXP_SYNC));
#endif
	
}

//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÍæ¼ÒÉýÒ»¼¶¼ÓÉúÃü
//-------------------------------------------------------------------------
void KPlayer::LevelAttriBaseLifeMax()
{
		if (m_nIndex < 0 || m_nIndex >= MAX_NPC)
		{
		printf("Loi !\n");
		return;
		}


	Npc[m_nIndex].m_LifeMax = Npc[m_nIndex].m_Level * PlayerSet.m_cLevelAdd.GetLifePerLevel(Npc[m_nIndex].m_Series) + m_nVitality * PlayerSet.m_cLevelAdd.GetLifePerVitality(Npc[m_nIndex].m_Series);
	Npc[m_nIndex].m_CurrentLifeMax = Npc[m_nIndex].m_LifeMax;
}

//-------------------------------------------------------------------------
//	¹¦ÄÜ?ºÍæ¼ÒÉýÒ»¼¶¼ÓÌåÁ?
//-------------------------------------------------------------------------
void KPlayer::LevelAttriBaseStaminaMax()
{
		if (m_nIndex < 0 || m_nIndex >= MAX_NPC)
		{
		printf("Loi !\n");
		return;
		}

	Npc[m_nIndex].m_StaminaMax = Npc[m_nIndex].m_Level * PlayerSet.m_cLevelAdd.GetStaminaPerLevel(Npc[m_nIndex].m_Series) + m_nVitality * PlayerSet.m_cLevelAdd.GetStaminaPerVitality(Npc[m_nIndex].m_Series);
	Npc[m_nIndex].m_CurrentStaminaMax = Npc[m_nIndex].m_StaminaMax;
}

//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÍæ¼ÒÉýÒ»¼¶¼ÓÄÚÁ¦
//-------------------------------------------------------------------------
void KPlayer::LevelAttriBaseManaMax()
{
		if (m_nIndex < 0 || m_nIndex >= MAX_NPC)
		{
		return;
		printf("Loi !\n");
		}


	Npc[m_nIndex].m_ManaMax = Npc[m_nIndex].m_Level * PlayerSet.m_cLevelAdd.GetManaPerLevel(Npc[m_nIndex].m_Series) + m_nEngergy * PlayerSet.m_cLevelAdd.GetManaPerEnergy(Npc[m_nIndex].m_Series);
	Npc[m_nIndex].m_CurrentManaMax = Npc[m_nIndex].m_ManaMax;
}

//-------------------------------------------------------------------------
//	¹¦ÄÜ£º¸Ä±äÍæ¼ÒÕóÓª
//-------------------------------------------------------------------------
void	KPlayer::ChangePlayerCamp(int nCamp)
{
	if (nCamp < camp_begin || nCamp >= camp_num)
		return;
	Npc[m_nIndex].m_Camp = (NPCCAMP)nCamp;
}

#ifndef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÏò·þÎñÆ÷ÉêÇë²éÑ¯Ä³¸önpcËùÔÚ¶ÓÎéµÄÐÅÏ¢
//-------------------------------------------------------------------------
void	KPlayer::ApplyTeamInfo(DWORD dwNpcID)
{
	if (dwNpcID <= 0)
		return;
	PLAYER_APPLY_TEAM_INFO	sApplyInfo;
	sApplyInfo.ProtocolType = (BYTE)c2s_teamapplyinfo;
	sApplyInfo.m_dwTarNpcID = dwNpcID;
	if (g_pClient)
		g_pClient->SendPackToServer(&sApplyInfo, sizeof(PLAYER_APPLY_TEAM_INFO));
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÏò·þÎñÆ÷ÉêÇë²éÑ¯Íæ¼Ò×ÔÉíµÄ¶ÓÎéÇé¿ö
//-------------------------------------------------------------------------
void	KPlayer::ApplySelfTeamInfo()
{
	ApplyTeamInfo(Npc[m_nIndex].m_dwID);
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÍæ¼ÒÏò·þÎñÆ÷ÉêÇë´´½¨¶ÓÎé
//-------------------------------------------------------------------------
BOOL	KPlayer::ApplyCreateTeam()//char *lpszTeamName)
{
	return m_cTeam.ApplyCreate();//lpszTeamName);
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£º¶Ó³¤Ïò·þÎñÆ÷ÉêÇë¿ª·Å¡¢¹Ø±Õ¶ÓÎéÊÇ·ñÔÊÐí¼ÓÈë³ÉÔ±×´Ì¬
//-------------------------------------------------------------------------
BOOL	KPlayer::ApplyTeamOpenClose(BOOL bFlag)
{
	if ( !m_cTeam.m_nFlag )					// Î´×é¶Ó
		return FALSE;
	if (m_cTeam.m_nFigure != TEAM_CAPTAIN)	// ²»ÊÇ¶Ó³¤
		return FALSE;
	if (this->CheckTrading())	// ´¦ÔÚ½»Ò××´Ì¬
		return FALSE;
	
	if (bFlag)
	{
		if (PlayerSet.m_cLeadExp.GetMemNumFromLevel(m_dwLeadLevel) <= g_Team[0].m_nMemNum)	// Í³Ë§Á¦²»¹»
			return FALSE;
	}
	
	PLAYER_TEAM_OPEN_CLOSE	sTeamState;
	sTeamState.ProtocolType = c2s_teamapplyopenclose;
	sTeamState.m_btOpenClose = bFlag;
	if (g_pClient)
		g_pClient->SendPackToServer(&sTeamState, sizeof(PLAYER_TEAM_OPEN_CLOSE));
	return TRUE;
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÍæ¼ÒÏò·þÎñÆ÷ÉêÇë¼ÓÈëÄ³¸ö¶ÓÎé
//	²ÎÊý£ºint nNpcIndex Ä¿±ê¶ÓÎé¶Ó³¤ÔÚ¿Í»§¶ËNpcÊý×éÖÐµÄÎ»ÖÃ
//-------------------------------------------------------------------------
void	KPlayer::ApplyAddTeam(int nNpcIndex)
{
	if (this->CheckTrading())
		return;
	if (m_cTeam.m_nFlag)					// ÒÑ¾­×é¶Ó£¬²»ÄÜÉêÇë¼ÓÈë±ðµÄ¶ÓÎé
		return;
	
	// ²»Í¬ÕóÓªÏÖÔÚ¿ÉÒÔ×é¶Ó£¬ÀÏÊÖ²»ÄÜ¼ÓÈëÐÂÈË¶ÓÎé£¬ÐÂÈË¿ÉÒÔ¼ÓÈëÀÏÊÖ¶ÓÎé
	if (Npc[nNpcIndex].m_Camp == camp_begin && Npc[m_nIndex].m_Camp != camp_begin)
		return;
	//	if (Npc[nNpcIndex].m_Camp != Npc[m_nIndex].m_Camp)	// ÕóÓª²»Í¬
	//		return;
	
	// ÅÐ¶ÏÄ¿±ê¶ÓÎé¶Ó³¤ÊÇ²»ÊÇ´¦ÔÚ¶ÓÎé¿ª·Å×´Ì¬
	if (Npc[nNpcIndex].GetMenuState() != PLAYER_MENU_STATE_TEAMOPEN)
		return;
	
	m_cTeam.m_nApplyCaptainID = Npc[nNpcIndex].m_dwID;
	m_cTeam.m_dwApplyTimer = MAX_APPLY_TEAM_TIME;
	PLAYER_APPLY_ADD_TEAM	sAddTeam;
	sAddTeam.ProtocolType = c2s_teamapplyadd;
	sAddTeam.m_dwTarNpcID = Npc[nNpcIndex].m_dwID;
	if (g_pClient)
		g_pClient->SendPackToServer(&sAddTeam, sizeof(PLAYER_APPLY_ADD_TEAM));
	
	KSystemMessage	sMsg;
	sprintf(sMsg.szMessage, MSG_TEAM_APPLY_ADD_SELF_MSG, Npc[nNpcIndex].Name);
	sMsg.eType = SMT_NORMAL;
	sMsg.byConfirmType = SMCT_NONE;
	sMsg.byPriority = 0;
	sMsg.byParamSize = 0;
	CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÍæ¼ÒÍ¨Öª·þÎñÆ÷½ÓÊÜÄ³¸önpcÎª¶ÓÎé³ÉÔ±
//-------------------------------------------------------------------------
void	KPlayer::AcceptTeamMember(DWORD dwNpcID)
{
	// ÅÐ¶Ï×´Ì¬ £¨È±ÉÙ¶Ônpc¶ÓÎé¿ª·Å×´Ì¬µÄ¼ì²â not end£©
	if ( !m_cTeam.m_nFlag || m_cTeam.m_nFigure != TEAM_CAPTAIN)
	{
		ApplySelfTeamInfo();
		return;
	}
	if (dwNpcID == 0)
		return;
	
	// ²éÕÒÉêÇëÁÐ±í
	
//	g_DebugLog("AAA 2");
	int		i;
	for (i = 0; i < MAX_TEAM_APPLY_LIST; i++)
	{
		if (m_cTeam.m_sApplyList[i].m_dwNpcID == dwNpcID)
		{
			m_cTeam.m_sApplyList[i].m_dwNpcID = 0;
			m_cTeam.UpdateInterface();
			break;
		}
	}
	// Èç¹ûÃ»ÕÒµ½
//	if (i >= MAX_TEAM_APPLY_LIST)
//		return;
	
	// Ïò·þÎñÆ÷·¢ÉêÇë
	PLAYER_ACCEPT_TEAM_MEMBER	sAcceptTeam;
	sAcceptTeam.ProtocolType = c2s_teamacceptmember;
	sAcceptTeam.m_dwNpcID = dwNpcID;
	if (g_pClient)
		g_pClient->SendPackToServer(&sAcceptTeam, sizeof(PLAYER_ACCEPT_TEAM_MEMBER));
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£º¶Ó³¤É¾³ý¼ÓÈë¶ÓÎéÉêÇëÁÐ±íÖÐµÄÄ³¸önpc
//-------------------------------------------------------------------------
void	KPlayer::TeamDropApplyOne(DWORD dwNpcID)
{
	if (dwNpcID == 0)
		return;
	// ²éÕÒÉêÇëÁÐ±í
	///g_DebugLog("AAA 3");
	int		i;
	for (i = 0; i < MAX_TEAM_APPLY_LIST; i++)
	{
		if (m_cTeam.m_sApplyList[i].m_dwNpcID == dwNpcID)
		{
			m_cTeam.m_sApplyList[i].m_dwNpcID = 0;
			m_cTeam.m_sApplyList[i].m_dwTimer = 0;
			m_cTeam.m_sApplyList[i].m_nLevel = 0;
			m_cTeam.m_sApplyList[i].m_szName[0] = 0;
			break;
		}
	}
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÍ¨Öª·þÎñÆ÷±¾Íæ¼ÒÀë¿ª¶ÓÎé
//-------------------------------------------------------------------------
void	KPlayer::LeaveTeam()
{
	PLAYER_APPLY_LEAVE_TEAM	sLeaveTeam;
	sLeaveTeam.ProtocolType = c2s_teamapplyleave;
	if (g_pClient)
		g_pClient->SendPackToServer(&sLeaveTeam, sizeof(PLAYER_APPLY_LEAVE_TEAM));
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£º¶Ó³¤Í¨Öª·þÎñÆ÷Ìß³ýÄ³¸ö¶ÓÔ±
//-------------------------------------------------------------------------
void	KPlayer::TeamKickMember(DWORD dwNpcID)
{
	if ( !m_cTeam.m_nFlag || m_cTeam.m_nFigure != TEAM_CAPTAIN)
		return;
	
	PLAYER_TEAM_KICK_MEMBER	sKickOne;
	sKickOne.ProtocolType = c2s_teamapplykickmember;
	sKickOne.m_dwNpcID = dwNpcID;
	if (g_pClient)
		g_pClient->SendPackToServer(&sKickOne, sizeof(PLAYER_TEAM_KICK_MEMBER));
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£º¶Ó³¤Ïò·þÎñÆ÷ÉêÇë°Ñ×Ô¼ºµÄ¶Ó³¤Éí·Ý½»¸ø±ðµÄ¶ÓÔ±
//-------------------------------------------------------------------------
void	KPlayer::ApplyTeamChangeCaptain(DWORD dwNpcID)
{
	if ( !m_cTeam.m_nFlag || m_cTeam.m_nFigure != TEAM_CAPTAIN)
		return;
	
	PLAYER_APPLY_TEAM_CHANGE_CAPTAIN	sTeamChange;
	sTeamChange.ProtocolType = c2s_teamapplychangecaptain;
	sTeamChange.m_dwNpcID = dwNpcID;
	if (g_pClient)
		g_pClient->SendPackToServer(&sTeamChange, sizeof(PLAYER_APPLY_TEAM_CHANGE_CAPTAIN));
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£º¶Ó³¤Ïò·þÎñÆ÷ÉêÇë½âÉ¢¶ÓÎé
//-------------------------------------------------------------------------
void	KPlayer::ApplyTeamDismiss()
{
	if ( !m_cTeam.m_nFlag || m_cTeam.m_nFigure != TEAM_CAPTAIN)
		return;
	
	PLAYER_APPLY_TEAM_DISMISS	sTeamDismiss;
	sTeamDismiss.ProtocolType = c2s_teamapplydismiss;
	if (g_pClient)
		g_pClient->SendPackToServer(&sTeamDismiss, sizeof(PLAYER_APPLY_TEAM_DISMISS));
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÍæ¼ÒÏò·þÎñÆ÷ÉêÇë´ò¿ª¡¢¹Ø±Õpk¿ª¹Ø
//-------------------------------------------------------------------------
void	KPlayer::ApplySetPK(BOOL bPK)
{
	PLAYER_SET_PK	sPK;
	sPK.ProtocolType = c2s_playerapplysetpk;
	sPK.m_btPKFlag = bPK;
	if (g_pClient)
		g_pClient->SendPackToServer(&sPK, sizeof(PLAYER_SET_PK));
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÍæ¼ÒÏò·þÎñÆ÷ÉêÇëÃÅÅÉÊý¾Ý
//-------------------------------------------------------------------------
void	KPlayer::ApplyFactionData()
{
	PLAYER_APPLY_FACTION_DATA	sFaction;
	sFaction.ProtocolType = c2s_playerapplyfactiondata;
	if (g_pClient)
		g_pClient->SendPackToServer(&sFaction, sizeof(PLAYER_APPLY_FACTION_DATA));
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£º¿Í»§¶Ë·¢ËÍÁÄÌìÓï¾ä¸ø·þÎñÆ÷
//-------------------------------------------------------------------------
void	KPlayer::SendChat(KUiMsgParam *pMsg, char *lpszSentence)
{
	if (!lpszSentence || !pMsg || pMsg->nMsgLength <= 0)
		return;
	int nLength = pMsg->nMsgLength;
	if (nLength >= MAX_SENTENCE_LENGTH)
		nLength = MAX_SENTENCE_LENGTH - 1;
	
	if (pMsg->eGenre == MSG_G_CHAT)
	{
		Npc[m_nIndex].SetChatInfo(pMsg->szName, lpszSentence, nLength);
		
		m_cChat.SendSentence(pMsg, Npc[m_nIndex].m_szChatBuffer);
	}
	else if (pMsg->eGenre == MSG_G_CMD)	// not end
	{
	}
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£º¶Ó³¤Ïò·þÎñÆ÷ÉêÇëÔö¼ÓËÄÏîÊôÐÔÖÐÄ³Ò»ÏîµÄµãÊý(0=Strength 1=Dexterity 2=Vitality 3=Engergy)
//-------------------------------------------------------------------------
void	KPlayer::ApplyAddBaseAttribute(int nAttribute, int nNo)
{
	if (nAttribute < 0 || nAttribute > 3)
		return;
	if (nNo <= 0 || nNo >= 255)
		return;
	PLAYER_ADD_BASE_ATTRIBUTE_COMMAND	sAdd;
	sAdd.ProtocolType = c2s_playeraddbaseattribute;
	sAdd.m_btAttribute = nAttribute;
	sAdd.m_nAddNo = nNo;
	if (g_pClient)
		g_pClient->SendPackToServer(&sAdd, sizeof(PLAYER_ADD_BASE_ATTRIBUTE_COMMAND));
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÏò·þÎñÆ÷ÉêÇëÄ³¸ö¼¼ÄÜÉý¼¶
//-------------------------------------------------------------------------
BOOL	KPlayer::ApplyAddSkillLevel(int nSkillID, int nAddPoint)
{
	if ( !Npc[m_nIndex].m_SkillList.FindSame(nSkillID) )
		return FALSE;

int nLevelSkillMax = 0;

if (m_cTask.GetSaveVal(139) == 1)
{
nLevelSkillMax += 1;
}
else if (m_cTask.GetSaveVal(139) == 2)
{
nLevelSkillMax += 2;
}
else if (m_cTask.GetSaveVal(139) == 3)
{
nLevelSkillMax += 4;
}
else if (m_cTask.GetSaveVal(139) == 4)
{
nLevelSkillMax += 6;
}
else if (m_cTask.GetSaveVal(139) == 5)
{
nLevelSkillMax += 8;
}



	if (m_nSkillPoint < nAddPoint || nAddPoint + Npc[m_nIndex].m_SkillList.GetLevel(nSkillID) > (nLevelSkillMax + MAX_SKILLLEVEL) || nAddPoint + Npc[m_nIndex].m_SkillList.GetCurrentLevel(nSkillID) > (nLevelSkillMax + MAX_SKILLLEVEL))
		return FALSE;
	
	PLAYER_ADD_SKILL_POINT_COMMAND	sSkill;
	sSkill.ProtocolType = c2s_playerapplyaddskillpoint;
	sSkill.m_nSkillID = nSkillID;
	sSkill.m_nAddPoint = nAddPoint;
	
	if (g_pClient)
		g_pClient->SendPackToServer(&sSkill, sizeof(PLAYER_ADD_SKILL_POINT_COMMAND));
	
	return TRUE;
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÏò·þÎñÆ÷ÉêÇëÊ¹ÓÃÄ³¸öÎïÆ·£¨Êó±êÓÒ¼üµã»÷¸ÃÎïÆ·£¬Ö»ÄÜÓÃÓÚ³ÔÒ©£©
//-------------------------------------------------------------------------
BOOL	KPlayer::ApplyUseItem(int nItemID, ItemPos SrcPos)
{
	//pius start 05/12/2023
	ItemPos DesPos;
    DesPos.nPlace = DesPos.nX = DesPos.nY = -1;
    KSystemMessage	sMsg;

    if (this->CheckTrading())
        return FALSE;
    int nRet = this->m_ItemList.UseItem(nItemID);
    if (nRet == 0)
    {

        nRet = m_ItemList.ChangeItemInPlayer(nItemID);//edit by phong kieu mac trang bi vao nguoi
        switch(nRet)
        {
            case equip_horse:
                DesPos.nPlace = 2;
                DesPos.nX = 10;
                strcpy(sMsg.szMessage, "Mang ngùa ...");
                break;
            case equip_meleeweapon:
                DesPos.nPlace = 2;
                DesPos.nX = 3;
                strcpy(sMsg.szMessage, "Mang vò khÝ cËn chiÕn ...");
                break;
            case equip_rangeweapon:
                DesPos.nPlace = 2;
                DesPos.nX = 3;
                strcpy(sMsg.szMessage, "Mang vò khÝ tÇm xa ...");
                break;
            case equip_armor:
                DesPos.nPlace = 2;
                DesPos.nX = 1;
                strcpy(sMsg.szMessage, "Mang y phôc ...");
                break;
            case equip_ring:
                DesPos.nPlace = 2;
                DesPos.nX = 7;
                strcpy(sMsg.szMessage, "Mang nhÉn1...");
				if(m_ItemList.m_EquipItem[7] > 0 && m_ItemList.m_EquipItem[8] == 0)
				DesPos.nX = 8;
				strcpy(sMsg.szMessage, "Mang nhÉn2 ...");
                break;
            case equip_amulet:
                DesPos.nPlace = 2;
                DesPos.nX = 6;
                strcpy(sMsg.szMessage, "Mang d©y chuyÒn ...");
                break;
            case equip_boots:
                DesPos.nPlace = 2;
                DesPos.nX = 4;
                strcpy(sMsg.szMessage, "Mang giµy ...");
                break;
            case equip_belt:
                DesPos.nPlace = 2;
                DesPos.nX = 2;
                strcpy(sMsg.szMessage, "Mang ®ai l­ng ...");
                break;
            case equip_helm:
                DesPos.nPlace = 2;
                DesPos.nX = 0;
                strcpy(sMsg.szMessage, "Mang mò ...");
                break;
            case equip_cuff:
                DesPos.nPlace = 2;
                DesPos.nX = 5;
                strcpy(sMsg.szMessage, "Mang bao tay ...");
                break;
            case equip_pendant:
                DesPos.nPlace = 2;
                DesPos.nX = 9;
                strcpy(sMsg.szMessage, "Mang ngäc béi ...");
                break;
			case equip_mask:
                DesPos.nPlace = 2;
                DesPos.nX = 11;
                strcpy(sMsg.szMessage, "Mang ngäc béi ...");
                break;
			case equip_fifong:
				DesPos.nPlace = 2;
				DesPos.nX = 12;
				strcpy(sMsg.szMessage, "Mang phi phong ...");
				break;
		//	case equip_signet:
          //      DesPos.nPlace = 2;
          //      DesPos.nX = 13;
          //      strcpy(sMsg.szMessage, "Mang ngäc béi ...");
          //      break;
		
        }
        if(DesPos.nPlace<0)
        {
            return FALSE;
        }

        PLAYER_MOVE_ITEM_COMMAND	sMove;
        sMove.ProtocolType = c2s_playermoveitem;
        sMove.m_btDownPos = SrcPos.nPlace;
        sMove.m_btDownX = SrcPos.nX;
        sMove.m_btDownY = SrcPos.nY;
        sMove.m_btUpPos = SrcPos.nPlace;
        sMove.m_btUpX = SrcPos.nX;
        sMove.m_btUpY = SrcPos.nY;
        if (g_pClient)
            g_pClient->SendPackToServer(&sMove, sizeof(PLAYER_MOVE_ITEM_COMMAND));

        PLAYER_MOVE_ITEM_COMMAND	sMove2;
        sMove2.ProtocolType = c2s_playermoveitem;
        sMove2.m_btDownPos = DesPos.nPlace;
        sMove2.m_btDownX = DesPos.nX;
        sMove2.m_btDownY = DesPos.nY;
        sMove2.m_btUpPos = DesPos.nPlace;
        sMove2.m_btUpX = DesPos.nX;
        sMove2.m_btUpY = DesPos.nY;
        if (g_pClient)
            g_pClient->SendPackToServer(&sMove2, sizeof(PLAYER_MOVE_ITEM_COMMAND));

        if (g_pClient)
            g_pClient->SendPackToServer(&sMove, sizeof(PLAYER_MOVE_ITEM_COMMAND));

        sMsg.byConfirmType = SMCT_NONE;
        sMsg.eType = SMT_NORMAL;
        sMsg.byParamSize = 0;
        sMsg.byPriority = 0;
        CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);

        m_ItemList.LockOperation();

        return FALSE;
    }


    if (m_dwRightMouse && m_dwRightMouse > g_SubWorldSet.GetGameTime())
        return FALSE;

    m_dwRightMouse = g_SubWorldSet.GetGameTime() + GAME_FPS / 3;
    m_nLastNpcIndex = 0;

    PLAYER_EAT_ITEM_COMMAND	sEat;
    sEat.ProtocolType = c2s_playereatitem;
    sEat.m_nItemID = nItemID;
    sEat.m_btPlace = SrcPos.nPlace;
    sEat.m_btX = SrcPos.nX;
    sEat.m_btY = SrcPos.nY;
    if (g_pClient)
        g_pClient->SendPackToServer(&sEat, sizeof(PLAYER_EAT_ITEM_COMMAND));

    if (nRet != REQUEST_EAT_MEDICINE && Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Doing == do_sit)
        Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SendCommand(do_stand);

    return TRUE;

	//pius end
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£º¿Í»§¶ËÊó±êµã»÷obj¼ìÆðÄ³¸öÎïÆ·£¬Ïò·þÎñÆ÷·¢ÏûÏ¢
//-------------------------------------------------------------------------
void	KPlayer::PickUpObj(int nObjIndex)
{
	if (this->CheckTrading())
		return;
	if (nObjIndex <= 0)
		return;
	if (Object[nObjIndex].m_nKind != Obj_Kind_Item && Object[nObjIndex].m_nKind != Obj_Kind_Money && Object[nObjIndex].m_nKind != Obj_Kind_Prop)
		return;
	
	PLAYER_PICKUP_ITEM_COMMAND	sPickUp;
	if (Object[nObjIndex].m_nKind == Obj_Kind_Money || Object[nObjIndex].m_nKind == Obj_Kind_Prop)
	{
		sPickUp.ProtocolType = c2s_playerpickupitem;
		sPickUp.m_nObjID = Object[nObjIndex].m_nID;
		sPickUp.m_btPosType = 0;
		sPickUp.m_btPosX = 0;
		sPickUp.m_btPosY = 0;
	}
	else
	{
		ItemPos	sItemPos;
		if ( FALSE == m_ItemList.SearchPosition(Object[nObjIndex].m_nItemWidth, Object[nObjIndex].m_nItemHeight, &sItemPos) )
		{
			KSystemMessage	sMsg;
			
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.eType = SMT_NORMAL;
			sMsg.byParamSize = 0;
			sMsg.byPriority = 0;
			
			strcpy(sMsg.szMessage, MSG_SHOP_NO_ROOM);
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
			return;
		}
		sPickUp.ProtocolType = c2s_playerpickupitem;
		sPickUp.m_nObjID = Object[nObjIndex].m_nID;
		sPickUp.m_btPosType = sItemPos.nPlace;
		sPickUp.m_btPosX = sItemPos.nX;
		sPickUp.m_btPosY = sItemPos.nY;
	}
	
	if (g_pClient)
		g_pClient->SendPackToServer(&sPickUp, sizeof(PLAYER_PICKUP_ITEM_COMMAND));
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£º¿Í»§¶ËÊó±êµã»÷obj£¬Ïò·þÎñÆ÷·¢ÏûÏ¢
//-------------------------------------------------------------------------
void	KPlayer::ObjMouseClick(int nObjIndex)
{
	if (this->CheckTrading())
		return;
	if (nObjIndex <= 0)
		return;
	if (Object[nObjIndex].m_nKind != Obj_Kind_Box)
		return;
	SendObjMouseClick(Object[nObjIndex].m_nID, SubWorld[0].m_Region[Object[nObjIndex].m_nRegionIdx].m_RegionID);
}
#endif

#ifndef _SERVER
// DownPos ÊÇÃæ°åÉÏµÄÎïÆ·µÄµ±Ç°×ø±ê£¬UpPos ±ØÐëÊÇÊÖÉÏÎïÆ··Åµ½Ãæ°åÉÏµÄ×ø±ê
void	KPlayer::MoveItem(ItemPos DownPos, ItemPos UpPos)
{
	if (this->CheckTrading() && DownPos.nPlace == pos_traderoom)
	{
		if (this->m_cTrade.m_nTradeLock)
			return;
		
		// ÈÎÎñÎïÆ·µÄÏÞÖÆ
		
	}
	//g_DebugLog("Ten Item: %s - %d",Item[m_ItemList.m_EquipItem[12]].GetName(),m_ItemList.m_EquipItem[12]);
	//g_DebugLog("MoveItem %d %d %d %d %d %d",DownPos.nPlace, DownPos.nX, DownPos.nY, UpPos.nPlace, UpPos.nX, UpPos.nY); 


	
	if (!CheckTrading() && DownPos.nPlace == pos_immediacy)
	{
		int nHandIdx = m_ItemList.Hand();
		if (nHandIdx)
		{
			if (m_ItemList.m_Room[room_immediacy].CheckSameDetailType(Item[nHandIdx].GetGenre(), Item[nHandIdx].GetDetailType()))
			{
				KSystemMessage	sMsg;
				sMsg.eType = SMT_NORMAL;
				sMsg.byConfirmType = SMCT_NONE;
				sMsg.byPriority = 0;
				sMsg.byParamSize = 0;
				sprintf(sMsg.szMessage, MSG_ITEM_SAME_DETAIL_IN_IMMEDIATE);
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
				return;
			}
		}
	}
	else if (DownPos.nPlace == pos_traderoom)
		{
		int nHandIdx = m_ItemList.Hand();
		if (nHandIdx)
		{
			if (((Item[nHandIdx].GetGenre() == item_equip || Item[nHandIdx].GetGenre() == item_medicine) && Item[nHandIdx].m_GeneratorParam.nVersion > 1) || (Item[nHandIdx].GetGenre() == item_task && Item[nHandIdx].GetBindItem() >= 1))
			{
				KSystemMessage	sMsg;
				sMsg.eType = SMT_NORMAL;
				sMsg.byConfirmType = SMCT_NONE;
				sMsg.byPriority = 0;
				sMsg.byParamSize = 0;
				sprintf(sMsg.szMessage, "Trang bÞ ®· ®Ýnh b¶o hiÓm, kh«ng thÓ giao dÞch !");
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
				return;
			}
		}
	}
	
	SendClientCmdMoveItem(&DownPos, &UpPos);
	//g_DebugLog("MoveItem %d %d %d %d %d %d",DownPos.nPlace, DownPos.nX, DownPos.nY, UpPos.nPlace, UpPos.nX, UpPos.nY); 
	
}
#endif

#ifndef _SERVER
int	KPlayer::ThrowAwayItem()
{
	if (this->CheckTrading())
		return 0;
	if ( !m_ItemList.Hand() )
		return 0;

	int nItemIdx = m_ItemList.m_Hand;

	if (((Item[nItemIdx].GetGenre() == item_equip || Item[nItemIdx].GetGenre() == item_medicine) && Item[nItemIdx].m_GeneratorParam.nVersion > 1 ) || ((Item[nItemIdx].GetGenre() == item_task && Item[nItemIdx].GetBindItem() >= 1)))
	{
			KSystemMessage	sMsg;
				sMsg.eType = SMT_NORMAL;
				sMsg.byConfirmType = SMCT_NONE;
				sMsg.byPriority = 0;
				sMsg.byParamSize = 0;
				sprintf(sMsg.szMessage, "Trang bÞ ®· ®Ýnh b¶o hiÓm, kh«ng thÓ nÐm ra ngoµi !");
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		return 0;
	}
	




	PLAYER_THROW_AWAY_ITEM_COMMAND	sThrow;
	
	sThrow.ProtocolType = c2s_playerthrowawayitem;
	
	if (g_pClient)
	{
		
		g_DebugLog("LocDo4 %d ",nItemIdx);
		g_pClient->SendPackToServer(&sThrow, sizeof(PLAYER_THROW_AWAY_ITEM_COMMAND));
	}
	
	return 1;
}
#endif

#ifndef _SERVER
void	KPlayer::ChatAddFriend(int nPlayerIdx)
{
	CHAT_ADD_FRIEND_COMMAND	sAdd;
	sAdd.ProtocolType = c2s_chataddfriend;
	sAdd.m_nTargetPlayerIdx = nPlayerIdx;
	if (g_pClient)
		g_pClient->SendPackToServer(&sAdd, sizeof(CHAT_ADD_FRIEND_COMMAND));
}
#endif

#ifndef _SERVER
void	KPlayer::ChatRefuseFriend(int nPlayerIdx)
{
	CHAT_REFUSE_FRIEND_COMMAND	sRefuse;
	sRefuse.ProtocolType = c2s_chatrefusefriend;
	sRefuse.m_nTargetPlayerIdx = nPlayerIdx;
	if (g_pClient)
		g_pClient->SendPackToServer(&sRefuse, sizeof(CHAT_REFUSE_FRIEND_COMMAND));
}
#endif

#ifndef _SERVER
void	KPlayer::TradeApplyOpen(char *lpszSentence, int nLength)
{
	if (this->CheckTrading())
		return;
	if (!lpszSentence)
		nLength = 0;
	TRADE_APPLY_OPEN_COMMAND	sOpen;
	sOpen.ProtocolType = c2s_tradeapplystateopen;
	if (nLength >= MAX_SENTENCE_LENGTH)
		nLength = MAX_SENTENCE_LENGTH - 1;
	sOpen.m_wLength = sizeof(TRADE_APPLY_OPEN_COMMAND) - 1 - sizeof(sOpen.m_szSentence) + nLength;
	memset(m_cMenuState.m_szSentence, 0, sizeof(m_cMenuState.m_szSentence));
	if (lpszSentence)
		memcpy(m_cMenuState.m_szSentence, lpszSentence, nLength);
	memset(sOpen.m_szSentence, 0, sizeof(sOpen.m_szSentence));
	memcpy(sOpen.m_szSentence, m_cMenuState.m_szSentence, nLength);
	if (g_pClient)
		g_pClient->SendPackToServer(&sOpen, sOpen.m_wLength + 1);
}
#endif

#ifndef _SERVER
void	KPlayer::TradeApplyClose()
{
	TRADE_APPLY_CLOSE_COMMAND	sApply;
	sApply.ProtocolType = c2s_tradeapplystateclose;
	if (g_pClient)
		g_pClient->SendPackToServer(&sApply, sizeof(TRADE_APPLY_CLOSE_COMMAND));
}
#endif

#ifndef _SERVER
void	KPlayer::TradeApplyStart(int nNpcIdx)
{
	if (nNpcIdx < 0 || nNpcIdx >= MAX_NPC)
		return;
	TRADE_APPLY_START_COMMAND	sStart;
	sStart.ProtocolType = c2s_tradeapplystart;
	sStart.m_dwID = Npc[nNpcIdx].m_dwID;
	if (g_pClient)
		g_pClient->SendPackToServer(&sStart, sizeof(TRADE_APPLY_START_COMMAND));
	
	KSystemMessage	sMsg;
	sprintf(sMsg.szMessage, MSG_TRADE_SEND_APPLY, Npc[nNpcIdx].Name);
	sMsg.eType = SMT_NORMAL;
	sMsg.byConfirmType = SMCT_NONE;
	sMsg.byPriority = 0;
	sMsg.byParamSize = 0;
	CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
}
#endif

#ifndef _SERVER
BOOL	KPlayer::TradeMoveMoney(int nMoney)
{
	if (!CheckTrading())
		return FALSE;
	if (this->m_cTrade.m_nTradeLock)
		return FALSE;
	// Ç®ÊýÁ¿´íÎó
	if (nMoney < 0)
		nMoney = 0;
	else if (nMoney > (m_ItemList.GetEquipmentMoney() + m_ItemList.GetMoney(room_trade)))
	    nMoney = (m_ItemList.GetEquipmentMoney() + m_ItemList.GetMoney(room_trade));
	
	
	TRADE_MOVE_MONEY_COMMAND	sMoney;
	
	sMoney.ProtocolType = c2s_trademovemoney;
	sMoney.m_nMoney = nMoney;
	if (g_pClient)
		g_pClient->SendPackToServer(&sMoney, sizeof(TRADE_MOVE_MONEY_COMMAND));
	
	return TRUE;
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£º½»Ò×È·¶¨»òÈ¡Ïû
//	if nDecision == 0 ÍÆ³ö½»Ò×  if nDecision == 1 È·¶¨½»Ò×  if nDecision == 2 È¡Ïû½»Ò×È·¶¨
//-------------------------------------------------------------------------
void	KPlayer::TradeDecision(int nDecision)
{
	
	if ( !CheckTrading() )
		return;
	TRADE_DECISION_COMMAND	sDecision;
	sDecision.ProtocolType = c2s_tradedecision;
	if (nDecision == 1)
	{
		sDecision.m_btDecision = 1;
	}
	else if (nDecision == 0)
	{
		sDecision.m_btDecision = 0;
	}
	else if (nDecision == 2)
	{
		sDecision.m_btDecision = 4;
	}
	if (g_pClient)
		g_pClient->SendPackToServer(&sDecision, sizeof(TRADE_DECISION_COMMAND));
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£º½»Ò×Ëø¶¨»òÈ¡ÏûËø¶¨
//-------------------------------------------------------------------------
void	KPlayer::TradeApplyLock(int nLockOrNot)
{
	if ( !CheckTrading() )
		return;
	TRADE_DECISION_COMMAND	sDecision;
	sDecision.ProtocolType = c2s_tradedecision;
	if (nLockOrNot)
	{
		sDecision.m_btDecision = 2;
	}
	else
	{
		sDecision.m_btDecision = 3;
	}
	if (g_pClient)
		g_pClient->SendPackToServer(&sDecision, sizeof(TRADE_DECISION_COMMAND));
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÊÕµ½·þÎñÆ÷Í¨ÖªÓÐÈËÉêÇë¼ÓÈë¶ÓÎé
//-------------------------------------------------------------------------
void	KPlayer::s2cApplyAddTeam(BYTE* pProtocol)
{
	// ×´Ì¬¼ì²é (»¹ÐèÒª¼ì²é npc µÄ¶ÓÎé¿ª·Å×´Ì¬ not end)
	if ( !m_cTeam.m_nFlag || m_cTeam.m_nFigure != TEAM_CAPTAIN)
	{
		ApplySelfTeamInfo();
		return;
	}
	
	g_DebugLog("AAAA 1");
	// Ñ°ÕÒ npc
	PLAYER_APPLY_ADD_TEAM	*pAddTeam = (PLAYER_APPLY_ADD_TEAM*)pProtocol;
	int nNpcNo = NpcSet.SearchID(pAddTeam->m_dwTarNpcID);
	if (nNpcNo == 0)
		return;
	
	int i, nFreeListNo;
	// Èç¹ûÒÑ¾­´æÔÚ£¬¸üÐÂ
	for (i = 0; i < MAX_TEAM_APPLY_LIST; i++)
	{
		if (m_cTeam.m_sApplyList[i].m_dwNpcID == pAddTeam->m_dwTarNpcID)
		{
			m_cTeam.m_sApplyList[i].m_nLevel = Npc[nNpcNo].m_Level;
			m_cTeam.m_sApplyList[i].m_dwTimer = MAX_APPLY_TEAM_TIME;
			strcpy(m_cTeam.m_sApplyList[i].m_szName, Npc[nNpcNo].Name);
			m_cTeam.UpdateInterface();
			return;
		}
	}
	// Ñ°ÕÒÉêÇëÈËlist¿ÕÎ»
	for (i = 0; i < MAX_TEAM_APPLY_LIST; i++)
	{
		if (m_cTeam.m_sApplyList[i].m_dwNpcID == 0)
		{
			nFreeListNo = i;
			break;
		}
	}
	if (i >= MAX_TEAM_APPLY_LIST)	// Ã»ÕÒµ½ÉêÇëÈË¿ÕÎ»
		return;
	
	m_cTeam.m_sApplyList[nFreeListNo].m_dwNpcID = pAddTeam->m_dwTarNpcID;
	m_cTeam.m_sApplyList[nFreeListNo].m_nLevel = Npc[nNpcNo].m_Level;
	m_cTeam.m_sApplyList[nFreeListNo].m_dwTimer = MAX_APPLY_TEAM_TIME;
	strcpy(m_cTeam.m_sApplyList[nFreeListNo].m_szName, Npc[nNpcNo].Name);
	
	m_cTeam.UpdateInterface();
	
	KSystemMessage	sMsg;
	sprintf(sMsg.szMessage, MSG_TEAM_APPLY_ADD, Npc[nNpcNo].Name);
	sMsg.eType = SMT_TEAM;
	sMsg.byConfirmType = SMCT_UI_TEAM_APPLY;
	sMsg.byPriority = 3;
	sMsg.byParamSize = sizeof(KUiPlayerItem);

	KUiPlayerItem	player;
	strcpy(player.Name, Npc[nNpcNo].Name);
	player.nIndex = 0;
	player.uId = pAddTeam->m_dwTarNpcID;
	CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, (int)&player);
	
	return;
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£º¼ÓÈëÃÅÅÉ
//-------------------------------------------------------------------------
BOOL	KPlayer::AddFaction(char *lpszFactionName)
{
	if ( !m_cFaction.AddFaction(Npc[m_nIndex].m_Series, lpszFactionName) )
		return FALSE;
	
	// ¸Ä±äÕóÓª
	Npc[m_nIndex].SetCamp(m_cFaction.GetGurFactionCamp());
	
	// ¸ø¿Í»§¶Ë·¢ÏûÏ¢
	SendFactionData();
	
	return TRUE;
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£º¼ÓÈëÃÅÅÉ
//-------------------------------------------------------------------------
BOOL	KPlayer::AddFaction(int nFactionID)
{
	if ( !m_cFaction.AddFaction(Npc[m_nIndex].m_Series, nFactionID) )
		return FALSE;
	
	// ¸Ä±äÕóÓª
	Npc[m_nIndex].SetCamp(m_cFaction.GetGurFactionCamp());
	
	// ¸ø¿Í»§¶Ë·¢ÏûÏ¢
	SendFactionData();
	
	return TRUE;
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÀë¿ªÃÅÅÉ
//-------------------------------------------------------------------------
BOOL	KPlayer::LeaveCurFaction()
{
	m_cFaction.LeaveFaction();
	
	// ¸Ä±äÕóÓª
	Npc[m_nIndex].SetCamp(camp_free);
	
	// ¸ø¿Í»§¶Ë·¢ÏûÏ¢
	PLAYER_LEAVE_FACTION	sLeave;
	sLeave.ProtocolType = s2c_playerleavefaction;
	g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sLeave, sizeof(PLAYER_LEAVE_FACTION));
	
	return TRUE;
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£º¿ª·Åµ±Ç°ÃÅÅÉÄ³¸öµÈ¼¶µÄ¼¼ÄÜ
//-------------------------------------------------------------------------
BOOL	KPlayer::CurFactionOpenSkill(int nLevel)
{
	if ( !m_cFaction.OpenCurSkillLevel(nLevel, &Npc[m_nIndex].m_SkillList) )
		return FALSE;
	
	PLAYER_FACTION_SKILL_LEVEL	sLevel;
	sLevel.ProtocolType = s2c_playerfactionskilllevel;
	sLevel.m_btCurFactionID = m_cFaction.m_nCurFaction;
	sLevel.m_btLevel = nLevel;
	g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sLevel, sizeof(PLAYER_FACTION_SKILL_LEVEL));
	
	return TRUE;
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÇå¿ÕÈÎÎñÁÙÊ±±äÁ¿
//-------------------------------------------------------------------------
void	KPlayer::TaskClearTempVal()
{
	this->m_cTask.ClearTempVar();
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºµÃµ½ÈÎÎñÍê³ÉÇé¿ö
//-------------------------------------------------------------------------
int		KPlayer::TaskGetSaveVal(int nNo)
{
	return this->m_cTask.GetSaveVal(nNo);
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÉè¶¨ÈÎÎñÍê³ÉÇé¿ö
//-------------------------------------------------------------------------
void	KPlayer::TaskSetSaveVal(int nNo, BOOL bFlag)
{
this->m_cTask.SetSaveVal(nNo, bFlag);
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºµÃµ½ÈÎÎñÁÙÊ±¹ý³Ì¿ØÖÆ±äÁ¿Öµ
//-------------------------------------------------------------------------
int		KPlayer::TaskGetClearVal(int nNo)
{
	return this->m_cTask.GetClearVal(nNo);
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÉè¶¨ÈÎÎñÁÙÊ±¹ý³Ì¿ØÖÆ±äÁ¿Öµ
//-------------------------------------------------------------------------
void	KPlayer::TaskSetClearVal(int nNo, int nVal)
{
	this->m_cTask.SetClearVal(nNo, nVal);
}
#endif


#ifndef _SERVER
void	KPlayer::SetDefaultImmedSkill()
{
	//»ñÈ¡Íæ¼Ò×óÓÒ¼ü¼¼ÄÜÎªÄ¬ÈÏÎïÀí¼¼ÄÜ
	int nDetailType = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetWeaponType();
	int nParticularType = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetWeaponParticular();
		
	//½üÉíÎäÆ÷
	if (nDetailType == 0)
	{
		Player[CLIENT_PLAYER_INDEX].SetLeftSkill(g_nMeleeWeaponSkill[nParticularType]);
		Player[CLIENT_PLAYER_INDEX].SetRightSkill(g_nMeleeWeaponSkill[nParticularType]);
	}//Ô¶³ÌÎäÆ÷
	else if (nDetailType == 1)
	{
		Player[CLIENT_PLAYER_INDEX].SetLeftSkill(g_nRangeWeaponSkill[nParticularType]);
		Player[CLIENT_PLAYER_INDEX].SetRightSkill(g_nRangeWeaponSkill[nParticularType]);
	}//¿ÕÊÖ
	else if (nDetailType == -1)
	{
		Player[CLIENT_PLAYER_INDEX].SetLeftSkill(g_nHandSkill);
		Player[CLIENT_PLAYER_INDEX].SetRightSkill(g_nHandSkill);
	}
}

void	KPlayer::SetLeftSkill(int nSkillID)
{
	if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.GetCurrentLevel(nSkillID) <= 0) return;
	m_nLeftSkillID = nSkillID;
	
	KUiGameObject Info;
	Info.uGenre = CGOG_SKILL_SHORTCUT;
	Info.uId = m_nLeftSkillID;
	CoreDataChanged(GDCNI_PLAYER_IMMED_ITEMSKILL, (unsigned int)&Info, -1);
}

void	KPlayer::SetRightSkill(int nSkillID)
{
	if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.GetCurrentLevel(nSkillID) <= 0) return;
	m_nRightSkillID = nSkillID;
	// ¼ì²éÊÇ·ñÊÇ¹â»·¼¼ÄÜ
	ISkill * pOrdinSkill = g_SkillManager.GetSkill(nSkillID, 1);
	if (!pOrdinSkill) 
        return;
	
	if (pOrdinSkill->IsAura())
	{
		Npc[m_nIndex].SetAuraSkill(nSkillID);
	}
	else
	{
		Npc[m_nIndex].SetAuraSkill(0);
	}
	
	KUiGameObject Info;
	Info.uGenre = CGOG_SKILL_SHORTCUT;
	Info.uId = m_nRightSkillID;
	CoreDataChanged(GDCNI_PLAYER_IMMED_ITEMSKILL, (unsigned int)&Info, -2);
}

void KPlayer::UpdateWeaponSkill()
{
	if (m_nLeftSkillID > 0)
	{
		ISkill * pISkill = g_SkillManager.GetSkill(m_nLeftSkillID, 1);
		if (!pISkill) 
            return;
		
		if (pISkill->IsPhysical())
		{
			SetLeftSkill(Npc[m_nIndex].GetCurActiveWeaponSkill());
		}
		
	}
	if (m_nRightSkillID > 0)
	{
		ISkill * pISkill = (KSkill *) g_SkillManager.GetSkill(m_nRightSkillID, 1);
		if (!pISkill) 
            return;
		
		if (pISkill->IsPhysical())
		{
			SetRightSkill(Npc[m_nIndex].GetCurActiveWeaponSkill());
		}
	}
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÉè¶¨µ±Ç°ÁÄÌìÆµµÀ
//-------------------------------------------------------------------------
void	KPlayer::SetChatCurChannel(int nChannelNo)
{
	m_cChat.SetCurChannel(nChannelNo);
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÑûÇë¼ÓÈë¶ÓÎé
//-------------------------------------------------------------------------
void	KPlayer::TeamInviteAdd(DWORD dwNpcID)
{
	m_cTeam.InviteAdd(dwNpcID);
}
#endif





#ifdef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÊÕµ½¿Í»§¶Ë·¢À´µÄÁÄÌìÓï¾ä
//-------------------------------------------------------------------------
void	KPlayer::ServerSendChat(BYTE* pProtocol)
{
	m_cChat.ServerSendChat(m_nPlayerIndex, pProtocol);
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÊÕµ½¿Í»§¶ËÒªÇóÔö¼Ó»ù±¾ÊôÐÔµã(0=Strength 1=Dexterity 2=Vitality 3=Engergy)
//-------------------------------------------------------------------------
void	KPlayer::AddBaseAttribute(BYTE* pProtocol)
{
	PLAYER_ADD_BASE_ATTRIBUTE_COMMAND	*pAdd = (PLAYER_ADD_BASE_ATTRIBUTE_COMMAND*)pProtocol;
	switch (pAdd->m_btAttribute)
	{
	case ATTRIBUTE_STRENGTH:
		AddBaseStrength(pAdd->m_nAddNo);
		break;
	case ATTRIBUTE_DEXTERITY:
		AddBaseDexterity(pAdd->m_nAddNo);
		break;
	case ATTRIBUTE_VITALITY:
		AddBaseVitality(pAdd->m_nAddNo);
		break;
	case ATTRIBUTE_ENGERGY:
		AddBaseEngergy(pAdd->m_nAddNo);
		break;
	}
}
#endif

#define MAX_ORDINSKILL_LEVEL_ALWAYS  20//²»°üÀ¨ÆäËüÇé¿ö¶Ô¼¼ÄÜµÈ¼¶µÄ±ä¶¯Ö®ÍâµÄ£¬Ò»°ã×î´ó¼¼ÄÜµÈ¼¶
#ifdef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÊÕµ½¿Í»§¶ËÒªÇóÔö¼ÓÄ³¸ö¼¼ÄÜµÄµãÊý
//-------------------------------------------------------------------------
void	KPlayer::AddSkillPoint(BYTE* pProtocol)
{
	PLAYER_ADD_SKILL_POINT_COMMAND	*pAdd = (PLAYER_ADD_SKILL_POINT_COMMAND*)pProtocol;
	
	int		nSkillIndex, nSkillLevel;
	
	nSkillIndex = Npc[m_nIndex].m_SkillList.FindSame(pAdd->m_nSkillID);
	if (nSkillIndex == 0)
		return;
	nSkillLevel = Npc[m_nIndex].m_SkillList.GetSkillIdxLevel(nSkillIndex);
	
	
	if (m_nSkillPoint >= pAdd->m_nAddPoint && nSkillIndex )//&& nSkillLevel + pAdd->m_nAddPoint <= MAX_SKILLLEVEL
	{
		ISkill * pSkill = NULL;
		if (nSkillLevel == 0)
		{
			pSkill =  g_SkillManager.GetSkill(pAdd->m_nSkillID, 1);
		}
		else
		{
			pSkill =  g_SkillManager.GetSkill(pAdd->m_nSkillID, nSkillLevel);
		}
		
		if (pSkill)
		{
			//ÓÐ¶ÔÓ¦µÄ¼¼ÄÜÉý¼¶½Å±¾,ÓÅÏÈÖ´ÐÐ½Å±¾
			if (pSkill->GetSkillLevelUpScriptId())
			{
				ExecuteScript(pSkill->GetSkillLevelUpScriptId(), "main", pAdd->m_nAddPoint);
				nSkillLevel = Npc[m_nIndex].m_SkillList.GetSkillIdxLevel(nSkillIndex);
			}
			else
			{
			/*
			¼¼ÄÜÉý¼¶µÄ³£¹æ
			1¡£ËùÓÐÄÜ¹»¼ÓµãµÄ¼¼ÄÜ£¬×î¸ßÖ»ÄÜ¼Óµ½20¼¶¡£
			2¡£¼ÙÉèÒ»¸öÍæ¼ÒµÄµÈ¼¶ÊÇx,Ò»¸ö¼¼ÄÜµÄ»ù±¾Ñ§Ï°µÈ¼¶ÊÇy£¬ÄÇÃ´ËûÄÜ¹»Ñ§Ï°Õâ¸ö¼¼ÄÜµÄ×î¸ßµÈ¼¶ÊÇx-y¡£
			3¡£Ê¹ÓÃ¼¼ÄÜÐèÒª¼ÓÈëÒ»Ð©Çé¿öµÄÅÐ¶Ï£¬µÚÒ»£¬ÊÇ·ñÊÇÆïÂí×´Ì¬£»µÚ¶þ£¬Ëû×°±¸µÄÎäÆ÷µÄÏêÏ¸Àà±ðÊÇ·ñÎªÖ¸¶¨ÊýÖµ£¬-1±íÊ¾Ã»ÓÐÐèÇó¡£
				*/
				switch(pSkill->GetSkillStyle())
				{
				case SKILL_SS_Missles:			//	×Óµ¯Àà		±¾¼¼ÄÜÓÃÓÚ·¢ËÍ×Óµ¯Àà
				case SKILL_SS_Melee:
				case SKILL_SS_InitiativeNpcState:	//	Ö÷¶¯Àà		±¾¼¼ÄÜÓÃÓÚ¸Ä±äµ±Ç°NpcµÄÖ÷¶¯×´Ì¬
				case SKILL_SS_PassivityNpcState:		//	±»¶¯Àà		±¾¼¼ÄÜÓÃÓÚ¸Ä±äNpcµÄ±»¶¯×´Ì¬
					{
						int nWantToBeLevel = nSkillLevel + pAdd->m_nAddPoint;
						//Limit2
						int nLevelSkillMax = g_SkillManager.GetSkillMaxLevel(pAdd->m_nSkillID);

int nLevelSkillMaxAdd = 0;
if (m_cTask.GetSaveVal(139) == 1)
{
nLevelSkillMaxAdd += 1;
}
else if (m_cTask.GetSaveVal(139) == 2)
{
nLevelSkillMaxAdd += 2;
}
else if (m_cTask.GetSaveVal(139) == 3)
{
nLevelSkillMaxAdd += 4;
}
else if (m_cTask.GetSaveVal(139) == 4)
{
nLevelSkillMaxAdd += 6;
}
else if (m_cTask.GetSaveVal(139) == 5)
{
nLevelSkillMaxAdd += 8;
}


						if (nLevelSkillMax > 1 && nWantToBeLevel <= (nLevelSkillMax + nLevelSkillMaxAdd) && nWantToBeLevel <= Npc[m_nIndex].m_Level - ((KSkill*)pSkill)->GetSkillReqLevel() + 1)
						{
							if ( !Npc[m_nIndex].m_SkillList.IncreaseLevel(nSkillIndex, pAdd->m_nAddPoint) )
								return;
								
							m_nSkillPoint -= pAdd->m_nAddPoint;
							nSkillLevel += pAdd->m_nAddPoint;
						}
						else
						{
							if (nWantToBeLevel >=  nLevelSkillMax)
							{
								char szMsg[100];
								sprintf(szMsg, "ÄúµÄ%s¼¼ÄÜµÈ¼¶ÒÑ¾­´ïµ½¼«ÏÞ£¬²»ÄÜÔÙÉý¼¶ÁË¡£", pSkill->GetSkillName());
								KPlayerChat::SendSystemInfo(1, m_nPlayerIndex, "¼¼ÄÜÉý¼¶ÏûÏ¢", (char *) szMsg, strlen(szMsg) );
							}
							else 
							{
								char szMsg[100];
								sprintf(szMsg, "ÄúµÄÕ½¶·¼¼ÄÜ±ØÐë´ïµ½%d¼¶²ÅÄÜÉý¼¶%s¼¼ÄÜ¡£", ((KSkill *)pSkill)->GetSkillReqLevel() -1 +  pAdd->m_nAddPoint + nSkillLevel,  ((KSkill*)pSkill)->GetSkillName());
								KPlayerChat::SendSystemInfo(1, m_nPlayerIndex, "¼¼ÄÜÉý¼¶ÏûÏ¢", (char *) szMsg, strlen(szMsg) );
							}
							
						}
					}
					break;
					
				case SKILL_SS_Thief:
					{
						return;
					}break;
					
				}
				
			}
			
			// °Ñµ±Ç°´Ë¼¼ÄÜµÄµãÊýºÍÊ£Óà¼¼ÄÜµã·¢¸ø¿Í»§¶Ë
			PLAYER_SKILL_LEVEL_SYNC	sSkill;
			sSkill.ProtocolType = s2c_playerskilllevel;
			sSkill.m_nSkillID = pAdd->m_nSkillID;
			sSkill.m_nSkillLevel = nSkillLevel;
			sSkill.m_nLeavePoint = m_nSkillPoint;
			g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sSkill, sizeof(PLAYER_SKILL_LEVEL_SYNC));
		}
	}
	
	
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£º·þÎñÆ÷¶Ë¼ðÎïÆ·
//-------------------------------------------------------------------------


	
int KPlayer::CheckOption(int Option,int Value)
	{
		if (Option == 0)
		return 0;

		for (int i=0; i < 38; i ++)
		{
			if (MangOption[i][1] == 0)
			continue;

			if (Option == MangOption[i][0])
			{
				if (Value >= MangOption[i][1])
				{
			//		printf("Check Tru %d %d %d %d \n",MangOption[i][0],MangOption[i][1],Option, Value);
					return 1;
				}
				else
					return 0;

			}
		}
		return 0;
	}



BOOL	KPlayer::ServerPickUpItem(BYTE* pProtocol)
{
	PLAYER_PICKUP_ITEM_COMMAND	*pPickUp = (PLAYER_PICKUP_ITEM_COMMAND*)pProtocol;
	
	int		nObjIndex, nNpcX, nNpcY, nObjX, nObjY;
	nObjIndex = ObjSet.FindID(pPickUp->m_nObjID);
	if (nObjIndex <= 0)
		return FALSE;



int nObjXC,nObjYC,nNpcXC,nNpcYC;
	

if (Object[nObjIndex].m_nSubWorldID != Npc[m_nIndex].m_SubWorldIndex)
{
printf("Nhat Obj khac Map [%s] [%s], loi !\n",GetPlayerName(),m_AccoutName);
return FALSE;
}
 
	SubWorld[Object[nObjIndex].m_nSubWorldID].Map2Mps(
		Object[nObjIndex].m_nRegionIdx,
		Object[nObjIndex].m_nMapX,
		Object[nObjIndex].m_nMapY,
		Object[nObjIndex].m_nOffX,
		Object[nObjIndex].m_nOffY,
		&nObjXC,
		&nObjYC);
	SubWorld[Npc[m_nIndex].m_SubWorldIndex].Map2Mps(
		Npc[m_nIndex].m_RegionIndex,
		Npc[m_nIndex].m_MapX,
		Npc[m_nIndex].m_MapY,
		Npc[m_nIndex].m_OffX,
		Npc[m_nIndex].m_OffY,
		&nNpcXC,
		&nNpcYC);

//if ((PLAYER_PICKUP_SERVER_DISTANCE * 2) < (nNpcXC - nObjXC) * (nNpcXC - nObjXC) + (nNpcYC - nObjYC) * (nNpcYC - nObjYC))
//{
//		return FALSE;
//}





	if (Object[nObjIndex].m_nBelong != -1)
	{
		if (!m_cTeam.m_nFlag)
		{
			if (Object[nObjIndex].m_nBelong != m_nPlayerIndex)
			{
				SHOW_MSG_SYNC	sMsg;
				sMsg.ProtocolType = s2c_msgshow;
				if (Object[nObjIndex].m_nKind == Obj_Kind_Money)
					sMsg.m_wMsgID = enumMSG_ID_MONEY_CANNOT_PICKUP;
				else
					sMsg.m_wMsgID = enumMSG_ID_OBJ_CANNOT_PICKUP;
				sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
				g_pServer->PackDataToClient(m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
				return FALSE;
			}
		}
		else
		{
			if (Object[nObjIndex].m_nKind == Obj_Kind_Money)
			{
				if (Object[nObjIndex].m_nBelong != m_nPlayerIndex &&
					!g_Team[m_cTeam.m_nID].CheckIn(Object[nObjIndex].m_nBelong))
				{
					SHOW_MSG_SYNC	sMsg;
					sMsg.ProtocolType = s2c_msgshow;
					sMsg.m_wMsgID = enumMSG_ID_MONEY_CANNOT_PICKUP;
					sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
					g_pServer->PackDataToClient(m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
					return FALSE;
				}
			}
			else if (Object[nObjIndex].m_nKind == Obj_Kind_Item)
			{
				if (Object[nObjIndex].m_nItemDataID <= 0 || Object[nObjIndex].m_nItemDataID >= MAX_ITEM)
				{
					_ASSERT(0);
					return FALSE;
				}
			//	if ((Item[Object[nObjIndex].m_nItemDataID].GetGenre() == item_task && Object[nObjIndex].m_nBelong != m_nPlayerIndex) ||
			//		!g_Team[m_cTeam.m_nID].CheckIn(Object[nObjIndex].m_nBelong))
				if (Object[nObjIndex].m_nBelong != m_nPlayerIndex &&
					!g_Team[m_cTeam.m_nID].CheckIn(Object[nObjIndex].m_nBelong))
				{
					SHOW_MSG_SYNC	sMsg;
					sMsg.ProtocolType = s2c_msgshow;
					sMsg.m_wMsgID = enumMSG_ID_OBJ_CANNOT_PICKUP;
					sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
					g_pServer->PackDataToClient(m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
					return FALSE;
				}
			}
			else if (Object[nObjIndex].m_nKind == Obj_Kind_Prop)
			{
				if (Object[nObjIndex].m_nBelong != m_nPlayerIndex &&
					!g_Team[m_cTeam.m_nID].CheckIn(Object[nObjIndex].m_nBelong))
				{
					SHOW_MSG_SYNC	sMsg;
					sMsg.ProtocolType = s2c_msgshow;
					sMsg.m_wMsgID = enumMSG_ID_OBJ_CANNOT_PICKUP;
					sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
					g_pServer->PackDataToClient(m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
					return FALSE;
				}
			}
			else
			{
				return FALSE;
			}
		}
	}
	// ÅÐ¶Ï¾àÀë
	if (Object[nObjIndex].m_nSubWorldID != Npc[m_nIndex].m_SubWorldIndex)
		return FALSE;
	SubWorld[Object[nObjIndex].m_nSubWorldID].Map2Mps(
		Object[nObjIndex].m_nRegionIdx,
		Object[nObjIndex].m_nMapX,
		Object[nObjIndex].m_nMapY,
		Object[nObjIndex].m_nOffX,
		Object[nObjIndex].m_nOffY,
		&nObjX,
		&nObjY);
	SubWorld[Npc[m_nIndex].m_SubWorldIndex].Map2Mps(
		Npc[m_nIndex].m_RegionIndex,
		Npc[m_nIndex].m_MapX,
		Npc[m_nIndex].m_MapY,
		Npc[m_nIndex].m_OffX,
		Npc[m_nIndex].m_OffY,
		&nNpcX,
		&nNpcY);
	
//	if ( PLAYER_PICKUP_SERVER_DISTANCE  > (nNpcX - nObjX) * (nNpcX - nObjX) + (nNpcY - nObjY) * (nNpcY - nObjY))
//	{
//		SHOW_MSG_SYNC	sMsg;
//		sMsg.ProtocolType = s2c_msgshow;
//		sMsg.m_wMsgID = enumMSG_ID_OBJ_TOO_FAR;
//		sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
//		g_pServer->PackDataToClient(m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
//		return FALSE;
//	}
	
	switch (Object[nObjIndex].m_nKind)
	{
	case Obj_Kind_Item:				// µôÔÚµØÉÏµÄ×°±¸
		{

			if (Object[nObjIndex].m_nItemDataID <= 0 || Object[nObjIndex].m_nItemDataID >= MAX_ITEM)
			{
			return FALSE;
			}

		int nWidth,nHeight;



	if (!ItemSet.Get_SizeItem(Item[Object[nObjIndex].m_nItemDataID].GetGenre(),
		Item[Object[nObjIndex].m_nItemDataID].GetDetailType(),
		Item[Object[nObjIndex].m_nItemDataID].GetParticular(),
		Item[Object[nObjIndex].m_nItemDataID].GetLevel(),&nWidth,&nHeight))
	{
		printf("Khong ton tai Item nhat, loi !\n");
        return FALSE;
	}

//	printf("Nhat Item: %d \n",nObjIndex);
	
	if (m_AutoNhatDoChonLoc)
	{
	//	printf("Test %d - %d - %d \n",Item[Object[nObjIndex].m_nItemDataID].GetDetailType(), Item[Object[nObjIndex].m_nItemDataID].GetGenre(),Item[Object[nObjIndex].m_nItemDataID].GetParticular());
		if (m_AutoRingSelect && !m_AutoWeaSelect)
		{
			if (Item[Object[nObjIndex].m_nItemDataID].GetGenre() == 0 && (Item[Object[nObjIndex].m_nItemDataID].GetDetailType() != 3 && Item[Object[nObjIndex].m_nItemDataID].GetDetailType() != 4 && Item[Object[nObjIndex].m_nItemDataID].GetDetailType() != 9) )
			return FALSE;
		}
		if (!m_AutoRingSelect && m_AutoWeaSelect)
		{
			if (Item[Object[nObjIndex].m_nItemDataID].GetGenre() == 0 && (Item[Object[nObjIndex].m_nItemDataID].GetDetailType() != 0 && Item[Object[nObjIndex].m_nItemDataID].GetDetailType() != 1 ))
			return FALSE;	
		}
		if (m_AutoRingSelect && m_AutoWeaSelect)
		{
			if (Item[Object[nObjIndex].m_nItemDataID].GetGenre() == 0 && (Item[Object[nObjIndex].m_nItemDataID].GetDetailType() != 0 && Item[Object[nObjIndex].m_nItemDataID].GetDetailType() != 1 && Item[Object[nObjIndex].m_nItemDataID].GetDetailType() != 3 && Item[Object[nObjIndex].m_nItemDataID].GetDetailType() != 4 && Item[Object[nObjIndex].m_nItemDataID].GetDetailType() != 9))
			return FALSE;	
		}


		int dong1 = Item[Object[nObjIndex].m_nItemDataID].m_aryMagicAttrib[0].nAttribType;
		int dong2 = Item[Object[nObjIndex].m_nItemDataID].m_aryMagicAttrib[1].nAttribType;
		int dong3 = Item[Object[nObjIndex].m_nItemDataID].m_aryMagicAttrib[2].nAttribType;
		int dong4 = Item[Object[nObjIndex].m_nItemDataID].m_aryMagicAttrib[3].nAttribType;
		int dong5 = Item[Object[nObjIndex].m_nItemDataID].m_aryMagicAttrib[4].nAttribType;
		int dong6 = Item[Object[nObjIndex].m_nItemDataID].m_aryMagicAttrib[5].nAttribType;
		int value1 = Item[Object[nObjIndex].m_nItemDataID].m_aryMagicAttrib[0].nValue[0];
		int value2 = Item[Object[nObjIndex].m_nItemDataID].m_aryMagicAttrib[1].nValue[0];
		int value3 = Item[Object[nObjIndex].m_nItemDataID].m_aryMagicAttrib[2].nValue[0];
		int value4 = Item[Object[nObjIndex].m_nItemDataID].m_aryMagicAttrib[3].nValue[0];
		int value5 = Item[Object[nObjIndex].m_nItemDataID].m_aryMagicAttrib[4].nValue[0];
		int value6 = Item[Object[nObjIndex].m_nItemDataID].m_aryMagicAttrib[5].nValue[0];

		int tongsodong = CheckOption(dong1,value1) + CheckOption(dong2,value2) + CheckOption(dong3,value3) + CheckOption(dong4,value4) + CheckOption(dong5,value5) + CheckOption(dong6,value6);
		if (m_AutoNumSelect == 0)
			m_AutoNumSelect = 1;
		if (m_AutoNumSelect > 6)
			m_AutoNumSelect = 6;	
		if (tongsodong < m_AutoNumSelect)
		return FALSE;

		for (int i = 0; i < 29; i ++)
		{
			if (MangDropItem[i] == 0)
				continue;
			if (MangDropItem[i] == nObjIndex)
			{
				KPlayerChat::SendSystemInfo(1,m_nPlayerIndex, MESSAGE_SYSTEM_ANNOUCE_HEAD, "VËt phÈm ®· qu¨ng ra kh«ng thÓ nhÆt l¹i, h·y chän NhÆt TÊt C¶ ®Ó cã thÓ nhÆt vËt phÈm !",strlen("VËt phÈm ®· qu¨ng ra kh«ng thÓ nhÆt l¹i, h·y chän NhÆt TÊt C¶ ®Ó cã thÓ nhÆt vËt phÈm !") );
				return FALSE;
			}
		}
	}

	  if (Item[Object[nObjIndex].m_nItemDataID].GetWidth() != nWidth || Item[Object[nObjIndex].m_nItemDataID].GetHeight() != nHeight)
	  {
		  return FALSE;
		printf("Kich thuoc goc thay doi, loi !\n");
	  }

	
		if (Item[Object[nObjIndex].m_nItemDataID].GetWidth() != Object[nObjIndex].m_nItemWidth || Item[Object[nObjIndex].m_nItemDataID].GetHeight() != Object[nObjIndex].m_nItemHeight)
		{
		printf("Kich thuoc thay doi, loi !\n");
		return FALSE;
		}

		if (pPickUp->m_btPosType != pos_hand && pPickUp->m_btPosType != pos_equiproom)
		{
         printf("Nhat Item nham ruong, loi !\n");
         return FALSE;
		}


		ItemPos	sItemPosCheck;

		if (!m_ItemList.SearchPosition(Object[nObjIndex].m_nItemWidth, Object[nObjIndex].m_nItemHeight, &sItemPosCheck) )
		{
		    printf("Khong con cho trong, loi !\n");
			return FALSE;
		}




		if ((sItemPosCheck.nPlace != pPickUp->m_btPosType) || (sItemPosCheck.nX != pPickUp->m_btPosX) || (sItemPosCheck.nY != pPickUp->m_btPosY))
		{
			printf("Vi tri thay doi, loi !\n");
			return FALSE;
		}


			int nItemIdx = m_ItemList.Add(Object[nObjIndex].m_nItemDataID, pPickUp->m_btPosType, pPickUp->m_btPosX, pPickUp->m_btPosY);
			if (nItemIdx <= 0 || nItemIdx >= MAX_PLAYER_ITEM)
				return FALSE;



        if (Object[nObjIndex].m_bByPlayer)
		{

		  if (this->CanSave())
			{
				if (this->Save())
				{
					this->m_uMustSave = SAVE_REQUEST;
				}
	
			}

		}


			if (Object[nObjIndex].m_nItemDataID <= 0 || Object[nObjIndex].m_nItemDataID >= MAX_ITEM)
				return FALSE;
			// ¸ø¿Í»§¶Ë·¢ËÍ»ñµÃ×°±¸µÄÏûÏ¢
			SHOW_MSG_SYNC	sMsg;
			sMsg.ProtocolType = s2c_msgshow;
			sMsg.m_wMsgID = enumMSG_ID_GET_ITEM;
			sMsg.m_lpBuf = (LPVOID)Item[Object[nObjIndex].m_nItemDataID].GetID();
			sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1;
			g_pServer->PackDataToClient(m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
			sMsg.m_lpBuf = 0;
			
			//char	szName[128];
			//sprintf(szName,"- %s nhÆt: <color=yellow>%s ",GetPlayerName(), Item[Object[nObjIndex].m_nItemDataID].GetName() );
			//KPlayerChat::SendSystemInfo(102,m_nPlayerIndex, MESSAGE_SYSTEM_ANNOUCE_HEAD, szName, strlen(szName) );
			// È¥µôObject[nObjIndex]ÓëµÀ¾ßµÄ¹ØÁª¡£±ÜÃâItemSetµÄRemove±»ObjectµÄRemoveµ÷ÓÃ
			Object[nObjIndex].m_nItemDataID = 0;
			Object[nObjIndex].m_nItemWidth = 0;
			Object[nObjIndex].m_nItemHeight = 0;
			Object[nObjIndex].Remove(FALSE);
			
		}	
		break;
	case Obj_Kind_Money:			// µôÔÚµØÉÏµÄÇ®

		int nMoneyMapT;
		nMoneyMapT = 0;
		int mMapAddMoney;
		mMapAddMoney = 0;
		if (SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_SubWorldID == 500) // Sac Mac Dia Bieu, Mac Cao Quat
			mMapAddMoney = 31; // Bien Kinh
		else if (SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_SubWorldID == 79 || SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_SubWorldID == 65) // truong bach nam, sa mac 3
			mMapAddMoney = 6;//6; // Phuong Tuong
		else if (SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_SubWorldID == 21 || SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_SubWorldID == 64) // truong bach nam, sa mac 3
			mMapAddMoney = 4;//14; // Tuong Duong
		else if (SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_SubWorldID == 500) // Mac bac, sa mac 1
			mMapAddMoney = 31; // Chi co 1 thanh thi, tat ca ban do thu thue ve Bien Kinh
		
		
		mMapAddMoney = 72; // Thanh Do
			
		if (mMapAddMoney != 0)
		{
		if (SubWorld[mMapAddMoney].m_bCheckTong)
			{
				//int nTongT = SubWorld[mMapAddMoney].m_nTongVG; 
				int nTongT = 5;
				if (nTongT >= 0 && nTongT <= 50)
				{
					if (SubWorld[mMapAddMoney].m_dwTongName == m_cTong.GetTongNameID())
					{
					nMoneyMapT = Object[nObjIndex].m_nMoneyNum * nTongT /200;
					}
					else
					{
					nMoneyMapT = Object[nObjIndex].m_nMoneyNum * nTongT /100;
					}
					
		
				if (Object[nObjIndex].m_nMoneyNum <= nMoneyMapT || nMoneyMapT < 0)
				{
					nMoneyMapT = 0;
				}
				}
			}
		}

		if ( !Earn(Object[nObjIndex].m_nMoneyNum - nMoneyMapT) )
			return FALSE;

		if (mMapAddMoney != 0)
		{
		if (SubWorld[mMapAddMoney].m_bCheckTong)
		{	
			try
			{

			bool bExecuteScriptMistake = true;
			KLuaScript * pScript = (KLuaScript* )g_GetScript("\\script\\admin\\banghoi\\banghoi.lua");;
			if (pScript)
			{

				int nTopIndex = 0;
				pScript->SafeCallBegin(&nTopIndex);
				if (pScript->CallFunction("AddMoneyMain",0, "dsd",SubWorld[mMapAddMoney].m_dwTongName,SubWorld[mMapAddMoney].m_szTongNameBC,nMoneyMapT));
				{
					bExecuteScriptMistake = false;
				}
				pScript->SafeCallEnd(nTopIndex);
			}
			}
			catch(...)
			{
				printf("Exception Have Caught When Execute Script[%d]!!!!!", g_FileName2Id("\\script\\admin\\banghoi\\banghoi.lua"));
		
			}
		}
		}


		if (Object[nObjIndex].m_bByPlayer)
		{

		  if (this->CanSave())
			{
				if (this->Save())
				{
					this->m_uMustSave = SAVE_REQUEST;
				}
	
			}

		}





		Object[nObjIndex].SyncRemove(TRUE);
		if (Object[nObjIndex].m_nRegionIdx >= 0)
			SubWorld[Object[nObjIndex].m_nSubWorldID].m_Region[Object[nObjIndex].m_nRegionIdx].RemoveObj(nObjIndex);
		ObjSet.Remove(nObjIndex);
		break;


	case Obj_Kind_Prop:	

		if (!Object[nObjIndex].m_dwScriptID)
		return FALSE;
	
		Object[nObjIndex].ExecScript(m_nPlayerIndex);

		Object[nObjIndex].SyncRemove(TRUE);
		if (Object[nObjIndex].m_nRegionIdx >= 0)
			SubWorld[Object[nObjIndex].m_nSubWorldID].m_Region[Object[nObjIndex].m_nRegionIdx].RemoveObj(nObjIndex);
		ObjSet.Remove(nObjIndex);
		break;


	}
	
	return TRUE;
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÊÕµ½¿Í»§¶ËÒªÇóÊ¹ÓÃÄ³¸öÎïÆ·(Êó±êÓÒ¼üµã»÷)
//-------------------------------------------------------------------------
void	KPlayer::EatItem(BYTE* pProtocol)
{
	PLAYER_EAT_ITEM_COMMAND	*pEat = (PLAYER_EAT_ITEM_COMMAND*)pProtocol;
	

	m_ItemList.EatMecidine(pEat->m_btPlace, pEat->m_btX, pEat->m_btY);
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÊÕµ½¿Í»§¶ËÒªÇóÊ¹ÓÃÄ³¸öÎïÆ·(Êó±êÓÒ¼üµã»÷)
//-------------------------------------------------------------------------
//void	KPlayer::UseItem(BYTE* pProtocol)
//{
//	PLAYER_USE_ITEM_COMMAND	*pItem = (PLAYER_USE_ITEM_COMMAND*)pProtocol;
//}
#endif

#ifdef _SERVER
void	KPlayer::ServerMoveItem(BYTE* pProtocol)
{
	PLAYER_MOVE_ITEM_COMMAND	*pMove = (PLAYER_MOVE_ITEM_COMMAND*)pProtocol;
	ItemPos		DownPos, UpPos;
	if (Npc[m_nIndex].m_FightMode && (pMove->m_btUpPos == 11 || pMove->m_btUpPos == 4))
	{
		BYTE	byFinished = s2c_itemexchangefinish;
		if (g_pServer)
			g_pServer->PackDataToClient(m_nNetConnectIdx, &byFinished, sizeof(BYTE));
		return;
	}
	
	DownPos.nPlace = pMove->m_btDownPos;
	DownPos.nX = pMove->m_btDownX;
	DownPos.nY = pMove->m_btDownY;
	UpPos.nPlace = pMove->m_btUpPos;
	UpPos.nX = pMove->m_btUpX;
	UpPos.nY = pMove->m_btUpY;

	//EXCHANGE_ITEM_RESPONE	eir;
	//eir.ProtocolType = s2c_exchangerespone;
	
	if (this->CheckTrading() && DownPos.nPlace == pos_traderoom)
	{
		if (this->m_cTrade.m_nTradeLock)
			return;
	}
	
	m_ItemList.ExchangeItem(&DownPos, &UpPos);
	
	BYTE	byFinished = s2c_itemexchangefinish;
	if (g_pServer)
		g_pServer->PackDataToClient(m_nNetConnectIdx, &byFinished, sizeof(BYTE));
}
#endif

#ifdef _SERVER
void	KPlayer::ServerThrowAwayItem(BYTE* pProtocol)
{
	PLAYER_THROW_AWAY_ITEM_COMMAND	*pThrow = (PLAYER_THROW_AWAY_ITEM_COMMAND*)pProtocol;
	KMapPos		sMapPos;
	
	if ( !this->m_ItemList.Hand() )
		return;

	

    if (g_SubWorldSet.GetGameTime() - m_dwTimeThrowAwayItem >= (1*18))
	{
	m_dwTimeThrowAwayItem = g_SubWorldSet.GetGameTime();
	}
	else
	{
	return;
	}



	int nItemIdx = m_ItemList.m_Hand;


	if (!m_ItemList.FindSame(nItemIdx))
	{
		printf("Hack ThrowAwayItem m_Hand 1 [%s] [%s]\n",m_AccoutName,m_PlayerName);
		return;
	}

	if (!m_ItemList.CheckItemInAll(nItemIdx))
	{
		printf("Hack ThrowAwayItem m_Hand 2 [%s] [%s]\n",m_AccoutName,m_PlayerName);
		return;
	}

   


	if ((Item[nItemIdx].GetGenre() == item_equip || Item[nItemIdx].GetGenre() == item_medicine) && Item[nItemIdx].m_GeneratorParam.nVersion > 1)
		return;
	if ((Item[nItemIdx].GetGenre() == item_task) && Item[nItemIdx].GetBindItem() >= 1)
		return;


     int nWidth,nHeight;

	if (!ItemSet.Get_SizeItem(Item[nItemIdx].GetGenre(),Item[nItemIdx].GetDetailType(),Item[nItemIdx].GetParticular(),Item[nItemIdx].GetLevel(),&nWidth,&nHeight))
        return;

	if (Item[nItemIdx].GetWidth() != nWidth || Item[nItemIdx].GetHeight() != nHeight)
		return;


		if (Item[nItemIdx].m_nPriceShop)
		{
		   Item[nItemIdx].m_nPriceShop = 0;
	       PLAYER_UPDATE_SHOP_PRICE_COMMAND	IDC;
	       IDC.ProtocolType = s2c_playerupdateshopprice;
	       IDC.m_ID = Item[nItemIdx].m_dwID;
	       IDC.m_Price = Item[nItemIdx].m_nPriceShop;
	       if (g_pServer)
		   g_pServer->PackDataToClient(m_nNetConnectIdx, &IDC, sizeof(PLAYER_UPDATE_SHOP_PRICE_COMMAND));
		}
	if((Item[nItemIdx].GetKind() == 1) || (Item[nItemIdx].GetGenre() == item_task && Item[nItemIdx].GetIsThrow() == 1))
	{
		char szMsg[500];
		sprintf(szMsg, "Kh«ng thÓ vøt bá vËt phÈm quÝ ");
		KPlayerChat::SendSystemInfo(1, m_nPlayerIndex, "Th«ng b¸o", (char *) szMsg, strlen(szMsg) );
		return;	
	}

	GetAboutPos(&sMapPos);
	if (m_ItemList.Remove(nItemIdx))
	{
		int		nObj;
		KObjItemInfo	sInfo;
		sInfo.m_nItemID = nItemIdx;
		sInfo.m_nItemWidth = Item[nItemIdx].GetWidth();
		sInfo.m_nItemHeight = Item[nItemIdx].GetHeight();
		sInfo.m_nMoneyNum = 0;
		strcpy(sInfo.m_szName, Item[nItemIdx].m_CommonAttrib.szItemName);
if (Item[nItemIdx].m_SpecialParam.uItemType == 1)
sInfo.m_nColorID = 1;
else if (Item[nItemIdx].m_SpecialParam.uItemType == 2)
sInfo.m_nColorID = 3;
else if (Item[nItemIdx].IsItemMagic())
sInfo.m_nColorID = 2;
else
sInfo.m_nColorID = 0;

		sInfo.m_nMovieFlag = 1;
		sInfo.m_nSoundFlag = 1;


		sInfo.m_bByPlayer = TRUE;
		
		nObj = ObjSet.Add(Item[nItemIdx].GetObjIdx(), sMapPos, sInfo);
		
		BOOL bCheckFull = TRUE;
		for (int t=0; t < 29; t ++)
		{
			if (MangDropItem[t] == 0)
			{
				MangDropItem[t] = nObj;
				bCheckFull = FALSE;
				break;
			}
			
		}

		if (bCheckFull)
		{
			for (int j=1 ; j < 29; j ++)
				MangDropItem[j] = 0;
			MangDropItem[0] = nObj;

		}


		//printf("Quang Item  %d\n",nObj);

		if (nObj >= 0)
		{
			if (Item[nItemIdx].GetGenre() == item_task)
			{
				Object[nObj].SetEntireBelong(this->m_nPlayerIndex);
			}
			else
			{
				Object[nObj].SetItemBelong(-1);
			}
		}
		else
		{
		ItemSet.m_checkdel = 8;
		ItemSet.Remove(nItemIdx);

		}




       if (this->CanSave())
			{
				if (this->Save())
				{
					this->m_uMustSave = SAVE_REQUEST;
				}
	
			}






	}
}
#endif





#ifdef _SERVER
void KPlayer::PlayerRestAP(int nsuc_manh_goc,int nthan_phap_goc,int nsinh_khi_goc,int nnoi_cong_goc)
{	

		m_nStrength = nsuc_manh_goc;
		m_nCurStrength = nsuc_manh_goc;
		m_nDexterity = nthan_phap_goc;
		m_nCurDexterity = nthan_phap_goc;	
		m_nVitality = nsinh_khi_goc;
		m_nCurVitality = nsinh_khi_goc;
		m_nEngergy = nnoi_cong_goc;
		m_nCurEngergy = nnoi_cong_goc;	
		
	     LevelAttriBaseLifeMax();
	     LevelAttriBaseStaminaMax();
	     LevelAttriBaseManaMax();
	
}
#endif















#ifdef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÊÕµ½¿Í»§¶ËÏûÏ¢Éè¶¨ÁÄÌì¶©ÔÄÆµµÀ
//-------------------------------------------------------------------------
void	KPlayer::ChatSetTakeChannel(BYTE* pProtocol)
{
	CHAT_SET_CHANNEL_COMMAND	*pChannel = (CHAT_SET_CHANNEL_COMMAND*)pProtocol;
	this->m_cChat.SetTakeChannel(pChannel->m_dwTakeChannel);
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÊÕµ½¿Í»§¶ËÇëÇó×ª·¢ÁÄÌìÌí¼ÓºÃÓÑÐÅÏ¢
//-------------------------------------------------------------------------
void	KPlayer::ChatTransmitApplyAddFriend(BYTE* pProtocol)
{
	CHAT_APPLY_ADD_FRIEND_COMMAND	*pAdd = (CHAT_APPLY_ADD_FRIEND_COMMAND*)pProtocol;
	int	nPlayerIdx = FindAroundPlayer(pAdd->m_dwTargetNpcID);
	if (nPlayerIdx < 0)
		return;
	
	m_cChat.m_nAddFriendList[m_cChat.m_nListPos++] = nPlayerIdx;
	m_cChat.m_nListPos %= CHAT_APPLY_ADD_FRIEND_LIST;
	
	CHAT_APPLY_ADD_FRIEND_SYNC	sAdd;
	sAdd.ProtocolType = s2c_chatapplyaddfriend;
	sAdd.m_nSrcPlayerIdx = m_nPlayerIndex;
	strcpy(sAdd.m_szSourceName, Npc[m_nIndex].Name);
	memset(sAdd.m_szInfo, 0, sizeof(sAdd.m_szInfo));
	memcpy(sAdd.m_szInfo, pAdd->m_szInfo, pAdd->m_wLength + 1 + sizeof(pAdd->m_szInfo) - sizeof(CHAT_APPLY_ADD_FRIEND_COMMAND));
	g_pServer->PackDataToClient(Player[nPlayerIdx].m_nNetConnectIdx, (BYTE*)&sAdd, sizeof(CHAT_APPLY_ADD_FRIEND_SYNC));
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÊÕµ½¿Í»§¶ËÏûÏ¢Ìí¼ÓÁÄÌìºÃÓÑ
//-------------------------------------------------------------------------
BOOL	KPlayer::ChatAddFriend(BYTE* pProtocol)
{
	CHAT_ADD_FRIEND_COMMAND	*pAdd = (CHAT_ADD_FRIEND_COMMAND*)pProtocol;
	
	if (pAdd->m_nTargetPlayerIdx < 0 || pAdd->m_nTargetPlayerIdx >= MAX_PLAYER)
		goto AddFalse;
	if ( !Player[pAdd->m_nTargetPlayerIdx].m_nIndex )
		goto AddFalse;
	int		i;
	for (i = 0; i < CHAT_APPLY_ADD_FRIEND_LIST; i++)
	{
		if (m_nPlayerIndex == Player[pAdd->m_nTargetPlayerIdx].m_cChat.m_nAddFriendList[i])
			break;
	}
	if (i >= CHAT_APPLY_ADD_FRIEND_LIST)
		goto AddFalse;
	
	m_cChat.AddFriendData(m_nPlayerIndex, pAdd->m_nTargetPlayerIdx);
	Player[pAdd->m_nTargetPlayerIdx].m_cChat.AddFriendData(pAdd->m_nTargetPlayerIdx, m_nPlayerIndex);
	
	return TRUE;
	
AddFalse:
	CHAT_ADD_FRIEND_FAIL_SYNC	sFail;
	sFail.ProtocolType = s2c_chataddfriendfail;
	sFail.m_nTargetPlayerIdx = pAdd->m_nTargetPlayerIdx;
	g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sFail, sizeof(CHAT_ADD_FRIEND_FAIL_SYNC));
	
	return FALSE;
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÊÕµ½¿Í»§¶ËÏûÏ¢¾Ü¾øÌí¼ÓÁÄÌìºÃÓÑ
//-------------------------------------------------------------------------
void	KPlayer::ChatRefuseFriend(BYTE* pProtocol)
{
	CHAT_REFUSE_FRIEND_COMMAND	*pRefuse = (CHAT_REFUSE_FRIEND_COMMAND*)pProtocol;
	
	if (pRefuse->m_nTargetPlayerIdx < 0 || pRefuse->m_nTargetPlayerIdx >= MAX_PLAYER)
		return;
	if (Player[pRefuse->m_nTargetPlayerIdx].m_nIndex == 0)
		return;
	for (int i = 0; i < CHAT_APPLY_ADD_FRIEND_LIST; i++)
	{
		if (m_nPlayerIndex == Player[pRefuse->m_nTargetPlayerIdx].m_cChat.m_nAddFriendList[i])
		{
			CHAT_REFUSE_FRIEND_SYNC	sRefuse;
			sRefuse.ProtocolType = s2c_chatrefusefriend;
			g_StrCpy(sRefuse.m_szName, Npc[m_nIndex].Name);
			sRefuse.m_wLength = sizeof(CHAT_REFUSE_FRIEND_SYNC) - 1 - sizeof(sRefuse.m_szName) + strlen(sRefuse.m_szName);
			
			g_pServer->PackDataToClient(Player[pRefuse->m_nTargetPlayerIdx].m_nNetConnectIdx, (BYTE*)&sRefuse, sRefuse.m_wLength + 1);
			return;
		}
	}
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÍæ¼ÒµÇÂ¼Ê±ÊÕµ½¿Í»§¶ËÇëÇóÖØÐÂ·¢ËÍËùÓÐÁÄÌìºÃÓÑÊý¾Ý£¨´øÃû×Ö£©
//-------------------------------------------------------------------------
void	KPlayer::ChatResendAllFriend(BYTE* pProtocol)
{
	if ( m_cChat.m_nSyncState != 1)
		return;
	m_cChat.m_nSyncState = 2;
	m_cChat.m_pStateSendPos = (CChatFriend*)m_cChat.m_cFriendTeam[0].m_cEveryOne.GetHead();
	if ( !m_cChat.m_pStateSendPos )
		m_cChat.m_nSyncState = 0;
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÍæ¼ÒµÇÂ¼Ê±ÊÕµ½¿Í»§¶ËÇëÇó·¢ËÍÄ³¸öÁÄÌìºÃÓÑÊý¾Ý£¨´øÃû×Ö£©
//-------------------------------------------------------------------------
void	KPlayer::ChatSendOneFriendData(BYTE* pProtocol)
{
	CHAT_APPLY_SEND_ONE_FRIEND_NAME_COMMAND	*pApply = (CHAT_APPLY_SEND_ONE_FRIEND_NAME_COMMAND*)pProtocol;
	
	m_cChat.ResendOneFriendData(pApply->m_dwID, m_nPlayerIndex);
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÊÕµ½¿Í»§¶ËÉêÇëÉ¾³ýÄ³¸öÁÄÌìºÃÓÑ
//-------------------------------------------------------------------------
void	KPlayer::ChatDeleteFriend(BYTE* pProtocol)
{
	CHAT_DELETE_FRIEND_COMMAND	*pDelete = (CHAT_DELETE_FRIEND_COMMAND*)pProtocol;
	
	m_cChat.DeleteFriend(pDelete->m_dwID, m_dwID);
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÊÕµ½¿Í»§¶ËÉêÇëÔÙ´ÎÉ¾³ýÄ³¸öÁÄÌìºÃÓÑ
//-------------------------------------------------------------------------
void	KPlayer::ChatRedeleteFriend(BYTE* pProtocol)
{
	CHAT_REDELETE_FRIEND_COMMAND	*pDelete = (CHAT_REDELETE_FRIEND_COMMAND*)pProtocol;
	
	this->m_cChat.RedeleteFriend(pDelete->m_dwID, m_nPlayerIndex);
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÊÕµ½¿Í»§¶ËÉêÇë½øÈë´ý½»Ò××´Ì¬
//-------------------------------------------------------------------------
void	KPlayer::TradeApplyOpen(BYTE* pProtocol)
{
	if ( CheckTrading() || m_bBlock)
		return;


	if (Npc[m_nIndex].m_SubWorldIndex == g_SubWorldSet.SearchWorld(43))
	{
	return;
	}

	// LevelTrade
//	if (Npc[m_nIndex].m_Level < 100)
//	{
//		KPlayerChat::SendSystemInfo(1,m_nPlayerIndex, MESSAGE_SYSTEM_ANNOUCE_HEAD, "§¼ng cÊp nhá h¬n 100, kh«ng thÓ giao dÞch !", strlen("§¼ng cÊp nhá h¬n 100, kh«ng thÓ giao dÞch !") );
//		return;
//	}

	if (m_cTeam.m_nFlag && m_cTeam.m_nID >= 0)
	{
		KPlayerChat::SendSystemInfo(1,m_nPlayerIndex, MESSAGE_SYSTEM_ANNOUCE_HEAD, "Kh«ng thÓ giao dÞch khi ®ang trong tæ ®éi !", strlen("Kh«ng thÓ giao dÞch khi ®ang trong tæ ®éi !") );
		return;
	}


	TRADE_APPLY_OPEN_COMMAND	*pApply = (TRADE_APPLY_OPEN_COMMAND*)pProtocol;
	char	szSentence[MAX_SENTENCE_LENGTH];
	int		nLength = pApply->m_wLength - (sizeof(TRADE_APPLY_OPEN_COMMAND) - 1 - sizeof(pApply->m_szSentence));
	if (nLength >= MAX_SENTENCE_LENGTH)
		nLength = MAX_SENTENCE_LENGTH - 1;
	memset(szSentence, 0, sizeof(szSentence));
	memcpy(szSentence, pApply->m_szSentence, nLength);
	
	switch (m_cMenuState.m_nState)
	{
	case PLAYER_MENU_STATE_NORMAL:
	case PLAYER_MENU_STATE_TRADEOPEN:
		m_cMenuState.SetState(m_nPlayerIndex, PLAYER_MENU_STATE_TRADEOPEN, szSentence, nLength);
		break;
	case PLAYER_MENU_STATE_TEAMOPEN:
		if (m_cTeam.m_nFlag && m_cTeam.m_nID >= 0)
			g_Team[m_cTeam.m_nID].SetTeamClose();
		m_cMenuState.SetState(m_nPlayerIndex, PLAYER_MENU_STATE_TRADEOPEN, szSentence, nLength);
		break;
	}
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÊÕµ½¿Í»§¶ËÉêÇëÈ¡Ïû´ý½»Ò××´Ì¬
//-------------------------------------------------------------------------
void	KPlayer::TradeApplyClose(BYTE* pProtocol)
{
	if ( CheckTrading() )
		return;

	m_cMenuState.SetState(m_nPlayerIndex, PLAYER_MENU_STATE_NORMAL);
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÊÕµ½¿Í»§¶ËÉêÇë¿ªÊ¼½»Ò×
//-------------------------------------------------------------------------
void	KPlayer::TradeApplyStart(BYTE* pProtocol)
{
	if (this->CheckTrading() || CheckShopOpen() || CheckShopBuyOpen())
		return;

	if (m_bBlock)
		return;
	// aaa
//	if (Npc[m_nIndex].m_Level < 100)
//	{	
//		KPlayerChat::SendSystemInfo(1,m_nPlayerIndex, MESSAGE_SYSTEM_ANNOUCE_HEAD, "§¼ng cÊp nhá h¬n 100, kh«ng thÓ giao dÞch !", strlen("§¼ng cÊp nhá h¬n 100, kh«ng thÓ giao dÞch !") );
//		return;
//	}

	if (Npc[m_nIndex].m_SubWorldIndex == g_SubWorldSet.SearchWorld(43))
	{
	return;
	}


	if (m_cTeam.m_nFlag && m_cTeam.m_nID >= 0)
	{
		KPlayerChat::SendSystemInfo(1,m_nPlayerIndex, MESSAGE_SYSTEM_ANNOUCE_HEAD, "Kh«ng thÓ giao dÞch khi ®ang trong tæ ®éi !", strlen("Kh«ng thÓ giao dÞch khi ®ang trong tæ ®éi !") );
		return;
	}

	
	TRADE_APPLY_START_COMMAND	*pStart = (TRADE_APPLY_START_COMMAND*)pProtocol;
	
	int		nPlayer;
	nPlayer = FindAroundPlayer(pStart->m_dwID);
	if (nPlayer < 0)
		return;

	if (Player[nPlayer].m_cMenuState.m_nState != PLAYER_MENU_STATE_TRADEOPEN)
		return;

	if (Player[nPlayer].CheckShopOpen() || Player[nPlayer].CheckTrading() || Player[nPlayer].CheckShopBuyOpen())
		return;

	for (int i=0;i<MAX_PLAYER;i++)
	{
    if (i != m_nPlayerIndex && !IsExchangingServer() && Player[i].m_cTrade.m_nTradeDest == m_nPlayerIndex)
	{
	printf("Xay ra hack lua giao dich, tu dong xua loi !\n");
	this->ExecuteScript("\\script\\admin\\gayroi.lua","main",0);
    return;
	}
	}

	this->m_cTrade.m_nApplyIdx = nPlayer;
	TRADE_APPLY_START_SYNC	sApply;
	sApply.ProtocolType = s2c_tradeapplystart;
	sApply.m_nDestIdx = this->m_nPlayerIndex;
	sApply.m_dwNpcId = Npc[m_nIndex].m_dwID;
	if (g_pServer)
		g_pServer->PackDataToClient(Player[nPlayer].m_nNetConnectIdx, (BYTE*)&sApply, sizeof(TRADE_APPLY_START_SYNC));
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£º
//-------------------------------------------------------------------------
void	KPlayer::TradeMoveMoney(BYTE* pProtocol)
{
	if ( !CheckTrading() )
		return;
	if (this->m_cTrade.m_nTradeLock)
		return;
	
	int		nMoney;
	TRADE_MOVE_MONEY_COMMAND	*pMoney = (TRADE_MOVE_MONEY_COMMAND*)pProtocol;
	if (pMoney->m_nMoney < 0)
		nMoney = 0;		
	else if (pMoney->m_nMoney > m_ItemList.GetEquipmentMoney())
		nMoney = m_ItemList.GetEquipmentMoney();
	else
		nMoney = pMoney->m_nMoney;

	m_ItemList.TradeMoveMoney(nMoney);
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÖ´ÐÐ»òÈ¡Ïû½»Ò×
//-------------------------------------------------------------------------
void	KPlayer::TradeDecision(BYTE* pProtocol)
{
	if ( !CheckTrading() )
		return;
	
	TRADE_DECISION_COMMAND	*pDecision = (TRADE_DECISION_COMMAND*)pProtocol;
	
	if (pDecision->m_btDecision == 2)	// ¼º·½Ëø¶¨
	{
		if (this->m_cTrade.m_nTradeLock == 0)
		{
			this->m_cTrade.m_nTradeLock = 1;
			this->m_cTrade.m_nTradeState = 0;
			Player[m_cTrade.m_nTradeDest].m_cTrade.m_nTradeState = 0;
		}
		
		SyncTradeState();
		return;
	}
	else if (pDecision->m_btDecision == 3)	// Ëø¶¨È¡Ïû
	{
		if (this->m_cTrade.m_nTradeLock == 1)
		{
			// È¡Ïû¼º·½Ëø¶¨µÄÍ¬Ê±Ò²È¡ÏûÁË¶Ô·½µÄËø¶¨
			this->m_cTrade.m_nTradeLock = 0;
			this->m_cTrade.m_nTradeState = 0;
			Player[m_cTrade.m_nTradeDest].m_cTrade.m_nTradeLock = 0;
			Player[m_cTrade.m_nTradeDest].m_cTrade.m_nTradeState = 0;
		}
		SyncTradeState();
		return;
	}
	else if (pDecision->m_btDecision == 4)	// È¡ÏûÈ·¶¨
	{
		if (m_cTrade.m_nTradeLock == 1 && Player[m_cTrade.m_nTradeDest].m_cTrade.m_nTradeLock == 1)
		{
			m_cTrade.m_nTradeState = 0;
			SyncTradeState();
		}
		return;
	}
	else if (pDecision->m_btDecision == 0)	// È¡Ïû½»Ò×
	{
		int	nDestIdx;
		nDestIdx = this->m_cTrade.m_nTradeDest;
		// È¡Ïû½»Ò×µÄÊý¾Ý»Ö¸´
		this->m_ItemList.RecoverTrade();
		this->m_ItemList.ClearRoom(room_trade);
		this->m_ItemList.ClearRoom(room_trade1);
		this->m_cTrade.Release();
		Player[nDestIdx].m_ItemList.RecoverTrade();
		Player[nDestIdx].m_ItemList.ClearRoom(room_trade);
		Player[nDestIdx].m_ItemList.ClearRoom(room_trade1);
		Player[nDestIdx].m_cTrade.Release();
		
		TRADE_DECISION_SYNC	sSync;
		sSync.ProtocolType = s2c_tradedecision;
		sSync.m_btDecision = 0;
		g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sSync, sizeof(TRADE_DECISION_SYNC));
		g_pServer->PackDataToClient(Player[nDestIdx].m_nNetConnectIdx, (BYTE*)&sSync, sizeof(TRADE_DECISION_SYNC));
		
		// È¡Ïû½»Ò×µÄ×´Ì¬»Ö¸´(±¸·Ý×´Ì¬Ò»¶¨²»»áÊÇ trading)(¼º·½)
		this->m_cMenuState.RestoreBackupState(m_nPlayerIndex);
		
		// È¡Ïû½»Ò×µÄ×´Ì¬»Ö¸´(±¸·Ý×´Ì¬Ò»¶¨²»»áÊÇ trading)(¶Ô·½)
		Player[nDestIdx].m_cMenuState.RestoreBackupState(nDestIdx);
		
		return;
	}
	else if (pDecision->m_btDecision == 1)	// Ö´ÐÐ½»Ò×
	{
		// Èç¹ûË«·½Ã»ÓÐÍ¬Ê±Ëø¶¨£¬²»ÄÜÈ·¶¨
		if (this->m_cTrade.m_nTradeLock != 1 ||
			Player[m_cTrade.m_nTradeDest].m_cTrade.m_nTradeLock != 1)
		{
			SyncTradeState();
			return;
		}
		// ¶Ô·½»¹Ã»È·¶¨£¬Í¨Öª¶Ô·½×Ô¼ºÒÑÈ·¶¨
		else if (Player[m_cTrade.m_nTradeDest].m_cTrade.m_nTradeState == 0)
		{
			m_cTrade.m_nTradeState = 1;
			
			SyncTradeState();
			
			return;
		}
		// ¶Ô·½ÒÑÈ·¶¨£¬Ö´ÐÐ½»Ò×
		else
		{
			// Ö´ÐÐ½»Ò×Ö®Ç°£¬ÏÈ¼ì²âÒ»ÏÂÊÇ·ñÓÐ´íÎó
			// -------------------- ¼ì²é money --------------------
			int	nSM1, nSM2, nDM1, nDM2;
			int	nDestIdx = m_cTrade.m_nTradeDest;
			nSM1 = m_ItemList.GetMoney(room_equipment);
			nSM2 = m_ItemList.GetMoney(room_trade);
			nDM1 = Player[nDestIdx].m_ItemList.GetMoney(room_equipment);
			nDM2 = Player[nDestIdx].m_ItemList.GetMoney(room_trade);
			if (nSM2 < 0 || nSM1 + nDM2 - nSM2 < 0)
			{
				this->m_ItemList.TradeMoveMoney(0);

				TRADE_DECISION_COMMAND	sTrade;
	            sTrade.ProtocolType = c2s_tradedecision;
	            sTrade.m_btDecision = 0;
	            TradeDecision((BYTE*)&sTrade);
				Player[nDestIdx].TradeDecision((BYTE*)&sTrade);

				// Í¨Öª¿Í»§¶ËÇ®ÓÐÎÊÌâ not end
				return;
			}
			if (nDM2 < 0 || nDM1 + nSM2 - nDM2 < 0)
			{
				Player[nDestIdx].m_ItemList.TradeMoveMoney(0);

				TRADE_DECISION_COMMAND	sTrade;
	            sTrade.ProtocolType = c2s_tradedecision;
	            sTrade.m_btDecision = 0;
	            TradeDecision((BYTE*)&sTrade);
				Player[nDestIdx].TradeDecision((BYTE*)&sTrade);

				// Í¨Öª¿Í»§¶ËÇ®ÓÐÎÊÌâ not end
				return;
			}
			// ------------------ ¼ì²é money end ------------------
			
			// --------- ¼ì²éË«·½ÎïÆ·À¸ÄÜ·ñ½ÓÊÜÂò½øµÄÎïÆ· ---------
			int		nSelfCanGet, nDestCanGet;
			
			// ÅÐ¶Ï¼º·½ÎïÆ·À¸ÄÜ·ñ½ÓÊÜÂò½øµÄÎïÆ·
			Player[nDestIdx].m_ItemList.GetTradeRoomItemInfo();
			nSelfCanGet = m_ItemList.TradeCheckCanPlace();
			ItemSet.BackItemInfo();
			
			// ÅÐ¶Ï¶Ô·½ÎïÆ·À¸ÄÜ·ñ½ÓÊÜÂò½øµÄÎïÆ·
			this->m_ItemList.GetTradeRoomItemInfo();
			nDestCanGet = Player[nDestIdx].m_ItemList.TradeCheckCanPlace();
			
			if (!nSelfCanGet || !nDestCanGet)
			{
				if (!nSelfCanGet)
				{
					this->m_cTrade.m_nTradeState = 0;
					
					// Í¨Öª¿Í»§¶ËÂò½øµÄ¶«Î÷·Å²»ÏÂ
					SHOW_MSG_SYNC	sMsg;
					sMsg.ProtocolType = s2c_msgshow;
					sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
					sMsg.m_wMsgID = enumMSG_ID_TRADE_SELF_ROOM_FULL;
					g_pServer->PackDataToClient(m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
					sMsg.m_wMsgID = enumMSG_ID_TRADE_DEST_ROOM_FULL;
					g_pServer->PackDataToClient(Player[nDestIdx].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
				}
				if (!nDestCanGet)
				{
					Player[nDestIdx].m_cTrade.m_nTradeState = 0;
					
					// Í¨Öª¿Í»§¶ËÂò½øµÄ¶«Î÷·Å²»ÏÂ
					SHOW_MSG_SYNC	sMsg;
					sMsg.ProtocolType = s2c_msgshow;
					sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
					sMsg.m_wMsgID = enumMSG_ID_TRADE_SELF_ROOM_FULL;
					g_pServer->PackDataToClient(Player[nDestIdx].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
					sMsg.m_wMsgID = enumMSG_ID_TRADE_DEST_ROOM_FULL;
					g_pServer->PackDataToClient(m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
				}
				SyncTradeState();
				return;
			}
			// ------- ¼ì²éË«·½ÎïÆ·À¸ÄÜ·ñ½ÓÊÜÂò½øµÄÎïÆ· end -------
			
			// Í¨Öª¿Í»§¶Ë½»Ò×Íê³É
			TRADE_DECISION_SYNC	sSync;
			sSync.ProtocolType = s2c_tradedecision;
			sSync.m_btDecision = 1;
			g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sSync, sizeof(TRADE_DECISION_SYNC));
			g_pServer->PackDataToClient(Player[nDestIdx].m_nNetConnectIdx, (BYTE*)&sSync, sizeof(TRADE_DECISION_SYNC));
			
			// ·þÎñÆ÷¶Ë½áÊø½»Ò××´Ì¬
			m_cMenuState.SetState(m_nPlayerIndex, PLAYER_MENU_STATE_NORMAL);
			Player[nDestIdx].m_cMenuState.SetState(nDestIdx, PLAYER_MENU_STATE_NORMAL);
			this->m_cTrade.Release();
			Player[nDestIdx].m_cTrade.Release();
			
			// ½»Ò×ÖÐ½ðÇ®µÄ½»»»
			if (nSM2 <= nDM2)
			{
				this->Earn(nDM2 - nSM2);
				Player[nDestIdx].Pay(nDM2 - nSM2);
			}
			else
			{
				this->Pay(nSM2 - nDM2);
				Player[nDestIdx].Earn(nSM2 - nDM2);
			}
			
			//--------------- ½»Ò×ÖÐÎïÆ·µÄ½»»» ---------------------
			// ¶Ô·½µÄÎïÆ·¸ø¼º·½
			int		nPos;
			nPos = 0;
			while (1)
			{
				if (nPos >= TRADE_ROOM_WIDTH * TRADE_ROOM_HEIGHT)
					break;
				if (!ItemSet.m_psBackItemInfo[nPos].m_nIdx)
					break;
				// ¶Ô·½È¥³ý´ËÎïÆ·
				Player[nDestIdx].m_ItemList.Remove(ItemSet.m_psBackItemInfo[nPos].m_nIdx);
				// ¼º·½µÃµ½´ËÎïÆ·
				m_ItemList.Add(
					ItemSet.m_psBackItemInfo[nPos].m_nIdx,
					pos_equiproom,
					ItemSet.m_psBackItemInfo[nPos].m_nX,
					ItemSet.m_psBackItemInfo[nPos].m_nY);
				nPos++;
			}
			// ¼º·½µÄÎïÆ·¸ø¶Ô·½
			nPos = 0;
			while (1)
			{
				if (nPos >= TRADE_ROOM_WIDTH * TRADE_ROOM_HEIGHT)
					break;
				if (!ItemSet.m_psItemInfo[nPos].m_nIdx)
					break;
				// ¼º·½È¥³ý´ËÎïÆ·
				this->m_ItemList.Remove(ItemSet.m_psItemInfo[nPos].m_nIdx);
				// ¶Ô·½µÃµ½´ËÎïÆ·
				Player[nDestIdx].m_ItemList.Add(
					ItemSet.m_psItemInfo[nPos].m_nIdx,
					pos_equiproom,
					ItemSet.m_psItemInfo[nPos].m_nX,
					ItemSet.m_psItemInfo[nPos].m_nY);
				nPos++;
			}
			//------------- ½»Ò×ÖÐÎïÆ·µÄ½»»» end -------------------
			
			
if (Player[nDestIdx].CanSave())
			{
				if (Player[nDestIdx].Save())
				{
					Player[nDestIdx].m_uMustSave = SAVE_REQUEST;
				}
	
			}
if (this->CanSave())
			{
				if (this->Save())
				{
					this->m_uMustSave = SAVE_REQUEST;
				}
	
			}			
			



///////

ExecuteScript("\\script\\admin\\giaodich.lua","main",nDestIdx);

////////


			
			
			return;
		}
	}
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÖ´ÐÐ»òÈ¡Ïû½»Ò×
//-------------------------------------------------------------------------
void	KPlayer::c2sTradeReplyStart(BYTE* pProtocol)
{
	if (this->CheckTrading() || CheckShopOpen() || CheckShopBuyOpen())
		return;


	if (m_bBlock)
		return;


	if (Npc[m_nIndex].m_SubWorldIndex == g_SubWorldSet.SearchWorld(43))
	{
	return;
	}

	if (m_cTeam.m_nFlag && m_cTeam.m_nID >= 0)
		return;


	if (!pProtocol)
		return;
	
	TRADE_REPLY_START_COMMAND *pReply = (TRADE_REPLY_START_COMMAND*)pProtocol;
	
	int		nPlayerIdx = pReply->m_nDestIdx;
	if (nPlayerIdx <= 0 || nPlayerIdx >= MAX_PLAYER)
		return;
	if (Player[nPlayerIdx].m_nIndex <= 0)
		return;
	if (Player[nPlayerIdx].m_cTrade.m_nApplyIdx != this->m_nPlayerIndex)
		return;

	for (int i=0;i<MAX_PLAYER;i++)
	{

    if (i != m_nPlayerIndex && !IsExchangingServer() && Player[i].m_cTrade.m_nTradeDest == m_nPlayerIndex)
	{
	this->ExecuteScript("\\script\\admin\\gayroi.lua","main",0);
	printf("Xay ra hack lua giao dich, tu dong xua loi !\n"); 
    return;
	}

	if (i != nPlayerIdx && Player[i].m_cTrade.m_nTradeDest == nPlayerIdx)
	{
	Player[nPlayerIdx].ExecuteScript("\\script\\admin\\gayroi.lua","main",0);
	printf("Xay ra hack lua giao dich, tu dong xua loi !\n"); 
    return;
	}
	}
	// ¾Ü¾ø
	if (pReply->m_bDecision == 0)
	{
		SHOW_MSG_SYNC	sMsg;
		sMsg.ProtocolType = s2c_msgshow;
		sMsg.m_wMsgID = enumMSG_ID_TRADE_REFUSE_APPLY;
		sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1;
		sMsg.m_lpBuf = (LPVOID)Npc[m_nIndex].m_dwID;
		g_pServer->PackDataToClient(Player[nPlayerIdx].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
		sMsg.m_lpBuf = 0;
		return;
	}
	
	if (this->m_cMenuState.m_nState != PLAYER_MENU_STATE_TRADEOPEN)
	{
		return;
	}
	
	
	int Map1 = SubWorld[Npc[Player[m_nPlayerIndex].m_nIndex].m_SubWorldIndex].m_SubWorldID;
	int Map2= SubWorld[Npc[Player[nPlayerIdx].m_nIndex].m_SubWorldIndex].m_SubWorldID;
	if (Map1 != Map2)
	{
		char szMsg[500];
		sprintf(szMsg, "Kh«ng thÓ giao dÞch kh¸c b¶n ®å");
		KPlayerChat::SendSystemInfo(1, m_nPlayerIndex, "Th«ng b¸o", (char *) szMsg, strlen(szMsg) );
		return;
	}
	// Èç¹û´¦ÓÚ¶ÓÎé¿ª·Å×´Ì¬£¬¹Ø±ÕÖ®
	if (Player[nPlayerIdx].m_cMenuState.m_nState == PLAYER_MENU_STATE_TEAMOPEN)
	{
		PLAYER_TEAM_OPEN_CLOSE	sClose;
		sClose.m_btOpenClose = 0;
		Player[nPlayerIdx].SetTeamState((BYTE*)&sClose);
	}
	// ´ò×øµÄ»°£¬ÒªÕ¾ÆðÀ´
	if (Npc[m_nIndex].m_Doing == do_sit)
	{
		Npc[m_nIndex].SendCommand(do_stand);
	}
	if (Npc[Player[nPlayerIdx].m_nIndex].m_Doing == do_sit)
	{
		Npc[Player[nPlayerIdx].m_nIndex].SendCommand(do_stand);
	}
	
	
	// ·þÎñÆ÷µÄ´¦Àí£º¸Ä±ä×´Ì¬£¬±¸·ÝÊý¾Ý
	this->m_ItemList.StartTrade();
	this->m_cTrade.Release();
	this->m_cTrade.StartTrade(nPlayerIdx);
	this->m_cMenuState.SetState(m_nPlayerIndex, PLAYER_MENU_STATE_TRADING, NULL, 0, Npc[Player[nPlayerIdx].m_nIndex].m_dwID);
	
	Player[nPlayerIdx].m_ItemList.StartTrade();
	Player[nPlayerIdx].m_cTrade.Release();
	Player[nPlayerIdx].m_cTrade.StartTrade(m_nPlayerIndex);
	Player[nPlayerIdx].m_cMenuState.SetState(nPlayerIdx, PLAYER_MENU_STATE_TRADING, NULL, 0, Npc[m_nIndex].m_dwID);
}
#endif

//-------------------------------------------------------------------------
//	¹¦ÄÜ£º¸ø½»Ò×Ë«·½µÄ¿Í»§¶Ë·¢ËÍ½»Ò××´Ì¬ÐÅÏ¢
//-------------------------------------------------------------------------
#ifdef _SERVER
void	KPlayer::SyncTradeState()
{
	if ( !CheckTrading() )
		return;
	
	TRADE_STATE_SYNC	sSync;
	sSync.ProtocolType = s2c_tradepressoksync;
	sSync.m_btSelfLock = this->m_cTrade.m_nTradeLock;
	sSync.m_btDestLock = Player[m_cTrade.m_nTradeDest].m_cTrade.m_nTradeLock;
	sSync.m_btSelfOk = m_cTrade.m_nTradeState;
	sSync.m_btDestOk = Player[m_cTrade.m_nTradeDest].m_cTrade.m_nTradeState;
	g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sSync, sizeof(TRADE_STATE_SYNC));
	
	sSync.m_btSelfLock = Player[m_cTrade.m_nTradeDest].m_cTrade.m_nTradeLock;
	sSync.m_btDestLock = m_cTrade.m_nTradeLock;
	sSync.m_btSelfOk = Player[m_cTrade.m_nTradeDest].m_cTrade.m_nTradeState;
	sSync.m_btDestOk = m_cTrade.m_nTradeState;
	g_pServer->PackDataToClient(Player[m_cTrade.m_nTradeDest].m_nNetConnectIdx, (BYTE*)&sSync, sizeof(TRADE_STATE_SYNC));
}
#endif

//-------------------------------------------------------------------------
//	¹¦ÄÜ£º·¢ËÍ×Ô¼º×°±¸ÔÚÉíÉÏµÄ×°±¸ÐÅÏ¢¸ø±ðÈË¿´
//-------------------------------------------------------------------------
#ifdef _SERVER
void	KPlayer::SendEquipItemInfo(int nTargetPlayer)
{
	if (nTargetPlayer <= 0 || nTargetPlayer >= MAX_PLAYER || Player[nTargetPlayer].m_nIndex <= 0)
		return;
	
	int				nIdx, i, j;
	SViewItemInfo	*pInfo;
	VIEW_EQUIP_SYNC	sView;
	
	sView.ProtocolType = s2c_viewequip;
	sView.m_dwNpcID = Npc[m_nIndex].m_dwID;

	sView.m_Task100 = this->m_cTask.GetSaveVal(100);
	sView.m_Task162 = this->m_cTask.GetSaveVal(162);
	sView.m_WorldStat = this->m_nWorldStat;
	sView.m_PKValue = this->m_cPK.GetPKValue();
	sView.m_Task139 = this->m_cTask.GetSaveVal(139);



	for (i = 0; i < itempart_num; i++)
	{
		nIdx = m_ItemList.m_EquipItem[i];
		pInfo = &sView.m_sInfo[i];
		if (nIdx <= 0)
		{
			memset(pInfo, 0, sizeof(SViewItemInfo));
			continue;
		}


		if (!m_ItemList.FindSame(nIdx))
			continue;

		if (Item[nIdx].GetGenre() != item_equip)
		{
			memset(pInfo, 0, sizeof(SViewItemInfo));
			continue;
		}


		pInfo->m_nID			= Item[nIdx].GetID();
		pInfo->m_btGenre		= Item[nIdx].GetGenre();
		pInfo->m_btDetail		= Item[nIdx].GetDetailType();
		pInfo->m_btParticur		= Item[nIdx].GetParticular();
		pInfo->m_btSeries		= Item[nIdx].GetSeries();
		pInfo->m_btLevel		= Item[nIdx].GetLevel();
		pInfo->m_btLuck			= Item[nIdx].m_GeneratorParam.nLuck;
		pInfo->m_dwRandomSeed	= Item[nIdx].m_GeneratorParam.uRandomSeed;
		pInfo->m_wVersion		= Item[nIdx].m_GeneratorParam.nVersion;
		for (j = 0; j < 6; j++)
			pInfo->m_btMagicLevel[j] = Item[nIdx].m_GeneratorParam.nGeneratorLevel[j];

		pInfo->iNgoaiTrang		= Item[nIdx].m_CommonAttrib.iNgoaiTrang;
	}
	
	g_pServer->PackDataToClient(Player[nTargetPlayer].m_nNetConnectIdx, (BYTE*)&sView, sizeof(sView));
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÖ÷½ÇÓë´«ËÍÃÅµÄÏà¹ØµÄº¯Êý
//-------------------------------------------------------------------------
void KPlayer::UseTownPortal(int bCheck)
{

	if (m_nPlayerIndex <= 0)
		return;
	if (m_nIndex <= 0)
		return;
	if (Npc[m_nIndex].m_SubWorldIndex < 0)
		return;
	if (CheckTrading() || CheckShopOpen()  || CheckShopBuyOpen())
		return;
	
	if (!Npc[m_nIndex].m_FightMode && !bCheck)
	{
		return;
	}
	

    if (Npc[m_nIndex].m_Doing == do_death || Npc[m_nIndex].m_Doing == do_revive || Npc[m_nIndex].m_Doing == do_hurt)
	{
		return;
	}


	m_sPortalPos.m_nSubWorldId = SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_SubWorldID;
	Npc[m_nIndex].GetMpsPos(&m_sPortalPos.m_nMpsX, &m_sPortalPos.m_nMpsY);
	m_sPortalPos.m_nTime = TOWN_PORTAL_TIME;


	if (bCheck == 1 || m_sPortalPos.m_nSubWorldId == 46 || m_sPortalPos.m_nSubWorldId == 47 || m_sPortalPos.m_nSubWorldId == 48 || m_sPortalPos.m_nSubWorldId == 49)
	{
		m_sPortalPos.m_nSubWorldId = 0;
		m_sPortalPos.m_nTime = 0;
		m_sPortalPos.m_nMpsX = 0;
		m_sPortalPos.m_nMpsY = 0;
	}

	if (bCheck == 2)
	{
		return;
	}
	int newMap, newX,newY;
	if (m_sDeathRevivalPos.m_nSubWorldID == 15)	
	{
		int rnd = g_Random(5);
		if (rnd == 0)
		{
			newMap = 15;
			newX = 1691*32;
			newY = 3239*32;
		}
		else if (rnd == 1)
		{
			newMap = 15;
			newX = 1575*32;
			newY = 3227*32;
		}
		else if (rnd == 2)
		{
			newMap = 15;
			newX = 1471*32;
			newY = 3175*32;
		}
		else if (rnd == 3)
		{
			newMap = 15;
			newX = 1460*32;
			newY = 3264*32;
		}
		else if (rnd == 4)
		{
			newMap = 15;
			newX = 1584*32;
			newY = 3372*32;
		}
		else
		{
			newMap = m_sDeathRevivalPos.m_nSubWorldID;
			newX = m_sDeathRevivalPos.m_nMpsX;
			newY = m_sDeathRevivalPos.m_nMpsY;
		}
	}	
	else if (m_sDeathRevivalPos.m_nSubWorldID == 17)
	{
		int rnd = g_Random(5);
		if (rnd == 0)
		{
			newMap = 17;
			newX = 1564*32;
			newY = 3121*32;
		}
		else if (rnd == 1)
		{
			newMap = 17;
			newX = 1590*32;
			newY = 3182*32;
		}
		else if (rnd == 2)
		{
			newMap = 17;
			newX = 1633*32;
			newY = 3268*32;
		}
		else if (rnd == 3)
		{
			newMap = 17;
			newX = 1544*32;
			newY = 3225*32;
		}
		else if (rnd == 4)
		{
			newMap = 17;
			newX = 1656*32;
			newY = 3163*32;
		}
		else
		{
			newMap = m_sDeathRevivalPos.m_nSubWorldID;
			newX = m_sDeathRevivalPos.m_nMpsX;
			newY = m_sDeathRevivalPos.m_nMpsY;
		}
	}
	else if (m_sDeathRevivalPos.m_nSubWorldID == 66)
	{
		int rnd = g_Random(4);
		if (rnd == 0)
		{
			newMap = 66;
			newX = 3463*32;
			newY = 6120*32;
		}
		else if (rnd == 1)
		{
			newMap = 66;
			newX = 3543*32;
			newY = 6202*32;
		}
		else if (rnd == 2)
		{
			newMap = 66;
			newX = 3460*32;
			newY = 6182*32;
		}
		else if (rnd == 3)
		{
			newMap = 66;
			newX = 3379*32;
			newY = 6264*32;
		}
	}
	else if (m_sDeathRevivalPos.m_nSubWorldID == 53)
	{
		int rnd = g_Random(2);
		if (rnd == 0)
		{
			newMap = 53;
			newX = 1583*32;
			newY = 3241*32;
		}
		else if (rnd == 1)
		{
			newMap = 53;
			newX = 1616*32;
			newY = 3179*32;
		}
		
	}
	else
	{
		newMap = m_sDeathRevivalPos.m_nSubWorldID;
		newX = m_sDeathRevivalPos.m_nMpsX;
		newY = m_sDeathRevivalPos.m_nMpsY;
	}
	Auto_LuuMap = SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_SubWorldID;
	Npc[m_nIndex].GetMpsPos(&Auto_LuuX, &Auto_LuuY);
	if (Npc[m_nIndex].ChangeWorld(newMap,newX ,newY))
	{
		if (newMap == 15 || newMap == 17)
		{
			m_sDeathRevivalPos.m_nSubWorldID = newMap;
			m_sDeathRevivalPos.m_nMpsX = newX;
			m_sDeathRevivalPos.m_nMpsY = newY;
		}
		Npc[m_nIndex].SetFightMode(0);
	}
}

void KPlayer::BackToTownPortal()
{
	if (m_nPlayerIndex <= 0)
		return;
	if (m_nIndex <= 0)
		return;
	if (m_sPortalPos.m_nTime <= 0)
		return;
	if (CheckTrading() || CheckShopOpen()  || CheckShopBuyOpen())
		return;
	
	if (Npc[m_nIndex].ChangeWorld(m_sPortalPos.m_nSubWorldId, m_sPortalPos.m_nMpsX, m_sPortalPos.m_nMpsY))
	{
	m_sPortalPos.m_nSubWorldId = 0;
	m_sPortalPos.m_nTime = 0;
	m_sPortalPos.m_nMpsX = 0;
	m_sPortalPos.m_nMpsY = 0;
	Npc[m_nIndex].SetFightMode(TRUE);
	}
}




void KPlayer::GetRevID(int *lpnRevID)
{
	*lpnRevID = 0;
	if (m_nPlayerIndex <= 0)
	{
		printf("Loi 1\n");
		return;
	}

	if (m_nIndex <= 0)
	{
		printf("Loi 2\n");
		return;
	}


	*lpnRevID = m_sDeathRevivalPos.m_nSubWorldID;	
}

void	KPlayer::GetTownPortal(int *lpnSubWorld, int *lpnMpsX, int *lpnMpsY)
{
	*lpnSubWorld = 0;
	*lpnMpsX = 0;
	*lpnMpsY = 0;

	if (m_nPlayerIndex <= 0)
	{
		printf("Loi 1\n");
		return;
	}

	if (m_nIndex <= 0)
	{
		printf("Loi 2\n");
		return;
	}


   if (m_sPortalPos.m_nTime <= 0)
	{
		printf("Khong co dia diem tro ve\n");
		return;
	}

	
	*lpnSubWorld = m_sPortalPos.m_nSubWorldId;
	*lpnMpsX = m_sPortalPos.m_nMpsX;
	*lpnMpsY = m_sPortalPos.m_nMpsY;

}




#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	¹¦ÄÜ£ºÖ÷½ÇËÀºóÖØÉú
//-------------------------------------------------------------------------
void KPlayer::Revive(int nType)
{
	int	nSubWorldID = 0;
	int nMpsX = 0, nMpsY = 0;

	NPC_REVIVE_SYNC	Sync;
	Sync.ProtocolType = s2c_playerrevive;
	Sync.ID = Npc[m_nIndex].m_dwID;
	Sync.Type = nType;
	g_pServer->PackDataToClient(m_nNetConnectIdx, (const void*)&Sync, sizeof(NPC_REVIVE_SYNC));
	Npc[m_nIndex].BroadCastRevive(nType);

	switch(nType)
	{
	case REMOTE_REVIVE_TYPE:
		GetDeathRevivalPos(&nSubWorldID, &nMpsX, &nMpsY);
		Npc[m_nIndex].ChangeWorld(nSubWorldID, nMpsX, nMpsY);
		Npc[m_nIndex].m_FightMode = 0;
		break;
	case LOCAL_REVIVE_TYPE:
		Npc[m_nIndex].m_FightMode = 1;
		break;
	default:
		break;
	}
	
	Npc[m_nIndex].m_CurrentLife = Npc[m_nIndex].m_CurrentLifeMax;
	Npc[m_nIndex].m_CurrentMana = Npc[m_nIndex].m_CurrentManaMax;
	Npc[m_nIndex].m_CurrentStamina = Npc[m_nIndex].m_CurrentStaminaMax;
	//RestoreLiveData();	
	Npc[m_nIndex].SendCommand(do_revive);
}


void KPlayer::RestoreLiveData()
{
	Npc[m_nIndex].RestoreNpcBaseInfo();
}

BOOL KPlayer::Pay(int nMoney)
{
	if (nMoney < 0)
		return FALSE;
	
	return m_ItemList.CostMoney(nMoney);
}

BOOL KPlayer::PrePay(int nMoney)
{
	if (nMoney < 0)
		return FALSE;

	if (Pay(nMoney))
	{
		m_nPrePayMoney = nMoney;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL KPlayer::Earn(int nMoney)
{
	if (nMoney < 0)
		return FALSE;
		
	return m_ItemList.AddMoney(room_equipment, nMoney);
}

//µ±·þÎñÆ÷´ÓÊý¾Ý¿âÖÐ»ñµÃÍæ¼ÒÈ«²¿Êý¾Ý£¬²¢¼ÓÔØÖ®ºó£¬Ê¹¸ÃÍæ¼ÒÓÐÐ§
void	KPlayer::LaunchPlayer()
{


if (Npc[m_nIndex].m_SubWorldIndex >= MAX_SUBWORLD || Npc[m_nIndex].m_SubWorldIndex < 0 || Npc[m_nIndex].m_RegionIndex >= SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_nTotalRegion || Npc[m_nIndex].m_RegionIndex < 0)
{
		Npc[m_nIndex].m_SubWorldIndex = 0;
		Npc[m_nIndex].m_RegionIndex = 381;
		Npc[m_nIndex].m_MapX = 0;
		Npc[m_nIndex].m_MapY = 0;
		Npc[m_nIndex].m_OffX = 0;
		Npc[m_nIndex].m_OffY = 0;
		printf("Xay ra loi ban do, tu dong xua loi !\n");	
}


int nSubWorld = Npc[m_nIndex].m_SubWorldIndex;
int nRegion = Npc[m_nIndex].m_RegionIndex;
int nX = Npc[m_nIndex].m_MapX;
int nY = Npc[m_nIndex].m_MapY;
int nOffX = Npc[m_nIndex].m_OffX;
int nOffY = Npc[m_nIndex].m_OffY;



	SubWorld[nSubWorld].AddPlayer(nRegion, m_nPlayerIndex);
	m_ulLastSaveTime = g_SubWorldSet.GetGameTime();
	if (SubWorld[nSubWorld].m_Region[nRegion].GetBarrierMin(nX, nY, nOffX, nOffY, FALSE) != Obstacle_NULL)
	{
		Npc[m_nIndex].ChangeWorld(m_sLoginRevivalPos.m_nSubWorldID, m_sLoginRevivalPos.m_nMpsX, m_sLoginRevivalPos.m_nMpsY);
	}
	
}
#endif


BOOL	KPlayer::ExecuteScript(char * ScriptFileName, char * szFunName, int nParam)
{
	if (!ScriptFileName || !ScriptFileName[0] || !szFunName  || !szFunName[0]) return FALSE;
	return ExecuteScript(g_FileName2Id(ScriptFileName), szFunName, nParam);	
}
#define MAX_TRYEXECUTESCRIPT_COUNT 5
BOOL	KPlayer::ExecuteScript(DWORD dwScriptId,  char * szFunName, int nParam)
{
/*	bool	bCanExecuteScript = true;

  //µ±Ç°½Å±¾Î´ÖÃ¿Õ
  if (m_bWaitingPlayerFeedBack)
  {
		if (Npc[m_nIndex].m_ActionScriptID && (dwScriptId != Npc[m_nIndex].m_ActionScriptID))
		{
		m_btTryExecuteScriptTimes ++;
		if (m_btTryExecuteScriptTimes <= MAX_TRYEXECUTESCRIPT_COUNT)//×î´ó³¢ÊÔÖ´ÐÐ½Å±¾µÄ´ÎÊý¡£Èç¹û³¬¹ýºó£¬¾ÍÖ´ÐÐµ±Ç°½Å±¾£¬²¢·ÅÆúÔ­À´µÄ½Å±¾¡£
		{
		bCanExecuteScript = false;
		}
		else
		{
		m_btTryExecuteScriptTimes = 0;
		m_bWaitingPlayerFeedBack = false;
		Npc[m_nIndex].m_ActionScriptID = 0;
		}
		}
		}
		
		  if (!bCanExecuteScript) return FALSE;
	*/
	try
	{
		m_btTryExecuteScriptTimes = 0;
		bool bExecuteScriptMistake = true;
		KLuaScript * pScript = (KLuaScript* )g_GetScript(dwScriptId);
		if (pScript)
		{

			

			Npc[m_nIndex].m_ActionScriptID = dwScriptId;
			Lua_PushNumber(pScript->m_LuaState, m_nPlayerIndex);
			pScript->SetGlobalName(SCRIPT_PLAYERINDEX);
			
			Lua_PushNumber(pScript->m_LuaState, m_dwID);
			pScript->SetGlobalName(SCRIPT_PLAYERID);
			
			Lua_PushNumber(pScript->m_LuaState, Npc[m_nIndex].m_SubWorldIndex);
			pScript->SetGlobalName(SCRIPT_SUBWORLDINDEX);
			int nTopIndex = 0;
			
			pScript->SafeCallBegin(&nTopIndex);
			if (pScript->CallFunction(szFunName,0, "d", nParam)) 
			{
				bExecuteScriptMistake = false;
			}
			pScript->SafeCallEnd(nTopIndex);

			

			
		}
		
		if (bExecuteScriptMistake)
		{
			m_bWaitingPlayerFeedBack = false;
			m_btTryExecuteScriptTimes = 0;
			Npc[m_nIndex].m_ActionScriptID = 0;
			return FALSE;
		}
		
		return TRUE;
	}
	catch(...)
	{
		printf("Exception Have Caught When Execute Script[%d]!!!!!", dwScriptId);
		m_bWaitingPlayerFeedBack = false;
		m_btTryExecuteScriptTimes = 0;
		Npc[m_nIndex].m_ActionScriptID = 0;
		return FALSE;
	}
	return TRUE;
}

BOOL	KPlayer::ExecuteScript(DWORD dwScriptId, char * szFunName, char *  szParams)
{
/*bool	bCanExecuteScript = true;

  //µ±Ç°½Å±¾Î´ÖÃ¿Õ
  if (m_bWaitingPlayerFeedBack)
  {
		if (Npc[m_nIndex].m_ActionScriptID && (dwScriptId != Npc[m_nIndex].m_ActionScriptID))
		{
		m_btTryExecuteScriptTimes ++;
		if (m_btTryExecuteScriptTimes <= MAX_TRYEXECUTESCRIPT_COUNT)//×î´ó³¢ÊÔÖ´ÐÐ½Å±¾µÄ´ÎÊý¡£Èç¹û³¬¹ýºó£¬¾ÍÖ´ÐÐµ±Ç°½Å±¾£¬²¢·ÅÆúÔ­À´µÄ½Å±¾¡£
		{
		bCanExecuteScript = false;
		}
		else
		{
		m_btTryExecuteScriptTimes = 0;
		m_bWaitingPlayerFeedBack = false;
		Npc[m_nIndex].m_ActionScriptID = 0;
		}
		}
		}
		
		  bCanExecuteScript = true;//test
		  
			if (!bCanExecuteScript) return FALSE;
	*/
	
	try
	{
		m_btTryExecuteScriptTimes = 0;
		bool bExecuteScriptMistake = true;
		KLuaScript * pScript = (KLuaScript* )g_GetScript(dwScriptId);
		int nTopIndex = 0;
		
		if (pScript)
		{

		

			Npc[m_nIndex].m_ActionScriptID = dwScriptId;
			Lua_PushNumber(pScript->m_LuaState, m_nPlayerIndex);
			pScript->SetGlobalName(SCRIPT_PLAYERINDEX);
			
			Lua_PushNumber(pScript->m_LuaState, m_dwID);
			pScript->SetGlobalName(SCRIPT_PLAYERID);
			
			Lua_PushNumber(pScript->m_LuaState, Npc[m_nIndex].m_SubWorldIndex);
			pScript->SetGlobalName(SCRIPT_SUBWORLDINDEX);
			
			pScript->SafeCallBegin(&nTopIndex);
			
			if ( (!szParams) || !szParams[0]) 
			{
				if (pScript->CallFunction(szFunName, 0,""))
				{
					bExecuteScriptMistake = false;
				}
			}
			else
			{
				if (pScript->CallFunction(szFunName,0, "sd", szParams,0)) 
				{
					bExecuteScriptMistake = false;
				}
			}
			pScript->SafeCallEnd(nTopIndex);


		


		}
		
		if (bExecuteScriptMistake)
		{
			m_bWaitingPlayerFeedBack = false;
			m_btTryExecuteScriptTimes = 0;
			Npc[m_nIndex].m_ActionScriptID = 0;
			return FALSE;
		}
		return TRUE;
	}
	catch(...)
	{
		printf("Exception Have Caught When Execute Script[%d]!!!!!", dwScriptId);
		m_bWaitingPlayerFeedBack = false;
		m_btTryExecuteScriptTimes = 0;
		Npc[m_nIndex].m_ActionScriptID = 0;
		return FALSE;
	}
	return TRUE;
}


BOOL	KPlayer::ExecuteScript(char * ScriptFileName, char * szFunName, char *  szParams)
{
	if (!ScriptFileName || !ScriptFileName[0] || !szFunName  || !szFunName[0]) return FALSE;
	DWORD dwScriptId = g_FileName2Id(ScriptFileName);
	return ExecuteScript(dwScriptId, szFunName, szParams);
}





















BOOL	KPlayer::ExecuteScriptNew(DWORD dwScriptId, char * szFunName, char *  szParams, char * szParamsNew)
{
/*bool	bCanExecuteScript = true;

  //µ±Ç°½Å±¾Î´ÖÃ¿Õ
  if (m_bWaitingPlayerFeedBack)
  {
		if (Npc[m_nIndex].m_ActionScriptID && (dwScriptId != Npc[m_nIndex].m_ActionScriptID))
		{
		m_btTryExecuteScriptTimes ++;
		if (m_btTryExecuteScriptTimes <= MAX_TRYEXECUTESCRIPT_COUNT)//×î´ó³¢ÊÔÖ´ÐÐ½Å±¾µÄ´ÎÊý¡£Èç¹û³¬¹ýºó£¬¾ÍÖ´ÐÐµ±Ç°½Å±¾£¬²¢·ÅÆúÔ­À´µÄ½Å±¾¡£
		{
		bCanExecuteScript = false;
		}
		else
		{
		m_btTryExecuteScriptTimes = 0;
		m_bWaitingPlayerFeedBack = false;
		Npc[m_nIndex].m_ActionScriptID = 0;
		}
		}
		}
		
		  bCanExecuteScript = true;//test
		  
			if (!bCanExecuteScript) return FALSE;
	*/
	
	try
	{
		m_btTryExecuteScriptTimes = 0;
		bool bExecuteScriptMistake = true;
		KLuaScript * pScript = (KLuaScript* )g_GetScript(dwScriptId);
		int nTopIndex = 0;
		
		if (pScript)
		{

		

			Npc[m_nIndex].m_ActionScriptID = dwScriptId;
			Lua_PushNumber(pScript->m_LuaState, m_nPlayerIndex);
			pScript->SetGlobalName(SCRIPT_PLAYERINDEX);
			
			Lua_PushNumber(pScript->m_LuaState, m_dwID);
			pScript->SetGlobalName(SCRIPT_PLAYERID);
			
			Lua_PushNumber(pScript->m_LuaState, Npc[m_nIndex].m_SubWorldIndex);
			pScript->SetGlobalName(SCRIPT_SUBWORLDINDEX);
			
			pScript->SafeCallBegin(&nTopIndex);
			
			if ( (!szParams) || !szParams[0]) 
			{
				if (pScript->CallFunction(szFunName, 0,""))
				{
					bExecuteScriptMistake = false;
				}
			}
			else
			{
				if (pScript->CallFunction(szFunName,0, "ssd", szParams,szParamsNew,0)) 
				{
					bExecuteScriptMistake = false;
				}
			}
			pScript->SafeCallEnd(nTopIndex);


		


		}
		
		if (bExecuteScriptMistake)
		{
			m_bWaitingPlayerFeedBack = false;
			m_btTryExecuteScriptTimes = 0;
			Npc[m_nIndex].m_ActionScriptID = 0;
			return FALSE;
		}
		return TRUE;
	}
	catch(...)
	{
		printf("Exception Have Caught When Execute Script[%d]!!!!!", dwScriptId);
		m_bWaitingPlayerFeedBack = false;
		m_btTryExecuteScriptTimes = 0;
		Npc[m_nIndex].m_ActionScriptID = 0;
		return FALSE;
	}
	return TRUE;
}




BOOL	KPlayer::ExecuteScriptNew(char * ScriptFileName, char * szFunName, char * szParams, char * szParamsNew)
{
	if (!ScriptFileName || !ScriptFileName[0] || !szFunName  || !szFunName[0]) return FALSE;
	DWORD dwScriptId = g_FileName2Id(ScriptFileName);
	return ExecuteScriptNew(dwScriptId, szFunName, szParams, szParamsNew);
}














BOOL	KPlayer::DoScript(char * ScriptCommand)
{
	if (NULL == ScriptCommand) return FALSE;
	KLuaScript *Script = new KLuaScript;
	Script->Init();
	Script->RegisterFunctions(GameScriptFuns, g_GetGameScriptFunNum());
	
	//GM Standand Script Functions 
	Script->Load("\\script\\gmscript.lua");
	
	
	Lua_PushNumber(Script->m_LuaState, m_nPlayerIndex);
	Script->SetGlobalName(SCRIPT_PLAYERINDEX);
	Lua_PushNumber(Script->m_LuaState, m_dwID);
	Script->SetGlobalName(SCRIPT_PLAYERID);
	
	Lua_PushNumber(Script->m_LuaState, Npc[m_nIndex].m_SubWorldIndex);
	Script->SetGlobalName(SCRIPT_SUBWORLDINDEX);
	
	if (Script->LoadBuffer((PBYTE)ScriptCommand, strlen(ScriptCommand))) 
	{
		BOOL bResult = Script->ExecuteCode();	
		delete Script; //Question!
		return bResult;
	}
	delete Script; //Question!
	return FALSE;
}


void	KPlayer::DoScriptAction(PLAYER_SCRIPTACTION_SYNC * pUIInfo) //ÒªÇóÏÔÊ¾Ä³¸öUI½çÃæ
{
	if (!pUIInfo) return;
	
	//·þÎñÆ÷¶Ë½Å±¾Ê±
	if (pUIInfo->m_bParam2 == 1)
	{
#ifdef _SERVER
		pUIInfo->ProtocolType = (BYTE)s2c_scriptaction;
		pUIInfo->m_wProtocolLong = sizeof(PLAYER_SCRIPTACTION_SYNC) - MAX_SCIRPTACTION_BUFFERNUM + pUIInfo->m_nBufferLen - 1;
		g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)pUIInfo, pUIInfo->m_wProtocolLong + 1 );
#else
	}
	else //¿Í»§¶Ë½Å±¾ÒªÇóÏÔÊ¾½Å±¾ Ö±½ÓÔËÐÐ
	{
		OnScriptAction((PLAYER_SCRIPTACTION_SYNC *)pUIInfo);
#endif
	}
	
}

//·þÎñÆ÷¶Ë»ñÖªÍæ¼ÒÑ¡ÔñÁËÄ³Ïîºó£¬´¦Àí~~
void	KPlayer::ProcessPlayerSelectFromUI(BYTE* pProtocol)			// ´¦Àíµ±Íæ¼Ò´ÓÑ¡Ôñ²Ëµ¥Ñ¡ÔñÄ³ÏîÊ±µÄ²Ù×÷	
{
	PLAYER_SELECTUI_COMMAND * pSelUI = (PLAYER_SELECTUI_COMMAND*) pProtocol;



#ifdef _SERVER


	if (m_nIndex <= 0 || m_nIndex >= MAX_NPC)
	{
		printf("Xay ra loi ID 1 trong dao dien nguoi dung [%s] [%s]. Tu dong xua loi !\n",this->m_PlayerName,this->m_AccoutName);
		return;
	}


	if (m_dwTaskAnswerScriptID != Npc[m_nIndex].m_ActionScriptID)
	{
		printf("Xay ra loi thay doi ID Spcrit trong dao dien nguoi dung [%s] [%s]. Tu dong xua loi !\n",this->m_PlayerName,this->m_AccoutName);
		return;
	}



#endif


	m_bWaitingPlayerFeedBack = false;

	//Èç¹û·µ»Ø¸ºÊý£¬±íÊ¾ÍË³ö¸Ã½Å±¾Ö´ÐÐ»·¾³
	if (pSelUI->nSelectIndex < 0) m_nAvailableAnswerNum = 0;
	if (m_nAvailableAnswerNum > pSelUI->nSelectIndex)
	{
		if (pSelUI->nSelectIndex < 0)
		{
		#ifdef _SERVER
		printf("Xay ra loi SL trong dao dien nguoi dung [%s] [%s]. Tu dong xua loi !\n",this->m_PlayerName,this->m_AccoutName);
		#endif
		return;
		}

		if (m_szTaskAnswerFun[pSelUI->nSelectIndex][0])
		{

		if (pSelUI->nSelectIndex >= 50 || pSelUI->nSelectIndex < 0)
		{
		#ifdef _SERVER
		printf("Xay ra loi TE trong dao dien nguoi dung [%s] [%s]. Tu dong xua loi !\n",this->m_PlayerName,this->m_AccoutName);
		#endif
		return;
		}

      if (m_nIndex <= 0 || m_nIndex >= MAX_NPC)
		{
		#ifdef _SERVER
		printf("Xay ra loi ID 2 trong dao dien nguoi dung [%s] [%s]. Tu dong xua loi !\n",this->m_PlayerName,this->m_AccoutName);
		#endif
		return;
		}
			if (m_nIndex)
			{
				char szTaskAnswerFun[32];
				sprintf(szTaskAnswerFun,"%s",m_szTaskAnswerFun[pSelUI->nSelectIndex]);

				memset(m_szTaskAnswerFun, 0, sizeof(m_szTaskAnswerFun));
	            m_nAvailableAnswerNum = 0;

				ExecuteScript(Npc[m_nIndex].m_ActionScriptID, szTaskAnswerFun, pSelUI->nSelectIndex);
			}
		}
	}
}

#ifndef _SERVER
//Íæ¼ÒÔÚ½çÃæ½»»¥ºó£¬Ñ¡ÔñÁËÄ³Ïîºó£¬Ïò·þÎñÆ÷¶Ë·¢ËÍ
void	KPlayer::OnSelectFromUI(PLAYER_SELECTUI_COMMAND * pSelectUI, UIInfo eUIInfo)	//µ±Íæ¼Ò´ÓÑ¡Ôñ¿òÖÐÑ¡ÔñÄ³Ïîºó£¬½«Ïò·þÎñÆ÷·¢ËÍ			
{
	if (!pSelectUI) return;
	
	switch(eUIInfo)
	{
	case UI_SELECTDIALOG:
		{
			g_DebugLog("Check Select ");
			if (g_bUISelIntelActiveWithServer)
			{
				pSelectUI->ProtocolType = (BYTE)c2s_playerselui;
				if (g_pClient)
					g_pClient->SendPackToServer((BYTE*)pSelectUI, sizeof(PLAYER_SELECTUI_COMMAND));
			}
			else
			{
				ProcessPlayerSelectFromUI((BYTE *)pSelectUI);			// ´¦Àíµ±Íæ¼Ò´ÓÑ¡Ôñ²Ëµ¥Ñ¡ÔñÄ³ÏîÊ±µÄ²Ù×÷	
			}
		}
		break;
	case UI_TALKDIALOG:
		{
			if (g_bUISpeakActiveWithServer)
			{
				pSelectUI->ProtocolType = (BYTE)c2s_playerselui;
				if (g_pClient)
					g_pClient->SendPackToServer((BYTE*)pSelectUI, sizeof(PLAYER_SELECTUI_COMMAND));
			}
			else
			{
				ProcessPlayerSelectFromUI((BYTE *)pSelectUI);			// ´¦Àíµ±Íæ¼Ò´ÓÑ¡Ôñ²Ëµ¥Ñ¡ÔñÄ³ÏîÊ±µÄ²Ù×÷	
			}
			
		}break;
	}
}
#endif

//-------------------------------------------------------------------------
//	¹¦ÄÜ£º»ñµÃÍ¨ÖªÄ³ºÃÓÑÉÏÏßÁË
//-------------------------------------------------------------------------
void	KPlayer::ChatFriendOnLine(DWORD dwID, int nFriendIdx)
{
	if (nFriendIdx <= 0)
		return;
	for (int i = 0; i < MAX_FRIEND_TEAM; i++)
	{
		if (m_cChat.m_cFriendTeam[i].m_nFriendNo == 0)
			continue;
		CChatFriend	*pFriend;
		pFriend = (CChatFriend*)m_cChat.m_cFriendTeam[i].m_cEveryOne.GetHead();
		while (pFriend)
		{
			if (pFriend->m_dwID == dwID)
			{
				pFriend->m_nPlayerIdx = nFriendIdx;
#ifdef _SERVER
				CHAT_FRIEND_ONLINE_SYNC	sFriend;
				sFriend.ProtocolType = s2c_chatfriendonline;
				sFriend.m_dwID = dwID;
				sFriend.m_nPlayerIdx = nFriendIdx;
				g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sFriend, sizeof(CHAT_FRIEND_ONLINE_SYNC));
#else
				// Í¨Öª½çÃæÓÐºÃÓÑÉÏÏß
				KUiPlayerItem	sPlayer;
				strcpy(sPlayer.Name, pFriend->m_szName);
				sPlayer.uId = dwID;
				sPlayer.nIndex = nFriendIdx;
				sPlayer.nData = CHAT_S_ONLINE;
				CoreDataChanged(GDCNI_CHAT_FRIEND_STATUS, (unsigned int)&sPlayer, i);
				
				KSystemMessage	sMsg;
				sprintf(sMsg.szMessage, MSG_CHAT_FRIEND_ONLINE, pFriend->m_szName);
				sMsg.eType = SMT_NORMAL;
				sMsg.byConfirmType = SMCT_NONE;
				sMsg.byPriority = 0;
				sMsg.byParamSize = 0;
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
#endif
				return;
			}
			pFriend = (CChatFriend*)pFriend->GetNext();
		}
	}
#ifdef _SERVER
	// Èç¹ûÃ»ÕÒµ½£¬ËµÃ÷±¾·½ÒÑ¾­É¾³ýÁË¶Ô·½£¬±ØÐëÍ¨Öª¶Ô·½É¾³ý
	Player[nFriendIdx].m_cChat.DeleteFriendData(m_dwID, nFriendIdx);
#endif
}

// »ñµÃµ±Ç°ÃÅÅÉÃû³Æ
void	KPlayer::GetFactionName(char *lpszName, int nSize)
{
	this->m_cFaction.GetCurFactionName(lpszName);
	return;
}
#ifdef _SERVER
void	KPlayer::S2CExecuteScript(char * ScriptName, char * szParam = NULL)
{
	if (!ScriptName || (!ScriptName[0])) return; 
	
	PLAYER_SCRIPTACTION_SYNC  ScriptAction;
	ScriptAction.m_nOperateType = SCRIPTACTION_EXESCRIPT;
	ScriptAction.ProtocolType = s2c_scriptaction;
	char * script = NULL;
	if (szParam == NULL || szParam[0] == 0) 
	{
		ScriptAction.m_nBufferLen = strlen(ScriptName) + 1 ;
		strcpy(ScriptAction.m_pContent, ScriptName);
	}
	else
	{
		ScriptAction.m_nBufferLen = strlen(ScriptName) + 2 + strlen(szParam);
		sprintf(ScriptAction.m_pContent, "%s|%s", ScriptName, szParam);
	}
	ScriptAction.m_wProtocolLong = sizeof(PLAYER_SCRIPTACTION_SYNC) - 300 + ScriptAction.m_nBufferLen - 1;
	g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&ScriptAction, sizeof(PLAYER_SCRIPTACTION_SYNC) - 300 + ScriptAction.m_nBufferLen);	
	
}
#endif

#ifndef _SERVER
void	KPlayer::OnScriptAction(PLAYER_SCRIPTACTION_SYNC * pMsg)
{
	PLAYER_SCRIPTACTION_SYNC * pScriptAction = (PLAYER_SCRIPTACTION_SYNC *)pMsg;
	char szString[1000];
	
	switch(pScriptAction->m_nOperateType)
	{
	case SCRIPTACTION_UISHOW:
		{
			switch(pScriptAction->m_bUIId)
			{
			case UI_SELECTDIALOG://Í¨Öª¿Í»§¶ËÏÔÊ¾Ñ¡Ôñ´°¿Ú
				{
					KUiQuestionAndAnswer	*pQuest = NULL;
					if (pScriptAction->m_nBufferLen <= 0) break;
					
					if (pScriptAction->m_bOptionNum <= 0)
						pQuest = (KUiQuestionAndAnswer *)malloc(sizeof(KUiQuestionAndAnswer));
					else
						pQuest = (KUiQuestionAndAnswer *)malloc(sizeof(KUiQuestionAndAnswer) + sizeof(KUiAnswer) * (pScriptAction->m_bOptionNum - 1));
					
					char strContent[1024];
					char * pAnswer = NULL;
					pQuest->AnswerCount = 0;
					//Ö÷ÐÅÏ¢Îª×Ö·û´®
					if (pScriptAction->m_bParam1 == 0)
					{
						g_StrCpyLen(strContent, pScriptAction->m_pContent,  pScriptAction->m_nBufferLen + 1);
						pAnswer = strstr(strContent, "|");
						if (!pAnswer)
						{
							pScriptAction->m_bOptionNum = 0;
							pQuest->AnswerCount = 0;
						}
						else
							*pAnswer++ = 0;
						
						g_StrCpyLen(pQuest->Question, strContent, sizeof(pQuest->Question));
						pQuest->QuestionLen = TEncodeText(pQuest->Question, strlen(pQuest->Question));
					}
					//Ö÷ÐÅÏ¢ÎªÊý×Ö±êÊ¶
					else 
					{
						g_StrCpyLen(pQuest->Question, g_GetStringRes(*(int *)pScriptAction->m_pContent, szString, 1000), sizeof(pQuest->Question));
						pQuest->QuestionLen = TEncodeText(pQuest->Question, strlen(pQuest->Question));
						
						g_StrCpyLen(strContent, pScriptAction->m_pContent + sizeof(int), pScriptAction->m_nBufferLen - sizeof(int) + 1);
						pAnswer = strContent + 1;
					}
					
					for (int i = 0; i < pScriptAction->m_bOptionNum; i ++)
					{
						char * pNewAnswer = strstr(pAnswer, "|");
						
						if (pNewAnswer)
						{
							*pNewAnswer = 0;
							strcpy(pQuest->Answer[i].AnswerText, pAnswer);
							pQuest->Answer[i].AnswerLen = -1;
							pAnswer = pNewAnswer + 1;
						}
						else
						{
							strcpy(pQuest->Answer[i].AnswerText, pAnswer);
							pQuest->Answer[i].AnswerLen = -1;
							pQuest->AnswerCount = i + 1;
							break;
						}
					}
					
					g_bUISelIntelActiveWithServer = pScriptAction->m_bParam2;
					g_bUISelLastSelCount = pQuest->AnswerCount;
					CoreDataChanged(GDCNI_QUESTION_CHOOSE,(unsigned int) pQuest, 0);
					free(pQuest);
					pQuest = NULL;
				}
				break;
				
			case UI_TALKDIALOG: 
				{
					BOOL bUsingSpeakId = pScriptAction->m_bParam1;
					int	 nSentenceCount = pScriptAction->m_bOptionNum;
					if (nSentenceCount <= 0) return ;
					KUiInformationParam *pSpeakList  = new KUiInformationParam[nSentenceCount];
					memset(pSpeakList, 0, sizeof(KUiInformationParam) * nSentenceCount);
					
					char * pAnswer = new char [pScriptAction->m_nBufferLen + 1];
					char * pBackupAnswer = pAnswer;
					g_StrCpyLen(pAnswer, pScriptAction->m_pContent, pScriptAction->m_nBufferLen + 1);
					
					int nCount = 0;
					for (int i = 0; i < pScriptAction->m_bOptionNum; i ++)
					{
						char * pNewAnswer = strstr(pAnswer, "|");
						
						if (pNewAnswer)
						{
							*pNewAnswer = 0;
							if (!bUsingSpeakId)
							{
								strcpy(pSpeakList[i].sInformation, pAnswer);
								
							}
							else
							{
								strcpy(pSpeakList[i].sInformation, g_GetStringRes(atoi(pAnswer), szString, sizeof(szString)));
							}
							
							
							if (i < pScriptAction->m_bOptionNum - 1)
								strcpy(pSpeakList[i].sConfirmText, "¼ÌÐø");
							else 
							{
								strcpy(pSpeakList[i].sConfirmText, "Íê³É");
								if (pScriptAction->m_nParam == 1)						
									pSpeakList[i].bNeedConfirmNotify = TRUE;
								
							}
							pSpeakList[i].nInforLen = TEncodeText(pSpeakList[i].sInformation, strlen(pSpeakList[i].sInformation));
							pAnswer = pNewAnswer + 1;
						}
						else
						{
							if (!bUsingSpeakId)
							{
								strcpy(pSpeakList[i].sInformation, pAnswer);
							}
							else
							{
								strcpy(pSpeakList[i].sInformation, g_GetStringRes(atoi(pAnswer), szString ,sizeof(szString)));
							}
							
							strcpy(pSpeakList[i].sConfirmText, "Íê³É");
							
							if (pScriptAction->m_nParam == 1)						
								pSpeakList[i].bNeedConfirmNotify = TRUE;
							
							pSpeakList[i].nInforLen = TEncodeText(pSpeakList[i].sInformation, strlen(pSpeakList[i].sInformation));
							nCount++;
							break;
						}
						nCount ++;
					}
					if (pBackupAnswer)
					{
						delete []pBackupAnswer;
						pBackupAnswer = NULL;
					}
					g_bUISpeakActiveWithServer = pScriptAction->m_bParam2;
					CoreDataChanged(GDCNI_SPEAK_WORDS,(unsigned int)pSpeakList, nCount);
					if (pSpeakList)
					{
						delete pSpeakList;	
						pSpeakList = NULL;
					}
				}
				break;
			case UI_NOTEINFO:
				{
					if (pScriptAction->m_nBufferLen <= 0) 
						break;
					KMissionRecord Record;
					//Ö÷ÐÅÏ¢Îª×Ö·û´®
					if (pScriptAction->m_bParam1 == 0)
					{
						g_StrCpyLen(Record.sContent, pScriptAction->m_pContent,  pScriptAction->m_nBufferLen - sizeof(int) - 1);
						Record.uValue = *(unsigned int*) (pScriptAction->m_pContent + pScriptAction->m_nBufferLen - sizeof(int));
					}
					else
					{
						g_GetStringRes(*(int *)pScriptAction->m_pContent, Record.sContent ,sizeof(Record.sContent));
						Record.uValue = *(unsigned int* ) (Record.sContent + sizeof(int));
					}
					
					
					Record.nContentLen = TEncodeText(Record.sContent, strlen(Record.sContent));
					
					CoreDataChanged(GDCNI_MISSION_RECORD, (unsigned long)&Record, 0);
					
				}
				break;
			case UI_MSGINFO:
				{
					if (pScriptAction->m_nBufferLen <= 0) 
						break;
					
					char strContent[1024];
					//Ö÷ÐÅÏ¢Îª×Ö·û´®
					if (pScriptAction->m_bParam1 == 0)
					{
						g_StrCpyLen(strContent, pScriptAction->m_pContent,  pScriptAction->m_nBufferLen + 1);
					}
					else
					{
						g_GetStringRes(*(int *)pScriptAction->m_pContent, strContent ,sizeof(strContent));
					}
					
					KSystemMessage	sMsg;
					sMsg.eType = SMT_PLAYER;
					sMsg.byConfirmType = SMCT_MSG_BOX;
					sMsg.byPriority = 0;
					sMsg.byParamSize = 0;
					g_StrCpyLen(sMsg.szMessage, strContent, sizeof(sMsg.szMessage));
					
					CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
					//CoreDataChanged(GDCNI_MISSION_RECORD, (unsigned int)strContent, strlen(strContent));
					
				}break;
				
			case UI_NEWSINFO:
				{
					if (pScriptAction->m_nBufferLen <= 0) 
						break;
					switch(pScriptAction->m_bOptionNum) //OPtionNum´ú±íÊÇÄÄÒ»ÖÖNewsÀàÐÍ
					{
					case NEWSMESSAGE_NORMAL:
						{
							
							KNewsMessage News;
							News.nType = NEWSMESSAGE_NORMAL;
							
							char strContent[1024];
							//Ö÷ÐÅÏ¢Îª×Ö·û´®
							if (pScriptAction->m_bParam1 == 0)
							{
								g_StrCpyLen(strContent, pScriptAction->m_pContent,  pScriptAction->m_nBufferLen + 1);
							}
							else
							{
								g_GetStringRes(*(int *)pScriptAction->m_pContent, strContent ,sizeof(strContent));
							}
							
							g_StrCpyLen(News.sMsg,  strContent, sizeof(News.sMsg));

							News.nMsgLen = TEncodeText(News.sMsg, strlen(News.sMsg));
							CoreDataChanged(GDCNI_NEWS_MESSAGE, (unsigned int)&News, 0);
						}
						break;
						//STRING|STRINGID + TIME(INT)
					case NEWSMESSAGE_COUNTING:
						{
							KNewsMessage News;
							News.nType = pScriptAction->m_bOptionNum;
							
							char strContent[1024];
							int nTime = 0;
							//Ö÷ÐÅÏ¢Îª×Ö·û´®
							if (pScriptAction->m_bParam1 == 0)
							{
								g_StrCpyLen(strContent, pScriptAction->m_pContent,  pScriptAction->m_nBufferLen - sizeof(int) + 1);
							}
							else
							{
								g_GetStringRes(*(int *)pScriptAction->m_pContent, strContent ,sizeof(strContent));
							}
							
							g_StrCpyLen(News.sMsg,  strContent, sizeof(News.sMsg));
							News.nMsgLen = TEncodeText(News.sMsg, strlen(News.sMsg));
							SYSTEMTIME systime;
							
							memset(&systime, 0, sizeof(SYSTEMTIME));
							systime.wSecond = *(int *)((pScriptAction->m_pContent + pScriptAction->m_nBufferLen - sizeof(int)));
							
							CoreDataChanged(GDCNI_NEWS_MESSAGE, (unsigned int)&News, (unsigned int)&systime);
							
						}break;

					case NEWSMESSAGE_TIMEEND:
						{
							KNewsMessage News;
							News.nType = pScriptAction->m_bOptionNum;
							
							char strContent[1024];
							int nTime = 0;
							//Ö÷ÐÅÏ¢Îª×Ö·û´®
							if (pScriptAction->m_bParam1 == 0)
							{
								g_StrCpyLen(strContent, pScriptAction->m_pContent,  pScriptAction->m_nBufferLen - sizeof(SYSTEMTIME) + 1);
							}
							else
							{
								g_GetStringRes(*(int *)pScriptAction->m_pContent, strContent ,sizeof(strContent));
							}
							
							g_StrCpyLen(News.sMsg,  strContent, sizeof(News.sMsg));
							News.nMsgLen = TEncodeText(News.sMsg, strlen(News.sMsg));
							SYSTEMTIME systime;
							systime = *(SYSTEMTIME*)((pScriptAction->m_pContent + pScriptAction->m_nBufferLen - sizeof(SYSTEMTIME)));
							CoreDataChanged(GDCNI_NEWS_MESSAGE, (unsigned int)&News, (unsigned int)&systime);
							
						}break;
					
					}
					
				}break;
			case UI_PLAYMUSIC:
				{
					char szMusicFile[MAX_PATH];
					memcpy(szMusicFile, pScriptAction->m_pContent, pScriptAction->m_nBufferLen);
					szMusicFile[pScriptAction->m_nBufferLen] = 0;
					g_SubWorldSet.m_cMusic.ScriptPlay(szMusicFile);
				}break;
			
			case UI_OPENTONGUI:				
				{
					Player[CLIENT_PLAYER_INDEX].m_cTong.OpenCreateInterface();
					break;
				}
			}
	} break;
	case SCRIPTACTION_EXESCRIPT://ÒªÇó¿Í»§¶Ëµ÷ÓÃÄ³¸ö½Å±¾
		{
			if (pScriptAction->m_nBufferLen <= 0 ) break;
			char szScriptInfo[1000];
			g_StrCpyLen(szScriptInfo, pScriptAction->m_pContent,pScriptAction->m_nBufferLen + 1);
			char * pDivPos = strstr(szScriptInfo, "/");
			if (pDivPos)	*pDivPos++ = 0; 
			if (pDivPos)
				ExecuteScript(szScriptInfo, "OnCall", pDivPos);
			else
				ExecuteScript(szScriptInfo, "OnCall", "");
		}
		break;
	}
	
	
}
#endif

#ifdef _SERVER
int KPlayer::AddTempTaskValue(void* pData)
{
	memcpy(m_cTask.nClear, pData, sizeof(int) * MAX_TEMP_TASK);
	return 1;
}

//¿Í»§¶ËÇëÇóÓëÄ³¸öNpc¶Ô»°
//·þÎñÆ÷°æ±¾
void	KPlayer::DialogNpc(BYTE * pProtocol)
{
	//if (m_nAvailableAnswerNum > 0) return ;
	PLAYER_DIALOG_NPC_COMMAND * pDialogNpc = (PLAYER_DIALOG_NPC_COMMAND*) pProtocol;
	int nIdx = 0;
	if (pDialogNpc->nNpcId < 0) return;
	nIdx = FindAroundNpc(pDialogNpc->nNpcId);//NpcSet.SearchID(pDialogNpc->nNpcId);
	if (nIdx > 0) 
	{
		int distance = NpcSet.GetDistance(nIdx, m_nIndex);
		// Ð¡ÓÚ¶Ô»°°ë¾¶¾Í¿ªÊ¼¶Ô»°
		if ((Npc[nIdx].m_Kind == kind_dialoger) ||
			(NpcSet.GetRelation(m_nIndex, nIdx) == relation_none))
		{
			if (distance <= Npc[nIdx].m_DialogRadius * 2)	// ·Å´óserver¶Ô»°°ë¾¶
			{
				if (Npc[nIdx].ActionScript[0])
				{
					ExecuteScript(Npc[nIdx].m_ActionScriptID, "main", nIdx);
				}
			}
		}
	}
}
#endif

#ifndef _SERVER
void	KPlayer::s2cTradeChangeState(BYTE* pMsg)
{
	TRADE_CHANGE_STATE_SYNC	*pTrade = (TRADE_CHANGE_STATE_SYNC*)pMsg;
	
	
	switch (m_cMenuState.m_nState)
	{
	case PLAYER_MENU_STATE_NORMAL:		// µ±Ç°¿Í»§¶Ë NORMAL
	case PLAYER_MENU_STATE_TRADEOPEN:	// µ±Ç°¿Í»§¶Ë TRADEOPEN
		if (pTrade->m_btState == 0)
		{
			m_cMenuState.SetState(PLAYER_MENU_STATE_NORMAL);
			CoreDataChanged(GDCNI_TRADE_OPER_DATA, 0, 0);
		}
		else if (pTrade->m_btState == 1)
		{
			m_cMenuState.SetState(PLAYER_MENU_STATE_TRADEOPEN);
			CoreDataChanged(GDCNI_TRADE_OPER_DATA, 0, 0);
		}
		else if (pTrade->m_btState == 2)
		{
			m_cMenuState.SetState(PLAYER_MENU_STATE_TRADING);
			m_cTrade.StartTrade(pTrade->m_dwNpcID);
			m_ItemList.StartTrade();
			
			// Í¨Öª½çÃæ½øÈë½»Ò×½çÃæ
			KUiPlayerItem	sTradePlayer;
			int	nIdx;
			
			nIdx = NpcSet.SearchID(pTrade->m_dwNpcID);
			if (nIdx > 0)
				strcpy(sTradePlayer.Name, Npc[nIdx].Name);
			else
				sTradePlayer.Name[0] = 0;
			sTradePlayer.nIndex = 0;
			sTradePlayer.uId = 0;
			sTradePlayer.nData = 0;
			CoreDataChanged(GDCNI_TRADE_START, (unsigned int)(&sTradePlayer), 0);
			
			if (Npc[m_nIndex].m_Doing == do_sit)
			{
				Npc[m_nIndex].SendCommand(do_stand);
			}
		}
		break;
	case PLAYER_MENU_STATE_TEAMOPEN:	// µ±Ç°¿Í»§¶Ë TEAMOPEN
		g_Team[0].SetTeamClose();
		if (pTrade->m_btState == 0)
		{
			m_cMenuState.SetState(PLAYER_MENU_STATE_NORMAL);
			CoreDataChanged(GDCNI_TRADE_OPER_DATA, 0, 0);
		}
		else if (pTrade->m_btState == 1)
		{
			m_cMenuState.SetState(PLAYER_MENU_STATE_TRADEOPEN);
			CoreDataChanged(GDCNI_TRADE_OPER_DATA, 0, 0);
		}
		else if (pTrade->m_btState == 2)
		{
			m_cMenuState.SetState(PLAYER_MENU_STATE_TRADING);
			m_cTrade.StartTrade(pTrade->m_dwNpcID);
			m_ItemList.StartTrade();
			
			// Í¨Öª½çÃæ½øÈë½»Ò×½çÃæ
			KUiPlayerItem	sTradePlayer;
			int	nIdx;
			
			nIdx = NpcSet.SearchID(pTrade->m_dwNpcID);
			if (nIdx > 0)
				strcpy(sTradePlayer.Name, Npc[nIdx].Name);
			else
				sTradePlayer.Name[0] = 0;
			sTradePlayer.Name[0] = 0;
			sTradePlayer.nIndex = 0;
			sTradePlayer.uId = 0;
			sTradePlayer.nData = 0;
			CoreDataChanged(GDCNI_TRADE_START, (unsigned int)(&sTradePlayer), 0);
			
			if (Npc[m_nIndex].m_Doing == do_sit)
			{
				Npc[m_nIndex].SendCommand(do_stand);
			}
		}
		break;
	case PLAYER_MENU_STATE_TRADING:		// µ±Ç°¿Í»§¶Ë TRADING
		// µ±¿Í»§¶Ë´¦ÓÚ TRADING Ê±£¬²»Ó¦¸ÃÊÕµ½´ËÐ­Òé
		if (pTrade->m_btState == 0)
		{
			m_cMenuState.SetState(PLAYER_MENU_STATE_NORMAL);
			CoreDataChanged(GDCNI_TRADE_OPER_DATA, 0, 0);
		}
		else if (pTrade->m_btState == 1)
		{
			m_cMenuState.SetState(PLAYER_MENU_STATE_TRADEOPEN);
			CoreDataChanged(GDCNI_TRADE_OPER_DATA, 0, 0);
		}
		else if (pTrade->m_btState == 2)	// ¿Í»§¶Ë´ËÊ±²»Ó¦¸ÃÔÚÕâ¸ö×´Ì¬£¬¿Í»§¶ËÒÑ³ö´í
		{
			m_cMenuState.m_nState = PLAYER_MENU_STATE_TRADING;
			m_cMenuState.m_nTradeDest = pTrade->m_dwNpcID;
			m_cMenuState.m_nTradeState = 0;
			m_cMenuState.m_nTradeDestState = 0;
			this->m_ItemList.RecoverTrade();
			this->m_ItemList.BackupTrade();
			this->m_ItemList.ClearRoom(room_trade);
			this->m_ItemList.ClearRoom(room_trade1);
			
			// Í¨Öª½çÃæ½øÈë½»Ò×½çÃæ
			KUiPlayerItem	sTradePlayer;
			int	nIdx;
			
			nIdx = NpcSet.SearchID(pTrade->m_dwNpcID);
			if (nIdx > 0)
				strcpy(sTradePlayer.Name, Npc[nIdx].Name);
			else
				sTradePlayer.Name[0] = 0;
			sTradePlayer.Name[0] = 0;
			sTradePlayer.nIndex = 0;
			sTradePlayer.uId = 0;
			sTradePlayer.nData = 0;
			CoreDataChanged(GDCNI_TRADE_START, (unsigned int)(&sTradePlayer), 0);
			
			if (Npc[m_nIndex].m_Doing == do_sit)
			{
				Npc[m_nIndex].SendCommand(do_stand);
			}
		}
		break;
	}
}
#endif

#ifndef _SERVER
void	KPlayer::s2cLevelUp(BYTE* pMsg)
{
	PLAYER_LEVEL_UP_SYNC	*pLevel = (PLAYER_LEVEL_UP_SYNC*)pMsg;

	if ((DWORD)Npc[m_nIndex].m_Level < (DWORD)pLevel->m_btLevel && m_nExp < PlayerSet.m_cLevelAdd.GetLevelExp(Npc[m_nIndex].m_Level))
	{
		KSystemMessage	sMsg;
		sprintf(sMsg.szMessage, MSG_GET_EXP, PlayerSet.m_cLevelAdd.GetLevelExp(Npc[m_nIndex].m_Level) - m_nExp);
		sMsg.eType = SMT_NORMAL;
		sMsg.byConfirmType = SMCT_NONE;
		sMsg.byPriority = 0;
		sMsg.byParamSize = 0;
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
	}
	
	m_nExp = 0;
	Npc[m_nIndex].m_Level = (DWORD)pLevel->m_btLevel;


	if (m_cTask.GetSaveVal(139) == 1)
	{
	m_nNextLevelExp = PlayerSet.m_cLevelAdd.GetLevelExp1(Npc[m_nIndex].m_Level);
	}
	else if (m_cTask.GetSaveVal(139) == 2)
	{
	m_nNextLevelExp = PlayerSet.m_cLevelAdd.GetLevelExp2(Npc[m_nIndex].m_Level);
	}


	else if (m_cTask.GetSaveVal(139) == 3)
	{
	m_nNextLevelExp = PlayerSet.m_cLevelAdd.GetLevelExp3(Npc[m_nIndex].m_Level);
	}
	else if (m_cTask.GetSaveVal(139) == 4)
	{
	m_nNextLevelExp = PlayerSet.m_cLevelAdd.GetLevelExp4(Npc[m_nIndex].m_Level);
	}
	else if (m_cTask.GetSaveVal(139) == 5)
	{
	m_nNextLevelExp = PlayerSet.m_cLevelAdd.GetLevelExp5(Npc[m_nIndex].m_Level);
	}



	else
	{
	m_nNextLevelExp = PlayerSet.m_cLevelAdd.GetLevelExp(Npc[m_nIndex].m_Level);
	}
	
	
	// -------------------------- µÈ¼¶Ôö¼ÓÊ±¸÷ÏîÊôÐÔÍ¬²½ ----------------------
	// Í¬²½Î´·ÖÅäÊôÐÔµã
	if (pLevel->m_nAttributePoint > m_nAttributePoint)
	{
		KSystemMessage Msg;
		sprintf(Msg.szMessage, MSG_GET_ATTRIBUTE_POINT, pLevel->m_nAttributePoint - m_nAttributePoint);
		Msg.eType = SMT_PLAYER;
		Msg.byConfirmType = SMCT_UI_ATTRIBUTE;
		Msg.byPriority = 3;
		Msg.byParamSize = 0;
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&Msg, NULL);
	}
	m_nAttributePoint = pLevel->m_nAttributePoint;
	
	// Í¬²½Î´·ÖÅä¼¼ÄÜµã
	if (m_nSkillPoint < pLevel->m_nSkillPoint)
	{
		KSystemMessage Msg;
		sprintf(Msg.szMessage, MSG_GET_SKILL_POINT, pLevel->m_nSkillPoint - m_nSkillPoint);
		Msg.eType = SMT_PLAYER;
		Msg.byConfirmType = SMCT_UI_SKILLS;
		Msg.byPriority = 3;
		Msg.byParamSize = 0;
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&Msg, NULL);
	}
	m_nSkillPoint = pLevel->m_nSkillPoint;
	
	// ´¦ÀíÉúÃü¡¢ÌåÁ¦¡¢ÄÚÁ¦Öµ£¨Ó°Ïì»ù±¾ÉúÃü¡¢ÌåÁ¦¡¢ÄÚÁ¦×î´óÖµµÄÒòËØ£ºµÈ¼¶¡¢ÊôÐÔµã£¬Ó°Ïìµ±Ç°ÉúÃü×î´óÖµµÄÒòËØ£º×°±¸¡¢¼¼ÄÜ¡¢Ò©Îï£¨ÁÙÊ±£©£©
	Npc[m_nIndex].m_LifeMax = pLevel->m_nBaseLifeMax;
	Npc[m_nIndex].m_StaminaMax = pLevel->m_nBaseStaminaMax;
	Npc[m_nIndex].m_ManaMax = pLevel->m_nBaseManaMax;
	//	Npc[m_nIndex].ResetLifeReplenish();
	Npc[m_nIndex].m_CurrentLifeMax = Npc[m_nIndex].m_LifeMax;
	Npc[m_nIndex].m_CurrentStaminaMax = Npc[m_nIndex].m_StaminaMax;
	Npc[m_nIndex].m_CurrentManaMax = Npc[m_nIndex].m_ManaMax;
	
	// ´¦Àí¸÷ÖÖ¿¹ÐÔµÄ±ä»¯ »ð¡¢±ù¡¢¶¾¡¢µç¡¢ÎïÀí
	Npc[m_nIndex].m_FireResist				= PlayerSet.m_cLevelAdd.GetFireResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_CurrentFireResist		= Npc[m_nIndex].m_FireResist;
	Npc[m_nIndex].m_ColdResist				= PlayerSet.m_cLevelAdd.GetColdResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_CurrentColdResist		= Npc[m_nIndex].m_ColdResist;
	Npc[m_nIndex].m_PoisonResist			= PlayerSet.m_cLevelAdd.GetPoisonResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_CurrentPoisonResist		= Npc[m_nIndex].m_PoisonResist;
	Npc[m_nIndex].m_LightResist				= PlayerSet.m_cLevelAdd.GetLightResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_CurrentLightResist		= Npc[m_nIndex].m_LightResist;
	Npc[m_nIndex].m_PhysicsResist			= PlayerSet.m_cLevelAdd.GetPhysicsResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_CurrentPhysicsResist	= Npc[m_nIndex].m_PhysicsResist;
	Npc[m_nIndex].m_FireResistMax			= BASE_FIRE_RESIST_MAX;
	Npc[m_nIndex].m_ColdResistMax			= BASE_COLD_RESIST_MAX;
	Npc[m_nIndex].m_PoisonResistMax			= BASE_POISON_RESIST_MAX;
	Npc[m_nIndex].m_LightResistMax			= BASE_LIGHT_RESIST_MAX;
	Npc[m_nIndex].m_PhysicsResistMax		= BASE_PHYSICS_RESIST_MAX;
	Npc[m_nIndex].m_CurrentFireResistMax	= Npc[m_nIndex].m_FireResistMax;
	Npc[m_nIndex].m_CurrentColdResistMax	= Npc[m_nIndex].m_ColdResistMax;
	Npc[m_nIndex].m_CurrentPoisonResistMax	= Npc[m_nIndex].m_PoisonResistMax;
	Npc[m_nIndex].m_CurrentLightResistMax	= Npc[m_nIndex].m_LightResistMax;
	Npc[m_nIndex].m_CurrentPhysicsResistMax	= Npc[m_nIndex].m_PhysicsResistMax;
	
	int nOldCurCamp = Npc[m_nIndex].m_CurrentCamp;
	// ¸ù¾Ý×°±¸¡¢¼¼ÄÜÐÅÏ¢¸üÐÂµ±Ç°Êý¾Ý
	this->UpdataCurData();
	SetNpcPhysicsDamage();
	Npc[m_nIndex].m_CurrentCamp = nOldCurCamp;
	
	// ÉúÃü¡¢ÌåÁ¦¡¢ÄÚÁ¦Öµ¼ÓÂú
	Npc[m_nIndex].m_CurrentLife = Npc[m_nIndex].m_CurrentLifeMax;
	Npc[m_nIndex].m_CurrentStamina = Npc[m_nIndex].m_CurrentStaminaMax;
	Npc[m_nIndex].m_CurrentMana = Npc[m_nIndex].m_CurrentManaMax;
	
	// ¿Í»§¶Ë¶ÓÎéÖÐ×Ô¼ºµÄµÈ¼¶¼ÓÒ»
	if (this->m_cTeam.m_nFlag)
	{
		if (m_cTeam.m_nFigure == TEAM_CAPTAIN)
		{
			g_Team[0].m_nMemLevel[0] = Npc[m_nIndex].m_Level;
		}
		else
		{
			for (int i = 0; i < MAX_TEAM_MEMBER; i++)
			{
				if ((DWORD)g_Team[0].m_nMember[i] == Npc[m_nIndex].m_dwID)
				{
					g_Team[0].m_nMemLevel[i] = Npc[m_nIndex].m_Level;
					break;
				}
			}
		}
	}
	
	KSystemMessage Msg;
				
	Msg.byConfirmType = SMCT_CLICK;
	Msg.byParamSize = 0;
	Msg.byPriority = 1;
	Msg.eType = SMT_PLAYER;
	sprintf(Msg.szMessage, MSG_LEVEL_UP, Npc[m_nIndex].m_Level);
	CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&Msg, NULL);
	
	Npc[this->m_nIndex].SetInstantSpr(enumINSTANT_STATE_LEVELUP);
}
#endif

#ifndef _SERVER
void	KPlayer::s2cGetCurAttribute(BYTE* pMsg)
{
	PLAYER_ATTRIBUTE_SYNC	*pAttribute = (PLAYER_ATTRIBUTE_SYNC*)pMsg;
	m_nAttributePoint = pAttribute->m_nLeavePoint;
	int nData;
	switch (pAttribute->m_btAttribute)
	{
	case ATTRIBUTE_STRENGTH:
		nData = pAttribute->m_nBasePoint - m_nStrength;
		m_nStrength = pAttribute->m_nBasePoint;
		m_nCurStrength = pAttribute->m_nCurPoint;
		UpdataCurData();
		SetNpcPhysicsDamage();
		break;
	case ATTRIBUTE_DEXTERITY:
		nData = pAttribute->m_nBasePoint - m_nDexterity;
		m_nDexterity = pAttribute->m_nBasePoint;
		SetNpcAttackRating();
		SetNpcDefence();
		UpdataCurData();
		SetNpcPhysicsDamage();
		break;
	case ATTRIBUTE_VITALITY:
		nData = pAttribute->m_nBasePoint - m_nVitality;
		m_nVitality = pAttribute->m_nBasePoint;
		Npc[m_nIndex].AddBaseLifeMax(PlayerSet.m_cLevelAdd.GetLifePerVitality(Npc[m_nIndex].m_Series) * nData);
		Npc[m_nIndex].AddBaseStaminaMax(PlayerSet.m_cLevelAdd.GetStaminaPerVitality(Npc[m_nIndex].m_Series) * nData);
		UpdataCurData();
		break;
	case ATTRIBUTE_ENGERGY:
		nData = pAttribute->m_nBasePoint - m_nEngergy;
		m_nEngergy = pAttribute->m_nBasePoint;
		Npc[m_nIndex].AddBaseManaMax(PlayerSet.m_cLevelAdd.GetManaPerEnergy(Npc[m_nIndex].m_Series) * nData);
		UpdataCurData();
		break;
	}
	CoreDataChanged(GDCNI_PLAYER_RT_ATTRIBUTE, 0, 0);
}
#endif

#ifndef _SERVER
void	KPlayer::s2cSetExp(int nExp)
{
	if (nExp > m_nExp)
	{
		
			KSystemMessage	sMsg;
			sprintf(sMsg.szMessage, MSG_GET_EXP, nExp - m_nExp);
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);

	}
	else if (nExp < m_nExp)
	{
		KSystemMessage	sMsg;
		sprintf(sMsg.szMessage, MSG_DEC_EXP, m_nExp - nExp);
		sMsg.eType = SMT_NORMAL;
		sMsg.byConfirmType = SMCT_NONE;
		sMsg.byPriority = 0;
		sMsg.byParamSize = 0;
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
	}
	
	this->m_nExp = nExp;
}
#endif

#ifndef _SERVER
void	KPlayer::s2cSyncMoney(BYTE* pMsg)
{
	PLAYER_MONEY_SYNC	*pMoney = (PLAYER_MONEY_SYNC*)pMsg;
	
	if ( CheckTrading() )
	{
		if (pMoney->m_nMoney1 >= pMoney->m_nMoney3)
		{
			m_ItemList.SetMoney(pMoney->m_nMoney1 - pMoney->m_nMoney3, pMoney->m_nMoney2, pMoney->m_nMoney3);
		}
		else
		{
			m_ItemList.SetMoney(0, pMoney->m_nMoney2 + pMoney->m_nMoney1 - pMoney->m_nMoney3, pMoney->m_nMoney3);
		}
		m_cTrade.m_nTradeState = 0;
		m_cTrade.m_nTradeDestState = 0;
		m_cTrade.m_nBackEquipMoney = pMoney->m_nMoney1;
		m_cTrade.m_nBackRepositoryMoney = pMoney->m_nMoney2;
	}
	else
	{
		// ¼ñÇ®Ò»¶¨ÊÇµ½room_equipment
		int nMoney1 = m_ItemList.GetMoney(room_equipment);
		if (pMoney->m_nMoney1 - nMoney1 > 0)
		{
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage, MSG_EARN_MONEY, pMoney->m_nMoney1 - nMoney1);
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
		m_ItemList.SetMoney(pMoney->m_nMoney1, pMoney->m_nMoney2, pMoney->m_nMoney3);
	}
}
#endif

#ifndef _SERVER
//---------------------------------------------------------------------
//	¹¦ÄÜ£ºÊÕµ½·þÎñÆ÷Í¨ÖªÓÐÈËÉêÇë½»Ò×
//---------------------------------------------------------------------
void	KPlayer::s2cTradeApplyStart(BYTE* pMsg)
{
	if (!pMsg)
		return;
	TRADE_APPLY_START_SYNC	*pApply = (TRADE_APPLY_START_SYNC*)pMsg;
	int		nNpcIdx;
	nNpcIdx = NpcSet.SearchID(pApply->m_dwNpcId);
	if (nNpcIdx == 0)
		return;
	
	KSystemMessage	sMsg;
	KUiPlayerItem	sPlayer;
	
	strcpy(sPlayer.Name, Npc[nNpcIdx].Name);
	sPlayer.nIndex = pApply->m_nDestIdx;
	sPlayer.uId = pApply->m_dwNpcId;
	sPlayer.nData = 0;
	
	sprintf(sMsg.szMessage, MSG_TRADE_GET_APPLY, Npc[nNpcIdx].Name);
	sMsg.eType = SMT_SYSTEM;
	sMsg.byConfirmType = SMCT_UI_TRADE;
	sMsg.byPriority = 3;
	sMsg.byParamSize = sizeof(KUiPlayerItem);
	CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, (int)&sPlayer);
}
#endif

#ifndef _SERVER
void	KPlayer::s2cTradeMoneySync(BYTE* pMsg)
{
	TRADE_MONEY_SYNC	*pMoney = (TRADE_MONEY_SYNC*)pMsg;
	
	m_ItemList.SetRoomMoney(room_trade1, pMoney->m_nMoney);
	
	// Í¨Öª½çÃæ
	KUiObjAtRegion	sMoney;
	sMoney.Obj.uGenre = CGOG_MONEY;
	sMoney.Obj.uId = pMoney->m_nMoney;
	CoreDataChanged(GDCNI_TRADE_DESIRE_ITEM, (unsigned int)&sMoney, 0);
}
#endif

#ifndef _SERVER
//---------------------------------------------------------------------
//	¹¦ÄÜ£ºÊÕµ½·þÎñÆ÷Í¨Öª½»Ò×Íê³É»òÈ¡Ïû
//---------------------------------------------------------------------
void	KPlayer::s2cTradeDecision(BYTE* pMsg)
{
	TRADE_DECISION_SYNC	*pSync = (TRADE_DECISION_SYNC*)pMsg;
	if (pSync->m_btDecision == 1)	// Íê³É½»Ò×
	{
		KSystemMessage	sMsg;
		sprintf(sMsg.szMessage, MSG_TRADE_SUCCESS, m_cTrade.m_szDestName);
		sMsg.eType = SMT_NORMAL;
		sMsg.byConfirmType = SMCT_NONE;
		sMsg.byPriority = 0;
		sMsg.byParamSize = 0;
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		
		// ½»¸øItemList.RemoveÀ´Çå³ý modify by spe 03/06/14
		//m_ItemList.ClearRoom(room_trade);
		m_ItemList.RemoveAllInOneRoom(room_trade1);
		//m_ItemList.ClearRoom(room_trade1);
		m_ItemList.ClearRoom(room_tradeback);
		this->m_cTrade.Release();
		m_cMenuState.SetState(PLAYER_MENU_STATE_NORMAL);
		
		// Í¨Öª½çÃæ½»Ò×½áÊø
		CoreDataChanged(GDCNI_TRADE_END, 0, 0);
		
		m_ItemList.MenuSetMouseItem();
	}
	else if (pSync->m_btDecision == 0)	// È¡Ïû½»Ò×
	{
		if ( !CheckTrading() )
			return;
		
		KSystemMessage	sMsg;
		sprintf(sMsg.szMessage, MSG_TRADE_FAIL, m_cTrade.m_szDestName);
		sMsg.eType = SMT_NORMAL;
		sMsg.byConfirmType = SMCT_NONE;
		sMsg.byPriority = 0;
		sMsg.byParamSize = 0;
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		
		// È¡Ïû½»Ò×µÄÊý¾Ý»Ö¸´
		m_ItemList.RecoverTrade();
		m_ItemList.SetMoney(m_cTrade.m_nBackEquipMoney, m_cTrade.m_nBackRepositoryMoney, 0);
		m_ItemList.ClearRoom(room_trade);
		m_ItemList.RemoveAllInOneRoom(room_trade1);
		//		m_ItemList.ClearRoom(room_trade1);
		m_ItemList.ClearRoom(room_tradeback);
		m_cTrade.Release();
		m_cMenuState.SetState(PLAYER_MENU_STATE_NORMAL);
		
		// Í¨Öª½çÃæ½»Ò×½áÊø
		CoreDataChanged(GDCNI_TRADE_END, 0, 0);
		// ¸üÐÂÎïÆ·Ãæ°æ
		CoreDataChanged(GDCNI_CONTAINER_OBJECT_CHANGED, UOC_ITEM_TAKE_WITH, 0);
		
		m_ItemList.MenuSetMouseItem();
	}
}
#endif

#ifndef _SERVER
void	KPlayer::SyncCurPlayer(BYTE* pMsg)
{
	if (dacheck)
		return;
	dacheck = TRUE;
	

	m_ItemList.RemoveAll();
	
	CURPLAYER_SYNC* PlaySync = (CURPLAYER_SYNC *)pMsg;
	this->m_nIndex = NpcSet.SearchID(PlaySync->m_dwID);
	this->m_dwID = g_FileName2Id(Npc[m_nIndex].Name); 
	
	Npc[m_nIndex].m_Kind = kind_player;
	Npc[m_nIndex].m_Level = (DWORD)PlaySync->m_btLevel;
	Npc[m_nIndex].m_nSex = PlaySync->m_btSex;
	Npc[m_nIndex].m_Series  = PlaySync->m_btSeries;
	Npc[m_nIndex].SetPlayerIdx(CLIENT_PLAYER_INDEX);
	
	m_nAttributePoint = PlaySync->m_wAttributePoint;
	m_nSkillPoint = PlaySync->m_wSkillPoint;
	m_nStrength = PlaySync->m_wStrength;
	m_nDexterity = PlaySync->m_wDexterity;
	m_nVitality = PlaySync->m_wVitality;
	m_nEngergy = PlaySync->m_wEngergy;
	m_nLucky = PlaySync->m_wLucky;
	
	m_nCurStrength = m_nStrength;
	m_nCurDexterity = m_nDexterity;
	m_nCurVitality = m_nVitality;
	m_nCurEngergy = m_nEngergy;
	m_nCurLucky = m_nLucky;
	SetFirstDamage();
	SetBaseAttackRating();
	SetBaseDefence();
	Npc[m_nIndex].m_ActionScriptID = 0;
	Npc[m_nIndex].m_TrapScriptID = 0;
	m_nExp = PlaySync->m_nExp;

	
	if (m_cTask.GetSaveVal(139) == 1)
	{
	m_nNextLevelExp = PlayerSet.m_cLevelAdd.GetLevelExp1(PlaySync->m_btLevel);
	}
	else if (m_cTask.GetSaveVal(139) == 2)
	{
	m_nNextLevelExp = PlayerSet.m_cLevelAdd.GetLevelExp2(PlaySync->m_btLevel);
	}


	else if (m_cTask.GetSaveVal(139) == 3)
	{
	m_nNextLevelExp = PlayerSet.m_cLevelAdd.GetLevelExp3(PlaySync->m_btLevel);
	}
	else if (m_cTask.GetSaveVal(139) == 4)
	{
	m_nNextLevelExp = PlayerSet.m_cLevelAdd.GetLevelExp4(PlaySync->m_btLevel);
	}
	else if (m_cTask.GetSaveVal(139) == 5)
	{
	m_nNextLevelExp = PlayerSet.m_cLevelAdd.GetLevelExp5(PlaySync->m_btLevel);
	}



	else
	{
	m_nNextLevelExp = PlayerSet.m_cLevelAdd.GetLevelExp(PlaySync->m_btLevel);
	}



	m_dwLeadExp = PlaySync->m_dwLeadExp;
	m_ImagePlayer = PlaySync->m_btImagePlayer;	// chan dung
	m_dwLeadLevel = PlayerSet.m_cLeadExp.GetLevel(m_dwLeadExp);
	m_dwNextLevelLeadExp = PlayerSet.m_cLeadExp.GetLevelExp(m_dwLeadLevel);
	
	m_cFaction.m_nCurFaction = (char)PlaySync->m_btCurFaction;
	m_cFaction.m_nFirstAddFaction = (char)PlaySync->m_btFirstFaction;
	m_cFaction.m_nAddTimes = PlaySync->m_nFactionAddTimes;
	
	m_nWorldStat = (int)PlaySync->m_wWorldStat;
	m_nSectStat = (int)PlaySync->m_wSectStat;

	m_ItemList.Init(CLIENT_PLAYER_INDEX);
	m_ItemList.SetMoney(PlaySync->m_nMoney1, PlaySync->m_nMoney2, 0);
	Npc[m_nIndex].m_LifeMax = PlaySync->m_wLifeMax;
	Npc[m_nIndex].m_ManaMax = PlaySync->m_wManaMax;
	Npc[m_nIndex].m_StaminaMax = PlaySync->m_wStaminaMax;
	Npc[m_nIndex].m_LifeReplenish = PLAYER_LIFE_REPLENISH;
	Npc[m_nIndex].m_ManaReplenish = PLAYER_MANA_REPLENISH;
	Npc[m_nIndex].m_StaminaGain = PLAYER_STAMINA_GAIN;
	Npc[m_nIndex].m_StaminaLoss = PLAYER_STAMINA_LOSS;

	m_nAutoPlay = FALSE;
	
	SetBaseResistData();
	SetBaseSpeedAndRadius();
	
	Npc[m_nIndex].RestoreNpcBaseInfo();
	
	m_BuyInfo.Clear();
	m_cMenuState.Release();
	m_cChat.Release();
	memset(m_szTaskAnswerFun, 0, sizeof(m_szTaskAnswerFun));
	m_nAvailableAnswerNum = 0;
	
	Npc[m_nIndex].m_Experience = 0;
	memset(Npc[m_nIndex].m_szChatBuffer, 0, sizeof(Npc[m_nIndex].m_szChatBuffer));
	Npc[m_nIndex].m_nCurChatTime = 0;
	
	m_RunStatus = 1;
	m_nLeftSkillID = 0;
	m_nLeftSkillLevel = 0;
	m_nRightSkillID = 0;
	m_nRightSkillLevel = 0;
	m_nPeapleIdx = 0;
	m_nObjectIdx = 0;
	m_MouseDown[0] = 0;
	m_MouseDown[1] = 0;
	
	Npc[m_nIndex].m_SyncSignal = 0;


	ZeroMemory(m_szNameOpenShop,32);
	m_dwTimeOpenShop = 0;
	m_dwTimeBuyShop = 0;

	ZeroMemory(Npc[m_nIndex].m_szNameOpenShop,32);
	Npc[m_nIndex].m_dwTimeOpenShop = 0;

	
	//Npc[m_nIndex].RestoreLiveData();
	//	SubWorld[0].AddPlayer(Npc[m_nIndex].m_RegionIndex, 0);//m_Region[Npc[m_nIndex].m_RegionIndex].AddPlayer(0);// m_WorldMessage.Send(GWM_PLAYER_ADD, Npc[m_nIndex].m_RegionIndex, 0);
}
#endif

BOOL	KPlayer::CheckTrading()
{
	return (m_cMenuState.m_nState == PLAYER_MENU_STATE_TRADING);
}


BOOL KPlayer::CheckShopOpen()
{

if (m_dwTimeOpenShop)
return TRUE;


return FALSE;

}

BOOL KPlayer::CheckShopBuyOpen()
{

if (m_dwTimeBuyShop)
return TRUE;

return FALSE;

}


void	KPlayer::SetFirstDamage()
{
	int nDamageBase;
	
	
	nDamageBase = m_nCurStrength / STRENGTH_SET_DAMAGE_VALUE + 1;
	
	Npc[m_nIndex].m_PhysicsDamage.nValue[0] = nDamageBase;
	Npc[m_nIndex].m_PhysicsDamage.nValue[2] = nDamageBase;
	Npc[m_nIndex].m_PhysicsDamage.nValue[1] = 0;

	Npc[m_nIndex].m_CurrentPhysicsDamage2.nValue[0] = 0;
	Npc[m_nIndex].m_CurrentPhysicsDamage2.nValue[2] = 0;
	Npc[m_nIndex].m_CurrentPhysicsDamage2.nValue[1] = 0;
	
	Npc[m_nIndex].m_CurrentFireDamage.nValue[0] = 0;
	Npc[m_nIndex].m_CurrentFireDamage.nValue[1] = 0;
	Npc[m_nIndex].m_CurrentFireDamage.nValue[2] = 0;

	Npc[m_nIndex].m_CurrentFireDamage2.nValue[0] = 0;
	Npc[m_nIndex].m_CurrentFireDamage2.nValue[1] = 0;
	Npc[m_nIndex].m_CurrentFireDamage2.nValue[2] = 0;
	
	Npc[m_nIndex].m_CurrentColdDamage.nValue[0] = 0;
	Npc[m_nIndex].m_CurrentColdDamage.nValue[1] = 0;
	Npc[m_nIndex].m_CurrentColdDamage.nValue[2] = 0;

	Npc[m_nIndex].m_CurrentColdDamage2.nValue[0] = 0;
	Npc[m_nIndex].m_CurrentColdDamage2.nValue[1] = 0;
	Npc[m_nIndex].m_CurrentColdDamage2.nValue[2] = 0;
	
	Npc[m_nIndex].m_CurrentLightDamage.nValue[0] = 0;
	Npc[m_nIndex].m_CurrentLightDamage.nValue[1] = 0;
	Npc[m_nIndex].m_CurrentLightDamage.nValue[2] = 0;

	Npc[m_nIndex].m_CurrentLightDamage2.nValue[0] = 0;
	Npc[m_nIndex].m_CurrentLightDamage2.nValue[1] = 0;
	Npc[m_nIndex].m_CurrentLightDamage2.nValue[2] = 0;
	
	Npc[m_nIndex].m_CurrentPoisonDamage.nValue[0] = 0;
	Npc[m_nIndex].m_CurrentPoisonDamage.nValue[1] = 0;
	Npc[m_nIndex].m_CurrentPoisonDamage.nValue[2] = 0;

	Npc[m_nIndex].m_CurrentPoisonDamage2.nValue[0] = 0;
	Npc[m_nIndex].m_CurrentPoisonDamage2.nValue[1] = 0;
	Npc[m_nIndex].m_CurrentPoisonDamage2.nValue[2] = 0;
	
}

void	KPlayer::SetBaseAttackRating()
{
	Npc[m_nIndex].m_AttackRating = m_nDexterity * 4 - 28;
}

void	KPlayer::SetBaseDefence()
{
	Npc[m_nIndex].m_Defend = m_nDexterity >> 2;
}

void	KPlayer::SetBaseResistData()
{
	Npc[m_nIndex].m_FireResist			= PlayerSet.m_cLevelAdd.GetFireResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_ColdResist			= PlayerSet.m_cLevelAdd.GetColdResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_PoisonResist		= PlayerSet.m_cLevelAdd.GetPoisonResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_LightResist			= PlayerSet.m_cLevelAdd.GetLightResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_PhysicsResist		= PlayerSet.m_cLevelAdd.GetPhysicsResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_FireResistMax		= BASE_FIRE_RESIST_MAX;
	Npc[m_nIndex].m_ColdResistMax		= BASE_COLD_RESIST_MAX;
	Npc[m_nIndex].m_PoisonResistMax		= BASE_POISON_RESIST_MAX;
	Npc[m_nIndex].m_LightResistMax		= BASE_LIGHT_RESIST_MAX;
	Npc[m_nIndex].m_PhysicsResistMax	= BASE_PHYSICS_RESIST_MAX;
}

void	KPlayer::SetBaseSpeedAndRadius()
{
	Npc[m_nIndex].m_WalkSpeed = BASE_WALK_SPEED;
	Npc[m_nIndex].m_RunSpeed = BASE_RUN_SPEED;
	Npc[m_nIndex].m_AttackSpeed = BASE_ATTACK_SPEED;
	Npc[m_nIndex].m_CastSpeed = BASE_CAST_SPEED;
	Npc[m_nIndex].m_VisionRadius = BASE_VISION_RADIUS;
	Npc[m_nIndex].m_HitRecover = BASE_HIT_RECOVER;
}

#ifndef _SERVER
//¿Í»§¶Ë°æ±¾
void KPlayer::DialogNpc(int nIndex)
{

	if (nIndex > 0 && Npc[nIndex].m_Index > 0)
	{
		if (Npc[nIndex].ActionScript[0])
		{
			ExecuteScript(Npc[nIndex].m_ActionScriptID,"main","");
		}
		else
		{
			PLAYER_DIALOG_NPC_COMMAND DialogNpcCmd;
			DialogNpcCmd.nNpcId = Npc[nIndex].m_dwID;
			DialogNpcCmd.ProtocolType = c2s_dialognpc;		
			if (g_pClient)
				g_pClient->SendPackToServer(&DialogNpcCmd, sizeof(PLAYER_DIALOG_NPC_COMMAND));
			
		}
	}
}
#endif

#ifndef _SERVER
void KPlayer::CheckObject(int nIdx)
{
/*	enum	// Îï¼þÀàÐÍ
{
Obj_Kind_MapObj = 0,		// µØÍ¼Îï¼þ£¬Ö÷ÒªÓÃÓÚµØÍ¼¶¯»­
Obj_Kind_Body,				// npc µÄÊ¬Ìå
Obj_Kind_Box,				// ±¦Ïä
Obj_Kind_Item,				// µôÔÚµØÉÏµÄ×°±¸
Obj_Kind_Money,				// µôÔÚµØÉÏµÄÇ®
Obj_Kind_LoopSound,			// Ñ­»·ÒôÐ§
Obj_Kind_RandSound,			// Ëæ»úÒôÐ§
Obj_Kind_Light,				// ¹âÔ´£¨3DÄ£Ê½ÖÐ·¢¹âµÄ¶«Î÷£©
Obj_Kind_Door,				// ÃÅÀà
Obj_Kind_Trap,				// ÏÝÚå
Obj_Kind_Prop,				// Ð¡µÀ¾ß£¬¿ÉÖØÉú
Obj_Kind_Num,				// Îï¼þµÄÖÖÀàÊý
};*/
	
	switch(Object[nIdx].m_nKind)
	{
	case Obj_Kind_Item:
	case Obj_Kind_Money:
	case Obj_Kind_Prop:
		PickUpObj(nIdx);
		Object[Npc[m_nIndex].m_nObjectIdx].m_nAutoNoActack = TRUE;
		Npc[m_nIndex].m_nObjectIdx = 0;
		break;
	case Obj_Kind_Box:
	case Obj_Kind_Door:
	case Obj_Kind_Trap:
		this->ObjMouseClick(nIdx);
		Npc[m_nIndex].m_nObjectIdx = 0;
		break;
	default:
		break;
	}
	m_nObjectIdx = 0;
	m_nPickObjectIdx = 0;
}
#endif

#ifndef _SERVER
void KPlayer::DrawSelectInfo()
{
	if (m_nIndex <= 0)
		return;
	
	int Map = SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_SubWorldID;
	//if (Map == 33 || Map == 37)
	//Npc[m_nIndex].PaintTopTongKim(TK_PK1,TK_PK2,TK_PK3,TK_PK4,m_cTask.GetSaveVal(485));
	Npc[m_nIndex].PaintEffect();
	
	if (m_nPeapleIdx)
	{
		//		Npc[m_nPeapleIdx].DrawBorder();

		if (Npc[m_nPeapleIdx].m_Kind == kind_player)
		{
			if (!NpcSet.CheckShowName())
			{
				Npc[m_nPeapleIdx].PaintInfo(Npc[m_nPeapleIdx].GetNpcPate(), true);
			}
		}
		else if (Npc[m_nPeapleIdx].m_Kind == kind_dialoger)
		{
			if (!NpcSet.CheckShowName())
			{
				Npc[m_nPeapleIdx].PaintInfo(Npc[m_nPeapleIdx].GetNpcPate(), true);
			}
		}
		else
		{
			Npc[m_nPeapleIdx].DrawBlood();
		}
		
		return;
	}
	if (m_nObjectIdx)
	{
		if (!ObjSet.CheckShowName())
			Object[m_nObjectIdx].DrawInfo();
		//		Object[m_nObjectIdx].DrawBorder();
		return;
	}
}
#endif

#ifndef _SERVER



void KPlayer::GetEchoAttack(int* nAttack, int nType)
{


	int nSkillId;
	*nAttack = 0;
	
	if (m_nIndex <= 0 || m_nIndex >= MAX_NPC 
		|| NULL == nAttack)
		return;
	
	nSkillId = (0 == nType)?m_nLeftSkillID:m_nRightSkillID;

	int nLevel = Npc[m_nIndex].m_SkillList.GetCurrentLevel(nSkillId);
	
	if (nSkillId <= 0 ||nLevel <= 0 || nLevel >= MAX_SKILLLEVEL)
		return;
	
	KMagicAttrib*	pMagicData = NULL;
	
	KSkill * pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(nSkillId, nLevel);
	if (!pOrdinSkill) 
        return ;

	switch(pOrdinSkill->GetSkillStyle())
	{
	case SKILL_SS_Missles:			
	case SKILL_SS_Melee:
	case SKILL_SS_InitiativeNpcState:
	case SKILL_SS_PassivityNpcState:
		{
		}
		break;
	default:
		return;
	}


	BOOL	bIsMele = pOrdinSkill->IsUseAR();

	if (!bIsMele)
		return;
	
	pMagicData = pOrdinSkill->GetDamageAttribs();
	
	if (!pMagicData)
		return;


	if (pMagicData->nAttribType == magic_attackrating_p)
	{
		*nAttack = Npc[m_nIndex].m_CurrentAttackRating + Npc[m_nIndex].m_AttackRating * pMagicData->nValue[0] / 100;
	}
	else
	{
		*nAttack = Npc[m_nIndex].m_CurrentAttackRating;
	}


}




void KPlayer::GetEchoDamage(int* nMin, int* nMax, int nType)
{
	int nSkillId;
	*nMin = 0;
	*nMax = 0;
	
	if (m_nIndex <= 0 || m_nIndex >= MAX_NPC 
		|| nType < 0 || nType > 1 
		|| NULL == nMin || NULL == nMax)
		return;
	
	nSkillId = (0 == nType)?m_nLeftSkillID:m_nRightSkillID;
	int nLevel = Npc[m_nIndex].m_SkillList.GetCurrentLevel(nSkillId);
	
	if (nSkillId <= 0 ||nLevel <= 0 || nLevel >= MAX_SKILLLEVEL)
		return;
	
	KMagicAttrib*	pMagicData = NULL;
	
	KSkill * pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(nSkillId, nLevel);
	if (!pOrdinSkill) 
        return ;

	switch(pOrdinSkill->GetSkillStyle())
	{
	case SKILL_SS_Missles:			//	×Óµ¯Àà		±¾¼¼ÄÜÓÃÓÚ·¢ËÍ×Óµ¯Àà
	case SKILL_SS_Melee:
	case SKILL_SS_InitiativeNpcState:	//	Ö÷¶¯Àà		±¾¼¼ÄÜÓÃÓÚ¸Ä±äµ±Ç°NpcµÄÖ÷¶¯×´Ì¬
	case SKILL_SS_PassivityNpcState:		//	±»¶¯Àà		±¾¼¼ÄÜÓÃÓÚ¸Ä±äNpcµÄ±»¶¯×´Ì¬
		{
		}
		break;
	default:
		return;
	}
	
	BOOL	bIsPhysical = pOrdinSkill->IsPhysical();
	BOOL	bIsSkillMagic = pOrdinSkill->IsSkillMagic();
	BOOL	bIsSkillPhysical = pOrdinSkill->IsSkillPhysical();

	
	pMagicData = pOrdinSkill->GetDamageAttribs();
	
	if (!pMagicData)
		return;


int CurrentSkillAddDameActive = 0;


if (nSkillId > 0 && nSkillId <= 500)
{
CurrentSkillAddDameActive = Npc[m_nIndex].m_SkillList.m_CurrentListSkillAddDame[nSkillId-1];
}



int DamePecentToLevel = 0;


if (Npc[m_nIndex].IsPlayer())
{
if (Npc[m_nIndex].m_Level > 100 && Npc[m_nIndex].m_Level <= 200)
{
DamePecentToLevel = (Npc[m_nIndex].m_Level - 100)/5;
}
}


	
	
	int nMinNpcDamage = Npc[m_nIndex].m_PhysicsDamage.nValue[0] + Npc[m_nIndex].m_CurrentAddPhysicsDamage;
	int nMaxNpcDamage = Npc[m_nIndex].m_PhysicsDamage.nValue[2] + Npc[m_nIndex].m_CurrentAddPhysicsDamage;
	// Skip attackrating
	pMagicData++;
	// Skip ignoredefense
	pMagicData++;
	// Calc physics damage
	if (magic_physicsenhance_p == pMagicData->nAttribType)
	{
		*nMin += nMinNpcDamage * (100 + pMagicData->nValue[0]*(100+CurrentSkillAddDameActive)/100) / 100;
		*nMax += nMaxNpcDamage * (100 + pMagicData->nValue[0]*(100+CurrentSkillAddDameActive)/100) / 100;
		
		int nEnhance;
		if (equip_meleeweapon == m_ItemList.GetWeaponType())
		{
			nEnhance = Npc[m_nIndex].m_CurrentMeleeEnhance[m_ItemList.GetWeaponParticular()];
		}
		else if (equip_rangeweapon == m_ItemList.GetWeaponType())
		{
			nEnhance = Npc[m_nIndex].m_CurrentRangeEnhance;
		}
		else 
		{
			nEnhance = Npc[m_nIndex].m_CurrentHandEnhance;
		}
		*nMin += nMinNpcDamage * nEnhance / 100;
		*nMax += nMaxNpcDamage * nEnhance / 100;
	}
	else if (magic_physicsdamage_v == pMagicData->nAttribType)
	{
		*nMin += pMagicData->nValue[0]*(100+CurrentSkillAddDameActive)/100;
		*nMax += pMagicData->nValue[2]*(100+CurrentSkillAddDameActive)/100;

		if (bIsSkillMagic)
	     {
		 *nMin += Npc[m_nIndex].m_CurrentPhysicsDamage2.nValue[0];
		 *nMax += Npc[m_nIndex].m_CurrentPhysicsDamage2.nValue[2];


	     }

	}
	pMagicData++;
	// Calc cold damage
	if (magic_colddamage_v == pMagicData->nAttribType)
	{
		*nMin += pMagicData->nValue[0]*(100+CurrentSkillAddDameActive)/100;
		*nMax += pMagicData->nValue[2]*(100+CurrentSkillAddDameActive)/100;

		if (bIsSkillMagic)
	     {
		 *nMin += Npc[m_nIndex].m_CurrentColdDamage2.nValue[0];
		 *nMax += Npc[m_nIndex].m_CurrentColdDamage2.nValue[2];


	     }

	}
	if (bIsSkillPhysical)
	{
		*nMin += Npc[m_nIndex].m_CurrentColdDamage.nValue[0];
		*nMax += Npc[m_nIndex].m_CurrentColdDamage.nValue[2];
	}
	pMagicData++;
	// Calc fire damage
	if (magic_firedamage_v == pMagicData->nAttribType)
	{
		*nMin += pMagicData->nValue[0]*(100+CurrentSkillAddDameActive)/100 + pMagicData->nValue[0]*(100+CurrentSkillAddDameActive)/100 * Npc[m_nIndex].m_CurrentFireEnhance/100;
		*nMax += pMagicData->nValue[2]*(100+CurrentSkillAddDameActive)/100 + pMagicData->nValue[2]*(100+CurrentSkillAddDameActive)/100 * Npc[m_nIndex].m_CurrentFireEnhance/100;
	
		if (bIsSkillMagic)
	     {
		 *nMin += Npc[m_nIndex].m_CurrentFireDamage2.nValue[0] + Npc[m_nIndex].m_CurrentFireDamage2.nValue[0] * Npc[m_nIndex].m_CurrentFireEnhance/100;
		 *nMax += Npc[m_nIndex].m_CurrentFireDamage2.nValue[2] + Npc[m_nIndex].m_CurrentFireDamage2.nValue[2] * Npc[m_nIndex].m_CurrentFireEnhance/100;


	     }
	
	}
	if (bIsSkillPhysical)
	{
		*nMin += Npc[m_nIndex].m_CurrentFireDamage.nValue[0] + Npc[m_nIndex].m_CurrentFireDamage.nValue[0] * Npc[m_nIndex].m_CurrentFireEnhance/100;
		*nMax += Npc[m_nIndex].m_CurrentFireDamage.nValue[2] + Npc[m_nIndex].m_CurrentFireDamage.nValue[2] * Npc[m_nIndex].m_CurrentFireEnhance/100;
	}
	pMagicData++;
	// Calc lighting damage
	if (magic_lightingdamage_v == pMagicData->nAttribType)
	{
		*nMin += pMagicData->nValue[0]*(100+CurrentSkillAddDameActive)/100 + (pMagicData->nValue[2]*(100+CurrentSkillAddDameActive)/100 - pMagicData->nValue[0]*(100+CurrentSkillAddDameActive)/100) * Npc[m_nIndex].m_CurrentLightEnhance / 100;
		*nMax += pMagicData->nValue[2]*(100+CurrentSkillAddDameActive)/100;

		if (bIsSkillMagic)
	     {
		 *nMin += Npc[m_nIndex].m_CurrentLightDamage2.nValue[0];
		 *nMax += Npc[m_nIndex].m_CurrentLightDamage2.nValue[2];

	     }

	}
	if (bIsSkillPhysical)
	{
		*nMin += Npc[m_nIndex].m_CurrentLightDamage.nValue[0];
		*nMax += Npc[m_nIndex].m_CurrentLightDamage.nValue[2];
	}
	pMagicData++;
	// Calc poison damage
	if (magic_poisondamage_v == pMagicData->nAttribType)
	{
		int nPoisonDamage = 0;
		if (pMagicData->nValue[2] * (100 - Npc[m_nIndex].m_CurrentPoisonEnhance) / 100 > 0)
		{
		nPoisonDamage = (pMagicData->nValue[0] * pMagicData->nValue[1] / (pMagicData->nValue[2] * (100 - Npc[m_nIndex].m_CurrentPoisonEnhance) / 100))*(100+CurrentSkillAddDameActive)/100;
		}
		else
		{
			nPoisonDamage = (pMagicData->nValue[0] * pMagicData->nValue[1])*(100+CurrentSkillAddDameActive)/100;
		}
			*nMin += nPoisonDamage;
			*nMax += nPoisonDamage;

		if (bIsSkillMagic)
	     {

		int nPoisonDamageAdd2 = 0;
		if (Npc[m_nIndex].m_CurrentPoisonDamage2.nValue[2] > 0)
		{
		nPoisonDamageAdd2 = Npc[m_nIndex].m_CurrentPoisonDamage2.nValue[0] * Npc[m_nIndex].m_CurrentPoisonDamage2.nValue[1] / Npc[m_nIndex].m_CurrentPoisonDamage2.nValue[2];
		}
		else
		{
		nPoisonDamageAdd2 = Npc[m_nIndex].m_CurrentPoisonDamage2.nValue[0] * Npc[m_nIndex].m_CurrentPoisonDamage2.nValue[1];
		}


			*nMin += nPoisonDamageAdd2;
			*nMax += nPoisonDamageAdd2;

	     }

	}
	if (bIsSkillPhysical)
	{
			int nPoisonDamageAdd = 0;
		if (Npc[m_nIndex].m_CurrentPoisonDamage.nValue[2] > 0)
		{
		nPoisonDamageAdd = Npc[m_nIndex].m_CurrentPoisonDamage.nValue[0] * Npc[m_nIndex].m_CurrentPoisonDamage.nValue[1] / Npc[m_nIndex].m_CurrentPoisonDamage.nValue[2];
		}
		else
		{
		nPoisonDamageAdd = Npc[m_nIndex].m_CurrentPoisonDamage.nValue[0] * Npc[m_nIndex].m_CurrentPoisonDamage.nValue[1];
		}


			*nMin += nPoisonDamageAdd;
			*nMax += nPoisonDamageAdd;
	}
	pMagicData++;
	// Calc magic damage
	if (magic_magicdamage_v == pMagicData->nAttribType)
	{
		*nMin += pMagicData->nValue[0]*(100+CurrentSkillAddDameActive)/100;
		*nMax += pMagicData->nValue[2]*(100+CurrentSkillAddDameActive)/100;
	}


	if (bIsSkillMagic)
	 {
	*nMin += ((*nMin * DamePecentToLevel)/100);
	*nMax += (*nMax * DamePecentToLevel)/100;
	}

}
#endif
void KPlayer::SetNpcDamageAttrib()
{
	SetNpcPhysicsDamage();
}

#ifdef _SERVER
void KPlayer::TobeExchangeServer(DWORD dwMapID, int nX, int nY)
{
	if (!m_dwID || m_nNetConnectIdx == -1)
		return;
	
	m_sExchangePos.m_dwMapID = dwMapID;
	m_sExchangePos.m_nX = nX;
	m_sExchangePos.m_nY = nY;
	m_bExchangeServer = TRUE;
}
#endif

/*
#ifdef _SERVER
void KPlayer::UpdateEnterGamePos(DWORD dwSubWorldID, int nX, int nY, int nFightMode)
{
	TRoleData* pData = (TRoleData *)&m_SaveBuffer;
	pData->BaseInfo.cFightMode = nFightMode;
	pData->BaseInfo.cUseRevive = 0;
	pData->BaseInfo.ientergameid = dwSubWorldID;
	pData->BaseInfo.ientergamex = nX;
	pData->BaseInfo.ientergamey = nY;
}
#endif
*/

#ifdef _SERVER
BOOL KPlayer::IsExchangingServer()
{
	return m_bExchangeServer;
}
#endif

#ifdef _SERVER
void KPlayer::SetLastNetOperationTime(int nTime)
{
	if (m_nNetConnectIdx == -1 || m_dwID == 0 || m_nIndex <= 0)
		return;
	
	m_nLastNetOperationTime = nTime;
	if (m_bSleepMode)
	{
		m_bSleepMode = FALSE;
		NPC_SLEEP_SYNC	SleepSync;
		SleepSync.ProtocolType = s2c_npcsleepmode;
		SleepSync.bSleep = m_bSleepMode;
		SleepSync.NpcID = Npc[m_nIndex].m_dwID;
		Npc[m_nIndex].SendDataToNearRegion(&SleepSync, sizeof(NPC_SLEEP_SYNC));
	}
}
#endif

#ifdef _SERVER
void 	KPlayer::SetNumImg(int nNumber)
{
	m_ImagePlayer = nNumber;
}
#endif

#ifdef _SERVER
BOOL	KPlayer::CreateTong(int nCamp, char *lpszTongName, char *lpszTitle)
{
	return this->m_cTong.Create(nCamp, lpszTongName, lpszTitle);
}

void	KPlayer::LoginTong(int nFigure,int nCamp,char* szName,char* szTitle,char* szTongName,char* szMaster)
{
this->m_cTong.Login(nFigure,nCamp,szName,szTitle,szTongName,szMaster);
}



void	KPlayer::SendTongInfo(int nSlect,int nPlayerSect,int nNumberDirector,int nNumberManager,int nNumberMember,char *szTongName,int nCamp,int nLevel,int nMoney,char *szListMember)
{
this->m_cTong.SendTongInfo(nSlect,nPlayerSect,nNumberDirector,nNumberManager,nNumberMember,szTongName,nCamp,nLevel,nMoney,szListMember);
}


void	KPlayer::MemberAddTong(int nCamp,char* szTitle,char* lpszTongName,char* lpszTongMaster)
{
this->m_cTong.MemberAddTong(nCamp,szTitle,lpszTongName,lpszTongMaster);
}



void	KPlayer::DeleteTong()
{
this->m_cTong.Delete();
}


void	KPlayer::UpdateTong(int dwTongId)
{
this->m_cTong.LoadLogin(dwTongId);
}


#endif





#ifdef _SERVER

int    KPlayer::GetIdxPGBoxItem(int nPage,int nLocation)
{


for (int i = 0; i < MAX_PLAYER_ITEM; i++)
{
int nIdx = m_ItemList.m_Items[i].nIdx;
int nPlace = m_ItemList.m_Items[i].nPlace;
int nX = m_ItemList.m_Items[i].nX;

if (nIdx > 0 && nIdx < MAX_ITEM && nPlace == pos_pgbox)
{


if (nPage == 1)
{
if (nX >= 0 && nX <= 2 && (nLocation - 1) == nX)
{
return nIdx;
}
}

else if (nPage == 2)
{
if (nX >= 3 && nX <= 13 && (nLocation - 1) == (nX-3))
{
return nIdx;
}
} 


else if (nPage == 3)
{
if (nX >= 14 && nX <= 15 && (nLocation - 1) == (nX-14))
{
return nIdx;
}
}

else if (nPage == 4)
{
if (nX >= 16 && nX <= 26 && (nLocation - 1) == (nX-16))
{
return nIdx;
}
}


}
}




return 0;

}

#endif










#ifdef _SERVER

int    KPlayer::GetIdMagicEventItem(int nItemIdx)
{


if (nItemIdx <= 0 || nItemIdx >= MAX_ITEM)
return -1;

BOOL bCheck = FALSE;
for (int i = 0; i < MAX_PLAYER_ITEM; i++)
{
int Idx = m_ItemList.m_Items[i].nIdx;
if (Idx > 0 && Idx < MAX_ITEM && Idx == nItemIdx)
{
bCheck = TRUE;
break;
}
}

if (!bCheck)
return -1;


if (Item[nItemIdx].GetGenre() != item_task)
return -1;


if (Item[nItemIdx].GetLevel() != 2)
return -1;

return Item[nItemIdx].m_GeneratorParam.nGeneratorLevel[0];

}

#endif









#ifdef _SERVER

int    KPlayer::GetLevelMagicEventItem(int nItemIdx)
{


if (nItemIdx <= 0 || nItemIdx >= MAX_ITEM)
return -1;

BOOL bCheck = FALSE;
for (int i = 0; i < MAX_PLAYER_ITEM; i++)
{
int Idx = m_ItemList.m_Items[i].nIdx;
if (Idx > 0 && Idx < MAX_ITEM && Idx == nItemIdx)
{
bCheck = TRUE;
break;
}
}

if (!bCheck)
return -1;

if (Item[nItemIdx].GetGenre() != item_task)
return -1;

if (Item[nItemIdx].GetLevel() != 2)
return -1;

return Item[nItemIdx].m_GeneratorParam.nGeneratorLevel[1];

}

#endif






#ifdef _SERVER

int    KPlayer::GetRequirementMagicEventItem(int nItemIdx)
{


if (nItemIdx <= 0 || nItemIdx >= MAX_ITEM)
return -1;

BOOL bCheck = FALSE;
for (int i = 0; i < MAX_PLAYER_ITEM; i++)
{
int Idx = m_ItemList.m_Items[i].nIdx;
if (Idx > 0 && Idx < MAX_ITEM && Idx == nItemIdx)
{
bCheck = TRUE;
break;
}
}

if (!bCheck)
return -1;

if (Item[nItemIdx].GetLevel() != 2)
return -1;

if (Item[nItemIdx].GetGenre() != item_task)
return -1;

return Item[nItemIdx].m_GeneratorParam.nGeneratorLevel[2];

}

#endif





#ifdef _SERVER

int    KPlayer::GetItemType(int nItemIdx)
{


if (nItemIdx <= 0 || nItemIdx >= MAX_ITEM)
return -1;

BOOL bCheck = FALSE;
for (int i = 0; i < MAX_PLAYER_ITEM; i++)
{
int Idx = m_ItemList.m_Items[i].nIdx;
if (Idx > 0 && Idx < MAX_ITEM && Idx == nItemIdx)
{
bCheck = TRUE;
break;
}
}

if (!bCheck)
return -1;



return Item[nItemIdx].m_SpecialParam.uItemType;

}

#endif





#ifdef _SERVER

int    KPlayer::GetEquipMagicId(int nItemIdx, int nNum)
{


if (nItemIdx <= 0 || nItemIdx >= MAX_ITEM)
return -1;


if (nNum < 1 || nNum >6)
return -1;


BOOL bCheck = FALSE;
for (int i = 0; i < MAX_PLAYER_ITEM; i++)
{
int Idx = m_ItemList.m_Items[i].nIdx;
if (Idx > 0 && Idx < MAX_ITEM && Idx == nItemIdx)
{
bCheck = TRUE;
break;
}
}

if (!bCheck)
return -1;

if (Item[nItemIdx].GetGenre() != item_equip)
return -1;

return Item[nItemIdx].m_aryMagicAttrib[nNum-1].nAttribType;

}

#endif





#ifdef _SERVER

int    KPlayer::GetEquipMagicLevel(int nItemIdx, int nNum)
{


if (nItemIdx <= 0 || nItemIdx >= MAX_ITEM)
return -1;


if (nNum < 1 || nNum >6)
return -1;


BOOL bCheck = FALSE;
for (int i = 0; i < MAX_PLAYER_ITEM; i++)
{
int Idx = m_ItemList.m_Items[i].nIdx;
if (Idx > 0 && Idx < MAX_ITEM && Idx == nItemIdx)
{
bCheck = TRUE;
break;
}
}

if (!bCheck)
return -1;

if (Item[nItemIdx].GetGenre() != item_equip)
return -1;

int nReturn = -1;

if (Item[nItemIdx].m_SpecialParam.uItemType == 2)
{
nReturn = (Item[nItemIdx].m_GeneratorParam.nGeneratorLevel[nNum-1]%100)/10 + 1;
}
else
{
nReturn = Item[nItemIdx].m_GeneratorParam.nGeneratorLevel[nNum-1];
}

return nReturn;


}

#endif




#ifdef _SERVER

int    KPlayer::GetEquipMagicPoint(int nItemIdx, int nNum)
{


if (nItemIdx <= 0 || nItemIdx >= MAX_ITEM)
return -1;


if (nNum < 1 || nNum >6)
return -1;


BOOL bCheck = FALSE;
for (int i = 0; i < MAX_PLAYER_ITEM; i++)
{
int Idx = m_ItemList.m_Items[i].nIdx;
if (Idx > 0 && Idx < MAX_ITEM && Idx == nItemIdx)
{
bCheck = TRUE;
break;
}
}

if (!bCheck)
return -1;

if (Item[nItemIdx].GetGenre() != item_equip)
return -1;

int nReturn = -1;
nReturn = Item[nItemIdx].m_aryMagicAttrib[nNum-1].nValue[0];

return nReturn;


}

#endif






#ifdef _SERVER

int    KPlayer::GetEquipMagicLucky(int nItemIdx, int nNum)
{

if (nItemIdx <= 0 || nItemIdx >= MAX_ITEM)
return -1;


if (nNum < 1 || nNum >6)
return -1;


BOOL bCheck = FALSE;
for (int i = 0; i < MAX_PLAYER_ITEM; i++)
{
int Idx = m_ItemList.m_Items[i].nIdx;
if (Idx > 0 && Idx < MAX_ITEM && Idx == nItemIdx)
{
bCheck = TRUE;
break;
}
}

if (!bCheck)
return -1;

if (Item[nItemIdx].GetGenre() != item_equip)
return -1;

int nReturn = -1;

if (Item[nItemIdx].m_SpecialParam.uItemType == 2)
{
nReturn = (Item[nItemIdx].m_GeneratorParam.nGeneratorLevel[nNum-1]%10000)/100 + 1;
}
else
{
nReturn = Item[nItemIdx].m_GeneratorParam.nLuck;
}

return nReturn;


}

#endif


#ifdef _SERVER

int    KPlayer::GetItemBoxLucky(int nItemIdx)
{


if (nItemIdx <= 0 || nItemIdx >= MAX_ITEM)
return -1;


BOOL bCheck = FALSE;
for (int i = 0; i < MAX_PLAYER_ITEM; i++)
{
int Idx = m_ItemList.m_Items[i].nIdx;
if (Idx > 0 && Idx < MAX_ITEM && Idx == nItemIdx)
{
bCheck = TRUE;
break;
}
}

if (!bCheck)
return -1;

if (Item[nItemIdx].GetGenre() != item_equip)
return -1;

int nReturn = -1;

nReturn = Item[nItemIdx].m_GeneratorParam.nLuck;

return nReturn;


}

#endif



#ifdef _SERVER

void   KPlayer::AddPropObj(int nId)
{

	int nX, nY;
	POINT	ptLocal;
	KMapPos	Pos;

	int nDataID = 0;
	int nColorID = 0;


    nDataID = nId;


	if (nDataID <= 0)
		return;

	if (m_nIndex <= 0 || m_nIndex >= MAX_NPC)
		return;

	Npc[m_nIndex].GetMpsPos(&nX, &nY);

	ptLocal.x = nX;
	ptLocal.y = nY;


	int nSubWorldIndex = Npc[m_nIndex].m_SubWorldIndex;

	if (nSubWorldIndex < 0 || nSubWorldIndex >= MAX_SUBWORLD)
		return;


	SubWorld[nSubWorldIndex].GetFreeObjPos(ptLocal);
	
	Pos.nSubWorld = nSubWorldIndex;
	SubWorld[nSubWorldIndex].Mps2Map(ptLocal.x, ptLocal.y, 
		&Pos.nRegion, &Pos.nMapX, &Pos.nMapY, 
		&Pos.nOffX, &Pos.nOffY);
	
	int nObjIdx = ObjSet.AddPropObj(Pos, nDataID, nColorID);
	if (nObjIdx > 0 && nObjIdx < MAX_OBJECT)
	{
			Object[nObjIdx].SetItemBelong(-1);
	}


}

#endif

#ifdef _SERVER

void   KPlayer::AddPropObjPos(int nId, int nIdMap, int nPosX, int nPosY)
{
	
}

#endif



#ifdef _SERVER

int    KPlayer::GetGetEquipMagicRandomSeed(int nItemIdx)
{

if (nItemIdx <= 0 || nItemIdx >= MAX_ITEM)
return -1;


BOOL bCheck = FALSE;
for (int i = 0; i < MAX_PLAYER_ITEM; i++)
{
int Idx = m_ItemList.m_Items[i].nIdx;
if (Idx > 0 && Idx < MAX_ITEM && Idx == nItemIdx)
{
bCheck = TRUE;
break;
}
}

if (!bCheck)
return -1;

if (Item[nItemIdx].GetGenre() != item_equip)
return -1;

return Item[nItemIdx].m_GeneratorParam.uRandomSeed;

}

#endif









#ifdef _SERVER

int    KPlayer::GetNumberNameInCpuTK()
{

int nNumberNameInCpuTK = 0;

for (int i=1;i<MAX_PLAYER;i++)
{


if (i == m_nPlayerIndex || Player[i].m_nNetConnectIdx == -1 || Player[i].m_bExchangeServer || Player[i].m_bIsQuiting)
continue;

if (Player[i].m_nIndex <= 0 || Player[i].m_nIndex >= MAX_NPC)
continue;

if (Npc[Player[i].m_nIndex].m_SubWorldIndex != g_SubWorldSet.SearchWorld(33) && Npc[Player[i].m_nIndex].m_SubWorldIndex != g_SubWorldSet.SearchWorld(37))
continue;

if (!Player[i].m_szCPUIPName || !m_szCPUIPName || !Player[i].m_szCPUIPName[0] || !m_szCPUIPName[0])
continue;

if (strcmp(Player[i].m_szCPUIPName,m_szCPUIPName) != 0 )
continue;

nNumberNameInCpuTK++;

}


return nNumberNameInCpuTK;

}

#endif



#ifdef _SERVER

int    KPlayer::GetIdxItemBox2()
{

int Number = 0;
int ItemIdx = -1;
for (int i = 0; i < MAX_PLAYER_ITEM; i++)
{
int Idx = m_ItemList.m_Items[i].nIdx;
int Place = m_ItemList.m_Items[i].nPlace;
if (Idx > 0 && Idx < MAX_ITEM && Place == pos_checkbox)
{
ItemIdx = Idx;
Number ++;
}
}

if (Number == 0)
{
return 0;
}
else if (Number == 1)
{


if (Item[ItemIdx].GetGenre() != item_equip)
return -2;
else if (Item[ItemIdx].GetVersion() > 1)
return ItemIdx;
else
return -3;

}
else
{
return -1;
}




return 0;

}

#endif




#ifdef _SERVER

int    KPlayer::GetIdxItemBox()
{

int Number = 0;
int ItemIdx = -1;
for (int i = 0; i < MAX_PLAYER_ITEM; i++)
{
int Idx = m_ItemList.m_Items[i].nIdx;
int Place = m_ItemList.m_Items[i].nPlace;
if (Idx > 0 && Idx < MAX_ITEM && Place == pos_checkbox)
{
ItemIdx = Idx;
Number ++;
}
}

if (Number == 0)
{
return 0;
}
else if (Number == 1)
{


if (Item[ItemIdx].GetGenre() != item_equip)
return -2;
else if (Item[ItemIdx].GetVersion() > 1)
return -3;
else
return ItemIdx;

}
else
{
return -1;
}




return 0;

}

#endif



#ifdef _SERVER

int    KPlayer::GetIdxItemBoxUpdateItem()
{
int Number = 0;
int ItemIdx = -1;
for (int i = 0; i < MAX_PLAYER_ITEM; i++)
{
int Idx = m_ItemList.m_Items[i].nIdx;
int Place = m_ItemList.m_Items[i].nPlace;
if (Idx > 0 && Idx < MAX_ITEM && Place == pos_checkbox && Item[Idx].GetGenre() == item_equip)
{
	ItemIdx = Idx;
	Number ++;
}
}

if (Number == 0)
{
return 0;
}
else if (Number == 1)
{
if (Item[ItemIdx].GetKind() != 0 && Item[ItemIdx].GetKind() != 2 )
return -2;
else if (Item[ItemIdx].GetVersion() > 1)
return -3;
else
return ItemIdx;
}
else
{
return -1;
}




return 0;

}

int    KPlayer::GetIdxItemBoxUpdateItem2()
{
int Number = 0;
int ItemIdx = -1;
for (int i = 0; i < MAX_PLAYER_ITEM; i++)
{
int Idx = m_ItemList.m_Items[i].nIdx;
int Place = m_ItemList.m_Items[i].nPlace;
if (Idx > 0 && Idx < MAX_ITEM && Place == pos_checkbox && Item[Idx].GetGenre() == item_equip)
{
	ItemIdx = Idx;
	Number ++;
}
}
if (Number == 0)
{
return 0;
}
else if (Number == 1)
{
	if (Item[ItemIdx].GetKind() != 1)
		return -2;
	else if (Item[ItemIdx].GetVersion() == 550324)
		return -3;
	else if (Item[ItemIdx].GetItemGroup() < 6 || Item[ItemIdx].GetItemGroup() > 33)
		return -4;
	else
	return ItemIdx;
}
else
{
return -1;
}
return 0;

}

int    KPlayer::GetIdxItemBoxUpdateItem3()
{
int Number = 0;
int ItemIdx = -1;
for (int i = 0; i < MAX_PLAYER_ITEM; i++)
{
int Idx = m_ItemList.m_Items[i].nIdx;
int Place = m_ItemList.m_Items[i].nPlace;
if (Idx > 0 && Idx < MAX_ITEM && Place == pos_checkbox && Item[Idx].GetGenre() == item_equip)
{
	ItemIdx = Idx;
	Number ++;
}
}
if (Number == 0)
{
return 0;
}
else if (Number == 1)
{
	return ItemIdx;
}
else
{
return -1;
}
return 0;
}

#endif
#ifdef _SERVER
int    KPlayer::GetIdxItemBoxQuestKey(int IdQuestKey)
{

int Number = 0;
int ItemIdx = -1;
for (int i = 0; i < MAX_PLAYER_ITEM; i++)
{
int Idx = m_ItemList.m_Items[i].nIdx;
int Place = m_ItemList.m_Items[i].nPlace;
if (Idx > 0 && Idx < MAX_ITEM && Place == pos_checkbox && Item[Idx].GetGenre() != item_equip)
{
	ItemIdx = Idx;
	Number ++;
}
}

if (Number == 0)
{
return 0;
}
else if (Number == 1)
{

	if (Item[ItemIdx].GetGenre() != 4 || Item[ItemIdx].GetDetailType() != IdQuestKey)
	return -2;
	else
	return ItemIdx;

}
else
{
return -1;
}


return 0;

}

#endif


#ifdef _SERVER
int    KPlayer::DelEquipItemQuestKey(int IdQuestKey)
{


for (int i = 0; i < MAX_PLAYER_ITEM; i++)
{
int Idx = m_ItemList.m_Items[i].nIdx;
int Place = m_ItemList.m_Items[i].nPlace;
if (Idx > 0 && Idx < MAX_ITEM && Place == pos_checkbox && Item[Idx].GetGenre() == 4 && Item[Idx].GetDetailType() == IdQuestKey)
{
	if (m_ItemList.CheckHaveItem(Idx))
	{
		ItemSet.m_checkdel = 13;
		m_ItemList.Remove(Idx);
		ItemSet.Remove(Idx);			
	}
}
}

return 0;

}

#endif

#ifdef _SERVER

int    KPlayer::CheckMagicItem(int nItemIdx,int nMagicId,int nMagicNumber1,int nMagicNumber2)
{


if (nItemIdx <= 0 || nItemIdx >= MAX_ITEM)
return 0;

BOOL bCheck = FALSE;
for (int i = 0; i < MAX_PLAYER_ITEM; i++)
{
int Idx = m_ItemList.m_Items[i].nIdx;
if (Idx > 0 && Idx < MAX_ITEM && Idx == nItemIdx)
{
bCheck = TRUE;
break;
}
}

if (!bCheck)
return 0;


return Item[nItemIdx].CheckMagicItem(nMagicId,nMagicNumber1,nMagicNumber2);

}

#endif






#ifdef _SERVER

int    KPlayer::GetLevelItem(int nItemIdx)
{


if (nItemIdx <= 0 || nItemIdx >= MAX_ITEM)
return -1;

BOOL bCheck = FALSE;
for (int i = 0; i < MAX_PLAYER_ITEM; i++)
{
int Idx = m_ItemList.m_Items[i].nIdx;
if (Idx > 0 && Idx < MAX_ITEM && Idx == nItemIdx)
{
bCheck = TRUE;
break;
}
}

if (!bCheck)
return -1;


return Item[nItemIdx].GetLevel();

}

#endif


#ifdef _SERVER

int    KPlayer::GetParticularItem(int nItemIdx)
{


if (nItemIdx <= 0 || nItemIdx >= MAX_ITEM)
return -1;

BOOL bCheck = FALSE;
for (int i = 0; i < MAX_PLAYER_ITEM; i++)
{
int Idx = m_ItemList.m_Items[i].nIdx;
if (Idx > 0 && Idx < MAX_ITEM && Idx == nItemIdx)
{
bCheck = TRUE;
break;
}
}

if (!bCheck)
return -1;


return Item[nItemIdx].GetParticular();

}

#endif


#ifdef _SERVER

int    KPlayer::GetTimeItem(int nItemIdx)
{


if (nItemIdx <= 0 || nItemIdx >= MAX_ITEM)
return -1;

BOOL bCheck = FALSE;
for (int i = 0; i < MAX_PLAYER_ITEM; i++)
{
int Idx = m_ItemList.m_Items[i].nIdx;
if (Idx > 0 && Idx < MAX_ITEM && Idx == nItemIdx)
{
bCheck = TRUE;
break;
}
}

if (!bCheck)
return -1;


return Item[nItemIdx].GetVersion();

}

#endif



#ifdef _SERVER

int    KPlayer::GetDetailTypeItem(int nItemIdx)
{


if (nItemIdx <= 0 || nItemIdx >= MAX_ITEM)
return -1;

BOOL bCheck = FALSE;
for (int i = 0; i < MAX_PLAYER_ITEM; i++)
{
int Idx = m_ItemList.m_Items[i].nIdx;
if (Idx > 0 && Idx < MAX_ITEM && Idx == nItemIdx)
{
bCheck = TRUE;
break;
}
}

if (!bCheck)
return -1;


return Item[nItemIdx].GetDetailType();

}

#endif



#ifdef _SERVER

int    KPlayer::GetClassItem(int nItemIdx)
{


if (nItemIdx <= 0 || nItemIdx >= MAX_ITEM)
return -1;

BOOL bCheck = FALSE;
for (int i = 0; i < MAX_PLAYER_ITEM; i++)
{
int Idx = m_ItemList.m_Items[i].nIdx;
if (Idx > 0 && Idx < MAX_ITEM && Idx == nItemIdx)
{
bCheck = TRUE;
break;
}
}

if (!bCheck)
return -1;


return Item[nItemIdx].GetGenre();

}

#endif


#ifdef _SERVER

int    KPlayer::GetGenreItem(int nItemIdx)
{


if (nItemIdx <= 0 || nItemIdx >= MAX_ITEM)
return -1;

BOOL bCheck = FALSE;
for (int i = 0; i < MAX_PLAYER_ITEM; i++)
{
int Idx = m_ItemList.m_Items[i].nIdx;
if (Idx > 0 && Idx < MAX_ITEM && Idx == nItemIdx)
{
bCheck = TRUE;
break;
}
}

if (!bCheck)
return -1;


return Item[nItemIdx].GetGenre();

}

#endif



#ifdef _SERVER

int    KPlayer::IsBlockItem(int nItemIdx)
{


if (nItemIdx <= 0 || nItemIdx >= MAX_ITEM)
return -1;

BOOL bCheck = FALSE;
for (int i = 0; i < MAX_PLAYER_ITEM; i++)
{
int Idx = m_ItemList.m_Items[i].nIdx;
if (Idx > 0 && Idx < MAX_ITEM && Idx == nItemIdx)
{
bCheck = TRUE;
break;
}
}

if (!bCheck)
return -1;

if (Item[nItemIdx].GetVersion() > 1)
{
return 1;
}

else
{
return 0;
}


}

#endif



#ifdef _SERVER

int    KPlayer::GetSeriItem(int nItemIdx)
{


if (nItemIdx <= 0 || nItemIdx >= MAX_ITEM)
return -1;

BOOL bCheck = FALSE;
for (int i = 0; i < MAX_PLAYER_ITEM; i++)
{
int Idx = m_ItemList.m_Items[i].nIdx;
if (Idx > 0 && Idx < MAX_ITEM && Idx == nItemIdx)
{
bCheck = TRUE;
break;
}
}

if (!bCheck)
return -1;


return Item[nItemIdx].GetSeries();

}

#endif








#ifdef _SERVER
void KPlayer::RepairItem(DWORD dwItemID)
{
	int nIdx = m_ItemList.SearchID(dwItemID);
	int nCost = Item[nIdx].GetRepairPrice();
	if (!nCost)
		return;
	int nMaxDur = Item[nIdx].GetMaxDurability();
	int nDur = Item[nIdx].GetDurability();
	if (Pay(nCost) && m_nNetConnectIdx >= 0)
	{
		Item[nIdx].SetDurability(nMaxDur);
		ITEM_DURABILITY_CHANGE	IDC;
		IDC.ProtocolType = s2c_itemdurabilitychange;
		IDC.dwItemID = dwItemID;
		IDC.nChange = nMaxDur - nDur;
		if (g_pServer)
			g_pServer->PackDataToClient(m_nNetConnectIdx, &IDC, sizeof(ITEM_DURABILITY_CHANGE));
	}
}
#endif

#ifdef _SERVER
void KPlayer::SetExtPoint(int nPoint, int nChangePoint)
{
	m_nExtPoint = nPoint;
	m_nChangeExtPoint = nChangePoint;
}

int KPlayer::GetExtPoint()
{
	printf("EXT %d ",m_nExtPoint);
	return m_nExtPoint;
}

BOOL KPlayer::PayExtPoint(int nPoint)
{
	if (m_nExtPoint < nPoint)
		return FALSE;

	m_nExtPoint -= nPoint;
	m_nChangeExtPoint += nPoint;
	return TRUE;
}

int KPlayer::GetExtPointChanged()
{
	return m_nChangeExtPoint;
}
#endif


#ifndef _SERVER
void KPlayer::PlayerStarBuyItemNumber(int nuId,int nIdx,int nWidth,int nHeight,int nPrice,int nNum)
{
	m_nBuyNumberUId = nuId;
	m_nBuyNumberIdx = nIdx;
	m_nBuyNumberWidth = nWidth;
	m_nBuyNumberHeigh = nHeight;
	m_nBuyNumberPrice = nPrice;
	m_nBuyNumberNum = nNum;
}

void KPlayer::PlayerSetAuto(BOOL bcheck,int nLife,int nMana, int nReturn, int nAtack,int nRange, int nNAtack, BOOL bMoneyObj, BOOL bItemObj, BOOL bPropObj, BOOL bItemSelect, int nNgaMyBuff, int nPhamViTuVe)
{
	//DelayAutoPlayer++;
	//if (DelayAutoPlayer >= 5*18)
	//{
//		DelayAutoPlayer = 5*18;
	//}
if (bcheck != m_nAutoPlay && bcheck)
{
Npc[m_nIndex].GetMpsPos(&m_nAutoStarNpcX,&m_nAutoStarNpcY);

BOOL nSetActiveSkill = FALSE;

int nSkillActiveIdx = Npc[m_nIndex].m_SkillList.FindSame(Npc[m_nIndex].m_ActiveSkillID);
if (nSkillActiveIdx > 0)
{
KSkill *pOrdinSkill = (KSkill *)g_SkillManager.GetSkill(Npc[m_nIndex].m_ActiveSkillID,Npc[m_nIndex].m_SkillList.GetCurrentLevel(Npc[m_nIndex].m_ActiveSkillID));
if (pOrdinSkill)
{
if (pOrdinSkill->IsTargetEnemy())
{
m_nAutoStarSkillIDx = nSkillActiveIdx;
nSetActiveSkill = TRUE;
}
}
}

if  (!nSetActiveSkill)
{
int nSkillLeftIdx = Npc[m_nIndex].m_SkillList.FindSame(m_nLeftSkillID);
if (nSkillLeftIdx > 0)
{
KSkill *pOrdinSkill = (KSkill *)g_SkillManager.GetSkill(m_nLeftSkillID,Npc[m_nIndex].m_SkillList.GetCurrentLevel(m_nLeftSkillID));
if (pOrdinSkill)
{
if (pOrdinSkill->IsTargetEnemy())
{
m_nAutoStarSkillIDx = nSkillLeftIdx;
}
}
}
}

}

m_nAutoReturn = nReturn;
m_nAutoAtack = nAtack;
m_nTempAutoAtack = nAtack;
m_nAutoLife = nLife;
m_nNgaMyBuff = nNgaMyBuff;
m_nAutoMana = nMana;
m_nAutoRange = nRange;
m_nAutoNAtack = nNAtack*18/1000;
m_bMoneyObj = bMoneyObj;
m_bItemObj = bItemObj;
m_bItemSelect = bItemSelect;
m_bPropObj = bPropObj;
m_nPhamViTuVe = nPhamViTuVe;
m_nAutoPlay = bcheck;

}


void KPlayer::CheckRideHouse(BOOL bCheckRideHorse)
{
	if (bCheckRideHorse && m_nAutoPlay)
	{
		if (Npc[m_nIndex].m_bRideHorse)
			SendClientCmdRide();

	}

}

void KPlayer::SendShopCost()
{
	
	for (int h=1; h < MAX_PLAYER_ITEM; h++)
	{
		int nIdxitem = m_ItemList.m_Items[h].nIdx;
		if (Item[nIdxitem].m_CommonAttrib.iShopCost > 0)
		{
			PLAYER_SHOP_PRICE_COMMAND PlayerPrice;
			PlayerPrice.ProtocolType = c2s_playershopprice;
			PlayerPrice.m_ID = Item[nIdxitem].m_dwID;
			PlayerPrice.m_Price = Item[nIdxitem].m_CommonAttrib.iShopCost;
			if (g_pClient)
				g_pClient->SendPackToServer((BYTE*)&PlayerPrice, sizeof(PLAYER_SHOP_PRICE_COMMAND));
		}
	}
}
void KPlayer::PlayerAutoOn()
{
if (Npc[m_nIndex].m_Doing == do_death || Npc[m_nIndex].m_Doing == do_revive || !Npc[m_nIndex].IsCanInput())
return;

if (!Npc[m_nIndex].m_FightMode)
{
	Npc[m_nIndex].m_nPeopleIdx = 0;
	Npc[m_nIndex].m_nPeopleIdxCheckClient = 0;
	return;
}

if (CheckTrading())
	return;


if (PlayerAutoOnStolen())
    return;

if (m_nAutoPause)
{
Npc[m_nIndex].m_nPeopleIdx = 0;
Npc[m_nIndex].m_nPeopleIdxCheckClient = 0;
return;
}


if (PlayerAutoOnBuffE())
return;

if (PlayerAutoOnBuffL())
return;

if (PlayerAutoOnObj())
return;

	//PlayerAutoOnSkill();
	//Npc[m_nIndex].m_MaskType = 879;
	PlayerAutoTeam(); // Auto Moi Nhom
	if (m_TargetPlayer) // Neu Bat Auto PK
	{
		PlayerAutoPK();
	}
	else
	{
		AutoTheoSau();
	}
}


BOOL KPlayer::PlayerAutoPK() // AUTO PK NGuoi Choi
{
		int Map = SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_SubWorldID;

ISkill * pISkill =  g_SkillManager.GetSkill(Npc[m_nIndex].m_SkillList.m_Skills[m_nAutoStarSkillIDx].SkillId,Npc[m_nIndex].m_SkillList.GetCurrentLevel(Npc[m_nIndex].m_SkillList.m_Skills[m_nAutoStarSkillIDx].SkillId));
		if (!pISkill) 
            return FALSE;

if ((!Npc[m_nIndex].m_SkillList.CanCast(Npc[m_nIndex].m_SkillList.m_Skills[m_nAutoStarSkillIDx].SkillId, SubWorld[0].m_dwCurrentTime))
			||
			(!Npc[m_nIndex].Cost(pISkill->GetSkillCostType(), pISkill->GetSkillCost(&Npc[m_nIndex]), TRUE))
			)
		{
			
			return FALSE;
		}

int nSkillLeftIdx = Npc[m_nIndex].m_SkillList.FindSame(m_nLeftSkillID);
int RangeSkill = 0;
if (nSkillLeftIdx > 0)
{
KSkill *pOrdinSkill = (KSkill *)g_SkillManager.GetSkill(m_nLeftSkillID,Npc[m_nIndex].m_SkillList.GetCurrentLevel(m_nLeftSkillID));
if (pOrdinSkill)
{
if (pOrdinSkill->IsTargetEnemy())
{
	m_nAutoStarSkillIDx = nSkillLeftIdx;
	RangeSkill = pOrdinSkill->GetAttackRadius();
}
}
}


Npc[m_nIndex].SetActiveSkill(m_nAutoStarSkillIDx);
if (m_nAutoRange >= RangeSkill)
Npc[m_nIndex].m_CurrentAttackRadius = RangeSkill;
else
Npc[m_nIndex].m_CurrentAttackRadius = m_nAutoRange;


 if (Npc[m_nIndex].m_nPeopleIdx > 0)
 {
	int khoangcach = NpcSet.GetDistance(Npc[m_nIndex].m_nPeopleIdx, m_nIndex);
	if (khoangcach > m_nAutoAtack)
	{
		Npc[m_nIndex].m_nPeopleIdx = 0;
		Npc[m_nIndex].m_nPeopleIdxCheckClient = 0;
	}
	if  (Npc[Npc[m_nIndex].m_nPeopleIdx].m_Kind != kind_player)
	{
		Npc[m_nIndex].m_nPeopleIdx = 0;
		Npc[m_nIndex].m_nPeopleIdxCheckClient = 0;
	}
 }
 
 


if (Npc[m_nIndex].m_nPeopleIdx <= 0)
{
    int nX0,nY0,nX1,nY1,nEnemyIdx,nMaxRangeDb;
	int KhoangCachGanNhat = 1000000;
	
	nEnemyIdx = 0;
	nMaxRangeDb = m_nAutoAtack * m_nAutoAtack;
	Npc[m_nIndex].GetMpsPos(&nX0,&nY0);

	for (int i = 1; i < MAX_NPC; i++)
	{
		if (!Npc[i].m_Index || 
			relation_enemy != NpcSet.GetRelation(m_nIndex,Npc[i].m_Index) ||
			Npc[i].m_Doing == do_death || 
			Npc[i].m_Doing == do_revive || Npc[i].m_CurrentLife <= 0 || Npc[i].m_CurrentLife == 100)
			continue;

		if (Npc[i].m_Kind != kind_player)	
			continue;
			
		  Npc[i].GetMpsPos(&nX1,&nY1);
		if (Npc[i].m_RegionIndex < 0 || !SubWorld[0].TestBarrier(nX1,nY1))
			continue;
				
	
		int distance = NpcSet.GetDistance(i, m_nIndex);	
		if (((nX1-nX0)*(nX1-nX0) + (nY1-nY0)*(nY1-nY0)) < nMaxRangeDb && distance < KhoangCachGanNhat)
		{
			KhoangCachGanNhat = distance;
			nMaxRangeDb = (nX1-nX0)*(nX1-nX0) + (nY1-nY0)*(nY1-nY0);
			nEnemyIdx = i;
		}
	}

if (nEnemyIdx > 0)
{
	Npc[m_nIndex].m_nPeopleIdx = nEnemyIdx;
	Npc[m_nIndex].m_nPeopleIdxCheckClient = (nEnemyIdx + 46) * 13 + 15743;
	m_nAutoOldCurenLifeActive = Npc[nEnemyIdx].m_CurrentLife;
	m_nAutoTimeNextNpc = g_SubWorldSet.GetGameTime() + m_eBoQuaPK;
}
}
else if (m_nAutoTimeNextNpc < g_SubWorldSet.GetGameTime())
{
	Npc[m_nIndex].m_nPeopleIdx = 0;
	Npc[m_nIndex].m_nPeopleIdxCheckClient = 0;
}

return FALSE;
}



BOOL KPlayer::PlayerAutoLocDo()
{

for (int h=1; h < MAX_PLAYER_ITEM; h++)
{
	if (m_ItemList.m_Items[h].nIdx > 0 && m_ItemList.m_Items[h].nIdx < MAX_ITEM && m_ItemList.m_Items[h].nPlace == pos_equiproom)
	{
		int ItemIdx = m_ItemList.m_Items[h].nIdx;
		if (Item[ItemIdx].GetGenre() == item_equip)
		{
			if (Item[ItemIdx].GetSeries() != 0)
			{
				if (Item[ItemIdx].GetVersion() > 1)
				continue;
				
				if (m_ItemList.m_Hand > 0)
				break;

				m_ItemList.m_Hand = ItemIdx;
				m_ItemList.m_Items[h].nPlace = pos_hand;
				m_ItemList.m_Items[h].nX = 0;
				m_ItemList.m_Items[h].nY = 0;
			//	m_ItemList.m_Items[m_ItemList.FindSame(m_ItemList.m_Hand)].nPlace = pos_hand;
			//	ItemSet.Add(m_ItemList.m_Items[h].nIdx,pos_hand,m_ItemList.m_Items[h].nX,m_ItemList.m_Items[h].nY);
			//	ItemSet.Remove(m_ItemList.m_Items[h].nIdx);

				m_ItemList.MenuSetMouseItem();
				g_DebugLog("LOCDO %s",Item[m_ItemList.m_Hand].GetName());
			}
		}
	}
}

if (m_ItemList.Hand())
{
	if (m_ItemList.m_Items[m_ItemList.m_Hand].nX != 0 && m_ItemList.m_Items[m_ItemList.m_Hand].nY != 0)
	ThrowAwayItem();
	g_DebugLog("LOCDO Vut Bo %d - %s",m_ItemList.m_Hand, Item[m_ItemList.m_Hand].GetName());
}

return FALSE;
}

int MANGTOADO[5][100][2] = 
{
{ // Ba Lang Huyen
	{6,6},//0,0 va 0,1
	{1600*32,3199*32},
	{1608*32,3210*32},
	{1595*32,3227*32},
	{1583*32,3243*32},
	{1578*32,3237*32},
},
{ //Phuong Tuong
	{8,8},
	{1595*32 ,3198*32},
	{1605*32,3204*32},
	{1614*32,3206*32},
	{1625*32,3195*32},
	{1635*32,3185*32},
	{1646*32,3174*32},
	{1648*32,3169*32},
}
};

int checkmap(int map)
{
	if (map == 53)
	return 0;
	else if (map == 17)
	return 1;
return 99;
};

BOOL	KPlayer::QuayLaiDiaDiemCu2()
{
	
	
return FALSE;
};

BOOL 	AutoMove(int x, int y, int m_nIndex)
{
	int nX0, nY0;
	Npc[m_nIndex].GetMpsPos(&nX0,&nY0);
	Npc[m_nIndex].SendCommand(do_run, x, y);
	SendClientCmdRun(x, y);
	g_DebugLog("TDT %d %d %d %d",x,nX0,y,nY0);
	if ( (x-nX0 > -32) && (x-nX0 < 32) && (y - nY0 > -32) && (y - nY0 < 32))
	{	
	
	g_DebugLog("TRUE: TDT %d - %d",x-nX0,y-nY0);	
		return TRUE;
	}
	g_DebugLog("FALSE: TDT %d - %d",x-nX0,y-nY0);	
	
return FALSE;
}
BOOL	KPlayer::QuayLaiDiaDiemCu()
{
//int nX0, nY0, nX1, nY1, nEnemyIdx, Map, vt;
int nX0, nY0,Map, vt;
Map = SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_SubWorldID;
Npc[m_nIndex].GetMpsPos(&nX0,&nY0);
vt = checkmap(Map);
int vitricuoi = MANGTOADO[vt][0][0];			
	
if (vt == 99)
{
	checktoado = 0;
	return FALSE;
}

if (checktoado == 0)
{
	checktoado = 1;
}

int i = checktoado;
if (AutoMove(MANGTOADO[vt][i][0],MANGTOADO[vt][i][1],m_nIndex) && i < vitricuoi)
{
	checktoado++; 
}
if (i == vitricuoi)
{
	
}
g_DebugLog("TDT2 %d - %d",i,vitricuoi);			
		/*
		if (i == vitricuoi-2)
		{
			for (int k = 1; k < MAX_NPC; k++)
			{
				if (!Npc[k].m_Index			||  
					Npc[k].m_Doing == do_death || 
					Npc[k].m_Doing == do_revive || Npc[k].m_CurrentLife <= 0 || Npc[k].m_CurrentLife == 100 ||
					Npc[k].m_Kind != kind_dialoger)
					continue;
				
		
				Npc[k].GetMpsPos(&nX1,&nY1);
				
				if (SubWorld[Npc[k].m_SubWorldIndex].m_SubWorldID == Map && nX1 == 50496 && nY1 == 103520 && strcmp(Npc[k].Name,"Xa phu") == 0)
				{
					nEnemyIdx = k;
					break;
					
				}	
			}
		if (nEnemyIdx > 0)
		{
			Npc[nEnemyIdx].GetMpsPos(&nX1,&nY1);	
			g_DebugLog("ToaDoTT: %d - %d",nX1,nY1);
			DialogNpc(nEnemyIdx);
			PLAYER_SELECTUI_COMMAND command;
			command.nSelectIndex = 0;
			OnSelectFromUI(&command,UI_SELECTDIALOG);
		}
		}
		*/

//g_DebugLog("TOADO %d",MANGTOADO[1][1]);	
	
	
return FALSE;	
};


BOOL KPlayer::PlayerAutoTeam()
{
SYSTEMTIME TimeNow;
GetLocalTime(&TimeNow);
if (!m_cTeam.m_AcceptAuto)
{
	m_cTeam.Check_AcceptAutoName = FALSE;
}
if (m_cTeam.m_InviteAuto)
{
	if (m_cTeam.m_nFlag)
	{

		if (m_cTeam.m_nFigure == TEAM_CAPTAIN && TimeNow.wSecond%15 == 0)
		{
		int nX0,nY0,nX1,nY1,nEnemyIdx,nMaxRangeDb;
		
		nEnemyIdx = 0;
		nMaxRangeDb = 1024 * 1024;

		Npc[m_nIndex].GetMpsPos(&nX0,&nY0);
			
			for (int i = 1; i < MAX_NPC; i++)
				{
					if (!Npc[i].m_Index			||  
						Npc[i].m_Doing == do_death || 
						Npc[i].m_Doing == do_revive || Npc[i].m_CurrentLife <= 0 || Npc[i].m_CurrentLife == 100 ||
						Npc[i].m_Kind != kind_player)
						continue;			
						
						if (m_nIndex == Npc[i].m_Index)
							continue;
						
						BOOL CHECKMEMBER = FALSE;
						for (int j = 0; j < MAX_TEAM_MEMBER; j++)
						{
							if ((DWORD)g_Team[0].m_nMember[j] == Npc[i].m_dwID)
							{
								CHECKMEMBER = TRUE;
								break;
							}
						}
						if (CHECKMEMBER)
						continue;
						
						///	g_DebugLog("AAAAAAAA");
						
						  Npc[i].GetMpsPos(&nX1,&nY1);

						if (Npc[i].m_nAutoNoActack && 
							(Npc[i].m_nAutoOldX - nX1) * (Npc[i].m_nAutoOldX - nX1) + (Npc[i].m_nAutoOldY - nY1) * (Npc[i].m_nAutoOldY - nY1) >= (120 * 120)
							)
							Npc[i].m_nAutoNoActack = FALSE;
							

						if (Npc[i].m_RegionIndex < 0 || !SubWorld[0].TestBarrier(nX1,nY1) || Npc[i].m_nAutoNoActack)
							continue;

						if (((nX1-nX0)*(nX1-nX0) + (nY1-nY0)*(nY1-nY0)) < nMaxRangeDb && ((nX1-m_nAutoStarNpcX)*(nX1-m_nAutoStarNpcX) + (nY1-m_nAutoStarNpcY)*(nY1-m_nAutoStarNpcY)) < nMaxRangeDb)
						{
							TeamInviteAdd(Npc[i].m_dwID);
							
						}
				}
		}
	}
	else
	{
		ApplyCreateTeam();
	}		
}
return FALSE;
}
BOOL KPlayer::PlayerAutoOnBuffH()
{

int nFaction;

nFaction = m_cFaction.GetCurFactionNo();

 if (nFaction == 3)
{

if (Npc[m_nIndex].m_nPeopleIdx > 0)
{

int nSkillLevel[3],nSkillId[3];
BOOL bCheckSkillEffec[3];
KStateNode* pNode;
KSkill * pOrdinSkill;

nSkillId[0] = 72;
nSkillId[1] = 73;
nSkillId[2] = 336;

for (int i=0;i<3;i++)
{

bCheckSkillEffec[i] = FALSE;

if (nSkillId[i] > 0)
{
nSkillLevel[i] = Npc[m_nIndex].m_SkillList.GetCurrentLevel(nSkillId[i]);
if (nSkillLevel[i] > 0)
{
bCheckSkillEffec[i] = TRUE;
}
}

}
    pNode = (KStateNode *)Npc[Npc[m_nIndex].m_nPeopleIdx].m_StateSkillList.GetHead();
	while(pNode)
	{
		for (int j=0;j<3;j++)
		{

		if (nSkillId[j] <= 0 || nSkillLevel[j] <= 0)
		{
			bCheckSkillEffec[j] = FALSE;
			continue;
		}
		
		pOrdinSkill = (KSkill *)g_SkillManager.GetSkill(nSkillId[j],nSkillLevel[j]);
	    if (pOrdinSkill)
		{
		if (pNode->m_StateGraphics == pOrdinSkill->GetStateSpecailId())
		{
				bCheckSkillEffec[j] = FALSE;
		}
		}

		}

		pNode = (KStateNode *)pNode->GetNext();
	}

for (int t=0;t<3;t++)
{
if (bCheckSkillEffec[t])
{
Npc[m_nIndex].SetActiveSkill(Npc[m_nIndex].m_SkillList.FindSame(nSkillId[t]));
return FALSE;
}
}



}

}

return FALSE;

}
BOOL KPlayer::AutoTheoSau()
{


if (m_cTeam.m_nFlag && !m_TargetPlayer && m_TheoSau)
{
	if (m_cTeam.m_nFigure == TEAM_CAPTAIN)
	{
		m_nAutoAtack = m_nTempAutoAtack;
		PlayerAutoOnSkill(); // Auto Danh Day
	}
	else
	{
		
		int nX0,nY0;
		Npc[m_nIndex].GetMpsPos(&nX0,&nY0);
		int Map = SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_SubWorldID;
		int nX1,nY1;
		int	nNpcIndex;
		int nMaxRangeDb;
		
			if (g_Team[0].m_nMemNum >= 0)
			{
				
				nNpcIndex = NpcSet.SearchID(g_Team[0].m_nCaptain);
				if (nNpcIndex <= 0)
				{
					PlayerAutoOnSkill();
					return FALSE;
				}
				if (nNpcIndex == m_nIndex)
				return FALSE;
				
				if (Npc[nNpcIndex].m_Doing == do_death || Npc[nNpcIndex].m_Doing == do_revive || !Npc[nNpcIndex].IsCanInput())
				return FALSE;
	
						nMaxRangeDb = m_nAutoAtack * m_nAutoAtack;
						Npc[nNpcIndex].GetMpsPos(&nX1,&nY1);
						int Map1 = SubWorld[Npc[nNpcIndex].m_SubWorldIndex].m_SubWorldID;
						int distance = NpcSet.GetDistance(nNpcIndex, m_nIndex);

						if (distance >= 100)
						{
							Npc[m_nIndex].m_nPeopleIdx = 0;
							if (Npc[m_nIndex].m_CurrentStamina > 0 || m_cPK.GetNormalPKState() != 2)
							{
								Npc[m_nIndex].SendCommand(do_run, nX1, nY1);
								SendClientCmdRun(nX1, nY1);
								g_DebugLog("Check Buoc 5\n");
							}			
							else
							{
								Npc[m_nIndex].SendCommand(do_walk, nX1, nY1);
								SendClientCmdWalk(nX1, nY1);
								g_DebugLog("Check Buoc 6\n");
							}
						}
						else	
						{
							g_DebugLog("Tu Danh Theo Sau 1\n");
							PlayerAutoOnSkillTheoSau();
						}
			}
		
	}
}
else
{		
	m_nAutoAtack = m_nTempAutoAtack;
	PlayerAutoOnSkill();
}
return FALSE;
}

BOOL KPlayer::PlayerAutoOnBuffL()
{

int nSkillLevel,nSkillId,nFaction;

nFaction = m_cFaction.GetCurFactionNo();
if (nFaction == 4)
{
nSkillId = 93;
nSkillLevel = Npc[m_nIndex].m_SkillList.GetCurrentLevel(nSkillId);

if (nSkillLevel > 0)
{

if (m_cTeam.m_nFlag)
{

int nX0,nY0;

Npc[m_nIndex].GetMpsPos(&nX0,&nY0);

int nX1,nY1;
int	nNpcIndex;
//int nMaxRangeDb;

if  (m_nNgaMyBuff == NULL || m_nNgaMyBuff < 0 || m_nNgaMyBuff >= 100)
{
	m_nNgaMyBuff = 50;
}
		
nNpcIndex = NpcSet.SearchID(g_Team[0].m_nCaptain);

if (nNpcIndex > 0 && Npc[nNpcIndex].m_Doing != do_death && Npc[nNpcIndex].m_Doing != do_revive && Npc[nNpcIndex].IsCanInput())
{

if (Npc[nNpcIndex].m_CurrentLife < (Npc[nNpcIndex].m_CurrentLifeMax * m_nNgaMyBuff / 100))
{
			Npc[m_nIndex].m_nPeopleIdx = 0;
			Npc[m_nIndex].m_nPeopleIdxCheckClient = (0 + 46) * 13 + 15743;
	
				Npc[nNpcIndex].GetMpsPos(&nX1,&nY1);

				if (((nX1-nX0)*(nX1-nX0) + (nY1-nY0)*(nY1-nY0)) < (120*120))
				{
				Npc[m_nIndex].SendCommand(do_skill, nSkillId, -1, m_nIndex);		
				SendClientCmdSkill(nSkillId, -1, Npc[m_nIndex].m_dwID);
				return TRUE;
				}
				else
				{

				if (Npc[m_nIndex].m_CurrentStamina > 0 || m_cPK.GetNormalPKState() != 2)
					{
						Npc[m_nIndex].SendCommand(do_run, nX1, nY1);
						SendClientCmdRun(nX1, nY1);
					}
					else
					{
						Npc[m_nIndex].SendCommand(do_walk, nX1, nY1);
						SendClientCmdWalk(nX1, nY1);
					}

				return TRUE;
				}
}

}


		
for (int i = 0; i < MAX_TEAM_MEMBER; i++)
{
if (g_Team[0].m_nMember[i] >= 0)
{
nNpcIndex = NpcSet.SearchID(g_Team[0].m_nMember[i]);

if (nNpcIndex <= 0)
continue;

if (Npc[nNpcIndex].m_Doing == do_death || Npc[nNpcIndex].m_Doing == do_revive || !Npc[nNpcIndex].IsCanInput())
continue;



if  (m_nNgaMyBuff == NULL || m_nNgaMyBuff < 0 || m_nNgaMyBuff >= 100)
{
	m_nNgaMyBuff = 50;
}
		

if (Npc[nNpcIndex].m_CurrentLife < (Npc[nNpcIndex].m_CurrentLifeMax * m_nNgaMyBuff / 100))
{
			Npc[m_nIndex].m_nPeopleIdx = 0;
			Npc[m_nIndex].m_nPeopleIdxCheckClient = (0 + 46) * 13 + 15743;
			Npc[nNpcIndex].GetMpsPos(&nX1,&nY1);

			if (((nX1-nX0)*(nX1-nX0) + (nY1-nY0)*(nY1-nY0)) < (120*120))
			{
			Npc[m_nIndex].SendCommand(do_skill, nSkillId, -1, m_nIndex);		
			SendClientCmdSkill(nSkillId, -1, Npc[m_nIndex].m_dwID);
			return TRUE;
			}
			else
			{

				if (Npc[m_nIndex].m_CurrentStamina > 0 || m_cPK.GetNormalPKState() != 2)
				{
					Npc[m_nIndex].SendCommand(do_run, nX1, nY1);
					SendClientCmdRun(nX1, nY1);
				}
				else
				{
					Npc[m_nIndex].SendCommand(do_walk, nX1, nY1);
					SendClientCmdWalk(nX1, nY1);
				}

			return TRUE;
			}

}





}
}

}

else if (Npc[m_nIndex].m_CurrentLife < (Npc[m_nIndex].m_CurrentLifeMax/2))

{
Npc[m_nIndex].SendCommand(do_skill, nSkillId, -1, m_nIndex);		
SendClientCmdSkill(nSkillId, -1, Npc[m_nIndex].m_dwID);
return TRUE;
}



}
}


return FALSE;
}



BOOL KPlayer::PlayerAutoOnBuffE()
{
	
int nSkillId[MAX_AUTO_SKILL_BUFFE] = {0};
int nSkillLevel[MAX_AUTO_SKILL_BUFFE] = {0};
int nFaction;
BOOL bCheckSkillEffec[MAX_AUTO_SKILL_BUFFE];
KStateNode* pNode;
//KSkill * pSkillBuff;

nFaction = m_cFaction.GetCurFactionNo();


if (nFaction == 0)
{
nSkillId[0] = 15;
nSkillId[1] = 273;
nSkillId[2] = 0;
}
else if (nFaction == 1)
{
nSkillId[0] = 42;
nSkillId[1] = 33;
nSkillId[2] = 0;
}
else if (nFaction == 2)
{
nSkillId[0] = 0;
nSkillId[1] = 0;
nSkillId[2] = 0;
}
else if (nFaction == 3)
{
nSkillId[0] = 0;
nSkillId[1] = 0;
nSkillId[2] = 0;
}
else if (nFaction == 4)
{
nSkillId[0] = 0;
nSkillId[1] = 0;
nSkillId[2] = 0;
}
else if (nFaction == 5)
{
nSkillId[0] = 109;
nSkillId[1] = 100;
nSkillId[2] = 0;
}
else if (nFaction == 6)
{
nSkillId[0] = 130;
nSkillId[1] = 277;
nSkillId[2] = 0;
}
else if (nFaction == 7)
{
nSkillId[0] = 150;
nSkillId[1] = 0;
nSkillId[2] = 0;
}
else if (nFaction == 8)
{
nSkillId[0] = 157;
nSkillId[1] = 0;
nSkillId[2] = 0;
}
else if (nFaction == 9)
{
nSkillId[0] = 171;
nSkillId[1] = 173;
nSkillId[2] = 178;
}

for (int i=0;i<MAX_AUTO_SKILL_BUFFE;i++)
{

bCheckSkillEffec[i] = FALSE;

if (nSkillId[i] > 0)
{
nSkillLevel[i] = Npc[m_nIndex].m_SkillList.GetCurrentLevel(nSkillId[i]);
if (nSkillLevel[i] > 0)
{
bCheckSkillEffec[i] = TRUE;
}
}

}
    pNode = (KStateNode *)Npc[m_nIndex].m_StateSkillList.GetHead();
	while(pNode)
	{
		for (int j=0;j<MAX_AUTO_SKILL_BUFFE;j++)
		{

		if (nSkillId[j] <= 0 || nSkillLevel[j] <= 0)
		{
			bCheckSkillEffec[j] = FALSE;
			continue;
		}

		if (pNode->m_SkillID == nSkillId[j])
		{
				bCheckSkillEffec[j] = FALSE;
		}

		}

		pNode = (KStateNode *)pNode->GetNext();
	}

for (int t=0;t<MAX_AUTO_SKILL_BUFFE;t++)
{
if (bCheckSkillEffec[t])
{

Npc[m_nIndex].SendCommand(do_skill, nSkillId[t], -1, m_nIndex);		
SendClientCmdSkill(nSkillId[t], -1, Npc[m_nIndex].m_dwID);
g_DebugLog("Send Skill: %d\n",nSkillId[t]);
return TRUE;
}
}


return FALSE;
}



BOOL KPlayer::PlayerAutoOnStolen()
{


BOOL CheckUseItemLife = FALSE;
BOOL CheckUseItemMana = FALSE;
BOOL CheckUseItemLifeOK = FALSE;
BOOL CheckUseItemManaOK = FALSE;




BOOL CheckItemLBMana = FALSE;
BOOL CheckItemLBLife = FALSE;
BOOL CheckItemLBMaLi = FALSE;




int Map = SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_SubWorldID;


if (m_cPK.GetNormalPKState() == 0 || Map == 24 || Map == 109)
{



if (g_SubWorldSet.GetGameTime() > m_nAutoTimeNextLBLife)
{
for (int i = 0; i < MAX_PLAYER_ITEM; i++)
{
if (m_ItemList.m_Items[i].nIdx && m_ItemList.m_Items[i].nPlace == pos_equiproom)
{
if (Item[m_ItemList.m_Items[i].nIdx].GetGenre() == item_medicine && Item[m_ItemList.m_Items[i].nIdx].GetDetailType() == 0)
{
CheckItemLBLife = TRUE;
break;
}
}		
}



if (!CheckItemLBLife)
{
for (int i = 0; i < MAX_PLAYER_ITEM; i++)
{
if (m_ItemList.m_Items[i].nIdx && m_ItemList.m_Items[i].nPlace == pos_equiproom)
{
if (Item[m_ItemList.m_Items[i].nIdx].GetGenre() == item_task && (Item[m_ItemList.m_Items[i].nIdx].GetDetailType() == 104 || Item[m_ItemList.m_Items[i].nIdx].GetDetailType() == 108 || Item[m_ItemList.m_Items[i].nIdx].GetDetailType() == 110))
{
ItemPos SrcPos;
SrcPos.nPlace = m_ItemList.m_Items[i].nPlace;
SrcPos.nX = m_ItemList.m_Items[i].nX;
SrcPos.nY = m_ItemList.m_Items[i].nY;
ApplyUseItem(m_ItemList.m_Items[i].nIdx,SrcPos);
m_nAutoTimeNextLBLife = g_SubWorldSet.GetGameTime() + 3 * 18;
break;
}
}		
}
}
}


if (g_SubWorldSet.GetGameTime() > m_nAutoTimeNextLBMana)
{
for (int i = 0; i < MAX_PLAYER_ITEM; i++)
{
if (m_ItemList.m_Items[i].nIdx && m_ItemList.m_Items[i].nPlace == pos_equiproom)
{
if (Item[m_ItemList.m_Items[i].nIdx].GetGenre() == item_medicine && Item[m_ItemList.m_Items[i].nIdx].GetDetailType() == 1)
{
CheckItemLBMana = TRUE;
break;
}
}		
}
if (!CheckItemLBMana)
{
for (int i = 0; i < MAX_PLAYER_ITEM; i++)
{
if (m_ItemList.m_Items[i].nIdx && m_ItemList.m_Items[i].nPlace == pos_equiproom)
{
if (Item[m_ItemList.m_Items[i].nIdx].GetGenre() == item_task && (Item[m_ItemList.m_Items[i].nIdx].GetDetailType() == 105 || Item[m_ItemList.m_Items[i].nIdx].GetDetailType() == 112 || Item[m_ItemList.m_Items[i].nIdx].GetDetailType() == 111))
{
ItemPos SrcPos;
SrcPos.nPlace = m_ItemList.m_Items[i].nPlace;
SrcPos.nX = m_ItemList.m_Items[i].nX;
SrcPos.nY = m_ItemList.m_Items[i].nY;
ApplyUseItem(m_ItemList.m_Items[i].nIdx,SrcPos);
m_nAutoTimeNextLBMana = g_SubWorldSet.GetGameTime() + 3 * 18;
break;
}
}		
}
}
}



if (g_SubWorldSet.GetGameTime() > m_nAutoTimeNextLBMaLi)
{
for (int i = 0; i < MAX_PLAYER_ITEM; i++)
{
if (m_ItemList.m_Items[i].nIdx && m_ItemList.m_Items[i].nPlace == pos_equiproom)
{
if (Item[m_ItemList.m_Items[i].nIdx].GetGenre() == item_medicine && Item[m_ItemList.m_Items[i].nIdx].GetDetailType() == 2)
{
CheckItemLBMaLi = TRUE;
break;
}
}		
}


if (!CheckItemLBMaLi)
{
for (int i = 0; i < MAX_PLAYER_ITEM; i++)
{
if (m_ItemList.m_Items[i].nIdx && m_ItemList.m_Items[i].nPlace == pos_equiproom)
{
if (Item[m_ItemList.m_Items[i].nIdx].GetGenre() == item_task && (Item[m_ItemList.m_Items[i].nIdx].GetDetailType() == 103 || Item[m_ItemList.m_Items[i].nIdx].GetDetailType() == 109 || Item[m_ItemList.m_Items[i].nIdx].GetDetailType() == 107 || Item[m_ItemList.m_Items[i].nIdx].GetDetailType() == 264))
{
ItemPos SrcPos;
SrcPos.nPlace = m_ItemList.m_Items[i].nPlace;
SrcPos.nX = m_ItemList.m_Items[i].nX;
SrcPos.nY = m_ItemList.m_Items[i].nY;
ApplyUseItem(m_ItemList.m_Items[i].nIdx,SrcPos);
m_nAutoTimeNextLBMaLi = g_SubWorldSet.GetGameTime() + 3 * 18;
break;
}
}		
}
}
}




}










if (Npc[m_nIndex].m_CurrentLife < m_nAutoLife && g_SubWorldSet.GetGameTime() > m_nAutoTimeNextLife)
{
CheckUseItemLife = TRUE;
for (int i = 0; i < MAX_PLAYER_ITEM; i++)
{
if (m_ItemList.m_Items[i].nIdx && m_ItemList.m_Items[i].nPlace == pos_equiproom)
{
if (Item[m_ItemList.m_Items[i].nIdx].GetGenre() == item_medicine && Item[m_ItemList.m_Items[i].nIdx].GetDetailType() == 0)
{
ItemPos SrcPos;
SrcPos.nPlace = m_ItemList.m_Items[i].nPlace;
SrcPos.nX = m_ItemList.m_Items[i].nX;
SrcPos.nY = m_ItemList.m_Items[i].nY;
ApplyUseItem(m_ItemList.m_Items[i].nIdx,SrcPos);
m_nAutoTimeNextLife = g_SubWorldSet.GetGameTime() + MAX_AUTO_LIFE_WAIL;
CheckUseItemLifeOK = TRUE;
break;
}
}		
}
}

if (Npc[m_nIndex].m_CurrentMana < m_nAutoMana && g_SubWorldSet.GetGameTime() > m_nAutoTimeNextMana)
{
CheckUseItemMana = TRUE;
for (int i = 0; i < MAX_PLAYER_ITEM; i++)
{
if (m_ItemList.m_Items[i].nIdx && m_ItemList.m_Items[i].nPlace == pos_equiproom)
{
if (Item[m_ItemList.m_Items[i].nIdx].GetGenre() == item_medicine && Item[m_ItemList.m_Items[i].nIdx].GetDetailType() == 1)
{
ItemPos SrcPos;
SrcPos.nPlace = m_ItemList.m_Items[i].nPlace;
SrcPos.nX = m_ItemList.m_Items[i].nX;
SrcPos.nY = m_ItemList.m_Items[i].nY;
ApplyUseItem(m_ItemList.m_Items[i].nIdx,SrcPos);
m_nAutoTimeNextMana = g_SubWorldSet.GetGameTime() + MAX_AUTO_MANA_WAIL;
CheckUseItemManaOK = TRUE;
break;
}
}		
}
}


if ((CheckUseItemLife && !CheckUseItemLifeOK) || (CheckUseItemMana && !CheckUseItemManaOK))
{
for (int i = 0; i < MAX_PLAYER_ITEM; i++)
{
if (m_ItemList.m_Items[i].nIdx && (m_ItemList.m_Items[i].nPlace == pos_equiproom || m_ItemList.m_Items[i].nPlace == pos_immediacy))
{
if (Item[m_ItemList.m_Items[i].nIdx].GetGenre() == item_medicine && Item[m_ItemList.m_Items[i].nIdx].GetDetailType() == 2)
{
ItemPos SrcPos;
SrcPos.nPlace = m_ItemList.m_Items[i].nPlace;
SrcPos.nX = m_ItemList.m_Items[i].nX;
SrcPos.nY = m_ItemList.m_Items[i].nY;
ApplyUseItem(m_ItemList.m_Items[i].nIdx,SrcPos);

if ((CheckUseItemLife && !CheckUseItemLifeOK))
{
m_nAutoTimeNextLife = g_SubWorldSet.GetGameTime() + MAX_AUTO_LIFE_WAIL;
CheckUseItemLifeOK = TRUE;
}

if ((CheckUseItemMana && !CheckUseItemManaOK))
m_nAutoTimeNextMana = g_SubWorldSet.GetGameTime() + MAX_AUTO_MANA_WAIL;
CheckUseItemManaOK = TRUE;
break;
}
}		
}
}


if (Npc[m_nIndex].m_CurrentLife < m_nAutoReturn)
for (int i = 0; i < MAX_PLAYER_ITEM; i++)
{
if (m_ItemList.m_Items[i].nIdx && (m_ItemList.m_Items[i].nPlace == pos_equiproom || m_ItemList.m_Items[i].nPlace == pos_immediacy))
{
if (Item[m_ItemList.m_Items[i].nIdx].GetGenre() == item_task && Item[m_ItemList.m_Items[i].nIdx].GetDetailType() == 3)
{

Npc[m_nIndex].m_nPeopleIdx = 0;
Npc[m_nIndex].m_nPeopleIdxCheckClient = 0;

Npc[m_nIndex].m_nObjectIdx = 0;


ItemPos SrcPos;
SrcPos.nPlace = m_ItemList.m_Items[i].nPlace;
SrcPos.nX = m_ItemList.m_Items[i].nX;
SrcPos.nY = m_ItemList.m_Items[i].nY;
g_DebugLog("NGAY DAY NE: %d - %d \n",Npc[m_nIndex].m_CurrentLife, m_nAutoReturn);
ApplyUseItem(m_ItemList.m_Items[i].nIdx,SrcPos);
return TRUE;
}
}		
}

return FALSE;

}



BOOL KPlayer::PlayerAutoOnSkill()
{

ISkill * pISkill =  g_SkillManager.GetSkill(Npc[m_nIndex].m_SkillList.m_Skills[m_nAutoStarSkillIDx].SkillId,Npc[m_nIndex].m_SkillList.GetCurrentLevel(Npc[m_nIndex].m_SkillList.m_Skills[m_nAutoStarSkillIDx].SkillId));
		if (!pISkill) 
            return FALSE;

if ((!Npc[m_nIndex].m_SkillList.CanCast(Npc[m_nIndex].m_SkillList.m_Skills[m_nAutoStarSkillIDx].SkillId, SubWorld[0].m_dwCurrentTime))
			||
			(!Npc[m_nIndex].Cost(pISkill->GetSkillCostType(), pISkill->GetSkillCost(&Npc[m_nIndex]), TRUE))
			)
		{
	
			return FALSE;
		}

int nSkillLeftIdx = Npc[m_nIndex].m_SkillList.FindSame(m_nLeftSkillID);
int RangeSkill = 0;
if (nSkillLeftIdx > 0)
{
KSkill *pOrdinSkill = (KSkill *)g_SkillManager.GetSkill(m_nLeftSkillID,Npc[m_nIndex].m_SkillList.GetCurrentLevel(m_nLeftSkillID));
if (pOrdinSkill)
{
if (pOrdinSkill->IsTargetEnemy())
{
	m_nAutoStarSkillIDx = nSkillLeftIdx;
	RangeSkill = pOrdinSkill->GetAttackRadius();
}
}
}



Npc[m_nIndex].SetActiveSkill(m_nAutoStarSkillIDx);
if (m_nAutoRange >= RangeSkill)
Npc[m_nIndex].m_CurrentAttackRadius = RangeSkill;
else
Npc[m_nIndex].m_CurrentAttackRadius = m_nAutoRange;





	
 if (Npc[m_nIndex].m_nPeopleIdx > 0)
 {
	if ((Npc[Npc[m_nIndex].m_nPeopleIdx].m_Kind == kind_player && !m_TargetPlayer)  || (Npc[Npc[m_nIndex].m_nPeopleIdx].m_Kind != kind_player && m_TargetPlayer) )
	{
		Npc[Npc[m_nIndex].m_nPeopleIdx].m_nAutoNoActack = TRUE;
		Npc[m_nIndex].m_nPeopleIdx = 0;
		Npc[m_nIndex].m_nPeopleIdxCheckClient = 0;
	}
 }
 
 if (m_TuVe)
 {
	Npc[m_nIndex].m_nPeopleIdx = 0;
	Npc[m_nIndex].m_nPeopleIdxCheckClient = 0;
}

if (Npc[m_nIndex].m_nPeopleIdx <= 0)
{
    int nX0,nY0,nX1,nY1,nEnemyIdx,nMaxRangeDb;
	int KhoangCachGanNhat = 1000000;
	
	nEnemyIdx = 0;
	nMaxRangeDb = m_nAutoAtack * m_nAutoAtack;
	Npc[m_nIndex].GetMpsPos(&nX0,&nY0);

	for (int i = 1; i < MAX_NPC; i++)
	{
		if (!Npc[i].m_Index || 
			relation_enemy != NpcSet.GetRelation(m_nIndex,Npc[i].m_Index) ||
			Npc[i].m_Doing == do_death || 
			Npc[i].m_Doing == do_revive || Npc[i].m_CurrentLife <= 0 || Npc[i].m_CurrentLife == 100)
			continue;

		if (Npc[i].m_Kind != kind_player && m_TargetPlayer)	 // Auto PK Nguoi Choi
			continue;
		
		if (!m_TargetPlayer)  // Auto Target Quai
		{
			if (Npc[i].m_Kind == kind_player) 
			{			
				if (!m_TuVe)
					continue;
				else if (m_TuVe && Npc[i].m_nPKFlag != 2)
					continue;
			}
		}
	
		
		  Npc[i].GetMpsPos(&nX1,&nY1);

		if (Npc[i].m_nAutoNoActack && 
			(Npc[i].m_nAutoOldX - nX1) * (Npc[i].m_nAutoOldX - nX1) + (Npc[i].m_nAutoOldY - nY1) * (Npc[i].m_nAutoOldY - nY1) >= (120 * 120)
			)
			Npc[i].m_nAutoNoActack = FALSE;
			

		if (Npc[i].m_RegionIndex < 0 || !SubWorld[0].TestBarrier(nX1,nY1) || Npc[i].m_nAutoNoActack)
			continue;
				
		if (Npc[i].m_Kind == kind_player) 
		{
			nMaxRangeDb = m_nPhamViTuVe * m_nPhamViTuVe;
			if ((nX1 - m_nAutoStarNpcX)*(nX1 - m_nAutoStarNpcX) + (nY1 - m_nAutoStarNpcY) * (nY1 - m_nAutoStarNpcY) < nMaxRangeDb)
			{
					nEnemyIdx = i;
					break;
			}
		}
		else
		{
			int distance = NpcSet.GetDistance(i, m_nIndex);			
			if (((nX1-nX0)*(nX1-nX0) + (nY1-nY0)*(nY1-nY0)) < nMaxRangeDb && ((nX1-m_nAutoStarNpcX)*(nX1-m_nAutoStarNpcX) + (nY1-m_nAutoStarNpcY)*(nY1-m_nAutoStarNpcY)) < nMaxRangeDb && distance < KhoangCachGanNhat)
		//	if (distance <= m_nAutoAtack)
			{
				KhoangCachGanNhat = distance;
				nMaxRangeDb = (nX1-nX0)*(nX1-nX0) + (nY1-nY0)*(nY1-nY0);
				nEnemyIdx = i;
			}
		}
	}

if (nEnemyIdx > 0)
{
	

	if (Npc[nEnemyIdx].m_Kind == kind_player)  // Neu Doi Tuong la Player thi Check
	{
		
		Npc[m_nIndex].GetMpsPos(&nX1,&nY1);
		nMaxRangeDb = m_nPhamViTuVe * m_nPhamViTuVe;
		if ((nX1 - m_nAutoStarNpcX)*(nX1 - m_nAutoStarNpcX) + (nY1 - m_nAutoStarNpcY) * (nY1 - m_nAutoStarNpcY) >= nMaxRangeDb)
		{
			Npc[m_nIndex].SendCommand(do_walk, m_nAutoStarNpcX, m_nAutoStarNpcY);
			SendClientCmdWalk(m_nAutoStarNpcX, m_nAutoStarNpcY);
			Npc[m_nIndex].m_nPeopleIdx = 0;
			Npc[m_nIndex].m_nPeopleIdxCheckClient = 0;
		}
		else
		{
			Npc[m_nIndex].m_nPeopleIdx = nEnemyIdx;
			Npc[m_nIndex].m_nPeopleIdxCheckClient = (nEnemyIdx + 46) * 13 + 15743;
			m_nAutoOldCurenLifeActive = Npc[nEnemyIdx].m_CurrentLife;
		}
	}
	else
	{
		PlayerAutoOnBuffE();
		Npc[m_nIndex].m_nPeopleIdx = nEnemyIdx;
		Npc[m_nIndex].m_nPeopleIdxCheckClient = (nEnemyIdx + 46) * 13 + 15743;
		
		m_nAutoOldCurenLifeActive = Npc[nEnemyIdx].m_CurrentLife;
		m_nAutoTimeNextNpc = g_SubWorldSet.GetGameTime() + m_nAutoNAtack;
		

	}
}

else
{
	
	if (Npc[m_nIndex].m_CurrentStamina > 0 || m_cPK.GetNormalPKState() != 2)
		{
			if (m_QuanhDiem)
			{
				Npc[m_nIndex].GetMpsPos(&nX1,&nY1);
				if (((nX1 - m_nAutoStarNpcX)*(nX1 - m_nAutoStarNpcX) > 8*8) || ((nY1 - m_nAutoStarNpcY) * (nY1 - m_nAutoStarNpcY) > 16*16))
				{
					Npc[m_nIndex].SendCommand(do_run, m_nAutoStarNpcX, m_nAutoStarNpcY);
					SendClientCmdRun(m_nAutoStarNpcX, m_nAutoStarNpcY);
				}
				else
				{
					if (!Npc[m_nIndex].m_bRideHorse)
					{
						if (Npc[m_nIndex].m_Doing != do_sit)
						{
							Npc[m_nIndex].SendCommand(do_sit);
							SendClientCmdSit(TRUE);
						}
					}
				}
			}
		}
		else
		{
			if (m_QuanhDiem)
			{
				Npc[m_nIndex].GetMpsPos(&nX1,&nY1);
				if (((nX1 - m_nAutoStarNpcX)*(nX1 - m_nAutoStarNpcX) > 8*8) || ((nY1 - m_nAutoStarNpcY) * (nY1 - m_nAutoStarNpcY) > 16*16))
				{
					Npc[m_nIndex].SendCommand(do_walk, m_nAutoStarNpcX, m_nAutoStarNpcY);
					SendClientCmdWalk(m_nAutoStarNpcX, m_nAutoStarNpcY);
				}
				else
				{
					if (!Npc[m_nIndex].m_bRideHorse)
					{
						if (Npc[m_nIndex].m_Doing != do_sit)
						{
							Npc[m_nIndex].SendCommand(do_sit);
							SendClientCmdSit(TRUE);
						}
					}
				}
			}
		}

}


}

else if (m_nAutoTimeNextNpc < g_SubWorldSet.GetGameTime())
{

if (m_nAutoOldCurenLifeActive == Npc[Npc[m_nIndex].m_nPeopleIdx].m_CurrentLife)
{
Npc[Npc[m_nIndex].m_nPeopleIdx].GetMpsPos(&Npc[Npc[m_nIndex].m_nPeopleIdx].m_nAutoOldX,&Npc[Npc[m_nIndex].m_nPeopleIdx].m_nAutoOldY);
Npc[Npc[m_nIndex].m_nPeopleIdx].m_nAutoOldLife = Npc[Npc[m_nIndex].m_nPeopleIdx].m_CurrentLife;
Npc[Npc[m_nIndex].m_nPeopleIdx].m_nAutoNoActack = TRUE;
Npc[m_nIndex].m_nPeopleIdx = 0;
Npc[m_nIndex].m_nPeopleIdxCheckClient = 0;
}
else
{
m_nAutoTimeNextNpc = g_SubWorldSet.GetGameTime() + m_nAutoNAtack;
m_nAutoOldCurenLifeActive = Npc[Npc[m_nIndex].m_nPeopleIdx].m_CurrentLife;
}

}

return FALSE;
}



BOOL KPlayer::PlayerAutoOnSkillTheoSau()
{
g_DebugLog("Toi Day Roi Ne 2 \n");
ISkill * pISkill =  g_SkillManager.GetSkill(Npc[m_nIndex].m_SkillList.m_Skills[m_nAutoStarSkillIDx].SkillId,Npc[m_nIndex].m_SkillList.GetCurrentLevel(Npc[m_nIndex].m_SkillList.m_Skills[m_nAutoStarSkillIDx].SkillId));
		if (!pISkill) 
            return FALSE;

if ((!Npc[m_nIndex].m_SkillList.CanCast(Npc[m_nIndex].m_SkillList.m_Skills[m_nAutoStarSkillIDx].SkillId, SubWorld[0].m_dwCurrentTime))
			||
			(!Npc[m_nIndex].Cost(pISkill->GetSkillCostType(), pISkill->GetSkillCost(&Npc[m_nIndex]), TRUE))
			)
		{
	
			return FALSE;
		}

		
int nSkillLeftIdx = Npc[m_nIndex].m_SkillList.FindSame(m_nLeftSkillID);
int RangeSkill = 0;
if (nSkillLeftIdx > 0)
{
KSkill *pOrdinSkill = (KSkill *)g_SkillManager.GetSkill(m_nLeftSkillID,Npc[m_nIndex].m_SkillList.GetCurrentLevel(m_nLeftSkillID));
if (pOrdinSkill)
{
if (pOrdinSkill->IsTargetEnemy())
{
	m_nAutoStarSkillIDx = nSkillLeftIdx;
	RangeSkill = pOrdinSkill->GetAttackRadius();
}
}
}


Npc[m_nIndex].SetActiveSkill(m_nAutoStarSkillIDx);
if (m_nAutoRange >= RangeSkill)
Npc[m_nIndex].m_CurrentAttackRadius = RangeSkill;
else
Npc[m_nIndex].m_CurrentAttackRadius = m_nAutoRange;

if (Npc[m_nIndex].m_nPeopleIdx <= 0)
{
    int nX0,nY0,nX1,nY1,nEnemyIdx,nMaxRangeDb;
	
	nEnemyIdx = 0;
	int MinRange = 2000;
	nMaxRangeDb = m_nAutoAtack * m_nAutoAtack;
	Npc[m_nIndex].GetMpsPos(&nX0,&nY0);

	for (int i = 1; i < MAX_NPC; i++)
	{
	if (!Npc[i].m_Index || 
		relation_enemy != NpcSet.GetRelation(m_nIndex,Npc[i].m_Index) ||
		Npc[i].m_Doing == do_death || 
		Npc[i].m_Doing == do_revive || Npc[i].m_CurrentLife <= 0 || Npc[i].m_CurrentLife == 100)
		continue;

	if (Npc[i].m_Kind == kind_player)
		continue;

	  Npc[i].GetMpsPos(&nX1,&nY1);

	if (Npc[i].m_nAutoNoActack && 
		(Npc[i].m_nAutoOldX - nX1) * (Npc[i].m_nAutoOldX - nX1) + (Npc[i].m_nAutoOldY - nY1) * (Npc[i].m_nAutoOldY - nY1) >= (120 * 120)
		)
		Npc[i].m_nAutoNoActack = FALSE;
		

	if (Npc[i].m_RegionIndex < 0 || !SubWorld[0].TestBarrier(nX1,nY1) || Npc[i].m_nAutoNoActack)
		continue;
			
	
	int distance = NpcSet.GetDistance(i, m_nIndex);
	if (distance <= m_nAutoAtack && distance < MinRange)
	{
		MinRange = distance;
		nEnemyIdx = i;
		//g_DebugLog("Con khac: %d\n",nEnemyIdx);
	}
	}

if (nEnemyIdx > 0)
{
	Npc[m_nIndex].m_nPeopleIdx = nEnemyIdx;
	Npc[m_nIndex].m_nPeopleIdxCheckClient = (nEnemyIdx + 46) * 13 + 15743;
	
}
}

return FALSE;
}

/*
BOOL KPlayer::PlayerAutoOnSkill()
{

ISkill * pISkill =  g_SkillManager.GetSkill(Npc[m_nIndex].m_SkillList.m_Skills[m_nAutoStarSkillIDx].SkillId,Npc[m_nIndex].m_SkillList.GetCurrentLevel(Npc[m_nIndex].m_SkillList.m_Skills[m_nAutoStarSkillIDx].SkillId));
		if (!pISkill) 
            return FALSE;

if ((!Npc[m_nIndex].m_SkillList.CanCast(Npc[m_nIndex].m_SkillList.m_Skills[m_nAutoStarSkillIDx].SkillId, SubWorld[0].m_dwCurrentTime))
			||
			(!Npc[m_nIndex].Cost(pISkill->GetSkillCostType(), pISkill->GetSkillCost(&Npc[m_nIndex]), TRUE))
			)
		{
	
			return FALSE;
		}




Npc[m_nIndex].SetActiveSkill(m_nAutoStarSkillIDx);
Npc[m_nIndex].m_CurrentAttackRadius = m_nAutoRange;

if (Npc[m_nIndex].m_nPeopleIdx <= 0)
{
    int nX0,nY0,nX1,nY1,nEnemyIdx,nMaxRangeDb;
	
	nEnemyIdx = 0;
	nMaxRangeDb = m_nAutoAtack * m_nAutoAtack;

	Npc[m_nIndex].GetMpsPos(&nX0,&nY0);
	if (m_cPK.GetNormalPKState() == 0)
	{
		int iNumPlayer = 0;
		if (m_TargetPlayer)
		{
			for (int i = 1; i < MAX_NPC; i++)
			{
				if (!Npc[i].m_Index			||  
					Npc[i].m_Doing == do_death || 
					Npc[i].m_Doing == do_revive || Npc[i].m_CurrentLife <= 0 || Npc[i].m_CurrentLife == 100 ||
					Npc[i].m_Kind != kind_player)
					continue;
					
				//	if 	(m_TargetPlayer != m_TargetEnemy)
				//	{
				//		m_TargetEnemy = m_TargetPlayer;
				//		Npc[m_nIndex].m_nPeopleIdx = 0;
				//		nEnemyIdx = 0;
				//		break;
				//	}
					
					
					  Npc[i].GetMpsPos(&nX1,&nY1);

					if (Npc[i].m_nAutoNoActack && 
						(Npc[i].m_nAutoOldX - nX1) * (Npc[i].m_nAutoOldX - nX1) + (Npc[i].m_nAutoOldY - nY1) * (Npc[i].m_nAutoOldY - nY1) >= (120 * 120)
						)
						Npc[i].m_nAutoNoActack = FALSE;
						

					if (Npc[i].m_RegionIndex < 0 || !SubWorld[0].TestBarrier(nX1,nY1) || Npc[i].m_nAutoNoActack)
						continue;

					if (((nX1-nX0)*(nX1-nX0) + (nY1-nY0)*(nY1-nY0)) < nMaxRangeDb && ((nX1-m_nAutoStarNpcX)*(nX1-m_nAutoStarNpcX) + (nY1-m_nAutoStarNpcY)*(nY1-m_nAutoStarNpcY)) < nMaxRangeDb)
					{
						if (Npc[i].m_nPKFlag == 2)
						{
							if (m_bUseTDP)
							{
								for (int t = 0; t < MAX_PLAYER_ITEM; t++)
								{
								if (m_ItemList.m_Items[t].nIdx && (m_ItemList.m_Items[t].nPlace == pos_equiproom || m_ItemList.m_Items[t].nPlace == pos_immediacy))
								{
								if (Item[m_ItemList.m_Items[t].nIdx].GetGenre() == item_task && Item[m_ItemList.m_Items[t].nIdx].GetDetailType() == 3)
								{

								Npc[m_nIndex].m_nPeopleIdx = 0;
								Npc[m_nIndex].m_nPeopleIdxCheckClient = 0;

								Npc[m_nIndex].m_nObjectIdx = 0;


								ItemPos SrcPos;
								SrcPos.nPlace = m_ItemList.m_Items[t].nPlace;
								SrcPos.nX = m_ItemList.m_Items[t].nX;
								SrcPos.nY = m_ItemList.m_Items[t].nY;
								ApplyUseItem(m_ItemList.m_Items[t].nIdx,SrcPos);
								return TRUE;
								}
								}		
								}
							}
							else
							{
								iNumPlayer = 1;
								m_cPK.ApplySetNormalPKState(1);
							}
							break;	
						}
					}
			}
		}
		
		if (iNumPlayer == 0)
		{
			for (int i = 1; i < MAX_NPC; i++)
			{
				if (!Npc[i].m_Index || 
					relation_enemy != NpcSet.GetRelation(m_nIndex,Npc[i].m_Index) ||
					Npc[i].m_Doing == do_death || 
					Npc[i].m_Doing == do_revive || Npc[i].m_CurrentLife <= 0 || Npc[i].m_CurrentLife == 100 ||
					Npc[i].m_Kind == kind_player)
					continue;

				//if 	(m_TargetPlayer != m_TargetEnemy)
				//{
				//	m_TargetEnemy = m_TargetPlayer;
				//	Npc[m_nIndex].m_nPeopleIdx = 0;
				//	nEnemyIdx = 0;
				//	break;
				//}
				
				
				  Npc[i].GetMpsPos(&nX1,&nY1);

				if (Npc[i].m_nAutoNoActack && 
					(Npc[i].m_nAutoOldX - nX1) * (Npc[i].m_nAutoOldX - nX1) + (Npc[i].m_nAutoOldY - nY1) * (Npc[i].m_nAutoOldY - nY1) >= (120 * 120)
					)
					Npc[i].m_nAutoNoActack = FALSE;
					

				if (Npc[i].m_RegionIndex < 0 || !SubWorld[0].TestBarrier(nX1,nY1) || Npc[i].m_nAutoNoActack)
					continue;

				if (((nX1-nX0)*(nX1-nX0) + (nY1-nY0)*(nY1-nY0)) < nMaxRangeDb && ((nX1-m_nAutoStarNpcX)*(nX1-m_nAutoStarNpcX) + (nY1-m_nAutoStarNpcY)*(nY1-m_nAutoStarNpcY)) < nMaxRangeDb)
				{
					nMaxRangeDb = (nX1-nX0)*(nX1-nX0) + (nY1-nY0)*(nY1-nY0);
					nEnemyIdx = i;
				}

			}
		}
	}
	else
	{
		int iNumPlayer = 0;
		for (int i = 1; i < MAX_NPC; i++)
		{
			if (!Npc[i].m_Index || 
				relation_enemy != NpcSet.GetRelation(m_nIndex,Npc[i].m_Index) ||
				Npc[i].m_Doing == do_death || 
				Npc[i].m_Doing == do_revive || Npc[i].m_CurrentLife <= 0 || Npc[i].m_CurrentLife == 100 ||
				Npc[i].m_Kind != kind_player)
				continue;

			//if 	(m_TargetPlayer != m_TargetEnemy)
			//{
			//	m_TargetEnemy = m_TargetPlayer;
			//	Npc[m_nIndex].m_nPeopleIdx = 0;
			//	nEnemyIdx = 0;
			//	break;
			//}
			
			
			  Npc[i].GetMpsPos(&nX1,&nY1);

			if (Npc[i].m_nAutoNoActack && 
				(Npc[i].m_nAutoOldX - nX1) * (Npc[i].m_nAutoOldX - nX1) + (Npc[i].m_nAutoOldY - nY1) * (Npc[i].m_nAutoOldY - nY1) >= (120 * 120)
				)
				Npc[i].m_nAutoNoActack = FALSE;
				

			if (Npc[i].m_RegionIndex < 0 || !SubWorld[0].TestBarrier(nX1,nY1) || Npc[i].m_nAutoNoActack)
				continue;

			if (((nX1-nX0)*(nX1-nX0) + (nY1-nY0)*(nY1-nY0)) < nMaxRangeDb && ((nX1-m_nAutoStarNpcX)*(nX1-m_nAutoStarNpcX) + (nY1-m_nAutoStarNpcY)*(nY1-m_nAutoStarNpcY)) < nMaxRangeDb)
			{
				nMaxRangeDb = (nX1-nX0)*(nX1-nX0) + (nY1-nY0)*(nY1-nY0);
				nEnemyIdx = i;
			}
			iNumPlayer = 1;
		}
		if (iNumPlayer == 0)
		{
			for (int i = 1; i < MAX_NPC; i++)
			{
				if (!Npc[i].m_Index || 
					relation_enemy != NpcSet.GetRelation(m_nIndex,Npc[i].m_Index) ||
					Npc[i].m_Doing == do_death || 
					Npc[i].m_Doing == do_revive || Npc[i].m_CurrentLife <= 0 || Npc[i].m_CurrentLife == 100 ||
					Npc[i].m_Kind == kind_player)
					continue;

			//	if 	(m_TargetPlayer != m_TargetEnemy)
			//	{
			//		m_TargetEnemy = m_TargetPlayer;
			//		Npc[m_nIndex].m_nPeopleIdx = 0;
			//		nEnemyIdx = 0;
			//		break;
			//	}
				
				
				  Npc[i].GetMpsPos(&nX1,&nY1);

				if (Npc[i].m_nAutoNoActack && 
					(Npc[i].m_nAutoOldX - nX1) * (Npc[i].m_nAutoOldX - nX1) + (Npc[i].m_nAutoOldY - nY1) * (Npc[i].m_nAutoOldY - nY1) >= (120 * 120)
					)
					Npc[i].m_nAutoNoActack = FALSE;
					

				if (Npc[i].m_RegionIndex < 0 || !SubWorld[0].TestBarrier(nX1,nY1) || Npc[i].m_nAutoNoActack)
					continue;

				if (((nX1-nX0)*(nX1-nX0) + (nY1-nY0)*(nY1-nY0)) < nMaxRangeDb && ((nX1-m_nAutoStarNpcX)*(nX1-m_nAutoStarNpcX) + (nY1-m_nAutoStarNpcY)*(nY1-m_nAutoStarNpcY)) < nMaxRangeDb)
				{
					nMaxRangeDb = (nX1-nX0)*(nX1-nX0) + (nY1-nY0)*(nY1-nY0);
					nEnemyIdx = i;
				}

			}
		}
	}

	if (nEnemyIdx > 0)
	{

	Npc[m_nIndex].m_nPeopleIdx = nEnemyIdx;
	Npc[m_nIndex].m_nPeopleIdxCheckClient = (nEnemyIdx + 46) * 13 + 15743;

	m_nAutoOldCurenLifeActive = Npc[nEnemyIdx].m_CurrentLife;
	m_nAutoTimeNextNpc = g_SubWorldSet.GetGameTime() + m_nAutoNAtack;

	}

	else
	{

	if (Npc[m_nIndex].m_CurrentStamina > 0 || m_cPK.GetNormalPKState() != 2)
		{
			if (m_QuanhDiem)
			{
				Npc[m_nIndex].SendCommand(do_run, m_nAutoStarNpcX, m_nAutoStarNpcY);
				SendClientCmdRun(m_nAutoStarNpcX, m_nAutoStarNpcY);
			}
		}
		else
		{
			if (m_QuanhDiem)
			{
				Npc[m_nIndex].SendCommand(do_walk, m_nAutoStarNpcX, m_nAutoStarNpcY);
				SendClientCmdWalk(m_nAutoStarNpcX, m_nAutoStarNpcY);
			}
		}

	}


}

else if (m_nAutoTimeNextNpc < g_SubWorldSet.GetGameTime() && !m_TargetPlayer)
{

	if (m_nAutoOldCurenLifeActive == Npc[Npc[m_nIndex].m_nPeopleIdx].m_CurrentLife)
	{
	Npc[Npc[m_nIndex].m_nPeopleIdx].GetMpsPos(&Npc[Npc[m_nIndex].m_nPeopleIdx].m_nAutoOldX,&Npc[Npc[m_nIndex].m_nPeopleIdx].m_nAutoOldY);
	Npc[Npc[m_nIndex].m_nPeopleIdx].m_nAutoOldLife = Npc[Npc[m_nIndex].m_nPeopleIdx].m_CurrentLife;
	Npc[Npc[m_nIndex].m_nPeopleIdx].m_nAutoNoActack = TRUE;
	Npc[m_nIndex].m_nPeopleIdx = 0;

	Npc[m_nIndex].m_nPeopleIdxCheckClient = 0;
	}
	else
	{
	m_nAutoTimeNextNpc = g_SubWorldSet.GetGameTime() + m_nAutoNAtack;
	m_nAutoOldCurenLifeActive = Npc[Npc[m_nIndex].m_nPeopleIdx].m_CurrentLife;
	}

}

return FALSE;
}

*/


// Ham moi
BOOL KPlayer::PlayerAutoOnObj()
{


if (Npc[m_nIndex].m_nObjectIdx > 0)
{

if (g_SubWorldSet.GetGameTime() > m_nAutoTimeNextObj)
{
Object[Npc[m_nIndex].m_nObjectIdx].m_nAutoNoActack = TRUE;
Npc[m_nIndex].m_nObjectIdx = 0;
return FALSE;
}



int nOX,nOY;
Object[Npc[m_nIndex].m_nObjectIdx].GetMpsPos(&nOX, &nOY);
if (Npc[m_nIndex].m_CurrentStamina > 0 || m_cPK.GetNormalPKState() != 2)
	{
		Npc[m_nIndex].SendCommand(do_run, nOX, nOY);
		SendClientCmdRun(nOX, nOY);
	}
	else
	{
	    Npc[m_nIndex].SendCommand(do_walk,nOX, nOY);
		SendClientCmdWalk(nOX, nOY);
	}

return TRUE;

}

else

{



    int nX0,nY0,nX1,nY1,nObjIdx,nMaxRangeDb;
	
	nObjIdx = 0;
	nMaxRangeDb = m_nAutoAtack * m_nAutoAtack;

	Npc[m_nIndex].GetMpsPos(&nX0,&nY0);

	for (int i = 1; i < MAX_OBJECT; i++)
	{

	if (Object[i].m_nIndex <= 0 || Object[i].m_nRegionIdx < 0 || (Object[i].m_nKind != Obj_Kind_Money && Object[i].m_nKind != Obj_Kind_Prop && Object[i].m_nKind != Obj_Kind_Item) || Object[i].m_nAutoNoActack)
		continue;

	if (Object[i].m_nKind == Obj_Kind_Money && !m_bMoneyObj)
		continue;

	if (Object[i].m_nKind == Obj_Kind_Item && !m_bItemObj && !m_bItemSelect)
		continue;
	
	if (Object[i].m_nKind == Obj_Kind_Prop && !m_bPropObj)
		continue;

	Object[i].GetMpsPos(&nX1, &nY1);



	if (((nX1-nX0)*(nX1-nX0) + (nY1-nY0)*(nY1-nY0)) < nMaxRangeDb)
	{
		nMaxRangeDb = (nX1-nX0)*(nX1-nX0) + (nY1-nY0)*(nY1-nY0);
        nObjIdx = i;
	}
	}

	//if (Item[Object[nObjIndex].m_nItemDataID].GetSeries() = 0)
	//continue;
	
	
	// Test Auto
	//m_ItemList.RemoveAllInOneRoom(room_repository);
	if (nObjIdx > 0)
	{
	//	g_DebugLog("NGUHANH %s",Object[nObjIdx].m_nItemDataID);
	
		Npc[m_nIndex].m_nObjectIdx = nObjIdx;
		
		m_nAutoTimeNextObj = g_SubWorldSet.GetGameTime() + MAX_AUTO_OBJ_WAIL;
	 return TRUE;
	}

}

return FALSE;
}


#endif



#ifdef _SERVER
void KPlayer::SetExp(int nExpAdd)
{
	if (nExpAdd < 0 && nExpAdd > 2000000000)
	return;
	
	m_nExp = nExpAdd;
	PLAYER_EXP_SYNC	sSyncExp;
	sSyncExp.ProtocolType = (BYTE)s2c_playerexp;
	sSyncExp.m_nExp = m_nExp;
	g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sSyncExp, sizeof(PLAYER_EXP_SYNC));
	
	if (CanSave())
			{
				
				if (Save())
				{
				
					m_uMustSave = SAVE_REQUEST;
					
				}
	
			}
}
#endif




