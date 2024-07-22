#include "KEngine.h"
#include "KCore.h"
#ifndef _SERVER
#include "../../Headers/IClient.h"
#include "CoreShell.h"
#include "KViewItem.h"
#endif
#include "KObjSet.h"
#include "KNpcSet.h"
#include "KPlayer.h"
#include "KPlayerSet.h"
#include "KPlayerTeam.h"
#include "KPlayerFaction.h"
#include "KNpc.h"
#include "KSubWorld.h"
#include "LuaFuns.h"
#include "KProtocolProcess.h"
#include "KSkills.h"
#include "KItemSet.h"
#include "KBuySell.h"
#include "KSubWorldSet.h"
//#include "MyAssert.h"
#include "Scene/ObstacleDef.h"
#include "KMath.h"
#ifndef _SERVER
#include "Scene\KScenePlaceC.h"
#endif
#include "KTongProtocol.h"
#include "KLadder.h"

//#define WAIGUA_ZROC

KProtocolProcess g_ProtocolProcess;
KProtocolProcess::KProtocolProcess()
{
	ZeroMemory(ProcessFunc, sizeof(ProcessFunc));
#ifndef _SERVER
	ProcessFunc[s2c_nodoing] = NULL;
	ProcessFunc[s2c_login] = NULL;
	ProcessFunc[s2c_logout] = NULL;
	ProcessFunc[s2c_syncend] = SyncEnd;
	ProcessFunc[s2c_synccurplayer] = SyncCurPlayer;
	ProcessFunc[s2c_synccurplayerskill] = s2cSyncAllSkill;
	ProcessFunc[s2c_synccurplayernormal] = SyncCurNormalData;
	ProcessFunc[s2c_newplayer] = NULL;
	ProcessFunc[s2c_removeplayer] = NULL;
	ProcessFunc[s2c_syncworld] = SyncWorld;
	ProcessFunc[s2c_syncplayer] = SyncPlayer;
	ProcessFunc[s2c_syncplayermin] = SyncPlayerMin;
	ProcessFunc[s2c_syncnpc] = SyncNpc;
	ProcessFunc[s2c_syncnpcmin] = SyncNpcMin;
	ProcessFunc[s2c_syncnpcminplayer] = SyncNpcMinPlayer;
	ProcessFunc[s2c_objadd] = SyncObjectAdd;
	ProcessFunc[s2c_syncobjstate] = SyncObjectState;
	ProcessFunc[s2c_syncobjdir] = SyncObjectDir;
	ProcessFunc[s2c_objremove] = SyncObjectRemove;
	ProcessFunc[s2c_objTrapAct] = SyncObjectTrap;
	ProcessFunc[s2c_npcremove] = NetCommandRemoveNpc;
	ProcessFunc[s2c_npcwalk] = NetCommandWalk;
	ProcessFunc[s2c_npcrun] = NetCommandRun;
	ProcessFunc[s2c_npcattack] = NULL;
	ProcessFunc[s2c_npcmagic] = NULL;
	ProcessFunc[s2c_npcjump] = NetCommandJump;
	ProcessFunc[s2c_npctalk] = NULL;
	ProcessFunc[s2c_npchurt] = NetCommandHurt;
	ProcessFunc[s2c_npcdeath] = NetCommandDeath;
	ProcessFunc[s2c_npcchgcurcamp] = NetCommandChgCurCamp;
	ProcessFunc[s2c_npcchgcamp] = NetCommandChgCamp;
	ProcessFunc[s2c_skillcast] = NetCommandSkill;
	ProcessFunc[s2c_playertalk] = NULL;
	ProcessFunc[s2c_playerexp] = s2cPlayerExp;
	ProcessFunc[s2c_teaminfo] = s2cShowTeamInfo;
	ProcessFunc[s2c_teamselfinfo] = s2cUpdataSelfTeamInfo;
	ProcessFunc[s2c_teamapplyinfofalse] = s2cApplyTeamInfoFalse;
	ProcessFunc[s2c_teamcreatesuccess] = s2cCreateTeam;
	ProcessFunc[s2c_teamcreatefalse] = s2cApplyCreateTeamFalse;
	ProcessFunc[s2c_teamopenclose] = s2cSetTeamState;
	ProcessFunc[s2c_teamgetapply] = s2cApplyAddTeam;
	ProcessFunc[s2c_teamaddmember] = s2cTeamAddMember;
	ProcessFunc[s2c_teamleave] = s2cLeaveTeam;
	ProcessFunc[s2c_teamchangecaptain] = s2cTeamChangeCaptain;
	ProcessFunc[s2c_playerfactiondata] = s2cSetFactionData;
	ProcessFunc[s2c_playerleavefaction] = s2cLeaveFaction;
	ProcessFunc[s2c_playerfactionskilllevel] = s2cFactionSkillOpen;
	ProcessFunc[s2c_playersendchat] = s2cGetChat;
	ProcessFunc[s2c_playersyncleadexp] = s2cGetLeadExp;
	ProcessFunc[s2c_playerlevelup] = s2cLevelUp;
	ProcessFunc[s2c_teammatelevel] = s2cGetTeammateLevel;
	ProcessFunc[s2c_playersyncattribute] = s2cGetCurAttribute;
	ProcessFunc[s2c_playerskilllevel] = s2cGetSkillLevel;
	ProcessFunc[s2c_syncitem] = s2cSyncItem;
	ProcessFunc[s2c_removeitem] = s2cRemoveItem;
	ProcessFunc[s2c_syncmoney] = s2cSyncMoney;
	ProcessFunc[s2c_playermoveitem] = s2cMoveItem;
	ProcessFunc[s2c_scriptaction] = SyncScriptAction;
	ProcessFunc[s2c_chatapplyaddfriend] = s2cChatGetApplyAddFriend;
	ProcessFunc[s2c_chataddfriend] = s2cChatAddFriend;
	ProcessFunc[s2c_chatrefusefriend] = s2cChatBeRefusedAddFriend;
	ProcessFunc[s2c_chataddfriendfail] = s2cChatAddFriendFail;
	ProcessFunc[s2c_chatloginfriendnoname] = s2cChatLoginFriendNoName;
	ProcessFunc[s2c_chatloginfriendname] = s2cChatLoginFriendName;
	ProcessFunc[s2c_chatonefrienddata] = s2cChatAddOneFriend;
	ProcessFunc[s2c_chatfriendonline] = s2cChatFriendOnline;
	ProcessFunc[s2c_chatdeletefriend] = s2cChatDeleteFriend;
	ProcessFunc[s2c_chatfriendoffline] = s2cChatFriendOffLine;
	ProcessFunc[s2c_syncrolelist] = s2cSyncRoleList;
	ProcessFunc[s2c_tradechangestate] = s2cTradeChangeState;
	ProcessFunc[s2c_npcsetmenustate] = s2cNpcSetMenuState;
	ProcessFunc[s2c_trademoneysync] = s2cTradeMoneySync;
	ProcessFunc[s2c_tradedecision] = s2cTradeDecision;
	ProcessFunc[s2c_chatscreensingleerror] = s2cChatScreenSingleError;
	ProcessFunc[s2c_syncnpcstate] = s2cUpdateNpcState;
	ProcessFunc[s2c_teaminviteadd] = s2cTeamInviteAdd;
	ProcessFunc[s2c_tradepressoksync] = s2cTradePressOkSync;
	ProcessFunc[s2c_ping] = s2cPing;
	ProcessFunc[s2c_npcsit] = NetCommandSit;
	ProcessFunc[s2c_opensalebox] = OpenSaleBox;
	ProcessFunc[s2c_castskilldirectly] = s2cDirectlyCastSkill;
	ProcessFunc[s2c_msgshow] = s2cShowMsg;
	ProcessFunc[s2c_syncstateeffect] = SyncStateEffect;
	ProcessFunc[s2c_openstorebox] = OpenStoreBox;
	ProcessFunc[s2c_playerrevive] = PlayerRevive;
	ProcessFunc[s2c_requestnpcfail] = RequestNpcFail;
	ProcessFunc[s2c_tradeapplystart] = s2cTradeApplyStart;
	ProcessFunc[s2c_rolenewdelresponse] = NULL;
	ProcessFunc[s2c_ItemAutoMove] = s2cItemAutoMove;
	ProcessFunc[s2c_itemexchangefinish] = FinishedItemExchange;
	ProcessFunc[s2c_changeweather] = s2cChangeWeather;
	ProcessFunc[s2c_pksyncnormalflag] = s2cPKSyncNormalFlag;
	ProcessFunc[s2c_pksyncenmitystate] = s2cPKSyncEnmityState;
	ProcessFunc[s2c_pksyncexercisestate] = s2cPKSyncExerciseState;
	ProcessFunc[s2c_pksyncpkvalue] = s2cPKValueSync;
	ProcessFunc[s2c_npcsleepmode] = NpcSleepSync;
	ProcessFunc[s2c_viewequip] = s2cViewEquip;
	ProcessFunc[s2c_ladderresult] = LadderResult;
	ProcessFunc[s2c_ladderlist] = LadderList;
	ProcessFunc[s2c_tongcreate] = s2cTongCreate;
	ProcessFunc[s2c_replyclientping] = ServerReplyClientPing;
	ProcessFunc[s2c_npcgoldchange] = s2cNpcGoldChange;
	ProcessFunc[s2c_itemdurabilitychange] = ItemChangeDurability;
	ProcessFunc[s2c_syncmasklock] = SyncMaskLock; // MASK
	ProcessFunc[s2c_playerblockitemreturn] = PlayerBlockItemReturn;
	ProcessFunc[s2c_playerunblockitemreturn] = PlayerUnBlockItemReturn;
	ProcessFunc[s2c_synccurplayerheight] = SyncCurPlayerHeight;
	ProcessFunc[s2c_playerunblockitemnow] = PlayerBlockItemNow;
	ProcessFunc[s2c_playerupdateitemvertion] = PlayerUpdateItemVertion;
	ProcessFunc[s2c_playerupdatetong] = PlayerUpdateTong;
	ProcessFunc[s2c_playeropenstringbox] = PlayerOpenStringBox;
	ProcessFunc[s2c_playeropencheckitembox] = PlayerOpenCheckItemBox;
	ProcessFunc[s2c_playeropenslectcheckbox] = PlayerOpenSlectCheckBox;
	ProcessFunc[s2c_playeropenpgboxitem] = PlayerOpenPGBoxItem;
	ProcessFunc[s2c_playershoppricereturn] = PlayerShopPriceReturn;
	ProcessFunc[s2c_playerupdateshopprice] = PlayerUpdateShopPrice;
	ProcessFunc[s2c_playershopopenreturn] = PlayerOpenShopReturn;
	ProcessFunc[s2c_playerupdateshop] = PlayerUpdateShop;
	ProcessFunc[s2c_synclistitembuyshop] = SysListItemBuyShop;
	ProcessFunc[s2c_syncitembuyshop] = SysItemBuyShop;
	ProcessFunc[s2c_syncposmin] = SysNcPosMin;
	ProcessFunc[s2c_playerblockbox] = PlayerBlockBox;
	ProcessFunc[s2c_playeropennewpwboxitem] = PlayerOpenNewPWBoxItem;
	ProcessFunc[s2c_playersendtoptknew] = PlayerSendTopTKNew;
	ProcessFunc[s2c_playertongkimsync] = PlayerTKSync;
	ProcessFunc[s2c_sync_item_param] = S2CSyncItemParam;
	ProcessFunc[s2c_refestitem] = s2cRefestItem;


	
	ProcessFunc[s2c_extend] = s2cExtend;
	ProcessFunc[s2c_extendchat] = s2cExtendChat;
	ProcessFunc[s2c_extendfriend] = s2cExtendFriend;
	ProcessFunc[s2c_extendtong] = s2cExtendTong;


#else
	ProcessFunc[c2s_login] = NULL;
	ProcessFunc[c2s_logiclogin] = NULL;
	ProcessFunc[c2s_syncend] = NULL;
	ProcessFunc[c2s_loadplayer] = NULL;
	ProcessFunc[c2s_newplayer] = NULL;
	ProcessFunc[c2s_removeplayer] = &KProtocolProcess::RemoveRole;
	ProcessFunc[c2s_requestworld] = NULL;
	ProcessFunc[c2s_requestplayer] = NULL;
	ProcessFunc[c2s_requestnpc] = &KProtocolProcess::NpcRequestCommand;
	ProcessFunc[c2s_requestobj] = &KProtocolProcess::ObjRequestCommand;
	ProcessFunc[c2s_npcwalk] = &KProtocolProcess::NpcWalkCommand;
	ProcessFunc[c2s_npcrun] = &KProtocolProcess::NpcRunCommand;
	ProcessFunc[c2s_npcskill] = &KProtocolProcess::NpcSkillCommand;
	ProcessFunc[c2s_npcjump] = &KProtocolProcess::NpcJumpCommand;
	ProcessFunc[c2s_npctalk] = &KProtocolProcess::NpcTalkCommand;
	ProcessFunc[c2s_npchurt] = NULL;
	ProcessFunc[c2s_npcdeath] = NULL;
	ProcessFunc[c2s_playertalk] = &KProtocolProcess::PlayerTalkCommand;
	ProcessFunc[c2s_teamapplyinfo] = &KProtocolProcess::PlayerApplyTeamInfo;
	ProcessFunc[c2s_teamapplycreate] = &KProtocolProcess::PlayerApplyCreateTeam;
	ProcessFunc[c2s_teamapplyopenclose] = &KProtocolProcess::PlayerApplyTeamOpenClose;
	ProcessFunc[c2s_teamapplyadd] = &KProtocolProcess::PlayerApplyAddTeam;
	ProcessFunc[c2s_teamacceptmember] = &KProtocolProcess::PlayerAcceptTeamMember;
	ProcessFunc[c2s_teamapplyleave] = &KProtocolProcess::PlayerApplyLeaveTeam;
	ProcessFunc[c2s_teamapplykickmember] = &KProtocolProcess::PlayerApplyTeamKickMember;
	ProcessFunc[c2s_teamapplychangecaptain] = &KProtocolProcess::PlayerApplyTeamChangeCaptain;
	ProcessFunc[c2s_teamapplydismiss] = &KProtocolProcess::PlayerApplyTeamDismiss;
	ProcessFunc[c2s_playerapplysetpk] = &KProtocolProcess::PlayerApplySetPK;
	ProcessFunc[c2s_playerapplyfactiondata] = &KProtocolProcess::PlayerApplyFactionData;
	ProcessFunc[c2s_playersendchat] = &KProtocolProcess::PlayerSendChat;
	ProcessFunc[c2s_playeraddbaseattribute] = &KProtocolProcess::PlayerAddBaseAttribute;
	ProcessFunc[c2s_playerapplyaddskillpoint] = &KProtocolProcess::PlayerApplyAddSkillPoint;
	ProcessFunc[c2s_playereatitem] = &KProtocolProcess::PlayerEatItem;
	ProcessFunc[c2s_playerpickupitem] = &KProtocolProcess::PlayerPickUpItem;
	ProcessFunc[c2s_playermoveitem] = &KProtocolProcess::PlayerMoveItem;
	ProcessFunc[c2s_playersellitem] = &KProtocolProcess::PlayerSellItem;
	ProcessFunc[c2s_playerbuyitem] = &KProtocolProcess::PlayerBuyItem;
	ProcessFunc[c2s_playerthrowawayitem] = &KProtocolProcess::PlayerDropItem;
	ProcessFunc[c2s_playerselui] = &KProtocolProcess::PlayerSelUI;
	ProcessFunc[c2s_chatsetchannel] = &KProtocolProcess::ChatSetChannel;
	ProcessFunc[c2s_chatapplyaddfriend] = &KProtocolProcess::ChatApplyAddFriend;
	ProcessFunc[c2s_chataddfriend] = &KProtocolProcess::ChatAddFriend;
	ProcessFunc[c2s_chatrefusefriend] = &KProtocolProcess::ChatRefuseFriend;
	ProcessFunc[c2s_dbplayerselect] = NULL;
	ProcessFunc[c2s_chatapplyresendallfriendname] = &KProtocolProcess::ChatApplyReSendAllFriendName;
	ProcessFunc[c2s_chatapplysendonefriendname] = &KProtocolProcess::ChatApplySendOneFriendName;
	ProcessFunc[c2s_chatdeletefriend] = &KProtocolProcess::ChatDeleteFriend;
	ProcessFunc[c2s_chatredeletefriend] = &KProtocolProcess::ChatReDeleteFriend;
	ProcessFunc[c2s_tradeapplystateopen] = &KProtocolProcess::TradeApplyOpen;
	ProcessFunc[c2s_tradeapplystateclose] = &KProtocolProcess::TradeApplyClose;
	ProcessFunc[c2s_tradeapplystart] = &KProtocolProcess::TradeApplyStart;
	ProcessFunc[c2s_trademovemoney] = &KProtocolProcess::TradeMoveMoney;
	ProcessFunc[c2s_tradedecision] = &KProtocolProcess::TradeDecision;
	ProcessFunc[c2s_dialognpc]	= &KProtocolProcess::DialogNpc;
	ProcessFunc[c2s_teaminviteadd]	= &KProtocolProcess::TeamInviteAdd;
	//ProcessFunc[c2s_toado] = &KProtocolProcess::NpcToaDoCommand;
	ProcessFunc[c2s_changeauraskill] = &KProtocolProcess::ChangeAuraSkill;
	
	ProcessFunc[c2s_teamreplyinvite] = &KProtocolProcess::TeamReplyInvite;
	ProcessFunc[c2s_ping] = NULL;//ReplyPing;
	ProcessFunc[c2s_npcsit] = &KProtocolProcess::NpcSitCommand;
	ProcessFunc[c2s_npcride] = &KProtocolProcess::NpcRideCommand;
	ProcessFunc[c2s_objmouseclick] = &KProtocolProcess::ObjMouseClick;
	ProcessFunc[c2s_storemoney] = &KProtocolProcess::StoreMoneyCommand;
	ProcessFunc[c2s_playerrevive] = &KProtocolProcess::NpcReviveCommand;
	ProcessFunc[c2s_tradereplystart] = &KProtocolProcess::c2sTradeReplyStart;
	ProcessFunc[c2s_pkapplychangenormalflag] = &KProtocolProcess::c2sPKApplyChangeNormalFlag;
	ProcessFunc[c2s_pkapplyenmity] = &KProtocolProcess::c2sPKApplyEnmity;
	ProcessFunc[c2s_viewequip] = &KProtocolProcess::c2sViewEquip;
	ProcessFunc[c2s_ladderquery] = &KProtocolProcess::LadderQuery;
	ProcessFunc[c2s_repairitem] = &KProtocolProcess::ItemRepair;
	ProcessFunc[c2s_cpsetimage] = &KProtocolProcess::c2sSetImage;
	ProcessFunc[c2s_playerblockitem] = &KProtocolProcess::PlayerBlockItem;
	ProcessFunc[c2s_playerunblockitem] = &KProtocolProcess::PlayerUnBlockItem;
	ProcessFunc[c2s_playerstaskitem] = &KProtocolProcess::PlayerStaskItem;
	ProcessFunc[c2s_playerstringbox] = &KProtocolProcess::PlayerStringBox;
	ProcessFunc[c2s_playercheckitembox] = &KProtocolProcess::PlayerCheckItemBox;
	ProcessFunc[c2s_playerslectcheckbox] = &KProtocolProcess::PlayerSlectCheckBox;
	ProcessFunc[c2s_playerbuttonpgbox] = &KProtocolProcess::PlayerButtonPGBox;
	ProcessFunc[c2s_playershopname] = &KProtocolProcess::PlayerShopName;
	ProcessFunc[c2s_playershopprice] = &KProtocolProcess::PlayerShopPrice;
	ProcessFunc[c2s_playershopopen] = &KProtocolProcess::PlayerShopOpen;
	ProcessFunc[c2s_playeropenbuyshop] = &KProtocolProcess::PlayerOpenBuyShop;
	ProcessFunc[c2s_playershopbuyitem] = &KProtocolProcess::PlayerShopBuyItem;
	ProcessFunc[c2s_playerclosebuyshop] = &KProtocolProcess::PlayerCloseBuyShop;
	ProcessFunc[c2s_openshopgold] = &KProtocolProcess::OpenShopGold;
	ProcessFunc[c2s_playerloginpwstorebox] = &KProtocolProcess::PlayerLoginPWStoreBox;
	ProcessFunc[c2s_playerpwstorebox] = &KProtocolProcess::PlayerPWStoreBox;
	ProcessFunc[c2s_playerblockstorebox] = &KProtocolProcess::PlayerBlockStoreBox;
	ProcessFunc[c2s_playernewpwstorebox] = &KProtocolProcess::PlayerNewPWStoreBox;
	ProcessFunc[c2s_opentoptknew] = &KProtocolProcess::PlayerOpenTopTKNew;
	ProcessFunc[c2s_playerlixian] = &KProtocolProcess::c2sLiXian;
	ProcessFunc[c2s_autoquaylai] = &KProtocolProcess::c2sQuayLai;
	ProcessFunc[c2s_autolocdo] = &KProtocolProcess::c2sLocDo;

#endif
}

KProtocolProcess::~KProtocolProcess()
{
}

#ifndef _SERVER
void KProtocolProcess::ProcessNetMsg(BYTE* pMsg)
{
	if (!pMsg || pMsg[0] <= s2c_clientbegin || pMsg[0] >= s2c_end || ProcessFunc[pMsg[0]] == NULL)
	{
		//g_DebugLog("[error]Net Msg Error");
		return;
	}
//	g_DebugLog("[net]Msg:%c", pMsg[0]);
	if (ProcessFunc[pMsg[0]])
		(this->*ProcessFunc[pMsg[0]])(pMsg);
}
#else
void KProtocolProcess::ProcessNetMsg(int nIndex, BYTE* pMsg)
{
	_ASSERT(pMsg && pMsg[0] > c2s_gameserverbegin && pMsg[0] < c2s_end);

	BYTE	byProtocol = pMsg[0];
	_ASSERT(nIndex > 0 && nIndex < MAX_PLAYER);
	if (ProcessFunc[byProtocol])
	{
		(this->*ProcessFunc[byProtocol])(nIndex, pMsg);
		Player[nIndex].SetLastNetOperationTime(g_SubWorldSet.GetGameTime());
	}
}
#endif

#ifndef _SERVER
void KProtocolProcess::NpcSleepSync(BYTE* pMsg)
{
	NPC_SLEEP_SYNC*	pSync = (NPC_SLEEP_SYNC *)pMsg;
	DWORD	dwNpcId = pSync->NpcID;

	int nIdx = NpcSet.SearchID(dwNpcId);

	if (nIdx > 0)
	{
		Npc[nIdx].SetSleepMode((BOOL)pSync->bSleep);
		Npc[nIdx].m_SyncSignal = SubWorld[0].m_dwCurrentTime;
	}
}

void KProtocolProcess::s2cPing(BYTE* pMsg)
{
//	DWORD	dwTimer = GetTickCount();
//	PING_COMMAND* PingCmd = (PING_COMMAND *)pMsg;
//
//	dwTimer -= PingCmd->m_dwTime;
//	dwTimer >>= 1;
//	g_SubWorldSet.SetPing(dwTimer);
//	g_bPingReply = TRUE;

	DWORD	dwTimer = GetTickCount();
	PING_COMMAND*	PingCmd = (PING_COMMAND *)pMsg;
	PING_CLIENTREPLY_COMMAND	pcc;
	pcc.ProtocolType = c2s_ping;
	pcc.m_dwReplyServerTime = PingCmd->m_dwTime;
	pcc.m_dwClientTime = dwTimer;
	g_pClient->SendPackToServer(&pcc, sizeof(PING_CLIENTREPLY_COMMAND));
	//g_SubWorldSet.SetPing(PingCmd->m_dwTime);
}

void KProtocolProcess::ServerReplyClientPing(BYTE* pMsg)
{
	DWORD	dwTimer = GetTickCount();
	PING_COMMAND* pPc = (PING_COMMAND *)pMsg;

	dwTimer -= pPc->m_dwTime;
	dwTimer >>= 1;
	g_SubWorldSet.SetPing(dwTimer);
}

void KProtocolProcess::s2cPlayerExp(BYTE* pMsg)
{
	PLAYER_EXP_SYNC	*pExp = (PLAYER_EXP_SYNC*)pMsg;
	Player[CLIENT_PLAYER_INDEX].s2cSetExp(pExp->m_nExp);
}

void	KProtocolProcess::s2cChatScreenSingleError(BYTE* pMsg)
{
	CHAT_SCREENSINGLE_ERROR_SYNC	*pError = (CHAT_SCREENSINGLE_ERROR_SYNC*)pMsg;
	
	// ֪ͨ������Ļ���Ĳ��ɹ�
	char	szName[32];
	memset(szName, 0, sizeof(szName));
	memcpy(szName, pError->m_szName, pError->m_wLength + 1 + sizeof(pError->m_szName) - sizeof(CHAT_SCREENSINGLE_ERROR_SYNC));

	KSystemMessage	sMsg;
	sprintf(sMsg.szMessage, MSG_CHAT_TAR_REFUSE_SINGLE_TALK, szName);
	sMsg.eType = SMT_NORMAL;
	sMsg.byConfirmType = SMCT_NONE;
	sMsg.byPriority = 0;
	sMsg.byParamSize = 0;
	CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
}

void KProtocolProcess::NetCommandChgCamp(BYTE* pMsg)
{
	DWORD	dwNpcId;

	dwNpcId = *(DWORD *)&pMsg[1];
	int nIdx = NpcSet.SearchID(dwNpcId);

	if (nIdx > 0)
	{
		Npc[nIdx].m_Camp = (int)pMsg[5];
		Npc[nIdx].m_SyncSignal = SubWorld[0].m_dwCurrentTime;
	}
}

void KProtocolProcess::NetCommandChgCurCamp(BYTE* pMsg)
{
	DWORD	dwNpcId;

	dwNpcId = *(DWORD *)&pMsg[1];
	int nIdx = NpcSet.SearchID(dwNpcId);

	if (nIdx > 0)
	{
		Npc[nIdx].m_CurrentCamp = (int)pMsg[5];
		Npc[nIdx].m_SyncSignal = SubWorld[0].m_dwCurrentTime;
	}
}

void KProtocolProcess::NetCommandDeath(BYTE* pMsg)
{
	DWORD	dwNpcID;
	
	dwNpcID = *(DWORD *)&pMsg[1];
	int nIdx = NpcSet.SearchID(dwNpcID);

	if (nIdx > 0)
	{
		//Npc[nIdx].SendCommand(do_death);
		Npc[nIdx].ProcNetCommand(do_death);
		Npc[nIdx].m_CurrentLife = 0;
		Npc[nIdx].m_SyncSignal = SubWorld[0].m_dwCurrentTime;
		g_DebugLog("[Death]Net command comes");
	}
}

void KProtocolProcess::NetCommandJump(BYTE* pMsg)
{
	NPC_JUMP_SYNC* pNetCommandJump = (NPC_JUMP_SYNC *)pMsg;
	DWORD dwNpcId = pNetCommandJump->ID;
	int nIdx = NpcSet.SearchID(dwNpcId);
	
	if (Player[CLIENT_PLAYER_INDEX].ConformIdx(nIdx))
	{
		Npc[nIdx].SendCommand(do_jump, pNetCommandJump->nMpsX, pNetCommandJump->nMpsY);
		Npc[nIdx].m_SyncSignal = SubWorld[0].m_dwCurrentTime;
	}
}

void KProtocolProcess::NetCommandHurt(BYTE* pMsg)
{
	NPC_HURT_SYNC*	pSync = (NPC_HURT_SYNC *)pMsg;
	
	int nIdx = NpcSet.SearchID(pSync->ID);
	if (nIdx > 0)
	{
		//Npc[nIdx].SendCommand(do_hurt, pSync->nFrames, pSync->nX, pSync->nY);
		Npc[nIdx].ProcNetCommand(do_hurt, pSync->nFrames, pSync->nX, pSync->nY);
		Npc[nIdx].m_SyncSignal = SubWorld[0].m_dwCurrentTime;
	}
}

void KProtocolProcess::NetCommandRemoveNpc(BYTE* pMsg)
{
	DWORD	dwNpcID;
	dwNpcID = *(DWORD *)&pMsg[1];
	int nIdx = NpcSet.SearchID(dwNpcID);

	if (Player[CLIENT_PLAYER_INDEX].ConformIdx(nIdx))
	{
		if (Npc[nIdx].m_RegionIndex >= 0)
		{
			// zroc change
			SubWorld[0].m_Region[Npc[nIdx].m_RegionIndex].RemoveNpc(nIdx);
			SubWorld[0].m_Region[Npc[nIdx].m_RegionIndex].DecRef(Npc[nIdx].m_MapX, Npc[nIdx].m_MapY, obj_npc);
//			SubWorld[Npc[nIdx].m_SubWorldIndex].m_Region[Npc[nIdx].m_RegionIndex].RemoveNpc(nIdx);
		}
		NpcSet.Remove(nIdx);
	}
}

void KProtocolProcess::NetCommandRun(BYTE* pMsg)
{
	DWORD	dwNpcID;
	DWORD	MapX, MapY;
	
	dwNpcID = *(DWORD *)&pMsg[1];
	MapX = *(int *)&pMsg[5];
	MapY = *(int *)&pMsg[9];
	int nIdx = NpcSet.SearchID(dwNpcID);
	if (Player[CLIENT_PLAYER_INDEX].ConformIdx(nIdx))
	{
		Npc[nIdx].SendCommand(do_run, MapX, MapY);
		Npc[nIdx].m_SyncSignal = SubWorld[0].m_dwCurrentTime;
	}
}

void KProtocolProcess::NetCommandSit(BYTE* pMsg)
{
	NPC_SIT_SYNC*	pSitSync;

	pSitSync = (NPC_SIT_SYNC *)pMsg;

	int nIdx = NpcSet.SearchID(pSitSync->ID);

	if (Player[CLIENT_PLAYER_INDEX].ConformIdx(nIdx))
	{
		Npc[nIdx].SendCommand(do_sit);
		Npc[nIdx].m_SyncSignal = SubWorld[0].m_dwCurrentTime;
	}
}

void KProtocolProcess::OpenSaleBox(BYTE* pMsg)
{
	SALE_BOX_SYNC* pSale = (SALE_BOX_SYNC *)pMsg;

	BuySell.OpenSale(pSale->nShopIndex,pSale->nSlect);
}

void KProtocolProcess::OpenStoreBox(BYTE* pMsg)
{
	CoreDataChanged(GDCNI_OPEN_STORE_BOX, NULL, NULL);
}

void KProtocolProcess::PlayerRevive(BYTE* pMsg)
{
	NPC_REVIVE_SYNC* pSync = (NPC_REVIVE_SYNC*)pMsg;

	int nIdx = NpcSet.SearchID(pSync->ID);
	if (nIdx > 0)
	{
		if (!Npc[nIdx].IsPlayer() && pSync->Type == REMOTE_REVIVE_TYPE)
		{
			SubWorld[0].m_WorldMessage.Send(GWM_NPC_DEL, nIdx);
			return;
		}
		else
		{
			Npc[nIdx].ProcNetCommand(do_revive);
		}
	}
}




void KProtocolProcess::PlayerUpdateShop(BYTE* pMsg)
{
	NPC_UPDATE_SHOP* pSync = (NPC_UPDATE_SHOP*)pMsg;

	int nIdx = NpcSet.SearchID(pSync->ID);
	if (nIdx > 0 && nIdx != Player[CLIENT_PLAYER_INDEX].m_nIndex)
	{


	Npc[nIdx].m_dwTimeOpenShop = 0;


	memset(Npc[nIdx].m_szNameOpenShop, 0, sizeof(Npc[nIdx].m_szNameOpenShop));

	Npc[nIdx].m_dwTimeOpenShop = pSync->m_dwTime;

	if (pSync->szName[0])
	{
	memcpy(Npc[nIdx].m_szNameOpenShop, pSync->szName, sizeof(Npc[nIdx].m_szNameOpenShop));
    }
		

	}

}












void KProtocolProcess::SysNcPosMin(BYTE* pMsg)
{

	NPC_POS_SYNC* NpcSync = (NPC_POS_SYNC *)pMsg;



	if (NpcSync->Doing == do_revive || NpcSync->Doing == do_death)
		return;

//	int nORegion, nOMapX, nOMapY, nOOffX, nOOffY;
	
	int nIdx = NpcSet.SearchID(NpcSync->ID);
	if (nIdx)
	{

		int nRegion, nMapX, nMapY, nOffX, nOffY;
		SubWorld[0].Mps2Map(NpcSync->MapX, NpcSync->MapY, &nRegion, &nMapX, &nMapY, &nOffX, &nOffY);

		Npc[nIdx].m_sSyncPos.m_dwRegionID = SubWorld[0].m_Region[nRegion].m_RegionID;
		Npc[nIdx].m_sSyncPos.m_nMapX = nMapX;
		Npc[nIdx].m_sSyncPos.m_nMapY = nMapY;
		Npc[nIdx].m_sSyncPos.m_nOffX = nOffX;
		Npc[nIdx].m_sSyncPos.m_nOffY = nOffY;
		Npc[nIdx].m_sSyncPos.m_nDoing = NpcSync->Doing;

		if (Npc[nIdx].m_RegionIndex == -1)	
		{
			if (nRegion == -1)
			{		
				return;
			}
			else
			{
				Npc[nIdx].m_MapX = nMapX;
				Npc[nIdx].m_MapY = nMapY;
				Npc[nIdx].m_OffX = nOffX;
				Npc[nIdx].m_OffY = nOffY;
				Npc[nIdx].m_RegionIndex = nRegion;
				Npc[nIdx].m_dwRegionID = SubWorld[0].m_Region[nRegion].m_RegionID;
				SubWorld[0].m_Region[nRegion].AddNpc(nIdx);
				SubWorld[0].m_Region[nRegion].AddRef(Npc[nIdx].m_MapX, Npc[nIdx].m_MapY, obj_npc);
			}
			
			if (NpcSync->Doing == do_stand)
				Npc[nIdx].ProcNetCommand(do_stand);
		}
		else
		{
		
				SubWorld[0].m_Region[Npc[nIdx].m_RegionIndex].RemoveNpc(nIdx);
				SubWorld[0].m_Region[Npc[nIdx].m_RegionIndex].DecRef(Npc[nIdx].m_MapX, Npc[nIdx].m_MapY, obj_npc);
				Npc[nIdx].m_MapX = nMapX;
				Npc[nIdx].m_MapY = nMapY;
				Npc[nIdx].m_OffX = nOffX;
				Npc[nIdx].m_OffY = nOffY;
				Npc[nIdx].m_RegionIndex = nRegion;
                
				if (nRegion >= 0)
				{
					SubWorld[0].m_Region[nRegion].AddNpc(nIdx);
					Npc[nIdx].m_dwRegionID = SubWorld[0].m_Region[nRegion].m_RegionID;
					SubWorld[0].m_Region[nRegion].AddRef(Npc[nIdx].m_MapX, Npc[nIdx].m_MapY, obj_npc);


			
				}

		}

	
	}


}












void KProtocolProcess::SysItemBuyShop(BYTE* pMsg)
{
	ITEM_BUY_SHOP_SYNC* pSync = (ITEM_BUY_SHOP_SYNC*)pMsg;




	int pnMagicParam[6];

	for (int j = 0; j < 6; j++)
	{
		pnMagicParam[j] = pSync->m_MagicLevel[j];
	}

	

	int nIndex = ItemSet.Add(
		pSync->m_Genre,
		pSync->m_Series,
		pSync->m_Level,
		pSync->m_Luck,
		pSync->m_Detail,
		pSync->m_Particur,
		pnMagicParam,
		pSync->m_Version,
		pSync->m_RandomSeed);

	if (nIndex > 0)
	{
		Item[nIndex].SetID(pSync->m_ID);
		Item[nIndex].SetDurability((short)pSync->m_Durability);
		Item[nIndex].SetNgoaiTrang(pSync->iNgoaiTrang);
		Item[nIndex].m_nPriceShop = pSync->m_nPriceShop;
		Player[CLIENT_PLAYER_INDEX].m_ItemList.Add(nIndex, pSync->m_btPlace, pSync->m_btX, pSync->m_btY);
	}




}


void KProtocolProcess::SysListItemBuyShop(BYTE* pMsg)
{
	BOX_BUY_SHOP_SYNC* pSync = (BOX_BUY_SHOP_SYNC*)pMsg;

	Player[CLIENT_PLAYER_INDEX].m_dwTimeBuyShop = pSync->m_nTime;
	Player[CLIENT_PLAYER_INDEX].m_ItemList.RemoveAllInOneRoom(room_buyshop);
	CoreDataChanged(GDCNI_OPEN_BUY_SHOP_ITEM, (unsigned int)&pSync->m_szName,0);
		

}


void KProtocolProcess::PlayerUpdateTong(BYTE* pMsg)
{
	NPC_UPDATE_TONG* pSync = (NPC_UPDATE_TONG*)pMsg;

	int nIdx = NpcSet.SearchID(pSync->ID);
	if (nIdx > 0 && nIdx != Player[CLIENT_PLAYER_INDEX].m_nIndex)
	{

	memset(Npc[nIdx].m_TongName, 0, sizeof(Npc[nIdx].m_TongName));

	if (pSync->m_TongName[0])
	{
	memcpy(Npc[nIdx].m_TongName, pSync->m_TongName, sizeof(pSync->m_TongName));
    }
	
	memset(Npc[nIdx].m_TongRank, 0, sizeof(Npc[nIdx].m_TongRank));

	if (pSync->m_TongRank[0])
	{
	memcpy(Npc[nIdx].m_TongRank, pSync->m_TongRank, sizeof(pSync->m_TongRank));
    }

	}
}



void KProtocolProcess::PlayerOpenStringBox(BYTE* pMsg)
{
	PLAYER_OPEN_STRING_BOX* pSync = (PLAYER_OPEN_STRING_BOX*)pMsg;

	CoreDataChanged(GDCNI_OPEN_STRING_BOX, (unsigned int)pSync->m_ID, (unsigned int)&pSync->m_Title);
}


void KProtocolProcess::PlayerOpenCheckItemBox(BYTE* pMsg)
{
	PLAYER_OPEN_CHECK_ITEM_BOX* pSync = (PLAYER_OPEN_CHECK_ITEM_BOX*)pMsg;

	CoreDataChanged(GDCNI_OPEN_CHECK_ITEM_BOX, (unsigned int)&pSync->m_TitleBox,(unsigned int)&pSync->m_TitleTex);
}


void KProtocolProcess::PlayerOpenSlectCheckBox(BYTE* pMsg)
{
	PLAYER_OPEN_SLECT_CHECK_BOX* pSync = (PLAYER_OPEN_SLECT_CHECK_BOX*)pMsg;

	CoreDataChanged(GDCNI_OPEN_SLECT_CHECK_BOX, (unsigned int)&pSync->m_szId,0);
}






void KProtocolProcess::PlayerBlockBox(BYTE* pMsg)
{
	PLAYER_BLOCK_BOX* pSync = (PLAYER_BLOCK_BOX*)pMsg;

	Player[CLIENT_PLAYER_INDEX].m_bBlock = pSync->m_bBlock;

	
	CoreDataChanged(GDCNI_OPEN_LOCK_BOX_ITEM, 0,0);
}







void KProtocolProcess::PlayerOpenPGBoxItem(BYTE* pMsg)
{
	PLAYER_OPEN_PG_BOX_ITEM* pSync = (PLAYER_OPEN_PG_BOX_ITEM*)pMsg;

	CoreDataChanged(GDCNI_OPEN_PG_BOX_ITEM, 0,0);
}





void KProtocolProcess::PlayerOpenNewPWBoxItem(BYTE* pMsg)
{
	PLAYER_OPEN_NEW_PW_BOX_ITEM* pSync = (PLAYER_OPEN_NEW_PW_BOX_ITEM*)pMsg;

	CoreDataChanged(GDCNI_OPEN_NEW_PW_BOX_ITEM, 0,0);
}








void KProtocolProcess::PlayerSendTopTKNew(BYTE* pMsg)
{
	PLAYER_SEND_TOP_TK_NEW* pSync = (PLAYER_SEND_TOP_TK_NEW*)pMsg;

	

	KUiTopTKNewData pDataTop;

	pDataTop.m_Phe = pSync->m_DataTop.m_Phe;
	pDataTop.m_Soluong = pSync->m_DataTop.m_Soluong;
	pDataTop.m_ThoiGian = pSync->m_DataTop.m_ThoiGian;
	pDataTop.m_NguoiChoi = pSync->m_DataTop.m_NguoiChoi;
	pDataTop.m_TNguoiChoi = pSync->m_DataTop.m_TNguoiChoi;
	pDataTop.m_NPC = pSync->m_DataTop.m_NPC;
	pDataTop.m_TNPC = pSync->m_DataTop.m_TNPC;
	pDataTop.m_LienTramHT = pSync->m_DataTop.m_LienTramHT;
	pDataTop.m_TLienTramHT = pSync->m_DataTop.m_TLienTramHT;
	pDataTop.m_BauVat = pSync->m_DataTop.m_BauVat;
	pDataTop.m_TBauVat = pSync->m_DataTop.m_TBauVat;
	pDataTop.m_TichLuy = pSync->m_DataTop.m_TichLuy;
	pDataTop.m_TuVong = pSync->m_DataTop.m_TuVong;
	pDataTop.m_LienTram = pSync->m_DataTop.m_LienTram;

	g_StrCpyLen(pDataTop.m_szTop1Name, pSync->m_DataTop.m_szTop1Name, sizeof(pDataTop.m_szTop1Name));
	pDataTop.m_Top1Phe = pSync->m_DataTop.m_Top1Phe;
	pDataTop.m_Top1TichLuy = pSync->m_DataTop.m_Top1TichLuy;
	pDataTop.m_Top1NguoiChoi = pSync->m_DataTop.m_Top1NguoiChoi;
	pDataTop.m_Top1NPC = pSync->m_DataTop.m_Top1NPC;
	pDataTop.m_Top1TuVong = pSync->m_DataTop.m_Top1TuVong;
	pDataTop.m_Top1LienTram = pSync->m_DataTop.m_Top1LienTram;
	pDataTop.m_Top1BaoVat = pSync->m_DataTop.m_Top1BaoVat;

	g_StrCpyLen(pDataTop.m_szTop2Name, pSync->m_DataTop.m_szTop2Name, sizeof(pDataTop.m_szTop2Name));
	pDataTop.m_Top2Phe = pSync->m_DataTop.m_Top2Phe;
	pDataTop.m_Top2TichLuy = pSync->m_DataTop.m_Top2TichLuy;
	pDataTop.m_Top2NguoiChoi = pSync->m_DataTop.m_Top2NguoiChoi;
	pDataTop.m_Top2NPC = pSync->m_DataTop.m_Top2NPC;
	pDataTop.m_Top2TuVong = pSync->m_DataTop.m_Top2TuVong;
	pDataTop.m_Top2LienTram = pSync->m_DataTop.m_Top2LienTram;
	pDataTop.m_Top2BaoVat = pSync->m_DataTop.m_Top2BaoVat;

	g_StrCpyLen(pDataTop.m_szTop3Name, pSync->m_DataTop.m_szTop3Name, sizeof(pDataTop.m_szTop3Name));
	pDataTop.m_Top3Phe = pSync->m_DataTop.m_Top3Phe;
	pDataTop.m_Top3TichLuy = pSync->m_DataTop.m_Top3TichLuy;
	pDataTop.m_Top3NguoiChoi = pSync->m_DataTop.m_Top3NguoiChoi;
	pDataTop.m_Top3NPC = pSync->m_DataTop.m_Top3NPC;
	pDataTop.m_Top3TuVong = pSync->m_DataTop.m_Top3TuVong;
	pDataTop.m_Top3LienTram = pSync->m_DataTop.m_Top3LienTram;
	pDataTop.m_Top3BaoVat = pSync->m_DataTop.m_Top3BaoVat;

	g_StrCpyLen(pDataTop.m_szTop4Name, pSync->m_DataTop.m_szTop4Name, sizeof(pDataTop.m_szTop4Name));
	pDataTop.m_Top4Phe = pSync->m_DataTop.m_Top4Phe;
	pDataTop.m_Top4TichLuy = pSync->m_DataTop.m_Top4TichLuy;
	pDataTop.m_Top4NguoiChoi = pSync->m_DataTop.m_Top4NguoiChoi;
	pDataTop.m_Top4NPC = pSync->m_DataTop.m_Top4NPC;
	pDataTop.m_Top4TuVong = pSync->m_DataTop.m_Top4TuVong;
	pDataTop.m_Top4LienTram = pSync->m_DataTop.m_Top4LienTram;
	pDataTop.m_Top4BaoVat = pSync->m_DataTop.m_Top4BaoVat;

	g_StrCpyLen(pDataTop.m_szTop5Name, pSync->m_DataTop.m_szTop5Name, sizeof(pDataTop.m_szTop5Name));
	pDataTop.m_Top5Phe = pSync->m_DataTop.m_Top5Phe;
	pDataTop.m_Top5TichLuy = pSync->m_DataTop.m_Top5TichLuy;
	pDataTop.m_Top5NguoiChoi = pSync->m_DataTop.m_Top5NguoiChoi;
	pDataTop.m_Top5NPC = pSync->m_DataTop.m_Top5NPC;
	pDataTop.m_Top5TuVong = pSync->m_DataTop.m_Top5TuVong;
	pDataTop.m_Top5LienTram = pSync->m_DataTop.m_Top5LienTram;
	pDataTop.m_Top5BaoVat = pSync->m_DataTop.m_Top5BaoVat;

	g_StrCpyLen(pDataTop.m_szTop6Name, pSync->m_DataTop.m_szTop6Name, sizeof(pDataTop.m_szTop6Name));
	pDataTop.m_Top6Phe = pSync->m_DataTop.m_Top6Phe;
	pDataTop.m_Top6TichLuy = pSync->m_DataTop.m_Top6TichLuy;
	pDataTop.m_Top6NguoiChoi = pSync->m_DataTop.m_Top6NguoiChoi;
	pDataTop.m_Top6NPC = pSync->m_DataTop.m_Top6NPC;
	pDataTop.m_Top6TuVong = pSync->m_DataTop.m_Top6TuVong;
	pDataTop.m_Top6LienTram = pSync->m_DataTop.m_Top6LienTram;
	pDataTop.m_Top6BaoVat = pSync->m_DataTop.m_Top6BaoVat;

	g_StrCpyLen(pDataTop.m_szTop7Name, pSync->m_DataTop.m_szTop7Name, sizeof(pDataTop.m_szTop7Name));
	pDataTop.m_Top7Phe = pSync->m_DataTop.m_Top7Phe;
	pDataTop.m_Top7TichLuy = pSync->m_DataTop.m_Top7TichLuy;
	pDataTop.m_Top7NguoiChoi = pSync->m_DataTop.m_Top7NguoiChoi;
	pDataTop.m_Top7NPC = pSync->m_DataTop.m_Top7NPC;
	pDataTop.m_Top7TuVong = pSync->m_DataTop.m_Top7TuVong;
	pDataTop.m_Top7LienTram = pSync->m_DataTop.m_Top7LienTram;
	pDataTop.m_Top7BaoVat = pSync->m_DataTop.m_Top7BaoVat;

	g_StrCpyLen(pDataTop.m_szTop8Name, pSync->m_DataTop.m_szTop8Name, sizeof(pDataTop.m_szTop8Name));
	pDataTop.m_Top8Phe = pSync->m_DataTop.m_Top8Phe;
	pDataTop.m_Top8TichLuy = pSync->m_DataTop.m_Top8TichLuy;
	pDataTop.m_Top8NguoiChoi = pSync->m_DataTop.m_Top8NguoiChoi;
	pDataTop.m_Top8NPC = pSync->m_DataTop.m_Top8NPC;
	pDataTop.m_Top8TuVong = pSync->m_DataTop.m_Top8TuVong;
	pDataTop.m_Top8LienTram = pSync->m_DataTop.m_Top8LienTram;
	pDataTop.m_Top8BaoVat = pSync->m_DataTop.m_Top8BaoVat;

	g_StrCpyLen(pDataTop.m_szTop9Name, pSync->m_DataTop.m_szTop9Name, sizeof(pDataTop.m_szTop9Name));
	pDataTop.m_Top9Phe = pSync->m_DataTop.m_Top9Phe;
	pDataTop.m_Top9TichLuy = pSync->m_DataTop.m_Top9TichLuy;
	pDataTop.m_Top9NguoiChoi = pSync->m_DataTop.m_Top9NguoiChoi;
	pDataTop.m_Top9NPC = pSync->m_DataTop.m_Top9NPC;
	pDataTop.m_Top9TuVong = pSync->m_DataTop.m_Top9TuVong;
	pDataTop.m_Top9LienTram = pSync->m_DataTop.m_Top9LienTram;
	pDataTop.m_Top9BaoVat = pSync->m_DataTop.m_Top9BaoVat;

	g_StrCpyLen(pDataTop.m_szTop10Name, pSync->m_DataTop.m_szTop10Name, sizeof(pDataTop.m_szTop10Name));
	pDataTop.m_Top10Phe = pSync->m_DataTop.m_Top10Phe;
	pDataTop.m_Top10TichLuy = pSync->m_DataTop.m_Top10TichLuy;
	pDataTop.m_Top10NguoiChoi = pSync->m_DataTop.m_Top10NguoiChoi;
	pDataTop.m_Top10NPC = pSync->m_DataTop.m_Top10NPC;
	pDataTop.m_Top10TuVong = pSync->m_DataTop.m_Top10TuVong;
	pDataTop.m_Top10LienTram = pSync->m_DataTop.m_Top10LienTram;
	pDataTop.m_Top10BaoVat = pSync->m_DataTop.m_Top10BaoVat;




	CoreDataChanged(GDCNI_SEND_TOP_TK_NEW_ITEM, (unsigned int)&pDataTop,0);
}










void KProtocolProcess::PlayerTKSync(BYTE* pMsg)
{
	PLAYER_TONGKIM_SYNC* pInfo = (PLAYER_TONGKIM_SYNC *)pMsg;

	Player[CLIENT_PLAYER_INDEX].m_cTask.SetSaveVal(pInfo->nValueIndex, pInfo->nValue);
	//if(pInfo->bCheck)
	CoreDataChanged(GDCNI_QUERY_TONGKIM, pInfo->nValue,0);
}


void KProtocolProcess::S2CSyncItemParam(BYTE* pMsg)
{
	S2C_SYNC_ITEM_PARAM* pInfo = (S2C_SYNC_ITEM_PARAM *)pMsg;
	int nIdx = ItemSet.SearchID(pInfo->m_ID);
	if ( nIdx > 0 && nIdx < MAX_ITEM )
	{
		Item[nIdx].SetTime(pInfo->m_Time.bYear,pInfo->m_Time.bMonth,pInfo->m_Time.bDay,pInfo->m_Time.bHour);
	}
}

void KProtocolProcess::RequestNpcFail(BYTE* pMsg)
{
	NPC_REQUEST_FAIL* pNpcSync = (NPC_REQUEST_FAIL *)pMsg;

	if (NpcSet.IsNpcRequestExist(pNpcSync->ID))
		NpcSet.RemoveNpcRequest(pNpcSync->ID);	
}

void KProtocolProcess::NetCommandSkill(BYTE* pMsg)
{
	DWORD	dwNpcID;
	int		nSkillID, nSkillLevel;
	int		MapX, MapY;
	
	dwNpcID = *(DWORD *)&pMsg[1];
	nSkillID = *(int *)&pMsg[5];
	nSkillLevel = *(int *)&pMsg[9];
	MapX = *(int *)&pMsg[13];
	MapY = *(int *)&pMsg[17];
	
	if (MapY < 0)
		return ;

	//��ָ��ĳ��Ŀ��ʱ(MapX == -1),MapYΪĿ���NpcdwID����Ҫת���ɱ��ص�NpcIndex����
	if (MapX < 0)
	{
		if (MapX != -1)
			return;
	
		MapY = NpcSet.SearchID(MapY);
		if (MapY == 0)
			return;

		if (Npc[MapY].m_RegionIndex < 0)
			return;

	}
	
	int nIdx = NpcSet.SearchID(dwNpcID);
	
	if (nIdx <= 0) 
		return;

	if (Player[CLIENT_PLAYER_INDEX].ConformIdx(nIdx))
	{
		Npc[nIdx].m_SkillList.SetSkillLevel(nSkillID, nSkillLevel);
		Npc[nIdx].SendCommand(do_skill, nSkillID, MapX, MapY);
		Npc[nIdx].m_SyncSignal = SubWorld[0].m_dwCurrentTime;
	}
}

void KProtocolProcess::NetCommandWalk(BYTE* pMsg)
{
	DWORD	dwNpcID;
	DWORD	MapX, MapY;
	
	dwNpcID = *(DWORD *)&pMsg[1];
	MapX = *(int *)&pMsg[5];
	MapY = *(int *)&pMsg[9];
	int nIdx = NpcSet.SearchID(dwNpcID);


if (nIdx && Npc[nIdx].m_MadnessState.nTime > 0)
{
Npc[nIdx].Madnessto(MapX,MapY);
Npc[nIdx].m_SyncSignal = SubWorld[0].m_dwCurrentTime;
}
else if (Player[CLIENT_PLAYER_INDEX].ConformIdx(nIdx))
{
Npc[nIdx].SendCommand(do_walk, MapX, MapY);
Npc[nIdx].m_SyncSignal = SubWorld[0].m_dwCurrentTime;
}

}

//-------------------------------------------------------------------------
//	���ܣ��յ�������֪ͨ���鴴��ʧ��
//-------------------------------------------------------------------------
void KProtocolProcess::s2cApplyCreateTeamFalse(BYTE* pMsg)
{
	PLAYER_SEND_CREATE_TEAM_FALSE *pCreateFalse = (PLAYER_SEND_CREATE_TEAM_FALSE*)pMsg;
	KSystemMessage	sMsg;

	switch (pCreateFalse->m_btErrorID)
	{
	// �Ѿ��ڶ����У�˵���ͻ��˶��������д����������»�ö�������
	case Team_Create_Error_InTeam:
		Player[CLIENT_PLAYER_INDEX].ApplySelfTeamInfo();
		break;

	// ��ǰ���ڲ������״̬
	case Team_Create_Error_CannotCreate:
		sprintf(sMsg.szMessage, MSG_TEAM_CANNOT_CREATE);
		sMsg.eType = SMT_NORMAL;
		sMsg.byConfirmType = SMCT_NONE;
		sMsg.byPriority = 0;
		sMsg.byParamSize = 0;
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		break;


	default:
		sprintf(sMsg.szMessage, MSG_TEAM_CREATE_FAIL);
		sMsg.eType = SMT_NORMAL;
		sMsg.byConfirmType = SMCT_NONE;
		sMsg.byPriority = 0;
		sMsg.byParamSize = 0;
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		break;
	}
}

//-------------------------------------------------------------------------
//	���ܣ��յ�������֪ͨ��ѯĳ������������ʧ��
//-------------------------------------------------------------------------
void KProtocolProcess::s2cApplyTeamInfoFalse(BYTE* pMsg)
{
	// ������ʾ�����ѯʧ��(not end)
}

//-------------------------------------------------------------------------
//	���ܣ�����һ������
//-------------------------------------------------------------------------
void KProtocolProcess::s2cChatAddFriend(BYTE* pMsg)
{
	CHAT_ADD_FRIEND_SYNC	*pFriend = (CHAT_ADD_FRIEND_SYNC*)pMsg;

	Player[CLIENT_PLAYER_INDEX].m_cChat.AddOne(pFriend->m_dwID, pFriend->m_nIdx, pFriend->m_szName, 0);
	Player[CLIENT_PLAYER_INDEX].m_cChat.SaveTeamInfo(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name);

	KSystemMessage	sMsg;
	KUiPlayerItem	sPlayer;

	memset(&sPlayer, 0, sizeof(KUiPlayerItem));
	strcpy(sPlayer.Name, pFriend->m_szName);
	sPlayer.uId = pFriend->m_dwID;
	sPlayer.nIndex = pFriend->m_nIdx;

	sprintf(sMsg.szMessage, MSG_CHAT_ADD_FRIEND_SUCCESS, pFriend->m_szName);
	sMsg.eType = SMT_FRIEND;
	sMsg.byConfirmType = SMCT_UI_INTERVIEW;
	sMsg.byPriority = 2;
	sMsg.byParamSize = sizeof(KUiPlayerItem);
	CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, (int)&sPlayer);
}

//-------------------------------------------------------------------------
//	���ܣ����Ӻ���ʧ��
//-------------------------------------------------------------------------
void KProtocolProcess::s2cChatAddFriendFail(BYTE* pMsg)
{
	CHAT_ADD_FRIEND_FAIL_SYNC	*pFail = (CHAT_ADD_FRIEND_FAIL_SYNC*)pMsg;

	char	szName[32];
	CChatApplyListNode	*pList;
	pList = (CChatApplyListNode*)Player[CLIENT_PLAYER_INDEX].m_cChat.m_cApplyAddList.GetHead();
	while (pList)
	{
		if (pList->m_nPlayerIdx == pFail->m_nTargetPlayerIdx)
		{
			g_StrCpy(szName, pList->m_szName);

			// ֪ͨ��������ĳ��Ϊ���Ѳ���ʧ��
			KSystemMessage	sMsg;
			sprintf(sMsg.szMessage, MSG_CHAT_ADD_FRIEND_FAIL, szName);
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);

			return;
		}
		pList = (CChatApplyListNode*)pList->GetNext();
	}	
}

//-------------------------------------------------------------------------
//	���ܣ��յ����������������ĳ��������ѵ����ݣ������֣�
//-------------------------------------------------------------------------
void KProtocolProcess::s2cChatAddOneFriend(BYTE* pMsg)
{
	CHAT_ONE_FRIEND_DATA_SYNC	*pData = (CHAT_ONE_FRIEND_DATA_SYNC*)pMsg;
	int		nTeamNo;

	nTeamNo = Player[CLIENT_PLAYER_INDEX].m_cChat.CheckTeamNo(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name, pData->m_szName);
	if (nTeamNo < 0)
		nTeamNo = 0;
	Player[CLIENT_PLAYER_INDEX].m_cChat.AddOne(pData->m_dwID, pData->m_nPlayerIdx, pData->m_szName, nTeamNo);
	Player[CLIENT_PLAYER_INDEX].m_cChat.SaveTeamInfo(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name);

	// ֪ͨ����õ��³�Ա not end
}

//-------------------------------------------------------------------------
//	���ܣ��������Ҿܾ�����
//-------------------------------------------------------------------------
void KProtocolProcess::s2cChatBeRefusedAddFriend(BYTE* pMsg)
{
	CHAT_REFUSE_FRIEND_SYNC	*pRefuse = (CHAT_REFUSE_FRIEND_SYNC*)pMsg;
	char	szName[64];

	memset(szName, 0, sizeof(szName));
	memcpy(szName, pRefuse->m_szName, pRefuse->m_wLength + 1 + sizeof(pRefuse->m_szName) - sizeof(CHAT_REFUSE_FRIEND_SYNC));

	// ֪ͨ����pRefuse->m_szName�ܾ������Ľ�������
	KSystemMessage	sMsg;

	sprintf(sMsg.szMessage, MSG_CHAT_REFUSE_FRIEND, szName);
	sMsg.eType = SMT_FRIEND;
	sMsg.byConfirmType = SMCT_CLICK;
	sMsg.byPriority = 1;
	sMsg.byParamSize = 0;
	CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
}

//-------------------------------------------------------------------------
//	���ܣ��õ�֪ͨ��������
//-------------------------------------------------------------------------
void KProtocolProcess::s2cChatFriendOnline(BYTE* pMsg)
{
	CHAT_FRIEND_ONLINE_SYNC	*pFriend = (CHAT_FRIEND_ONLINE_SYNC*)pMsg;

	Player[CLIENT_PLAYER_INDEX].ChatFriendOnLine(pFriend->m_dwID, pFriend->m_nPlayerIdx);
}

//-------------------------------------------------------------------------
// �յ�������ת���ı�������������ѵ�����
//-------------------------------------------------------------------------
void KProtocolProcess::s2cChatGetApplyAddFriend(BYTE* pMsg)
{
	CHAT_APPLY_ADD_FRIEND_SYNC	*pApply = (CHAT_APPLY_ADD_FRIEND_SYNC*)pMsg;

	if ( Player[CLIENT_PLAYER_INDEX].m_cChat.CheckIsFriend(pApply->m_nSrcPlayerIdx) )
		return;

	CChatApplyListNode	*pNode = NULL;
	pNode = new CChatApplyListNode;
	if (pNode == NULL)
		return;
	pNode->m_nPlayerIdx = pApply->m_nSrcPlayerIdx;
	strcpy(pNode->m_szName, pApply->m_szSourceName);
	strcpy(pNode->m_szInfo, pApply->m_szInfo);

	Player[CLIENT_PLAYER_INDEX].m_cChat.m_cApplyAddList.AddTail(pNode);

	// ֪ͨ�����������������������
	KSystemMessage	sMsg;
	KUiPlayerItem	sPlayer;

	strcpy(sPlayer.Name, pApply->m_szSourceName);
	sPlayer.nIndex = pApply->m_nSrcPlayerIdx;
	sPlayer.uId = 0;
	sPlayer.nData = 0;

	sprintf(sMsg.szMessage, MSG_CHAT_GET_FRIEND_APPLY, pNode->m_szName);
	sMsg.eType = SMT_FRIEND;
	sMsg.byConfirmType = SMCT_UI_FRIEND_INVITE;
	sMsg.byPriority = 3;
	sMsg.byParamSize = sizeof(KUiPlayerItem);
	CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, (int)&sPlayer);
}

//-------------------------------------------------------------------------
//	���ܣ��յ���������������ҵ�¼ʱ�������������ݣ������֣�
//-------------------------------------------------------------------------
void KProtocolProcess::s2cChatLoginFriendName(BYTE* pMsg)
{
	CHAT_LOGIN_FRIEND_NAME_SYNC	*pFriend = (CHAT_LOGIN_FRIEND_NAME_SYNC*)pMsg;
	int		nTeamNo;
	char	szName[64];

	memset(szName, 0, sizeof(szName));
	memcpy(szName, pFriend->m_szName, pFriend->m_wLength + 1 + sizeof(pFriend->m_szName) - sizeof(CHAT_LOGIN_FRIEND_NAME_SYNC));
	nTeamNo = Player[CLIENT_PLAYER_INDEX].m_cChat.CheckTeamNo(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name, szName);
	if (nTeamNo < 0)
		nTeamNo = 0;
	Player[CLIENT_PLAYER_INDEX].m_cChat.AddOne(pFriend->m_dwID, pFriend->m_nPlayerIdx, szName, nTeamNo);
}

//-------------------------------------------------------------------------
//	���ܣ��յ���������������ҵ�¼ʱ�������������ݣ��������֣�
//-------------------------------------------------------------------------
void KProtocolProcess::s2cChatLoginFriendNoName(BYTE* pMsg)
{
	CHAT_LOGIN_FRIEND_NONAME_SYNC	*pFriend = (CHAT_LOGIN_FRIEND_NONAME_SYNC*)pMsg;
	char	szName[32];
	int		nTeamNo;

	szName[0] = 0;
	nTeamNo = Player[CLIENT_PLAYER_INDEX].m_cChat.CheckTeamNo(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name, NULL);
	if (nTeamNo < 0)
	{
		if (Player[CLIENT_PLAYER_INDEX].m_cChat.m_nLoginGetFirstOneFriendFlag == 0)		// �������·������к������ݣ�������
		{
			Player[CLIENT_PLAYER_INDEX].m_cChat.m_nLoginGetFirstOneFriendFlag = 1;

			CHAT_APPLY_RESEND_ALL_FRIEND_NAME_COMMAND	sApply;
			sApply.ProtocolType = c2s_chatapplyresendallfriendname;

			if (g_pClient)
				g_pClient->SendPackToServer((BYTE*)&sApply, sizeof(CHAT_APPLY_RESEND_ALL_FRIEND_NAME_COMMAND));
		}
		else												// �������·�������������ݣ�������
		{
			CHAT_APPLY_SEND_ONE_FRIEND_NAME_COMMAND	sApply;
			sApply.ProtocolType = c2s_chatapplysendonefriendname;
			sApply.m_dwID = pFriend->m_dwID;

			if (g_pClient)
				g_pClient->SendPackToServer((BYTE*)&sApply, sizeof(CHAT_APPLY_SEND_ONE_FRIEND_NAME_COMMAND));
		}
		return;
	}
	Player[CLIENT_PLAYER_INDEX].m_cChat.AddOne(pFriend->m_dwID, pFriend->m_nPlayerIdx, szName, nTeamNo);
	Player[CLIENT_PLAYER_INDEX].m_cChat.SaveTeamInfo(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name);
	Player[CLIENT_PLAYER_INDEX].m_cChat.m_nLoginGetFirstOneFriendFlag = 1;

}

//-------------------------------------------------------------------------
//	���ܣ��յ�������֪ͨ����һ֧����
//-------------------------------------------------------------------------
void KProtocolProcess::s2cCreateTeam(BYTE* pMsg)
{
	PLAYER_SEND_CREATE_TEAM_SUCCESS	*pCreateSuccess = (PLAYER_SEND_CREATE_TEAM_SUCCESS*)pMsg;

	g_Team[0].CreateTeam(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_dwID, Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name, Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Level, pCreateSuccess->nTeamServerID);
	Player[CLIENT_PLAYER_INDEX].m_cTeam.Release();
	Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag = 1;
	Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFigure = TEAM_CAPTAIN;
	Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nApplyCaptainID = 0;
	Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].RestoreCurrentCamp();

	KSystemMessage	sMsg;
	sprintf(sMsg.szMessage, MSG_TEAM_CREATE);
	sMsg.eType = SMT_NORMAL;
	sMsg.byConfirmType = SMCT_NONE;
	sMsg.byPriority = 0;
	sMsg.byParamSize = 0;
	CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);

	sprintf(sMsg.szMessage, MSG_TEAM_CREATE);
	sMsg.eType = SMT_TEAM;
	sMsg.byConfirmType = SMCT_CLICK;
	sMsg.byPriority = 1;
	sMsg.byParamSize = 0;
	CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);

	Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SetInstantSpr(enumINSTANT_STATE_CREATE_TEAM);
}

//-------------------------------------------------------------------------
//	���ܣ��յ�������֪ͨ���ŵ�ǰ���ɼ���ĳ�ȼ�
//-------------------------------------------------------------------------
void KProtocolProcess::s2cFactionSkillOpen(BYTE* pMsg)
{
	PLAYER_FACTION_SKILL_LEVEL	*pSkill = (PLAYER_FACTION_SKILL_LEVEL*)pMsg;

	if (Player[CLIENT_PLAYER_INDEX].m_cFaction.m_nCurFaction < 0 || Player[CLIENT_PLAYER_INDEX].m_cFaction.m_nCurFaction != pSkill->m_btCurFactionID)
	{
		// �������������Ϣ
		Player[CLIENT_PLAYER_INDEX].ApplyFactionData();
		return;
	}

//	for (int i = 0; i < FACTIONS_PRR_SERIES; i++)
//	{
//		if (Player[CLIENT_PLAYER_INDEX].m_cFaction.m_sSkillOpen[i].m_nID == Player[CLIENT_PLAYER_INDEX].m_cFaction.m_nCurFaction)
//		{
//			Player[CLIENT_PLAYER_INDEX].m_cFaction.m_sSkillOpen[i].m_nOpenLevel = pSkill->m_btLevel;
//			Player[CLIENT_PLAYER_INDEX].m_cFaction.OpenCurSkillLevel(Player[CLIENT_PLAYER_INDEX].m_cFaction.m_sSkillOpen[i].m_nOpenLevel, &Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList);
//			break;
//		}
//	}
}

//-------------------------------------------------------------------------
//	���ܣ��յ�������������������Ϣ
//-------------------------------------------------------------------------
void KProtocolProcess::s2cGetChat(BYTE* pMsg)
{
	PLAYER_SEND_CHAT_SYNC	*pChat = (PLAYER_SEND_CHAT_SYNC*)pMsg;

	Player[CLIENT_PLAYER_INDEX].m_cChat.GetChat(pChat);
}

//-------------------------------------------------------------------------
//	���ܣ��յ��������������ĵ�ǰĳ���Եĵ���
//-------------------------------------------------------------------------
void KProtocolProcess::s2cGetCurAttribute(BYTE* pMsg)
{
	Player[CLIENT_PLAYER_INDEX].s2cGetCurAttribute(pMsg);
}

//-------------------------------------------------------------------------
//	���ܣ��յ�������������ͳ��������ֵ
//-------------------------------------------------------------------------
void KProtocolProcess::s2cGetLeadExp(BYTE* pMsg)
{
	DWORD	dwLevel = Player[CLIENT_PLAYER_INDEX].m_dwLeadLevel;

	PLAYER_LEAD_EXP_SYNC	*pLeadExp = (PLAYER_LEAD_EXP_SYNC*)pMsg;
	Player[CLIENT_PLAYER_INDEX].m_dwLeadExp = pLeadExp->m_dwLeadExp;
	Player[CLIENT_PLAYER_INDEX].m_dwLeadLevel = PlayerSet.m_cLeadExp.GetLevel(Player[CLIENT_PLAYER_INDEX].m_dwLeadExp, Player[CLIENT_PLAYER_INDEX].m_dwLeadLevel);
	Player[CLIENT_PLAYER_INDEX].m_dwNextLevelLeadExp = PlayerSet.m_cLeadExp.GetLevelExp(Player[CLIENT_PLAYER_INDEX].m_dwLeadLevel);

	if (dwLevel < Player[CLIENT_PLAYER_INDEX].m_dwLeadLevel)
	{
		KSystemMessage	sMsg;
		sprintf(sMsg.szMessage, MSG_LEADER_LEVEL_UP, Player[CLIENT_PLAYER_INDEX].m_dwLeadLevel);
		sMsg.eType = SMT_PLAYER;
		sMsg.byConfirmType = SMCT_CLICK;
		sMsg.byPriority = 1;
		sMsg.byParamSize = 0;
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
	}
}

//-------------------------------------------------------------------------
//	���ܣ��յ���������������ĳ���ܵ���
//-------------------------------------------------------------------------
void KProtocolProcess::s2cGetSkillLevel(BYTE* pMsg)
{
	int		nSkillIndex;
	PLAYER_SKILL_LEVEL_SYNC	*pSkill = (PLAYER_SKILL_LEVEL_SYNC*)pMsg;
	if (pSkill->m_nSkillLevel > MAX_SKILLLEVEL)
		return;
	nSkillIndex = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.FindSame(pSkill->m_nSkillID);
	Player[CLIENT_PLAYER_INDEX].m_nSkillPoint = pSkill->m_nLeavePoint;
	if ( !nSkillIndex )
	{
		//����ü��ܵĵȼ�Ϊ�㣬��ʾ��ѧ��ֱ����ͻ������
		if (pSkill->m_nSkillLevel == 0)
		{
			if ( Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.Add(pSkill->m_nSkillID, 0) == 0 )
				return;
		}
		else
		{
			if ( Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.Add(pSkill->m_nSkillID, pSkill->m_nSkillLevel) == 0 )
				return;
		}
	}
	else
	{
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.SetLevel(nSkillIndex, pSkill->m_nSkillLevel);
	}

	// ֪ͨ������¼�������
	CoreDataChanged(GDCNI_FIGHT_SKILL_POINT, 0, Player[CLIENT_PLAYER_INDEX].m_nSkillPoint);
	KUiSkillData SkillData;
	SkillData.uGenre = CGOG_SKILL_FIGHT;
	SkillData.uId = pSkill->m_nSkillID;
	SkillData.nLevel = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.GetLevel(pSkill->m_nSkillID);
	SkillData.nCurrentLevel = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.GetCurrentLevel(pSkill->m_nSkillID);


	

	int nSkillPosition = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.GetSkillPosition(pSkill->m_nSkillID);
	if(nSkillPosition < 0) return;
	CoreDataChanged(GDCNI_SKILL_CHANGE, (unsigned int)&SkillData, nSkillPosition);
}

//-------------------------------------------------------------------------
//	���ܣ��յ������������Ķ��ѵȼ��仯֪ͨ
//-------------------------------------------------------------------------
void KProtocolProcess::s2cGetTeammateLevel(BYTE* pMsg)
{
	PLAYER_TEAMMATE_LEVEL_SYNC	*pLevel = (PLAYER_TEAMMATE_LEVEL_SYNC*)pMsg;
	if (!Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag)
	{
		Player[CLIENT_PLAYER_INDEX].ApplySelfTeamInfo();
		return;
	}

	if ((DWORD)g_Team[0].m_nCaptain == pLevel->m_dwTeammateID)
	{
		g_Team[0].m_nMemLevel[0] = (DWORD)pLevel->m_btLevel;
		return;
	}

	for (int i = 0; i < MAX_TEAM_MEMBER; i++)
	{
		if ((DWORD)g_Team[0].m_nMember[i] == pLevel->m_dwTeammateID)
		{
			g_Team[0].m_nMemLevel[i + 1] = (DWORD)pLevel->m_btLevel;
			break;
		}
	}
}

//-------------------------------------------------------------------------
//	���ܣ��յ�������֪ͨ�뿪����
//-------------------------------------------------------------------------
void KProtocolProcess::s2cLeaveFaction(BYTE* pMsg)
{
	Player[CLIENT_PLAYER_INDEX].m_cFaction.LeaveFaction();

	// �ı���Ӫ
	Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SetCamp(camp_free);

	CoreDataChanged(GDCNI_PLAYER_BASE_INFO, 0, 0);
}

//-------------------------------------------------------------------------
//	���ܣ��յ�������֪ͨ�г�Ա�뿪(�����Լ��뿪)
//-------------------------------------------------------------------------
void KProtocolProcess::s2cLeaveTeam(BYTE* pMsg)
{
	if (!Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag)
	{
		Player[CLIENT_PLAYER_INDEX].m_cTeam.UpdateInterface();
		return;
	}

	PLAYER_LEAVE_TEAM	*pLeaveTeam = (PLAYER_LEAVE_TEAM*)pMsg;

	// �Լ��뿪
	if (pLeaveTeam->m_dwNpcID == Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_dwID)
	{
		Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag = 0;
		Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nApplyCaptainID = 0;
		g_Team[0].Release();
		if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].GetMenuState() == PLAYER_MENU_STATE_TEAMOPEN)
			Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SetMenuState(PLAYER_MENU_STATE_NORMAL);
	}
	// �����뿪
	else
	{
		g_Team[0].DeleteMember(pLeaveTeam->m_dwNpcID);
	}

	Player[CLIENT_PLAYER_INDEX].m_cTeam.UpdateInterface();
	return;
}

//-------------------------------------------------------------------------
//	���ܣ��յ�������֪ͨ����
//-------------------------------------------------------------------------
void KProtocolProcess::s2cLevelUp(BYTE* pMsg)
{
	Player[CLIENT_PLAYER_INDEX].s2cLevelUp(pMsg);
}

void KProtocolProcess::s2cMoveItem(BYTE* pMsg)
{
	PLAYER_MOVE_ITEM_SYNC	*pMove = (PLAYER_MOVE_ITEM_SYNC*)pMsg;

	ItemPos		DownPos, UpPos;
	DownPos.nPlace = pMove->m_btDownPos;
	DownPos.nX = pMove->m_btDownX;
	DownPos.nY = pMove->m_btDownY;
	UpPos.nPlace = pMove->m_btUpPos;
	UpPos.nX = pMove->m_btUpX;
	UpPos.nY = pMove->m_btUpY;

	Player[CLIENT_PLAYER_INDEX].m_ItemList.ExchangeItem(&DownPos, &UpPos);
}



void KProtocolProcess::s2cRemoveItem(BYTE* pMsg)
{
	ITEM_REMOVE_SYNC	*pRemove = (ITEM_REMOVE_SYNC*)pMsg;

	int		nIdx;
	nIdx = Player[CLIENT_PLAYER_INDEX].m_ItemList.SearchID(pRemove->m_ID);
	if (nIdx > 0)
	{
		Player[CLIENT_PLAYER_INDEX].m_ItemList.Remove(nIdx);
		Player[CLIENT_PLAYER_INDEX].m_ItemList.UnlockOperation();
	}

	
}

//-------------------------------------------------------------------------
//	���ܣ��յ�������������������Ϣ
//-------------------------------------------------------------------------
void KProtocolProcess::s2cSetFactionData(BYTE* pMsg)
{
	PLAYER_FACTION_DATA	*pData = (PLAYER_FACTION_DATA*)pMsg;

	Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SetCamp(pData->m_btCamp);
	Player[CLIENT_PLAYER_INDEX].m_cFaction.m_nCurFaction = (char)pData->m_btCurFaction;
	Player[CLIENT_PLAYER_INDEX].m_cFaction.m_nFirstAddFaction = (char)pData->m_btFirstFaction;
	Player[CLIENT_PLAYER_INDEX].m_cFaction.m_nAddTimes = pData->m_nAddTimes;

	CoreDataChanged(GDCNI_PLAYER_BASE_INFO, 0, 0);

//	Player[CLIENT_PLAYER_INDEX].m_cFaction.m_sSkillOpen[0].m_nOpenLevel = pData->m_btNo1SkillOpenLevel;
//	Player[CLIENT_PLAYER_INDEX].m_cFaction.m_sSkillOpen[1].m_nOpenLevel = pData->m_btNo2SkillOpenLevel;
//	if (Player[CLIENT_PLAYER_INDEX].m_cFaction.m_nCurFaction == Player[CLIENT_PLAYER_INDEX].m_cFaction.m_sSkillOpen[0].m_nID)
//		Player[CLIENT_PLAYER_INDEX].m_cFaction.OpenCurSkillLevel(Player[CLIENT_PLAYER_INDEX].m_cFaction.m_sSkillOpen[0].m_nOpenLevel, &Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList);
//	else
//		Player[CLIENT_PLAYER_INDEX].m_cFaction.OpenCurSkillLevel(Player[CLIENT_PLAYER_INDEX].m_cFaction.m_sSkillOpen[1].m_nOpenLevel, &Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList);
}

//-------------------------------------------------------------------------
//	���ܣ��յ�������֪ͨ����״̬(�ӳ����뿪�š��رն���״̬ʱ�Ż��յ�����Ϣ)
//-------------------------------------------------------------------------
void KProtocolProcess::s2cSetTeamState(BYTE* pMsg)
{
	// �ͻ����������������������
	if (!Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag || Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFigure != TEAM_CAPTAIN)
	{
		Player[CLIENT_PLAYER_INDEX].ApplySelfTeamInfo();
		return;
	}

	PLAYER_TEAM_OPEN_CLOSE	*pTeamState = (PLAYER_TEAM_OPEN_CLOSE*)pMsg;
	Player[CLIENT_PLAYER_INDEX].m_cTeam.ReleaseList();
	if (pTeamState->m_btOpenClose)
	{
		g_Team[0].SetTeamOpen();

		KSystemMessage	sMsg;
		sprintf(sMsg.szMessage, MSG_TEAM_OPEN);
		sMsg.eType = SMT_NORMAL;
		sMsg.byConfirmType = SMCT_NONE;
		sMsg.byPriority = 0;
		sMsg.byParamSize = 0;
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
	}
	else
	{
		g_Team[0].SetTeamClose();

		KSystemMessage	sMsg;
		sprintf(sMsg.szMessage, MSG_TEAM_CLOSE);
		sMsg.eType = SMT_NORMAL;
		sMsg.byConfirmType = SMCT_NONE;
		sMsg.byPriority = 0;
		sMsg.byParamSize = 0;
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
	}
}

//-------------------------------------------------------------------------
//	���ܣ��յ�������֪ͨ����״̬(�ӳ����뿪�š��رն���״̬ʱ�Ż��յ�����Ϣ)
//-------------------------------------------------------------------------
void	KProtocolProcess::s2cApplyAddTeam(BYTE* pMsg)
{
	Player[CLIENT_PLAYER_INDEX].s2cApplyAddTeam(pMsg);
}

//-------------------------------------------------------------------------
//	���ܣ��յ�������������ĳ�����������������ʾ֮
//-------------------------------------------------------------------------
void KProtocolProcess::s2cShowTeamInfo(BYTE* pMsg)
{
}

void KProtocolProcess::s2cSyncItem(BYTE* pMsg)
{
	ITEM_SYNC	*pItemSync = (ITEM_SYNC*)pMsg;

 
	int nIdxBS = Player[CLIENT_PLAYER_INDEX].m_ItemList.SearchID(pItemSync->m_ID);

	if (nIdxBS > 0 && nIdxBS < MAX_ITEM)
	{
	
		Player[CLIENT_PLAYER_INDEX].m_ItemList.Remove(nIdxBS);
	}



	int pnMagicParam[6];
	for (int i = 0; i < 6; i++)
	{
		pnMagicParam[i] = pItemSync->m_MagicLevel[i];
	}

	

	int nIndex = ItemSet.Add(
		pItemSync->m_Genre,
		pItemSync->m_Series,
		pItemSync->m_Level,
		pItemSync->m_Luck,
		pItemSync->m_Detail,
		pItemSync->m_Particur,
		pnMagicParam,
		pItemSync->m_Version,
		pItemSync->m_RandomSeed,
		pItemSync->m_Time.bYear,
		pItemSync->m_Time.bMonth,
		pItemSync->m_Time.bDay,
		pItemSync->m_Time.bHour); // Time Item
		
	if (nIndex > 0)
	{
		Item[nIndex].SetID(pItemSync->m_ID);
		Item[nIndex].SetDurability((short)pItemSync->m_Durability);
		Item[nIndex].SetFiFongArmor(pItemSync->m_nFifong);
		Item[nIndex].SetBindItem(pItemSync->m_nBindItem);
		Item[nIndex].SetShopCost(pItemSync->iShopCost); 
		Item[nIndex].SetNgoaiTrang(pItemSync->iNgoaiTrang); 

		Player[CLIENT_PLAYER_INDEX].m_ItemList.Add(nIndex, pItemSync->m_btPlace, pItemSync->m_btX, pItemSync->m_btY);
	}

	Player[CLIENT_PLAYER_INDEX].m_ItemList.UnlockOperation();
}

//-------------------------------------------------------------------------
//	���ܣ��յ���������������ͬ��money����Ϣ
//-------------------------------------------------------------------------
void KProtocolProcess::s2cSyncMoney(BYTE* pMsg)
{
	Player[CLIENT_PLAYER_INDEX].s2cSyncMoney(pMsg);
}

void KProtocolProcess::s2cSyncRoleList(BYTE* pMsg)
{
	ROLE_LIST_SYNC	*pSync = (ROLE_LIST_SYNC *)pMsg;
	// TODO: ͨ��CORESHELL�������棿
}

//-------------------------------------------------------------------------
//	���ܣ��յ�������֪ͨ�����˼������
//-------------------------------------------------------------------------
void KProtocolProcess::s2cTeamAddMember(BYTE* pMsg)
{
	if (!Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag )
	{
		Player[CLIENT_PLAYER_INDEX].ApplySelfTeamInfo();
		return;
	}

	PLAYER_TEAM_ADD_MEMBER	*pAddMem = (PLAYER_TEAM_ADD_MEMBER*)pMsg;
	int		nMemNum;

	nMemNum = g_Team[0].FindFree();
	if (nMemNum < 0)
		return;
	g_Team[0].m_nMember[nMemNum] = pAddMem->m_dwNpcID;
	g_Team[0].m_nMemLevel[nMemNum + 1] = (DWORD)pAddMem->m_btLevel;
	strcpy(g_Team[0].m_szMemName[nMemNum + 1], pAddMem->m_szName);
	g_Team[0].m_nMemNum++;
	Player[CLIENT_PLAYER_INDEX].m_cTeam.DeleteOneFromApplyList(pAddMem->m_dwNpcID);
	Player[CLIENT_PLAYER_INDEX].m_cTeam.UpdateInterface();

	KSystemMessage	sMsg;
	sprintf(sMsg.szMessage, MSG_TEAM_ADD_MEMBER, pAddMem->m_szName);
	sMsg.eType = SMT_NORMAL;
	sMsg.byConfirmType = SMCT_NONE;
	sMsg.byPriority = 0;
	sMsg.byParamSize = 0;
	CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
}

//-------------------------------------------------------------------------
//	���ܣ��յ�������֪ͨ�����ӳ�
//-------------------------------------------------------------------------
void KProtocolProcess::s2cTeamChangeCaptain(BYTE* pMsg)
{
	if (!Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag)
	{
		Player[CLIENT_PLAYER_INDEX].ApplySelfTeamInfo();
		return;
	}

	int		nLevel, nMemNo;
	char	szName[32];
	PLAYER_TEAM_CHANGE_CAPTAIN	*pChange = (PLAYER_TEAM_CHANGE_CAPTAIN*)pMsg;

	nMemNo = g_Team[0].FindMemberID(pChange->m_dwCaptainID);
	// ��ǰ������������
	if ((DWORD)g_Team[0].m_nCaptain != pChange->m_dwMemberID || nMemNo < 0)
	{
		Player[CLIENT_PLAYER_INDEX].ApplySelfTeamInfo();
		return;
	}

	// �Լ�������Ϊ�ӳ�
	if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_dwID == pChange->m_dwCaptainID)
	{
		KSystemMessage	sMsg;
		sprintf(sMsg.szMessage, MSG_TEAM_CHANGE_CAPTAIN_SELF, g_Team[0].m_szMemName[0]);
		sMsg.eType = SMT_TEAM;
		sMsg.byConfirmType = SMCT_UI_TEAM;
		sMsg.byPriority = 3;
		sMsg.byParamSize = 0;
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
	}
	else	// ���˱�����Ϊ�ӳ�
	{
		KSystemMessage	sMsg;
		sprintf(sMsg.szMessage, MSG_TEAM_CHANGE_CAPTAIN, g_Team[0].m_szMemName[nMemNo + 1]);
		sMsg.eType = SMT_TEAM;
		sMsg.byConfirmType = SMCT_UI_TEAM;
		sMsg.byPriority = 3;
		sMsg.byParamSize = 0;
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
	}

	// ������Ա
	nLevel = g_Team[0].m_nMemLevel[0];
	strcpy(szName, g_Team[0].m_szMemName[0]);
	g_Team[0].m_nCaptain = pChange->m_dwCaptainID;
	g_Team[0].m_nMemLevel[0] = g_Team[0].m_nMemLevel[nMemNo + 1];
	strcpy(g_Team[0].m_szMemName[0], g_Team[0].m_szMemName[nMemNo + 1]);
	g_Team[0].m_nMember[nMemNo] = pChange->m_dwMemberID;
	g_Team[0].m_nMemLevel[nMemNo + 1] = nLevel;
	strcpy(g_Team[0].m_szMemName[nMemNo + 1], szName);

	if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_dwID == pChange->m_dwCaptainID)
	{
		Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFigure = TEAM_CAPTAIN;
		Player[CLIENT_PLAYER_INDEX].m_cTeam.ReleaseList();
	}
	else if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_dwID == pChange->m_dwMemberID)
	{
		Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFigure = TEAM_MEMBER;
		Player[CLIENT_PLAYER_INDEX].m_cTeam.ReleaseList();
	}

	Player[CLIENT_PLAYER_INDEX].m_cTeam.UpdateInterface();
}

//-------------------------------------------------------------------------
//	���ܣ��յ��������������Լ��������������������Ӧ��Ϣ
//-------------------------------------------------------------------------
void KProtocolProcess::s2cUpdataSelfTeamInfo(BYTE* pMsg)
{
	int		i;
	PLAYER_SEND_SELF_TEAM_INFO	*pSelfInfo = (PLAYER_SEND_SELF_TEAM_INFO*)pMsg;

	// ���ݰ�����
	if (pSelfInfo->m_dwNpcID[0] == 0)
		return;

	Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag = 1;
	Player[CLIENT_PLAYER_INDEX].m_dwLeadExp = pSelfInfo->m_dwLeadExp;
	Player[CLIENT_PLAYER_INDEX].m_dwLeadLevel = PlayerSet.m_cLeadExp.GetLevel(Player[CLIENT_PLAYER_INDEX].m_dwLeadExp);
	Player[CLIENT_PLAYER_INDEX].m_dwNextLevelLeadExp = PlayerSet.m_cLeadExp.GetLevelExp(Player[CLIENT_PLAYER_INDEX].m_dwLeadLevel);
	if (Player[CLIENT_PLAYER_INDEX].m_dwNextLevelLeadExp == 0)
		Player[CLIENT_PLAYER_INDEX].m_dwNextLevelLeadExp = Player[CLIENT_PLAYER_INDEX].m_dwLeadLevel;

	if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_dwID == pSelfInfo->m_dwNpcID[0])	// �ӳ�
	{
		Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFigure = TEAM_CAPTAIN;
	}
	else													// ��Ա
	{
		Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFigure = TEAM_MEMBER;
		for (i = 0; i < MAX_TEAM_APPLY_LIST; i++)
			Player[CLIENT_PLAYER_INDEX].m_cTeam.m_sApplyList[i].Release();
	}
	g_Team[0].m_nCaptain = pSelfInfo->m_dwNpcID[0];
	if (pSelfInfo->m_btState == 0)
		g_Team[0].SetTeamClose();
	else
		g_Team[0].SetTeamOpen();
	g_Team[0].m_nTeamServerID = pSelfInfo->nTeamServerID;
	g_Team[0].m_nMemLevel[0] = (DWORD)pSelfInfo->m_btLevel[0];
	strcpy(g_Team[0].m_szMemName[0], pSelfInfo->m_szNpcName[0]);
	g_Team[0].m_nMemNum = 0;
	for (i = 0; i < MAX_TEAM_MEMBER; i++)
	{
		if (pSelfInfo->m_dwNpcID[i + 1] > 0)
		{
			g_Team[0].m_nMember[i] = pSelfInfo->m_dwNpcID[i + 1];
			g_Team[0].m_nMemLevel[i + 1] = (DWORD)pSelfInfo->m_btLevel[i + 1];
			strcpy(g_Team[0].m_szMemName[i + 1], pSelfInfo->m_szNpcName[i + 1]);
			g_Team[0].m_nMemNum++;
		}
		else
		{
			g_Team[0].m_nMember[i] = -1;
			g_Team[0].m_nMemLevel[i + 1] = 0;
			g_Team[0].m_szMemName[i + 1][0] = 0;
		}
	}
	Player[CLIENT_PLAYER_INDEX].m_cTeam.UpdateInterface();
}



void KProtocolProcess::SyncCurPlayerHeight(BYTE* pMsg)
{

CURPLAYER_HEIGHT_SYNC	*pSync = (CURPLAYER_HEIGHT_SYNC*)pMsg;

    Player[CLIENT_PLAYER_INDEX].m_cTask.SetSaveVal(100,pSync->m_Task100);
	Player[CLIENT_PLAYER_INDEX].m_cTask.SetSaveVal(162,pSync->m_Task162);
	Player[CLIENT_PLAYER_INDEX].m_cTask.SetSaveVal(191,pSync->m_Task191);
	Player[CLIENT_PLAYER_INDEX].m_cTask.SetSaveVal(192,pSync->m_Task192);
	Player[CLIENT_PLAYER_INDEX].m_cTask.SetSaveVal(193,pSync->m_Task193);
	Player[CLIENT_PLAYER_INDEX].m_cTask.SetSaveVal(140,pSync->m_Task140);
	Player[CLIENT_PLAYER_INDEX].m_cTask.SetSaveVal(139,pSync->m_Task139);
	Player[CLIENT_PLAYER_INDEX].m_cTask.SetSaveVal(485,pSync->m_Task485);
	Player[CLIENT_PLAYER_INDEX].m_cTask.SetSaveVal(128,pSync->m_Task128);
	Player[CLIENT_PLAYER_INDEX].m_cTask.SetSaveVal(99,pSync->m_Task99);
	Player[CLIENT_PLAYER_INDEX].m_cTask.SetSaveVal(15,pSync->m_Task15);
	Player[CLIENT_PLAYER_INDEX].m_cTask.SetSaveVal(124,pSync->m_Task124);

	
	if (Player[CLIENT_PLAYER_INDEX].m_cTask.GetSaveVal(139) == 1)
	{
	int nIndex = Player[CLIENT_PLAYER_INDEX].m_nIndex;
	if (nIndex > 0)
	Player[CLIENT_PLAYER_INDEX].m_nNextLevelExp = PlayerSet.m_cLevelAdd.GetLevelExp1(Npc[nIndex].m_Level);
	}
	else if (Player[CLIENT_PLAYER_INDEX].m_cTask.GetSaveVal(139) == 2)
	{
	int nIndex = Player[CLIENT_PLAYER_INDEX].m_nIndex;
	if (nIndex > 0)
	Player[CLIENT_PLAYER_INDEX].m_nNextLevelExp = PlayerSet.m_cLevelAdd.GetLevelExp2(Npc[nIndex].m_Level);
	}



	else if (Player[CLIENT_PLAYER_INDEX].m_cTask.GetSaveVal(139) == 3)
	{
	int nIndex = Player[CLIENT_PLAYER_INDEX].m_nIndex;
	if (nIndex > 0)
	Player[CLIENT_PLAYER_INDEX].m_nNextLevelExp = PlayerSet.m_cLevelAdd.GetLevelExp3(Npc[nIndex].m_Level);
	}
	else if (Player[CLIENT_PLAYER_INDEX].m_cTask.GetSaveVal(139) == 4)
	{
	int nIndex = Player[CLIENT_PLAYER_INDEX].m_nIndex;
	if (nIndex > 0)
	Player[CLIENT_PLAYER_INDEX].m_nNextLevelExp = PlayerSet.m_cLevelAdd.GetLevelExp4(Npc[nIndex].m_Level);
	}
	else if (Player[CLIENT_PLAYER_INDEX].m_cTask.GetSaveVal(139) == 5)
	{
	int nIndex = Player[CLIENT_PLAYER_INDEX].m_nIndex;
	if (nIndex > 0)
	Player[CLIENT_PLAYER_INDEX].m_nNextLevelExp = PlayerSet.m_cLevelAdd.GetLevelExp5(Npc[nIndex].m_Level);
	}



	else
	{
	int nIndex = Player[CLIENT_PLAYER_INDEX].m_nIndex;
	if (nIndex > 0)
	Player[CLIENT_PLAYER_INDEX].m_nNextLevelExp = PlayerSet.m_cLevelAdd.GetLevelExp(Npc[nIndex].m_Level);
	}

}

void KProtocolProcess::SyncCurNormalData(BYTE* pMsg)
{
	CURPLAYER_NORMAL_SYNC	*pSync = (CURPLAYER_NORMAL_SYNC*)pMsg;
	if (pSync->m_shLife > 0)
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentLife = pSync->m_shLife;
	else
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentLife = 0;
	if (pSync->m_shStamina > 0)
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentStamina = pSync->m_shStamina;
	else
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentStamina = 0;

	if (pSync->m_shMana > 0)
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentMana = pSync->m_shMana;
	else
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentMana = 0;

 


//	if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Doing == do_sit)
//	{
//		if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentLife >= Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentLifeMax &&
//			Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentMana >= Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentManaMax)
//		{
//			Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SendCommand(do_stand);
//		}
//	}

	Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SyncSignal = SubWorld[0].m_dwCurrentTime;
	switch (pSync->m_btTeamData)
	{
	case 0x00:
		if ( Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag )
		{
			Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag = 0;
			Player[CLIENT_PLAYER_INDEX].ApplySelfTeamInfo();
		}
		break;
	case 0x03:
		if ( !Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag || Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFigure != TEAM_CAPTAIN)
		{
			Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag = 1;
			Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFigure = TEAM_CAPTAIN;
			Player[CLIENT_PLAYER_INDEX].ApplySelfTeamInfo();
		}
		break;
	case 0x01:
		if ( !Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag || Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFigure != TEAM_MEMBER)
		{
			Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag = 1;
			Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFigure = TEAM_MEMBER;
			Player[CLIENT_PLAYER_INDEX].ApplySelfTeamInfo();
		}
		break;
	}
}

void KProtocolProcess::SyncCurPlayer(BYTE* pMsg)
{
	Player[CLIENT_PLAYER_INDEX].SyncCurPlayer(pMsg);
}

void KProtocolProcess::SyncNpc(BYTE* pMsg)
{
	NPC_SYNC* NpcSync = (NPC_SYNC *)pMsg;
	
/*
	int nRegion = SubWorld[0].FindRegion(NpcSync->RegionID);
	// ���Region�����ڣ�˵����������������⣬�������ˡ�
	if (nRegion == -1)
		return;
*/
	int nRegion, nMapX, nMapY, nOffX, nOffY;
	SubWorld[0].Mps2Map(NpcSync->MapX, NpcSync->MapY, &nRegion, &nMapX, &nMapY, &nOffX, &nOffY);

	if (nRegion == -1)
		return;

	int nIdx = NpcSet.SearchID(NpcSync->ID);
	if (!nIdx)
	{
		nIdx = NpcSet.Add(NpcSync->NpcSettingIdx, 0, NpcSync->MapX, NpcSync->MapY);
		Npc[nIdx].m_dwID = NpcSync->ID;
		Npc[nIdx].m_Kind = NpcSync->m_btKind;
		Npc[nIdx].m_Height = 0;
		
		if (NpcSet.IsNpcRequestExist(NpcSync->ID))
			NpcSet.RemoveNpcRequest(NpcSync->ID);
	}
	else
	{
		if (Npc[nIdx].m_RegionIndex >= 0)
		{
			SubWorld[0].m_Region[Npc[nIdx].m_RegionIndex].DecRef(Npc[nIdx].m_MapX, Npc[nIdx].m_MapY, obj_npc);



						



		}

		Npc[nIdx].m_MapX = nMapX;
		Npc[nIdx].m_MapY = nMapY;
		Npc[nIdx].m_OffX = nOffX;
		Npc[nIdx].m_OffY = nOffY;

		Npc[nIdx].m_NpcSettingIdx = (short)HIWORD(NpcSync->NpcSettingIdx);
		Npc[nIdx].m_Level = LOWORD(NpcSync->NpcSettingIdx);

		if (Npc[nIdx].m_RegionIndex >= 0)
		{
			if (Npc[nIdx].m_RegionIndex != nRegion)
			{
				// zroc change
				SubWorld[0].NpcChangeRegion(SubWorld[0].m_Region[Npc[nIdx].m_RegionIndex].m_RegionID, SubWorld[0].m_Region[nRegion].m_RegionID, nIdx);
				Npc[nIdx].m_RegionIndex = nRegion;
			}


			SubWorld[0].m_Region[Npc[nIdx].m_RegionIndex].AddRef(Npc[nIdx].m_MapX, Npc[nIdx].m_MapY, obj_npc);


























		}
		else
		{
			SubWorld[0].m_Region[nRegion].AddNpc(nIdx);
			Npc[nIdx].m_RegionIndex = nRegion;
			SubWorld[0].m_Region[Npc[nIdx].m_RegionIndex].AddRef(Npc[nIdx].m_MapX, Npc[nIdx].m_MapY, obj_npc);







		}

	}
	Npc[nIdx].m_Camp			= (NPCCAMP)NpcSync->Camp;
	Npc[nIdx].m_CurrentCamp		= (NPCCAMP)NpcSync->CurrentCamp;
	Npc[nIdx].m_Series			= NpcSync->m_bySeries;
	Npc[nIdx].m_IsBoss			= NpcSync->m_IsBoss;


	if (NpcSync->CurrentLife > 0 && NpcSync->CurrentLifeMax >= NpcSync->CurrentLife)
	{
		Npc[nIdx].m_CurrentLife	= NpcSync->CurrentLife;
		Npc[nIdx].m_CurrentLifeMax	= NpcSync->CurrentLifeMax;
	}
	else
	{
		Npc[nIdx].m_CurrentLife	= 0;
		Npc[nIdx].m_CurrentLifeMax = 0;
	}
	
	if (Npc[nIdx].m_Doing != do_death || Npc[nIdx].m_Doing != do_revive) // need check later -- spe 03/05/27
		Npc[nIdx].SendCommand((NPCCMD)NpcSync->m_Doing, NpcSync->MapX, NpcSync->MapY);

	Npc[nIdx].m_SyncSignal = SubWorld[0].m_dwCurrentTime;
	Npc[nIdx].SetMenuState(NpcSync->m_btMenuState);
	memset(Npc[nIdx].Name, 0, sizeof(Npc[nIdx].Name));
	memcpy(Npc[nIdx].Name, NpcSync->m_szName, NpcSync->m_wLength - (sizeof(NPC_SYNC) - 1 - sizeof(NpcSync->m_szName)));
}

void KProtocolProcess::SyncNpcMin(BYTE* pMsg)
{
	NPC_NORMAL_SYNC* NpcSync = (NPC_NORMAL_SYNC *)pMsg;

//	if (NpcSync->ID == Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_dwID)
//		return;

	if (NpcSync->Doing == do_revive || NpcSync->Doing == do_death)
		return;

//	int nORegion, nOMapX, nOMapY, nOOffX, nOOffY;
	
	int nIdx = NpcSet.SearchID(NpcSync->ID);
	if (!nIdx)
	{
		// �����������ͬ�����NPC��ȫ������
		if (!NpcSet.IsNpcRequestExist(NpcSync->ID))
		{
			
			if (NpcSet.InsertNpcRequest(NpcSync->ID))
				SendClientCmdRequestNpc(NpcSync->ID);
		}
	}
	else
	{
//		int nRegion = SubWorld[0].FindRegion(NpcSync->RegionID);
		int nRegion, nMapX, nMapY, nOffX, nOffY;
		SubWorld[0].Mps2Map(NpcSync->MapX, NpcSync->MapY, &nRegion, &nMapX, &nMapY, &nOffX, &nOffY);

		Npc[nIdx].m_sSyncPos.m_dwRegionID = SubWorld[0].m_Region[nRegion].m_RegionID;
		Npc[nIdx].m_sSyncPos.m_nMapX = nMapX;
		Npc[nIdx].m_sSyncPos.m_nMapY = nMapY;
		Npc[nIdx].m_sSyncPos.m_nOffX = nOffX;
		Npc[nIdx].m_sSyncPos.m_nOffY = nOffY;
		Npc[nIdx].m_sSyncPos.m_nDoing = NpcSync->Doing;

		if (Npc[nIdx].m_RegionIndex == -1 && nIdx != Player[CLIENT_PLAYER_INDEX].m_nIndex)	
		{
			if (nRegion == -1)
			{		
				return;
			}
			else
			{
				Npc[nIdx].m_MapX = nMapX;
				Npc[nIdx].m_MapY = nMapY;
				Npc[nIdx].m_OffX = nOffX;
				Npc[nIdx].m_OffY = nOffY;
				Npc[nIdx].m_RegionIndex = nRegion;
				Npc[nIdx].m_dwRegionID = SubWorld[0].m_Region[nRegion].m_RegionID;
				SubWorld[0].m_Region[nRegion].AddNpc(nIdx);
				SubWorld[0].m_Region[nRegion].AddRef(Npc[nIdx].m_MapX, Npc[nIdx].m_MapY, obj_npc);






			}
			
			if (NpcSync->Doing == do_stand)
				Npc[nIdx].ProcNetCommand(do_stand);
		}
		else
		{
			if (Npc[nIdx].m_RegionIndex != nRegion && nIdx != Player[CLIENT_PLAYER_INDEX].m_nIndex)
			{
				SubWorld[0].m_Region[Npc[nIdx].m_RegionIndex].RemoveNpc(nIdx);
				SubWorld[0].m_Region[Npc[nIdx].m_RegionIndex].DecRef(Npc[nIdx].m_MapX, Npc[nIdx].m_MapY, obj_npc);














				Npc[nIdx].m_MapX = nMapX;
				Npc[nIdx].m_MapY = nMapY;
				Npc[nIdx].m_OffX = nOffX;
				Npc[nIdx].m_OffY = nOffY;
				Npc[nIdx].m_RegionIndex = nRegion;
                
				if (nRegion >= 0)
				{
					SubWorld[0].m_Region[nRegion].AddNpc(nIdx);
					Npc[nIdx].m_dwRegionID = SubWorld[0].m_Region[nRegion].m_RegionID;
					SubWorld[0].m_Region[nRegion].AddRef(Npc[nIdx].m_MapX, Npc[nIdx].m_MapY, obj_npc);









				}
			}

		}

		if (nIdx != Player[CLIENT_PLAYER_INDEX].m_nIndex)	// �����
		{
			int	nOldLife = Npc[nIdx].m_CurrentLife;
			if (NpcSync->CurrentLife > 0 && NpcSync->CurrentLifeMax >= NpcSync->CurrentLife)
			{
				Npc[nIdx].m_CurrentLife = NpcSync->CurrentLife;
				Npc[nIdx].m_CurrentLifeMax = NpcSync->CurrentLifeMax;
			}
			else
			{
				Npc[nIdx].m_CurrentLife = 0;
				Npc[nIdx].m_CurrentLifeMax = 0;
			}
			// ðѪ����
			if (Npc[nIdx].m_Kind == kind_normal)
			{
				Npc[nIdx].SetBlood(nOldLife - Npc[nIdx].m_CurrentLife);
			}
			Npc[nIdx].m_CurrentCamp = NpcSync->Camp;

		}
		Npc[nIdx].m_FreezeState.nTime = NpcSync->State & STATE_FREEZE;
		Npc[nIdx].m_PoisonState.nTime = NpcSync->State & STATE_POISON;
		Npc[nIdx].m_ConfuseState.nTime =NpcSync->State & STATE_CONFUSE;
		Npc[nIdx].m_StunState.nTime = NpcSync->State & STATE_STUN;

		Npc[nIdx].m_InvisibilityState.nTime = NpcSync->State & STATE_INVISIBILITY;
		Npc[nIdx].m_NstatusState.nTime = NpcSync->State & STATE_NSTATUS;
		Npc[nIdx].m_MadnessState.nTime = NpcSync->State & STATE_MADNESS;

		Npc[nIdx].m_SyncSignal = SubWorld[0].m_dwCurrentTime;
	}
}





//-------------------------------------------------------------------------
//	���ܣ��յ���������Ϣͬ�������npc����
//-------------------------------------------------------------------------
void KProtocolProcess::SyncNpcMinPlayer(BYTE* pMsg)
{	
	NPC_PLAYER_TYPE_NORMAL_SYNC	*pSync = (NPC_PLAYER_TYPE_NORMAL_SYNC*)pMsg;

	_ASSERT(pSync->m_dwNpcID == Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_dwID);
	if (pSync->m_dwNpcID != Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_dwID)
		return;

	int nRegion, nMapX, nMapY, nOffX, nOffY, nNpcIdx;
	SubWorld[0].Mps2Map(pSync->m_dwMapX, pSync->m_dwMapY, &nRegion, &nMapX, &nMapY, &nOffX, &nOffY);

	nNpcIdx = Player[CLIENT_PLAYER_INDEX].m_nIndex;







	// �л���ͼ�ˣ����ǵ�RegionIndexΪ-1
	if (Npc[nNpcIdx].m_RegionIndex == -1)
	{
		//nRegionIdx = SubWorld[0].FindRegion(pSync->m_nRegionID);
		if (nRegion < 0)
		{
			// �ոջ��ĵ�ͼ�����ʱ��Ӧ�����ҵõ�region������Ҳ�����˵�����ݰ����ܳ�����
//			_ASSERT(0);
			return;
		}
		Npc[nNpcIdx].m_RegionIndex = nRegion;
		Npc[nNpcIdx].m_dwRegionID = SubWorld[0].m_Region[nRegion].m_RegionID;
		//pSync->m_nRegionID;
		SubWorld[0].NpcChangeRegion(-1, SubWorld[0].m_Region[nRegion].m_RegionID, nNpcIdx);
		SubWorld[0].m_Region[nRegion].AddRef(nMapX, nMapY, obj_npc);

		Npc[nNpcIdx].m_MapX = nMapX;
		Npc[nNpcIdx].m_MapY = nMapY;
		Npc[nNpcIdx].m_OffX = pSync->m_wOffX;
		Npc[nNpcIdx].m_OffY = pSync->m_wOffY;
		memset(&Npc[nNpcIdx].m_sSyncPos, 0, sizeof(Npc[nNpcIdx].m_sSyncPos));
		Npc[nNpcIdx].m_SyncSignal = SubWorld[0].m_dwCurrentTime;
		return;
	}

	// ��̫Զ������9����Χ
	//nRegionIdx = SubWorld[0].FindRegion(pSync->m_nRegionID);
	if (nRegion == -1)
	{
		SubWorld[0].m_Region[Npc[nNpcIdx].m_RegionIndex].DecRef(Npc[nNpcIdx].m_MapX, Npc[nNpcIdx].m_MapY, obj_npc);

		int nRegionX = pSync->m_dwMapX / (SubWorld[0].m_nCellWidth * SubWorld[0].m_nRegionWidth);
		int nRegionY = pSync->m_dwMapY / (SubWorld[0].m_nCellHeight * SubWorld[0].m_nRegionHeight);
		
		DWORD	dwRegionID = MAKELONG(nRegionX, nRegionY);
		SubWorld[0].LoadMap(SubWorld[0].m_SubWorldID, dwRegionID);

//		SubWorld[0].LoadMap(SubWorld[0].m_SubWorldID, pSync->m_nRegionID);
		nRegion = SubWorld[0].FindRegion(dwRegionID);
		_ASSERT(nRegion >= 0);
		Npc[nNpcIdx].m_RegionIndex = nRegion;
		Npc[nNpcIdx].m_dwRegionID = dwRegionID;//pSync->m_nRegionID;
		SubWorld[0].NpcChangeRegion(-1, SubWorld[0].m_Region[nRegion].m_RegionID, nNpcIdx);

		SubWorld[0].Mps2Map(pSync->m_dwMapX, pSync->m_dwMapY, &nRegion, &nMapX, &nMapY, &nOffX, &nOffY);
		Npc[nNpcIdx].m_MapX = nMapX;
		Npc[nNpcIdx].m_MapY = nMapY;
		Npc[nNpcIdx].m_OffX = pSync->m_wOffX;
		Npc[nNpcIdx].m_OffY = pSync->m_wOffY;
		
		SubWorld[0].m_Region[Npc[nNpcIdx].m_RegionIndex].AddRef(Npc[nNpcIdx].m_MapX, Npc[nNpcIdx].m_MapY, obj_npc);
		memset(&Npc[nNpcIdx].m_sSyncPos, 0, sizeof(Npc[nNpcIdx].m_sSyncPos));
		Npc[nNpcIdx].m_SyncSignal = SubWorld[0].m_dwCurrentTime;

		return;
	}

	BYTE	byBarrier = SubWorld[0].m_Region[Npc[nNpcIdx].m_RegionIndex].GetBarrier(Npc[nNpcIdx].m_MapX, Npc[nNpcIdx].m_MapY, Npc[nNpcIdx].m_OffX, Npc[nNpcIdx].m_OffY);
	if (0 != byBarrier && Obstacle_JumpFly != byBarrier)
	{
		g_DebugLog("[Barrier]Player in Barrier");
	}
}

//-------------------------------------------------------------------------
//	���ܣ��յ���������Ϣ����һ��obj
//-------------------------------------------------------------------------
void KProtocolProcess::SyncObjectAdd(BYTE* pMsg)
{
	OBJ_ADD_SYNC	*pObjSyncAdd = (OBJ_ADD_SYNC*)pMsg;
	int				nObjIndex;
	KObjItemInfo	sInfo;

	nObjIndex = ObjSet.FindID(pObjSyncAdd->m_nID);
	if (nObjIndex > 0)
		return;

	sInfo.m_nItemID = pObjSyncAdd->m_nItemID;
	sInfo.m_nItemWidth = pObjSyncAdd->m_btItemWidth;
	sInfo.m_nItemHeight = pObjSyncAdd->m_btItemHeight;
	sInfo.m_nMoneyNum = pObjSyncAdd->m_nMoneyNum;
	sInfo.m_nColorID = pObjSyncAdd->m_btColorID;
	sInfo.m_nMovieFlag = ((pObjSyncAdd->m_btFlag & 0x02) > 0 ? 1 : 0);
	sInfo.m_nSoundFlag = ((pObjSyncAdd->m_btFlag & 0x01) > 0 ? 1 : 0);
	memset(sInfo.m_szName, 0, sizeof(sInfo.m_szName));
	memcpy(sInfo.m_szName, pObjSyncAdd->m_szName, pObjSyncAdd->m_wLength + 1 + sizeof(pObjSyncAdd->m_szName) - sizeof(OBJ_ADD_SYNC));

	nObjIndex = ObjSet.ClientAdd(
		pObjSyncAdd->m_nID,
		pObjSyncAdd->m_nDataID,
		pObjSyncAdd->m_btState,
		pObjSyncAdd->m_btDir,
		pObjSyncAdd->m_wCurFrame,
		pObjSyncAdd->m_nXpos,
		pObjSyncAdd->m_nYpos,
		sInfo);

	if (nObjIndex <= 0)
		return;

	Object[nObjIndex].m_nAutoNoActack = FALSE;

#ifdef WAIGUA_ZROC
	if (nObjIndex <= 0)
		return;
	PLAYER_PICKUP_ITEM_COMMAND	sPickUp;
	if (Object[nObjIndex].m_nKind == Obj_Kind_Money)
	{
//		sPickUp.ProtocolType = c2s_playerpickupitem;
//		sPickUp.m_nObjID = Object[nObjIndex].m_nID;
//		sPickUp.m_btPosType = 0;
//		sPickUp.m_btPosX = 0;
//		sPickUp.m_btPosY = 0;
//		if (g_pClient)
//			g_pClient->SendPackToServer(&sPickUp, sizeof(PLAYER_PICKUP_ITEM_COMMAND));
	}
	else if (Object[nObjIndex].m_nKind == Obj_Kind_Item)
	{
		ItemPos	sItemPos;
		if ( FALSE == Player[CLIENT_PLAYER_INDEX].m_ItemList.SearchPosition(Object[nObjIndex].m_nItemWidth, Object[nObjIndex].m_nItemHeight, &sItemPos) )
			return;
		sPickUp.ProtocolType = c2s_playerpickupitem;
		sPickUp.m_nObjID = Object[nObjIndex].m_nID;
		sPickUp.m_btPosType = sItemPos.nPlace;
		sPickUp.m_btPosX = sItemPos.nX;
		sPickUp.m_btPosY = sItemPos.nY;
		if (g_pClient)
			g_pClient->SendPackToServer(&sPickUp, sizeof(PLAYER_PICKUP_ITEM_COMMAND));
	}
#endif
}

//-------------------------------------------------------------------------
//	���ܣ��յ���������Ϣ����ĳ��obj����
//-------------------------------------------------------------------------
void KProtocolProcess::SyncObjectDir(BYTE* pMsg)
{
	OBJ_SYNC_DIR	*pObjSyncDir = (OBJ_SYNC_DIR*)pMsg;
	int				nObjIndex;
	nObjIndex = ObjSet.FindID(pObjSyncDir->m_nID);
	if (nObjIndex <= 0)
	{
		// �����������������
		OBJ_CLIENT_SYNC_ADD	sObjClientSyncAdd;
		sObjClientSyncAdd.ProtocolType = c2s_requestobj;
		sObjClientSyncAdd.m_nID = pObjSyncDir->m_nID;
		if (g_pClient)
			g_pClient->SendPackToServer(&sObjClientSyncAdd, sizeof(sObjClientSyncAdd));
	}
	else
	{	// ͬ������
		Object[nObjIndex].SetDir(pObjSyncDir->m_btDir);
	}
}

//-------------------------------------------------------------------------
//	���ܣ��յ���������Ϣɾ��ĳ��obj
//-------------------------------------------------------------------------
void KProtocolProcess::SyncObjectRemove(BYTE* pMsg)
{
	OBJ_SYNC_REMOVE	*pObjSyncRemove = (OBJ_SYNC_REMOVE*)pMsg;
	int				nObjIndex;
	nObjIndex = ObjSet.FindID(pObjSyncRemove->m_nID);
	if (nObjIndex > 0)
	{	// ɾ��
		Object[nObjIndex].Remove(pObjSyncRemove->m_btSoundFlag);
	}
}

//-------------------------------------------------------------------------
//	���ܣ��յ���������Ϣ����ĳ��obj״̬
//-------------------------------------------------------------------------
void KProtocolProcess::SyncObjectState(BYTE* pMsg)
{
	OBJ_SYNC_STATE	*pObjSyncState = (OBJ_SYNC_STATE*)pMsg;
	int				nObjIndex;
	nObjIndex = ObjSet.FindID(pObjSyncState->m_nID);
	if (nObjIndex <= 0)
	{
		// �����������������
		OBJ_CLIENT_SYNC_ADD	sObjClientSyncAdd;
		sObjClientSyncAdd.ProtocolType = c2s_requestobj;
		sObjClientSyncAdd.m_nID = pObjSyncState->m_nID;
		if (g_pClient)
			g_pClient->SendPackToServer(&sObjClientSyncAdd, sizeof(sObjClientSyncAdd));
	}
	else
	{	// ͬ��״̬
		if (Object[nObjIndex].m_nRegionIdx == -1)
		{
			int	nRegion;
			nRegion = SubWorld[0].FindRegion(Object[nObjIndex].m_nBelongRegion);
			if (nRegion >= 0)
			{
				Object[nObjIndex].m_nRegionIdx = nRegion;
				SubWorld[0].m_Region[nRegion].AddObj(nObjIndex);
			}
		}
		Object[nObjIndex].SetState(pObjSyncState->m_btState);
	}
}

//-------------------------------------------------------------------------
//	���ܣ��յ���������Ϣͬ��ĳ�� trap �� obj ������
//-------------------------------------------------------------------------
void KProtocolProcess::SyncObjectTrap(BYTE* pMsg)
{
	OBJ_SYNC_TRAP_ACT *pObjTrapSync = (OBJ_SYNC_TRAP_ACT*)pMsg;
	int		nObjIndex;
	nObjIndex = ObjSet.FindID(pObjTrapSync->m_nID);
	if (nObjIndex <= 0)
	{
		// �����������������
		OBJ_CLIENT_SYNC_ADD	sObjClientSyncAdd;
		sObjClientSyncAdd.ProtocolType = c2s_requestobj;
		sObjClientSyncAdd.m_nID = pObjTrapSync->m_nID;
		if (g_pClient)
			g_pClient->SendPackToServer(&sObjClientSyncAdd, sizeof(sObjClientSyncAdd));
	}
	else
	{
		Object[nObjIndex].m_nState = OBJ_TRAP_STATE_ACTING;
		Object[nObjIndex].m_cImage.SetDirStart();
		Object[nObjIndex].m_cSkill.m_nTarX = pObjTrapSync->m_nTarX;
		Object[nObjIndex].m_cSkill.m_nTarY = pObjTrapSync->m_nTarY;
	}
}

void KProtocolProcess::SyncPlayer(BYTE* pMsg)
{
	PLAYER_SYNC*	pPlaySync = (PLAYER_SYNC *)pMsg;

	int nIdx = NpcSet.SearchID(pPlaySync->ID);

	Npc[nIdx].m_ArmorType			= pPlaySync->ArmorType;
	Npc[nIdx].m_HelmType			= pPlaySync->HelmType;
	Npc[nIdx].m_MaskType			= pPlaySync->MaskType; // MASK
	Npc[nIdx].m_FifongType			= pPlaySync->FifongType;
//	g_DebugLog("Npc[nIdx].m_FifongType: %d",Npc[nIdx].m_FifongType);
	if (Npc[nIdx].m_FifongType > 0)
		{
			Npc[nIdx].m_bFifongState = TRUE;
		}
		else
		{
			Npc[nIdx].m_bFifongState = FALSE;
		}	
	Npc[nIdx].m_HorseType			= (char)pPlaySync->HorseType;
	Npc[nIdx].m_WeaponType			= pPlaySync->WeaponType;	
	Npc[nIdx].m_Kind				= kind_player;
	Npc[nIdx].m_btRankId			= pPlaySync->RankID;
	// chan dung
	Npc[nIdx].m_ImagePlayer			= pPlaySync->ImagePlayer;	
	if (nIdx == Player[CLIENT_PLAYER_INDEX].m_nIndex)
	{
		Player[CLIENT_PLAYER_INDEX].m_ImagePlayer		= pPlaySync->ImagePlayer;
	}
	// end
	if (Npc[nIdx].m_HorseType == -1)
	{

    if (nIdx == Player[CLIENT_PLAYER_INDEX].m_nIndex)
	{
    if (Npc[nIdx].m_bRideHorse == TRUE)
    {
    int ItemActiveIdx = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquipment(itempart_horse);
    if (ItemActiveIdx != 0)
    {
	int ActiveMagic = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquipEnhanceP(itempart_horse);
	Item[ItemActiveIdx].RemoveMagicAttribFromNPC(&Npc[nIdx],ActiveMagic);
    }
    }
	}

		Npc[nIdx].m_bRideHorse = FALSE;
	}
	else
	{

	if (nIdx == Player[CLIENT_PLAYER_INDEX].m_nIndex)
	{
	if (Npc[nIdx].m_bRideHorse == FALSE)
    {
    int ItemActiveIdx = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquipment(itempart_horse);
    if (ItemActiveIdx != 0)
    {
	int ActiveMagic = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquipEnhanceP(itempart_horse);
	Item[ItemActiveIdx].ApplyMagicAttribToNPC(&Npc[nIdx],ActiveMagic);
    }
    }
	}
	
		Npc[nIdx].m_bRideHorse = TRUE;
	}


	memset(Npc[nIdx].m_TongName, 0, sizeof(Npc[nIdx].m_TongName));

	if (pPlaySync->m_TongName[0])
	{
	memcpy(Npc[nIdx].m_TongName, pPlaySync->m_TongName, sizeof(pPlaySync->m_TongName));
    }
	
	memset(Npc[nIdx].m_TongRank, 0, sizeof(Npc[nIdx].m_TongRank));

	if (pPlaySync->m_TongRank[0])
	{
	memcpy(Npc[nIdx].m_TongRank, pPlaySync->m_TongRank, sizeof(pPlaySync->m_TongRank));
    }

	Npc[nIdx].m_CurrentAttackSpeed	= pPlaySync->AttackSpeed;
	Npc[nIdx].m_CurrentCastSpeed	= pPlaySync->CastSpeed;
	Npc[nIdx].m_CurrentRunSpeed		= pPlaySync->RunSpeed;
	Npc[nIdx].m_CurrentWalkSpeed	= pPlaySync->WalkSpeed;



	if (pPlaySync->m_btSomeFlag & 0x01)
		Npc[nIdx].m_nPKFlag			= 1;
	else if (pPlaySync->m_btSomeFlag & 0x10)
		Npc[nIdx].m_nPKFlag			= 2;
	else
		Npc[nIdx].m_nPKFlag			= 0;

	if (pPlaySync->m_btSomeFlag & 0x02)
		Npc[nIdx].m_FightMode		= 1;
	else
		Npc[nIdx].m_FightMode		= 0;
	if (pPlaySync->m_btSomeFlag & 0x04)
		Npc[nIdx].SetSleepMode(1);
	else
		Npc[nIdx].SetSleepMode(0);

	Npc[nIdx].m_Task139 = pPlaySync->m_Task139;
	Npc[nIdx].m_Task485 = pPlaySync->m_Task485;



	if (nIdx == Player[CLIENT_PLAYER_INDEX].m_nIndex)
	{
	Player[CLIENT_PLAYER_INDEX].m_cTask.SetSaveVal(139,pPlaySync->m_Task139);
	Player[CLIENT_PLAYER_INDEX].m_cTask.SetSaveVal(485,pPlaySync->m_Task485);
	}


	Npc[nIdx].m_dwTimeOpenShop = 0;

	memset(Npc[nIdx].m_szNameOpenShop, 0, sizeof(Npc[nIdx].m_szNameOpenShop));

	Npc[nIdx].m_dwTimeOpenShop = pPlaySync->m_dwTime;

	if (pPlaySync->szName[0])
	{
	memcpy(Npc[nIdx].m_szNameOpenShop, pPlaySync->szName, sizeof(Npc[nIdx].m_szNameOpenShop));
    }
		


	
}

void KProtocolProcess::SyncPlayerMin(BYTE* pMsg)
{
	PLAYER_NORMAL_SYNC* pPlaySync = (PLAYER_NORMAL_SYNC *)pMsg;

	int nIdx = NpcSet.SearchID(pPlaySync->ID);


	Npc[nIdx].m_HelmType			= pPlaySync->HelmType;
	Npc[nIdx].m_MaskType			= pPlaySync->MaskType; // MASK
	Npc[nIdx].m_FifongType			= pPlaySync->FifongType;
	if (Npc[nIdx].m_FifongType > 0)
	{
		Npc[nIdx].m_bFifongState = TRUE;
	}
	else
	{
		Npc[nIdx].m_bFifongState = FALSE;
	}
//	g_DebugLog("Npc[nIdx].m_FifongType: %d",Npc[nIdx].m_FifongType);
	Npc[nIdx].m_ArmorType			= pPlaySync->ArmorType;
	Npc[nIdx].m_HorseType			= (char)pPlaySync->HorseType;
	Npc[nIdx].m_WeaponType			= pPlaySync->WeaponType;
	Npc[nIdx].m_Kind				= kind_player;
	Npc[nIdx].m_btRankId			= pPlaySync->RankID;
	// chan dung
	Npc[nIdx].m_ImagePlayer			= pPlaySync->ImagePlayer;	
	if (nIdx == Player[CLIENT_PLAYER_INDEX].m_nIndex)
	{
		Player[CLIENT_PLAYER_INDEX].m_ImagePlayer		= pPlaySync->ImagePlayer;
	}
	// end
	Npc[nIdx].m_PKValue             = pPlaySync->m_PKValue;

	if (Npc[nIdx].m_HorseType == -1)
	{
    if (nIdx == Player[CLIENT_PLAYER_INDEX].m_nIndex)
	{
    if (Npc[nIdx].m_bRideHorse == TRUE)
    {
    int ItemActiveIdx = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquipment(itempart_horse);
    if (ItemActiveIdx != 0)
    {
	int ActiveMagic = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquipEnhanceP(itempart_horse);
	Item[ItemActiveIdx].RemoveMagicAttribFromNPC(&Npc[nIdx],ActiveMagic);
    }
    }
	}

		Npc[nIdx].m_bRideHorse = FALSE;
	}
	else
	{

	if (nIdx == Player[CLIENT_PLAYER_INDEX].m_nIndex)
	{
	if (Npc[nIdx].m_bRideHorse == FALSE)
    {
    int ItemActiveIdx = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquipment(itempart_horse);
    if (ItemActiveIdx != 0)
    {
	int ActiveMagic = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquipEnhanceP(itempart_horse);
	Item[ItemActiveIdx].ApplyMagicAttribToNPC(&Npc[nIdx],ActiveMagic);
    }
    }
	}
	
		Npc[nIdx].m_bRideHorse = TRUE;
	}


	Npc[nIdx].m_CurrentAttackSpeed	= pPlaySync->AttackSpeed;
	Npc[nIdx].m_CurrentCastSpeed	= pPlaySync->CastSpeed;
	Npc[nIdx].m_CurrentRunSpeed		= pPlaySync->RunSpeed;
	Npc[nIdx].m_CurrentWalkSpeed	= pPlaySync->WalkSpeed;
	Npc[nIdx].m_Task139 = pPlaySync->m_Task139;
	Npc[nIdx].m_Task485 = pPlaySync->m_Task485;

	if (nIdx == Player[CLIENT_PLAYER_INDEX].m_nIndex)
	{
	Player[CLIENT_PLAYER_INDEX].m_cTask.SetSaveVal(139,pPlaySync->m_Task139);
	Player[CLIENT_PLAYER_INDEX].m_cTask.SetSaveVal(485,pPlaySync->m_Task485);
	}

	if (pPlaySync->m_btSomeFlag & 0x01)
		Npc[nIdx].m_nPKFlag			= 1;
	else if (pPlaySync->m_btSomeFlag & 0x10)
		Npc[nIdx].m_nPKFlag			= 2;
	else
		Npc[nIdx].m_nPKFlag			= 0;

	if (pPlaySync->m_btSomeFlag & 0x02)
		Npc[nIdx].m_FightMode		= 1;
	else
		Npc[nIdx].m_FightMode		= 0;
	if (pPlaySync->m_btSomeFlag & 0x04)
		Npc[nIdx].SetSleepMode(1);
	else
		Npc[nIdx].SetSleepMode(0);
	if (pPlaySync->m_btSomeFlag & 0x08)
		Npc[nIdx].m_nTongFlag		= 1;
	else
		Npc[nIdx].m_nTongFlag		= 0;
}

void KProtocolProcess::SyncScriptAction(BYTE* pMsg)
{
	Player[CLIENT_PLAYER_INDEX].OnScriptAction((PLAYER_SCRIPTACTION_SYNC *)pMsg);
}

void KProtocolProcess::SyncWorld(BYTE* pMsg)
{
	WORLD_SYNC *WorldSync = (WORLD_SYNC *)pMsg;

	if (Player[CLIENT_PLAYER_INDEX].m_nIndex > 0)
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SendCommand(do_stand);
	if (SubWorld[0].m_SubWorldID != WorldSync->SubWorld)
	{
		SubWorld[0].LoadMap(WorldSync->SubWorld, WorldSync->Region);
/*		SubWorld[0].m_Region[0].Load(WorldSync->SubWorld, WorldSync->Region);
		SubWorld[0].m_Region[0].Init(SubWorld[0].m_nRegionWidth, SubWorld[0].m_nRegionHeight);
		for (int i = 0; i < 8; i++)
		{
			if (SubWorld[0].m_Region[0].m_nConRegionID[i] == -1)
				continue;
			SubWorld[0].m_Region[i+1].Load(WorldSync->SubWorld, SubWorld[0].m_Region[0].m_nConRegionID[i]);
			SubWorld[0].m_Region[i+1].Init(SubWorld[0].m_nRegionWidth, SubWorld[0].m_nRegionHeight);
		}*/
	}
	
	if (SubWorld[0].m_dwCurrentTime > WorldSync->Frame)
	{
		// �ͻ��˿��ˣ����ٴ�����������Lag����ľ���Ϣ����Ҫ����
	}
	else if (SubWorld[0].m_dwCurrentTime < WorldSync->Frame)
	{
		// �ͻ������ˣ����ٴ���
	}

	SubWorld[0].m_dwCurrentTime = WorldSync->Frame;


	for (int i = 0 ; i < MAX_NPCSKILL; i ++)
	{
		if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.m_Skills[i].SkillId > 0)
		{
			Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.SetBeginCastTime(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.m_Skills[i].SkillId, SubWorld[0].m_dwCurrentTime);
	        Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.SetNextCastTime(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.m_Skills[i].SkillId, SubWorld[0].m_dwCurrentTime);

		}
	}

	



	// ͬ������
	SubWorld[0].m_nWeather = WorldSync->Weather;
	g_ScenePlace.ChangeWeather(WorldSync->Weather);




	memcpy(SubWorld[0].m_szTongName,WorldSync->TongName,sizeof(SubWorld[0].m_szTongName));
	memcpy(SubWorld[0].m_szTongNameBC,WorldSync->TongNameBC, sizeof(SubWorld[0].m_szTongNameBC));
	SubWorld[0].m_nTongT = WorldSync->TongT;
	SubWorld[0].m_nTongVG = WorldSync->TongVG;
	SubWorld[0].m_bCheckTong = WorldSync->CheckTong;


}

//-------------------------------------------------------------------------
//	���ܣ��յ���������Ϣĳ�����ֹ���뱾�ͻ��˵ĺ��ѹ�ϵ
//-------------------------------------------------------------------------
void	KProtocolProcess::s2cChatDeleteFriend(BYTE* pMsg)
{
	CHAT_DELETE_FRIEND_SYNC	*pDelete = (CHAT_DELETE_FRIEND_SYNC*)pMsg;

	CChatFriend	*pFriend;
	for (int i = 0; i < MAX_FRIEND_TEAM; i++)
	{
		pFriend = (CChatFriend*)Player[CLIENT_PLAYER_INDEX].m_cChat.m_cFriendTeam[i].m_cEveryOne.GetHead();
		while (pFriend)
		{
			if (pFriend->m_dwID == pDelete->m_dwID)
			{
				pFriend->Remove();
				Player[CLIENT_PLAYER_INDEX].m_cChat.m_cFriendTeam[i].m_nFriendNo--;

				// ֪ͨ���� ĳ�����ֹ���뱾�ͻ��˵ĺ��ѹ�ϵ
//				CoreDataChanged(GDCNI_CHAT_GROUP, 0, 0);
				CoreDataChanged(GDCNI_CHAT_FRIEND, 0, i);

				KSystemMessage	sMsg;
				sprintf(sMsg.szMessage, MSG_CHAT_DELETED_FRIEND, pFriend->m_szName);
				sMsg.eType = SMT_NORMAL;
				sMsg.byConfirmType = SMCT_NONE;
				sMsg.byPriority = 0;
				sMsg.byParamSize = 0;
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);

				delete pFriend;
				return;
			}
			pFriend = (CChatFriend*)pFriend->GetNext();
		}
	}
}

//-------------------------------------------------------------------------
//	���ܣ��յ�������֪ͨĳ�����������
//-------------------------------------------------------------------------
void	KProtocolProcess::s2cChatFriendOffLine(BYTE* pMsg)
{
	CHAT_FRIEND_OFFLINE_SYNC	*pSync = (CHAT_FRIEND_OFFLINE_SYNC*)pMsg;
	Player[CLIENT_PLAYER_INDEX].m_cChat.GetMsgOffLine(pSync->m_dwID);
}

void	KProtocolProcess::s2cSyncAllSkill(BYTE * pMsg)
{
	SKILL_SEND_ALL_SYNC	* pSync = (SKILL_SEND_ALL_SYNC*) pMsg;
	int nSkillCount = (pSync->m_wProtocolLong - 2) / sizeof(SKILL_SEND_ALL_SYNC_DATA);
	int nNpcIndex = Player[CLIENT_PLAYER_INDEX].m_nIndex;
	KSkill * pOrdinSkill = NULL;
	Npc[nNpcIndex].m_SkillList.Clear();
	for (int i = 0; i < nSkillCount; i ++)
	{
		if (pSync->m_sAllSkill[i].SkillId)
		{
			
//			pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(pSync->m_sAllSkill[i].SkillId, pSync->m_sAllSkill[i].SkillLevel);
//			if (!pOrdinSkill) 
//              continue;

			Npc[nNpcIndex].m_SkillList.Add(
				pSync->m_sAllSkill[i].SkillId,
				pSync->m_sAllSkill[i].SkillLevel,
				0
            );
		}
	}

}

void KProtocolProcess::SyncEnd(BYTE* pMsg)
{
	BYTE	SyncEnd = (BYTE)c2s_syncend;
	if (g_pClient)
		g_pClient->SendPackToServer(&SyncEnd, sizeof(BYTE));
	Player[CLIENT_PLAYER_INDEX].SetDefaultImmedSkill();
	g_DebugLog("[TRACE]SyncEnd to Server");
	CoreDataChanged(GDCNI_GAME_START, 0, 0);	
//	g_bPingReply = TRUE;
	// �趨���충��Ƶ��
//	Player[CLIENT_PLAYER_INDEX].m_cChat.LoadTeamName(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name);
//	Player[CLIENT_PLAYER_INDEX].m_cChat.SaveTeamInfo(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name);
//	Player[CLIENT_PLAYER_INDEX].m_cChat.LoadTakeChannelInfo(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name);

	Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SetInstantSpr(enumINSTANT_STATE_LOGIN);

	// ��ʼ������
	g_SubWorldSet.m_cMusic.Start(SubWorld[0].m_SubWorldID, SubWorld[0].m_dwCurrentTime, Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_FightMode);

	// PK��ʼ��
	Player[CLIENT_PLAYER_INDEX].m_cPK.Init();


}

void	KProtocolProcess::s2cTradeChangeState(BYTE* pMsg)
{
	Player[CLIENT_PLAYER_INDEX].s2cTradeChangeState(pMsg);
}

void	KProtocolProcess::s2cNpcSetMenuState(BYTE* pMsg)
{
	NPC_SET_MENU_STATE_SYNC	*pState = (NPC_SET_MENU_STATE_SYNC*)pMsg;
	int		nNpc;
	if (pState->m_dwID == Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_dwID)
	{
		nNpc = Player[CLIENT_PLAYER_INDEX].m_nIndex;
		if (pState->m_btState == PLAYER_MENU_STATE_TRADEOPEN)
		{
			KSystemMessage	sMsg;
			sprintf(sMsg.szMessage, MSG_TRADE_STATE_OPEN);
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
		else if (pState->m_btState == PLAYER_MENU_STATE_NORMAL)
		{
			if (Npc[nNpc].GetMenuState() == PLAYER_MENU_STATE_TRADEOPEN)
			{
				KSystemMessage	sMsg;
				sprintf(sMsg.szMessage, MSG_TRADE_STATE_CLOSE);
				sMsg.eType = SMT_NORMAL;
				sMsg.byConfirmType = SMCT_NONE;
				sMsg.byPriority = 0;
				sMsg.byParamSize = 0;
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
			}
		}
	}
	else
	{
		nNpc = NpcSet.SearchID(pState->m_dwID);
		if (nNpc == 0)
			return;
	}
	if (pState->m_wLength > sizeof(NPC_SET_MENU_STATE_SYNC) - 1 - sizeof(pState->m_szSentence))
		Npc[nNpc].SetMenuState(pState->m_btState, pState->m_szSentence, pState->m_wLength - (sizeof(NPC_SET_MENU_STATE_SYNC) - 1 - sizeof(pState->m_szSentence)));
	else
		Npc[nNpc].SetMenuState(pState->m_btState);
}

void	KProtocolProcess::s2cTradeMoneySync(BYTE* pMsg)
{
	Player[CLIENT_PLAYER_INDEX].s2cTradeMoneySync(pMsg);
}

void	KProtocolProcess::s2cTradeDecision(BYTE* pMsg)
{
	Player[CLIENT_PLAYER_INDEX].s2cTradeDecision(pMsg);
	Player[CLIENT_PLAYER_INDEX].m_ItemList.UnlockOperation();
}

void	KProtocolProcess::s2cUpdateNpcState(BYTE * pMsg)
{
	NPC_SYNC_STATEINFO * pInfo = (NPC_SYNC_STATEINFO*) pMsg;
	int nIndex = NpcSet.SearchID(pInfo->m_ID);	
	
	if (nIndex > 0 && !Npc[nIndex].IsPlayer() && Npc[nIndex].m_RegionIndex >= 0)
	{
		Npc[nIndex].SetNpcState(pInfo->m_btStateInfo);
	}
}

void	KProtocolProcess::s2cTeamInviteAdd(BYTE * pMsg)
{
	Player[CLIENT_PLAYER_INDEX].m_cTeam.ReceiveInvite((TEAM_INVITE_ADD_SYNC*)pMsg);
}

void	KProtocolProcess::s2cTradePressOkSync(BYTE * pMsg)
{
	TRADE_STATE_SYNC	*pPress = (TRADE_STATE_SYNC*)pMsg;
	Player[CLIENT_PLAYER_INDEX].m_cTrade.SetTradeState(pPress->m_btSelfLock, pPress->m_btDestLock, pPress->m_btSelfOk, pPress->m_btDestOk);
}

void	KProtocolProcess::s2cDirectlyCastSkill(BYTE * pMsg)
{
	DWORD	dwNpcID;
	int		nSkillID, nSkillLevel;
	DWORD	MapX, MapY;
	
	dwNpcID = *(DWORD *)&pMsg[1];
	nSkillID = *(int *)&pMsg[5];
	nSkillLevel = *(int *)&pMsg[9];
	MapX = *(int *)&pMsg[13];
	MapY = *(int *)&pMsg[17];
	
	//��ָ��ĳ��Ŀ��ʱ(MapX == -1),MapYΪĿ���NpcdwID����Ҫת���ɱ��ص�NpcIndex����
	if (MapX == -1)
	{
		if (MapY < 0 ) return;
		MapY = NpcSet.SearchID(MapY);
		if (MapY == 0)	return;
		if (Npc[MapY].m_RegionIndex < 0)
			return;
		int nX, nY;
		nX = Npc[MapY].m_MapX;
		nY = Npc[MapY].m_MapY;
		SubWorld[0].Map2Mps(Npc[MapY].m_RegionIndex, nX, nY, 0, 0, (int *)&MapX, (int *)&MapY);
	}
	
	int nIdx = NpcSet.SearchID(dwNpcID);
	
///	if (Player[CLIENT_PLAYER_INDEX].ConformIdx(nIdx))
	{
		_ASSERT (nSkillID > 0 && nSkillLevel > 0);
		KSkill * pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(nSkillID, nSkillLevel);
		if (!pOrdinSkill) 
            return ;
		
        pOrdinSkill->Cast(nIdx, MapX, MapY);
	}

}

void	KProtocolProcess::s2cShowMsg(BYTE *pMsg)
{
	if (!pMsg)
		return;
	SHOW_MSG_SYNC	*pShowMsg = (SHOW_MSG_SYNC*)pMsg;

	switch (pShowMsg->m_wMsgID)
	{
	case enumMSG_ID_TEAM_KICK_One:
		{
			char	szName[32];
			KSystemMessage	sMsg;

			memset(szName, 0, sizeof(szName));
			memcpy(szName, pMsg + sizeof(SHOW_MSG_SYNC) - sizeof(LPVOID), pShowMsg->m_wLength + 1 + sizeof(LPVOID) - sizeof(SHOW_MSG_SYNC));
			if (strcmp(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name, szName) == 0)
			{
				sprintf(sMsg.szMessage, MSG_TEAM_BE_KICKEN);
				sMsg.eType = SMT_TEAM;
				sMsg.byConfirmType = SMCT_CLICK;
				sMsg.byPriority = 1;
			}
			else
			{
				sprintf(sMsg.szMessage, MSG_TEAM_KICK_ONE, szName);
				sMsg.eType = SMT_NORMAL;
				sMsg.byConfirmType = SMCT_NONE;
				sMsg.byPriority = 0;
			}
			sMsg.byParamSize = 0;
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
		break;
	case enumMSG_ID_TEAM_DISMISS:
		{
			if (!Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag)
				break;
			KSystemMessage	sMsg;
			if (Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFigure == TEAM_CAPTAIN)
			{
				sprintf(sMsg.szMessage, MSG_TEAM_DISMISS_CAPTAIN);
			}
			else
			{
				sprintf(sMsg.szMessage, MSG_TEAM_DISMISS_MEMBER, g_Team[0].m_szMemName[0]);
			}
			sMsg.eType = SMT_TEAM;
			sMsg.byConfirmType = SMCT_CLICK;
			sMsg.byPriority = 1;
			sMsg.byParamSize = 0;
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
		break;
	case enumMSG_ID_TEAM_LEAVE:
		{
			if (!Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag)
				break;
			KSystemMessage	sMsg;
			DWORD	dwID = *(DWORD*)(&pShowMsg->m_lpBuf);
			if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_dwID == dwID)
			{
				sprintf(sMsg.szMessage, MSG_TEAM_LEAVE_SELF_MSG, g_Team[0].m_szMemName[0]);
			}
			else
			{
				sprintf(sMsg.szMessage, MSG_TEAM_LEAVE, "����");
				for (int i = 0; i < MAX_TEAM_MEMBER; i++)
				{
					if ((DWORD)g_Team[0].m_nMember[i] == dwID)
					{
						sprintf(sMsg.szMessage, MSG_TEAM_LEAVE, g_Team[0].m_szMemName[i + 1]);
						break;
					}
				}
			}
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
		break;
	case enumMSG_ID_TEAM_REFUSE_INVITE:
		{
			char	szName[32];
			memset(szName, 0, sizeof(szName));
			memcpy(szName, &pShowMsg->m_lpBuf, pShowMsg->m_wLength + 1 + sizeof(LPVOID) - sizeof(SHOW_MSG_SYNC));

			KSystemMessage	sMsg;
			sprintf(sMsg.szMessage, MSG_TEAM_REFUSE_INVITE, szName);
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
		break;
	case enumMSG_ID_TEAM_SELF_ADD:
		{
			KSystemMessage	sMsg;
			sprintf(sMsg.szMessage, MSG_TEAM_SELF_ADD, g_Team[0].m_szMemName[0]);
			sMsg.eType = SMT_TEAM;
			sMsg.byConfirmType = SMCT_UI_TEAM;
			sMsg.byPriority = 3;
			sMsg.byParamSize = 0;
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
		break;
	case enumMSG_ID_TEAM_CHANGE_CAPTAIN_FAIL:
		{
			int		nMember;
			DWORD	dwID = *(DWORD*)(&pShowMsg->m_lpBuf);
			nMember = g_Team[0].FindMemberID(dwID);
			if (nMember < 0)
				break;
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;

			sprintf(sMsg.szMessage, MSG_TEAM_CHANGE_CAPTAIN_FAIL1);
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
			sprintf(sMsg.szMessage, MSG_TEAM_CHANGE_CAPTAIN_FAIL2, g_Team[0].m_szMemName[nMember + 1]);
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
		break;
	case enumMSG_ID_TEAM_CHANGE_CAPTAIN_FAIL2:
		{
			int		nMember;
			DWORD	dwID = *(DWORD*)(&pShowMsg->m_lpBuf);
			nMember = g_Team[0].FindMemberID(dwID);
			if (nMember < 0)
				break;
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;

			sprintf(sMsg.szMessage, MSG_TEAM_CHANGE_CAPTAIN_FAIL1);
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
			sprintf(sMsg.szMessage, MSG_TEAM_CHANGE_CAPTAIN_FAIL3);
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
		break;
	case enumMSG_ID_OBJ_CANNOT_PICKUP:
		{
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;

			strcpy(sMsg.szMessage, MSG_OBJ_CANNOT_PICKUP);
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
		break;
	case enumMSG_ID_OBJ_TOO_FAR:
		{
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			strcpy(sMsg.szMessage, MSG_OBJ_TOO_FAR);
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
		break;
	case enumMSG_ID_DEC_MONEY:
		{
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage, MSG_DEC_MONEY, (int)pShowMsg->m_lpBuf);
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
		break;
	case enumMSG_ID_TRADE_SELF_ROOM_FULL:
		{
			KSystemMessage	sMsg;
			sprintf(sMsg.szMessage, MSG_TRADE_SELF_ROOM_FULL);
			sMsg.eType = SMT_SYSTEM;
			sMsg.byConfirmType = SMCT_CLICK;
			sMsg.byPriority = 1;
			sMsg.byParamSize = 0;
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
		break;
	case enumMSG_ID_TRADE_DEST_ROOM_FULL:
		{
			KSystemMessage	sMsg;
			sprintf(sMsg.szMessage, MSG_TRADE_DEST_ROOM_FULL, Player[CLIENT_PLAYER_INDEX].m_cTrade.m_szDestName);
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
		break;
	case enumMSG_ID_TRADE_REFUSE_APPLY:
		{
			int	nIdx = NpcSet.SearchID(*((DWORD*)&pShowMsg->m_lpBuf));
			if (nIdx <= 0)
				return;
			KSystemMessage	sMsg;
			sprintf(sMsg.szMessage, MSG_TRADE_REFUSE_APPLY, Npc[nIdx].Name);
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
		break;
	case enumMSG_ID_TRADE_TASK_ITEM:
		{
			KSystemMessage	sMsg;
			sprintf(sMsg.szMessage, MSG_TRADE_TASK_ITEM);
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
		break;
	case enumMSG_ID_ITEM_DAMAGED:
		{
			int nItemID = (int)pShowMsg->m_lpBuf;
			int nIdx = ItemSet.SearchID(nItemID);
			if (!nIdx)
				break;

			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 1;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage, MSG_ITEM_DAMAGED, Item[nIdx].GetName());
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
		break;
	case enumMSG_ID_GET_ITEM:
		{
			DWORD	dwID = *(DWORD*)(&pShowMsg->m_lpBuf);
			
			int nItemIdx = ItemSet.SearchID(dwID);
			if (nItemIdx <= 0 || nItemIdx >= MAX_ITEM)
				break;

			char	szName[128];
			KSystemMessage	sMsg;

			strcpy(szName, Item[nItemIdx].GetName());

			if (strlen(szName) >= sizeof(sMsg.szMessage) - strlen(MSG_ADD_ITEM))
				break;
			
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage, MSG_ADD_ITEM, szName);
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
		break;
	case enumMSG_ID_MONEY_CANNOT_PICKUP:
		{
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;

			strcpy(sMsg.szMessage, MSG_MONEY_CANNOT_PICKUP);
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
		break;
	case enumMSG_ID_CANNOT_ADD_TEAM:
		{
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage, MSG_TEAM_CANNOT_CREATE);
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
		break;
	case enumMSG_ID_TARGET_CANNOT_ADD_TEAM:
		{
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage, MSG_TEAM_TARGET_CANNOT_ADD_TEAM);
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
		break;
	case enumMSG_ID_PK_ERROR_1:
		{
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage, MSG_PK_ERROR_1);
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
		break;
	case enumMSG_ID_PK_ERROR_2:
		{
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage, MSG_PK_ERROR_2);
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
		break;
	case enumMSG_ID_PK_ERROR_3:
		{
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage, MSG_PK_ERROR_3);
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
		break;
	case enumMSG_ID_PK_ERROR_4:
		{
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage, MSG_PK_ERROR_4);
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
		break;
	case enumMSG_ID_PK_ERROR_5:
		{
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage, MSG_PK_ERROR_5);
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
		break;
	case enumMSG_ID_PK_ERROR_6:
		{
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage, MSG_PK_ERROR_6);
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
		break;
	case enumMSG_ID_PK_ERROR_7:
		{
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage, MSG_PK_ERROR_7);
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
		break;
	case enumMSG_ID_DEATH_LOSE_ITEM:
		{
			char	szName[128];
			memset(szName, 0, sizeof(szName));
			memcpy(szName, &pShowMsg->m_lpBuf, pShowMsg->m_wLength + 1 + sizeof(LPVOID) - sizeof(SHOW_MSG_SYNC));

			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage, MSG_DEATH_LOSE_ITEM, szName);
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
		break;
	case enumMSG_ID_TONG_REFUSE_ADD:
		{
			char	szName[32];

			memset(szName, 0, sizeof(szName));
			memcpy(szName, pMsg + sizeof(SHOW_MSG_SYNC) - sizeof(LPVOID), pShowMsg->m_wLength + 1 + sizeof(LPVOID) - sizeof(SHOW_MSG_SYNC));

			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage, MSG_TONG_REFUSE_ADD, szName);
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
		break;
	case enumMSG_ID_TONG_BE_KICK:
		{
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage, MSG_TONG_BE_KICKED);
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);

			KUiGameObjectWithName	sUi;
			strcpy(sUi.szName, Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name);
			sUi.nData = TONG_ACTION_DISMISS;
			sUi.nParam = 0;
			sUi.uParam = 0;
			sUi.szString[0] = 0;
			CoreDataChanged(GDCNI_TONG_ACTION_RESULT, (unsigned int)&sUi, 1);
		}
		break;
	case enumMSG_ID_TONG_LEAVE_SUCCESS:
		{
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage, MSG_TONG_LEAVE_SUCCESS);
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
		break;
	case enumMSG_ID_TONG_LEAVE_FAIL:
		{
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage, MSG_TONG_LEAVE_FAIL);
//			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
		break;
	case enumMSG_ID_TONG_CHANGE_AS_MASTER:
		{
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage, MSG_TONG_CHANGE_AS_MASTER);
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);

			KUiGameObjectWithName	sUi;
			strcpy(sUi.szName, Player[CLIENT_PLAYER_INDEX].m_cTong.m_szMasterName);
			sUi.nData = TONG_ACTION_DEMISE;
			sUi.nParam = 0;
			sUi.uParam = 0;
			sUi.szString[0] = 0;
			CoreDataChanged(GDCNI_TONG_ACTION_RESULT, (unsigned int)&sUi, 1);
		}
		break;
	case enumMSG_ID_TONG_CHANGE_AS_MEMBER:
		{
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage, MSG_TONG_CHANGE_AS_MEMBER);
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);

			KUiGameObjectWithName	sUi;
			strcpy(sUi.szName, Player[CLIENT_PLAYER_INDEX].m_cTong.m_szMasterName);
			sUi.nData = TONG_ACTION_DEMISE;
			sUi.nParam = 0;
			sUi.uParam = 0;
			sUi.szString[0] = 0;
			CoreDataChanged(GDCNI_TONG_ACTION_RESULT, (unsigned int)&sUi, 1);
		}
		break;

	default:
		break;
	}
}

void	KProtocolProcess::SyncStateEffect(BYTE* pMsg)
{
	STATE_EFFECT_SYNC*	pSync = (STATE_EFFECT_SYNC *)pMsg;

	int nDataNum = MAX_SKILL_STATE - (sizeof(STATE_EFFECT_SYNC) - pSync->m_wLength) / sizeof(KMagicAttrib);
	Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SetStateSkillEffect(Player[CLIENT_PLAYER_INDEX].m_nIndex, pSync->m_dwSkillID, pSync->m_nLevel, pSync->m_MagicAttrib, nDataNum, pSync->m_nTime);
}

void	KProtocolProcess::s2cTradeApplyStart(BYTE* pMsg)
{
	Player[CLIENT_PLAYER_INDEX].s2cTradeApplyStart(pMsg);
}

void	KProtocolProcess::s2cItemAutoMove(BYTE* pMsg)
{
	ITEM_AUTO_MOVE_SYNC	*pSync = (ITEM_AUTO_MOVE_SYNC*)pMsg;
	ItemPos	sSrc, sDest;
	sSrc.nPlace = pSync->m_btSrcPos;
	sSrc.nX = pSync->m_btSrcX;
	sSrc.nY = pSync->m_btSrcY;
	sDest.nPlace = pSync->m_btDestPos;
	sDest.nX = pSync->m_btDestX;
	sDest.nY = pSync->m_btDestY;
	
	Player[CLIENT_PLAYER_INDEX].m_ItemList.AutoMoveItem(sSrc, sDest);
}

void KProtocolProcess::FinishedItemExchange(BYTE* pMsg)
{
	Player[CLIENT_PLAYER_INDEX].m_ItemList.UnlockOperation();
}

extern IClientCallback* l_pDataChangedNotifyFunc;

void KProtocolProcess::s2cExtend(BYTE* pMsg)
{
	EXTEND_HEADER* pHeader = (EXTEND_HEADER*)(pMsg + sizeof(tagExtendProtoHeader));

	if (pHeader->ProtocolFamily == pf_playercommunity)
	{
		if (pHeader->ProtocolID == playercomm_s2c_notifychannelid)
		{
			PLAYERCOMM_NOTIFYCHANNELID* pNChann = (PLAYERCOMM_NOTIFYCHANNELID*)pHeader;

			l_pDataChangedNotifyFunc->NotifyChannelID(pNChann->channel, pNChann->channelid, pNChann->cost);
		}
	}
}

void KProtocolProcess::s2cExtendChat(BYTE* pMsg)
{
	tagExtendProtoHeader* pExHdr = (tagExtendProtoHeader*)pMsg;
	void* pExPckg = pExHdr + 1;
	BYTE protocol = *(BYTE*)(pExPckg);

	if (protocol == chat_someonechat)
	{
		CHAT_SOMEONECHAT_SYNC* pCscSync = (CHAT_SOMEONECHAT_SYNC*)pExPckg;

		l_pDataChangedNotifyFunc->MSNMessageArrival(
			pCscSync->someone, Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name,
			(const char*)(pCscSync + 1), pCscSync->sentlen, true);
	}
	else if (protocol == chat_channelchat)
	{
		CHAT_CHANNELCHAT_SYNC* pCccSync = (CHAT_CHANNELCHAT_SYNC*)pExPckg;
		l_pDataChangedNotifyFunc->ChannelMessageArrival(pCccSync->figure,
			pCccSync->channelid, pCccSync->someone,
			(const char*)(pCccSync + 1), pCccSync->sentlen, true);

	}
	else if (protocol == chat_feedback)
	{
		////X
		CHAT_FEEDBACK* pCfb = (CHAT_FEEDBACK*)pExPckg;
		DWORD* pChannelid = (DWORD*)(pCfb + 1);

		if (*pChannelid == -1)
		{//someone
			char* pDstName = (char*)(pChannelid + 1);
			BYTE* pSentlen = (BYTE*)(pDstName + _NAME_LEN);
			void* pSent = pSentlen + 1;

			l_pDataChangedNotifyFunc->MSNMessageArrival(
				Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name, pDstName,
				(const char*)pSent, *pSentlen, pCfb->code != codeFail);
		}
		else
		{//channel
			BYTE* pSentlen = (BYTE*)(pChannelid + 1);
			void* pSent = pSentlen + 1;
			l_pDataChangedNotifyFunc->ChannelMessageArrival(0,
				*pChannelid, Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name,
				(const char*)pSent, *pSentlen, pCfb->code != codeFail);
		}
		////X
	}
}


static BOOL sParseUGName(const std::string& name, std::string* pUnit, std::string* pGroup)
{
	static const char char_split = '\n';

	size_t pos = name.find(char_split);
	if (pos == name.npos)
	{
		if (pUnit)
			pUnit->resize(0);
		if (pGroup)
			pGroup->assign(name);
	}
	else
	{
		std::string::const_iterator itSplit = name.begin() + pos;

		if (pUnit)
			pUnit->assign(name.begin(), itSplit);
		if (pGroup)
			pGroup->assign(itSplit + 1, name.end());
	}

	return TRUE;
}

void KProtocolProcess::s2cExtendFriend(BYTE* pMsg)
{
	tagExtendProtoHeader* pExHdr = (tagExtendProtoHeader*)pMsg;
	void* pExPckg = pExHdr + 1;

	EXTEND_HEADER* pHeader = (EXTEND_HEADER*)(pExPckg);

	if (pHeader->ProtocolFamily == pf_tong)
	{
		_ASSERT(FALSE);
		//tong message
		//...
	}
	else if (pHeader->ProtocolFamily == pf_friend)
	{
		if (pHeader->ProtocolID == friend_c2c_askaddfriend)
		{
			ASK_ADDFRIEND_SYNC* pAafSync = (ASK_ADDFRIEND_SYNC*)pHeader;
			l_pDataChangedNotifyFunc->FriendInvite(pAafSync->srcrole);
		}
		else if (pHeader->ProtocolID == friend_c2c_repaddfriend)
		{
			REP_ADDFRIEND_SYNC* pRafSync = (REP_ADDFRIEND_SYNC*)pHeader;
			l_pDataChangedNotifyFunc->AddFriend(pRafSync->srcrole, pRafSync->answer);
		}
		else if (pHeader->ProtocolID == friend_s2c_repsyncfriendlist)
		{
			REP_SYNCFRIENDLIST* pRsfl = (REP_SYNCFRIENDLIST*)pHeader;
			char* pGroupTag = (char*)(pRsfl + 1);
			if (*pGroupTag != specGroup)
				goto on_error;
			{{
			char* pGroup = (char*)(pGroupTag + 1);
on_newgroup:
			std::string theUnit, theGroup;
			sParseUGName(std::string(pGroup), &theUnit, &theGroup);

			for (char* pRoleTag = pGroup + strlen(pGroup) + 1; ; )
			{
				if (*pRoleTag == specOver)
					goto on_over;
				else if (*pRoleTag == specGroup)
				{
					pGroup = pRoleTag + 1;
					goto on_newgroup;
				}
				else if (*pRoleTag == specRole)
				{
					char* pState = pRoleTag + 1;
					char* pRole = pState + 1;
					l_pDataChangedNotifyFunc->FriendInfo(pRole, (char*)theUnit.c_str(), (char*)theGroup.c_str(), (BYTE)*pState);
					pRoleTag = pRole + strlen(pRole) + 1;
					continue;
				}
				else
					goto on_error;
			}
			}}
on_error:
			_ASSERT(FALSE);
on_over:
			0;
		}
		else if (pHeader->ProtocolID == friend_s2c_friendstate)
		{
			FRIEND_STATE* pFs = (FRIEND_STATE*)pHeader;
			for (char* pRole = (char*)(pFs + 1); *pRole; pRole += strlen(pRole) + 1)
				l_pDataChangedNotifyFunc->FriendStatus(pRole, pFs->state);
		}
		else if (pHeader->ProtocolID == friend_s2c_syncassociate)
		{
			FRIEND_SYNCASSOCIATE* pFsa = (FRIEND_SYNCASSOCIATE*)pHeader;

			char* szGroup = (char*)(pFsa + 1);
			std::string group(szGroup);

			std::string theUnit, theGroup;
			sParseUGName(group, &theUnit, &theGroup);
			_ASSERT(theGroup.empty());

			for (char* szRole = szGroup + group.size() + 1; *szRole; szRole += strlen(szRole) + 1)
				l_pDataChangedNotifyFunc->AddPeople((char*)theUnit.c_str(), szRole);
		}
	}
}




void KProtocolProcess::s2cExtendTong(BYTE* pMsg)
{
	S2C_TONG_HEAD	*pHead = (S2C_TONG_HEAD*)pMsg;
	switch (pHead->m_btMsgId)
	{
	case enumTONG_SYNC_ID_CREATE_FAIL:
		{
			TONG_CREATE_FAIL_SYNC *pFail = (TONG_CREATE_FAIL_SYNC*)pMsg;

			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;

			sprintf(sMsg.szMessage, "��ᴴ��ʧ�ܣ�");
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);

			switch (pFail->m_btFailId)
			{
			case enumTONG_CREATE_ERROR_ID1:		// Player[m_nPlayerIndex].m_nIndex <= 0
				break;
			case enumTONG_CREATE_ERROR_ID2:		// ���׹�����
				break;
			case enumTONG_CREATE_ERROR_ID3:		// ���������
				sprintf(sMsg.szMessage, MSG_TONG_CREATE_ERROR09);
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
				break;
			case enumTONG_CREATE_ERROR_ID4:		// �����Ӫ����
				sprintf(sMsg.szMessage, MSG_TONG_CREATE_ERROR02);
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
				break;
			case enumTONG_CREATE_ERROR_ID5:		// �Ѿ��ǰ���Ա
				sprintf(sMsg.szMessage, MSG_TONG_CREATE_ERROR03);
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
				break;
			case enumTONG_CREATE_ERROR_ID6:		// �Լ�����Ӫ����
				sprintf(sMsg.szMessage, MSG_TONG_CREATE_ERROR04);
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
				break;
			case enumTONG_CREATE_ERROR_ID7:		// �ȼ�����
				sprintf(sMsg.szMessage, MSG_TONG_CREATE_ERROR05);
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
				break;
			case enumTONG_CREATE_ERROR_ID8:		// Ǯ����
				sprintf(sMsg.szMessage, MSG_TONG_CREATE_ERROR10);
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
				break;
			case enumTONG_CREATE_ERROR_ID9:		// ��Ӳ��ܽ����
				sprintf(sMsg.szMessage, MSG_TONG_CREATE_ERROR08);
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
				break;
			case enumTONG_CREATE_ERROR_ID10:	// ���ģ�����
				break;
			case enumTONG_CREATE_ERROR_ID11:	// �����ַ�������
				sprintf(sMsg.szMessage, MSG_TONG_CREATE_ERROR11);
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
				break;
			case enumTONG_CREATE_ERROR_ID12:	// �����ַ�������
				sprintf(sMsg.szMessage, MSG_TONG_CREATE_ERROR11);
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
				break;
			case enumTONG_CREATE_ERROR_ID13:	// ���ͬ������
				sprintf(sMsg.szMessage, MSG_TONG_CREATE_ERROR11);
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
				break;
			case enumTONG_CREATE_ERROR_ID14:	// ������ʧ��
				break;
			}
		}
		break;
	case enumTONG_SYNC_ID_TRANSFER_ADD_APPLY:
		{
			TONG_APPLY_ADD_SYNC	*pApply = (TONG_APPLY_ADD_SYNC*)pMsg;
			char	szName[32];
			DWORD	dwNameID;
			int		nPlayerIdx;

			memset(szName, 0, sizeof(szName));
			memcpy(szName, pApply->m_szName, pApply->m_wLength + 1 + sizeof(pApply->m_szName) - sizeof(TONG_APPLY_ADD_SYNC));
			dwNameID = g_FileName2Id(szName);
			nPlayerIdx = pApply->m_nPlayerIdx;

			// �����淢��Ϣ���յ����룬�Ƿ�ͬ��
			KSystemMessage	sMsg;
			sprintf(sMsg.szMessage, "%s��������ᣡ", szName);
			sMsg.eType = SMT_CLIQUE;
			sMsg.byConfirmType = SMCT_UI_TONG_JOIN_APPLY;
			sMsg.byPriority = 3;
			sMsg.byParamSize = sizeof(KUiPlayerItem);
			
			KUiPlayerItem	player;
			strcpy(player.Name, szName);
			player.nIndex = pApply->m_nPlayerIdx;
			player.uId = 0;
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, (int)&player);

		}
		break;
	case enumTONG_SYNC_ID_ADD:
		{
			TONG_Add_SYNC	*pAdd = (TONG_Add_SYNC*)pMsg;
			char	szName[32], szTitle[32], szMaster[32];

			memcpy(szName, pAdd->m_szTongName,sizeof(szName));
			memcpy(szTitle, pAdd->m_szTitle, sizeof(szTitle));
			memcpy(szMaster, pAdd->m_szMaster, sizeof(szMaster));

			Player[CLIENT_PLAYER_INDEX].m_cTong.AddTong(pAdd->m_btCamp, szName, szTitle, szMaster);
			// ֪ͨ�����������
			KUiGameObjectWithName	sUi;
			memset(&sUi, 0, sizeof(sUi));
			strcpy(sUi.szName, szName);
			sUi.nData = TONG_ACTION_APPLY;
			strcpy(sUi.szString, szTitle);
			CoreDataChanged(GDCNI_TONG_ACTION_RESULT, (unsigned int)&sUi, 1);
		/*------------------------------*/
			CoreDataChanged(GDCNI_PLAYER_BASE_INFO, 0, 0);
		}
		break;

	case enumTONG_SYNC_ID_HEAD_INFO:
		{
			TONG_HEAD_INFO_SYNC	*pInfo = (TONG_HEAD_INFO_SYNC*)pMsg;

			// ֪ͨ����õ�ĳ�����Ϣ
			int nIdx = NpcSet.SearchID(pInfo->m_dwNpcID);
			if (nIdx <= 0)
				break;

			KUiPlayerRelationWithOther	sUi;
			sUi.nIndex = nIdx;
			sUi.uId = Npc[nIdx].m_dwID;
			if (nIdx == Player[CLIENT_PLAYER_INDEX].m_nIndex)
				sUi.nRelation = Player[CLIENT_PLAYER_INDEX].m_cTong.GetFigure();
			else
				sUi.nRelation = -1;
			sUi.nData = 0;
			sUi.nParam = 0;
			strcpy(sUi.Name, Npc[nIdx].Name);

			KTongInfo	sInfo;
			memset(&sInfo, 0, sizeof(sInfo));
			sInfo.nFaction = pInfo->m_btCamp;
			sInfo.nMemberCount = (int)pInfo->m_dwMemberNum;
			sInfo.nManagerCount = (int)pInfo->m_btManagerNum;
			sInfo.nDirectorCount = (int)pInfo->m_btDirectorNum;
			sInfo.nMoney = pInfo->m_dwMoney;
			sInfo.nLevel = pInfo->m_btLevel;
			strcpy(sInfo.szMasterName, pInfo->m_sMember[0].m_szName);
			strcpy(sInfo.szName, pInfo->m_szTongName);

			CoreDataChanged(GDCNI_TONG_INFO, (unsigned int)&sUi, (unsigned int)&sInfo);

			KUiGameObjectWithName	sObj;
			strcpy(sObj.szName, pInfo->m_szTongName);
			sObj.nData = enumTONG_FIGURE_DIRECTOR;
			sObj.nParam = 1;
			sObj.uParam = defTONG_MAX_DIRECTOR;
			sObj.szString[0] = 0;

			KTongMemberItem	sItem[defTONG_MAX_DIRECTOR];
			memset(sItem, 0, sizeof(sItem));
			for (int i = 0; i < (pInfo->m_btDirectorNum-1); i++)
			{
				sItem[i].nData = enumTONG_FIGURE_DIRECTOR;
				strcpy(sItem[i].Name, pInfo->m_sMember[i + 1].m_szName);
				strcpy(sItem[i].szAgname, pInfo->m_sMember[i + 1].m_szTitle);
				sItem[i].nStatus = pInfo->m_sMember[i + 1].m_nStatus;
				sItem[i].nPlayerLevel = pInfo->m_sMember[i + 1].m_PlayerLevel;
				sItem[i].nPlayerPoint = pInfo->m_sMember[i + 1].m_PlayerPoint;
				sItem[i].nPlayerTime = pInfo->m_sMember[i + 1].m_PlayerTime;
				sItem[i].nPlayerTimeEnd = pInfo->m_sMember[i + 1].m_PlayerTimeEnd;

//				g_DebugLog("Truyen 3 - %d\n",sItem[i].nStatus);

			}


			CoreDataChanged(GDCNI_TONG_MEMBER_LIST, (unsigned int)&sObj, (unsigned int)sItem);
		}
		break;
	case enumTONG_SYNC_ID_SELF_INFO:
		{
			TONG_SELF_INFO_SYNC	*pInfo = (TONG_SELF_INFO_SYNC*)pMsg;
			Player[CLIENT_PLAYER_INDEX].m_cTong.SetSelfInfo(pInfo);
			// ֪ͨ�����������
			CoreDataChanged(GDCNI_PLAYER_BASE_INFO, 0, 0);
		}
		break;
	case enumTONG_SYNC_ID_MANAGER_INFO:
		{
			TONG_MANAGER_INFO_SYNC	*pInfo = (TONG_MANAGER_INFO_SYNC*)pMsg;

			// ֪ͨ����õ�ĳ���ӳ���Ϣ
			KUiGameObjectWithName	sObj;
			strcpy(sObj.szName, pInfo->m_szTongName);
			sObj.nData = enumTONG_FIGURE_MANAGER;
			sObj.nParam = pInfo->m_btStateNo;
			sObj.uParam = pInfo->m_btCurNum;
			sObj.szString[0] = 0;

			KTongMemberItem	sItem[defTONG_ONE_PAGE_MAX_NUM];
			memset(sItem, 0, sizeof(sItem));
			for (int i = 0; i < pInfo->m_btCurNum; i++)
			{
				sItem[i].nData = enumTONG_FIGURE_MANAGER;
				strcpy(sItem[i].Name, pInfo->m_sMember[i].m_szName);
				strcpy(sItem[i].szAgname, pInfo->m_sMember[i].m_szTitle);
				sItem[i].nStatus = pInfo->m_sMember[i].m_nStatus;
				sItem[i].nPlayerLevel = pInfo->m_sMember[i].m_PlayerLevel;
				sItem[i].nPlayerPoint = pInfo->m_sMember[i].m_PlayerPoint;
				sItem[i].nPlayerTime = pInfo->m_sMember[i].m_PlayerTime;
				sItem[i].nPlayerTimeEnd = pInfo->m_sMember[i].m_PlayerTimeEnd;


			//	g_DebugLog("Truyen 1\n");

			}

			CoreDataChanged(GDCNI_TONG_MEMBER_LIST, (unsigned int)&sObj, (unsigned int)sItem);
		}
		break;
	case enumTONG_SYNC_ID_MEMBER_INFO:
		{
			TONG_MEMBER_INFO_SYNC	*pInfo = (TONG_MEMBER_INFO_SYNC*)pMsg;

			// ֪ͨ����õ�ĳ��������Ϣ
			KUiGameObjectWithName	sObj;
			strcpy(sObj.szName, pInfo->m_szTongName);
			sObj.nData = enumTONG_FIGURE_MEMBER;
			sObj.nParam = pInfo->m_btStateNo;
			sObj.uParam = pInfo->m_btCurNum;
			sObj.szString[0] = 0;

			KTongMemberItem	sItem[defTONG_ONE_PAGE_MAX_NUM];
			memset(sItem, 0, sizeof(sItem));
			for (int i = 0; i < pInfo->m_btCurNum; i++)
			{
				sItem[i].nData = enumTONG_FIGURE_MEMBER;
				strcpy(sItem[i].Name, pInfo->m_sMember[i].m_szName);
				strcpy(sItem[i].szAgname, pInfo->m_sMember[i].m_szTitle);
				sItem[i].nStatus = pInfo->m_sMember[i].m_nStatus;
				sItem[i].nPlayerLevel = pInfo->m_sMember[i].m_PlayerLevel;
				sItem[i].nPlayerPoint = pInfo->m_sMember[i].m_PlayerPoint;
				sItem[i].nPlayerTime = pInfo->m_sMember[i].m_PlayerTime;
				sItem[i].nPlayerTimeEnd = pInfo->m_sMember[i].m_PlayerTimeEnd;
			//	g_DebugLog("Truyen 2\n");
			}

			CoreDataChanged(GDCNI_TONG_MEMBER_LIST, (unsigned int)&sObj, (unsigned int)sItem);
		}
		break;
	case enumTONG_SYNC_ID_INSTATE:
		{
			TONG_INSTATE_SYNC	*pInstate = (TONG_INSTATE_SYNC*)pMsg;
			// ֪ͨ���������Ƿ�ɹ�
			KUiGameObjectWithName	sUi;
			strcpy(sUi.szName, pInstate->m_szName);
			sUi.nData = TONG_ACTION_ASSIGN;
			sUi.nParam = pInstate->m_btNewFigure;
			sUi.uParam = pInstate->m_btOldFigure;
			strcpy(sUi.szString, pInstate->m_szTitle);
			CoreDataChanged(GDCNI_TONG_ACTION_RESULT, (unsigned int)&sUi, pInstate->m_btSuccessFlag);
		}
		break;
	case enumTONG_SYNC_ID_KICK:
		{
			TONG_KICK_SYNC	*pKick = (TONG_KICK_SYNC*)pMsg;
			// ֪ͨ���������Ƿ�ɹ�
			KUiGameObjectWithName	sUi;
			strcpy(sUi.szName, pKick->m_szName);
			sUi.nData = TONG_ACTION_DISMISS;
			sUi.nParam = pKick->m_btFigure;
			sUi.uParam = pKick->m_btPos;
			sUi.szString[0] = 0;
			CoreDataChanged(GDCNI_TONG_ACTION_RESULT, (unsigned int)&sUi, pKick->m_btSuccessFlag);
		}
		break;
	case enumTONG_SYNC_ID_CHANGE_MASTER_FAIL:
		{
			TONG_CHANGE_MASTER_FAIL_SYNC	*pFail = (TONG_CHANGE_MASTER_FAIL_SYNC*)pMsg;

			switch (pFail->m_btFailID)
			{
			case 0:		// �Է�������
				{
					KSystemMessage	sMsg;
					sMsg.eType = SMT_NORMAL;
					sMsg.byConfirmType = SMCT_NONE;
					sMsg.byPriority = 0;
					sMsg.byParamSize = 0;
					sprintf(sMsg.szMessage, "��λʧ�ܣ��Է������ߣ�");
					CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
				}
				break;
			case 1:		// �Է�����������
				{
					KSystemMessage	sMsg;
					sMsg.eType = SMT_NORMAL;
					sMsg.byConfirmType = SMCT_NONE;
					sMsg.byPriority = 0;
					sMsg.byParamSize = 0;
					sprintf(sMsg.szMessage, "��λʧ�ܣ��Է�����������");
					CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
				}
				break;
			case 2:		// ���ֲ��ԣ�
				{
					KSystemMessage	sMsg;
					sMsg.eType = SMT_NORMAL;
					sMsg.byConfirmType = SMCT_NONE;
					sMsg.byPriority = 0;
					sMsg.byParamSize = 0;
					sprintf(sMsg.szMessage, "��λʧ�ܣ��������޴��ˣ�");
					CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
				}
				break;

			default:
				break;
			}
		}
		break;
	default:
		break;
	}
}


void KProtocolProcess::s2cChangeWeather(BYTE* pMsg)
{
	g_ScenePlace.ChangeWeather(((SYNC_WEATHER*)pMsg)->WeatherID);
}

void	KProtocolProcess::s2cPKSyncNormalFlag(BYTE* pMsg)
{
	PK_NORMAL_FLAG_SYNC	*pFlag = (PK_NORMAL_FLAG_SYNC*)pMsg;
	Player[CLIENT_PLAYER_INDEX].m_cPK.SetNormalPKState(pFlag->m_btFlag);
}

void	KProtocolProcess::s2cPKSyncEnmityState(BYTE* pMsg)
{
	PK_ENMITY_STATE_SYNC	*pState = (PK_ENMITY_STATE_SYNC*)pMsg;
	char	szName[32];

	memset(szName, 0, sizeof(szName));
	memcpy(szName, pState->m_szName, pState->m_wLength + 1 + sizeof(pState->m_szName) - sizeof(PK_ENMITY_STATE_SYNC));
	Player[CLIENT_PLAYER_INDEX].m_cPK.SetEnmityPKState(pState->m_btState, pState->m_dwNpcID, szName);
}

void	KProtocolProcess::s2cPKSyncExerciseState(BYTE* pMsg)
{
	PK_EXERCISE_STATE_SYNC	*pState = (PK_EXERCISE_STATE_SYNC*)pMsg;
	char	szName[32];
	memset(szName, 0, sizeof(szName));
	memcpy(szName, pState->m_szName, pState->m_wLength + 1 + sizeof(pState->m_szName) - sizeof(PK_EXERCISE_STATE_SYNC));
	Player[CLIENT_PLAYER_INDEX].m_cPK.SetExercisePKState(pState->m_btState, pState->m_dwNpcID, szName);
}

void	KProtocolProcess::s2cPKValueSync(BYTE* pMsg)
{
	PK_VALUE_SYNC	*pValue = (PK_VALUE_SYNC*)pMsg;
	Player[CLIENT_PLAYER_INDEX].m_cPK.SetPKValue(pValue->m_nPKValue);
}

void	KProtocolProcess::s2cViewEquip(BYTE* pMsg)
{
	g_cViewItem.GetData(pMsg);
}

void	KProtocolProcess::s2cTongCreate(BYTE* pMsg)
{
	Player[CLIENT_PLAYER_INDEX].m_cTong.Create((TONG_CREATE_SYNC*)pMsg);
}

void	KProtocolProcess::s2cNpcGoldChange(BYTE* pMsg)
{

}

void	KProtocolProcess::ItemChangeDurability(BYTE* pMsg)
{
	ITEM_DURABILITY_CHANGE	*pIDC = (ITEM_DURABILITY_CHANGE *)pMsg;

	int nIdx = ItemSet.SearchID(pIDC->dwItemID);
	
	if (nIdx)
	{
		

		if ((Item[nIdx].GetDurability() + pIDC->nChange) == 0)
		{


	   int nNpcIdx = Player[CLIENT_PLAYER_INDEX].m_nIndex;

	   if (nNpcIdx > 0)
	   {
	    Player[CLIENT_PLAYER_INDEX].m_ItemList.RemoveMagicDurability(nIdx);
	   }

	

		}

		Item[nIdx].SetDurability(Item[nIdx].GetDurability() + pIDC->nChange);
		_ASSERT(Item[nIdx].GetDurability() > 0);
		if (Item[nIdx].GetDurability() <= 3)
		{
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 1;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage, MSG_ITEM_NEARLY_DAMAGED, Item[nIdx].GetName());
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}







	}
}


void	KProtocolProcess::PlayerBlockItemReturn(BYTE* pMsg)
{
	PLAYER_BLOCK_ITEM_RETURN_COMMAND	*pIDC = (PLAYER_BLOCK_ITEM_RETURN_COMMAND *)pMsg;

	int nIdx = ItemSet.SearchID(pIDC->m_ID);
	
	if (nIdx)
	{
		Item[nIdx].m_GeneratorParam.nVersion = pIDC->m_Vesion;
		Player[CLIENT_PLAYER_INDEX].m_ItemList.UnlockOperation();
	
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 1;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage,"%s%s%s", "�� ��nh b�o hi�m trang b?", Item[nIdx].GetName()," !");
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		
	}
}



void	KProtocolProcess::PlayerUnBlockItemReturn(BYTE* pMsg)
{
	PLAYER_UNBLOCK_ITEM_RETURN_COMMAND	*pIDC = (PLAYER_UNBLOCK_ITEM_RETURN_COMMAND *)pMsg;

	int nIdx = ItemSet.SearchID(pIDC->m_ID);
	
	if (nIdx)
	{
		 Item[nIdx].m_GeneratorParam.nVersion = pIDC->m_Vesion;
		 Player[CLIENT_PLAYER_INDEX].m_ItemList.UnlockOperation();
	
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 1;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage,"%s%s%s", "�� x�c nh�n th�o b�o hi�m trang b?", Item[nIdx].GetName()," !");
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
	
	}
}


void	KProtocolProcess::PlayerOpenShopReturn(BYTE* pMsg)
{
	
	PLAYER_SHOP_OPEN_RETURN_COMMAND	*pIDC = (PLAYER_SHOP_OPEN_RETURN_COMMAND *)pMsg;

	
	memcpy(Player[CLIENT_PLAYER_INDEX].m_szNameOpenShop, pIDC->szName, sizeof(Player[CLIENT_PLAYER_INDEX].m_szNameOpenShop));



	
	memcpy(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_szNameOpenShop, pIDC->szName, sizeof(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_szNameOpenShop));



	if (pIDC->m_dwTime)
	{
	Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SendCommand(do_sit);
	Player[CLIENT_PLAYER_INDEX].m_dwTimeOpenShop = pIDC->m_dwTime;
	Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_dwTimeOpenShop = pIDC->m_dwTime;
	CoreDataChanged(GDCNI_SHOP_OPEN_CHECK, 1, 0);
	}
	else 
	{
	Player[CLIENT_PLAYER_INDEX].m_dwTimeOpenShop = pIDC->m_dwTime;
	Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SendCommand(do_stand);
	Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_dwTimeOpenShop = pIDC->m_dwTime;
	CoreDataChanged(GDCNI_SHOP_OPEN_CHECK, 0, 0);
	}



	
	
	

}





void	KProtocolProcess::PlayerShopPriceReturn(BYTE* pMsg)
{
	PLAYER_SHOP_PRICE_RETURN_COMMAND	*pIDC = (PLAYER_SHOP_PRICE_RETURN_COMMAND *)pMsg;

	int nIdx = ItemSet.SearchID(pIDC->m_ID);
	
	if (nIdx)
	{
		 Item[nIdx].m_nPriceShop = pIDC->m_Price;
		Item[nIdx].m_CommonAttrib.iShopCost = pIDC->m_Price;
		 Player[CLIENT_PLAYER_INDEX].m_ItemList.UnlockOperation();
	
	}
}



void	KProtocolProcess::PlayerUpdateShopPrice(BYTE* pMsg)
{
	PLAYER_UPDATE_SHOP_PRICE_COMMAND	*pIDC = (PLAYER_UPDATE_SHOP_PRICE_COMMAND *)pMsg;

	int nIdx = ItemSet.SearchID(pIDC->m_ID);
	
	if (nIdx)
	{
		 Item[nIdx].m_nPriceShop = pIDC->m_Price;
		 Item[nIdx].m_CommonAttrib.iShopCost = pIDC->m_Price;
	}
}






void	KProtocolProcess::PlayerUpdateItemVertion(BYTE* pMsg)
{
	PLAYER_ITEM_UPDATE_VERSION	*pIDC = (PLAYER_ITEM_UPDATE_VERSION *)pMsg;

	int nIdx = ItemSet.SearchID(pIDC->m_ID);
	
	if (nIdx)
	{
		 Item[nIdx].SetVersion(pIDC->m_Vesion);
	}
}






void	KProtocolProcess::PlayerBlockItemNow(BYTE* pMsg)
{
	PLAYER_UNBLOCK_ITEM_NOW_COMMAND	*pIDC = (PLAYER_UNBLOCK_ITEM_NOW_COMMAND *)pMsg;

	int nIdx = ItemSet.SearchID(pIDC->m_ID);
	
	if (nIdx)
	{
		 Item[nIdx].m_GeneratorParam.nVersion = 1;
		 
	
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 1;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage,"%s%s%s", "�� x�c nh�n th�o b�o hi�m trang b� ", Item[nIdx].GetName()," !");
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
	
	}
}




void KProtocolProcess::LadderList(BYTE* pMsg)
{
	LADDER_LIST*	pList = (LADDER_LIST *)pMsg;
	KRankIndex		*pLadderListIndex = NULL;
	
	if (pList->nCount > 0 && pList->nCount < enumLadderEnd)
	{
		pLadderListIndex = new KRankIndex[pList->nCount];
	}
	if (pLadderListIndex)
	{
		for (int i = 0; i < pList->nCount; i++)
		{
			pLadderListIndex[i].usIndexId = pList->dwLadderID[i];
			pLadderListIndex[i].bValueAppened = true;
			pLadderListIndex[i].bSortFlag = true;
		}
		CoreDataChanged(GDCNII_RANK_INDEX_LIST_ARRIVE, pList->nCount, (int)pLadderListIndex);
		delete [] pLadderListIndex;
		pLadderListIndex = NULL;
	}
}

void KProtocolProcess::LadderResult(BYTE* pMsg)
{
	LADDER_DATA*	pLadderData = (LADDER_DATA *)pMsg;
	KRankMessage*	pLadderMessage = NULL;

	pLadderMessage = new KRankMessage[10];
	if (pLadderMessage)
	{
		for (int i = 0; i < 10; i++)
		{
			pLadderMessage[i].usMsgLen = strlen(pLadderData->StatData[i].Name);
			strcpy(pLadderMessage[i].szMsg, pLadderData->StatData[i].Name);
			pLadderMessage[i].nValueAppend = pLadderData->StatData[i].nValue;
			pLadderMessage[i].cSortFlag = (char)pLadderData->StatData[i].bySort;
			pLadderMessage[i].HeightValua = pLadderData->StatData[i].nHeightValue;
		}
		unsigned int uParam = 10 | (((WORD)pLadderData->dwLadderID) << 16);
		CoreDataChanged(GDCNII_RANK_INFORMATION_ARRIVE, uParam, (int)pLadderMessage);
		delete [] pLadderMessage;
		pLadderMessage = NULL;
	}
}
// MASK
void KProtocolProcess::SyncMaskLock(BYTE* pMsg)
	{
		MASK_SYNC *pInfo = (MASK_SYNC *)pMsg;
		Player[CLIENT_PLAYER_INDEX].m_ItemList.SetMaskLock(pInfo->ID);
	}
// END	

void KProtocolProcess::s2cRefestItem(BYTE* pMsg)
{
	S2C_ITEM_REFRESH *pItemSync = (S2C_ITEM_REFRESH*)pMsg;
	int nIdx2 = (pItemSync->m_nId);
	int nIdx = ItemSet.SearchID(pItemSync->m_dwID);
	if ( nIdx > 0 && nIdx < MAX_ITEM )
	{
		Item[nIdx].SetDetailType(pItemSync->m_Detail);
		Item[nIdx].SetGenre(pItemSync->m_Genre);
		Item[nIdx].SetParticular( pItemSync->m_Particur);
		Item[nIdx].SetLevel(pItemSync->m_Level);
		Item[nIdx].SetSeries(pItemSync->m_Series);
		Item[nIdx].SetBindItem(pItemSync->m_BindItem);
		Item[nIdx].SetShopCost(pItemSync->iShopCost);
		Item[nIdx].m_SpecialParam.uItemType=pItemSync->m_nKind;
		Item[nIdx].SetTime(pItemSync->m_Time.bYear, pItemSync->m_Time.bMonth, pItemSync->m_Time.bDay, pItemSync->m_Time.bHour);
		Item[nIdx].SetFiFongArmor(pItemSync->m_nFiFongArmor);
		Item[nIdx].SetNgoaiTrang(pItemSync->iNgoaiTrang);
	}
}

#else

void KProtocolProcess::RemoveRole(int nIndex, BYTE * pProtocol)
{

}

void KProtocolProcess::NpcRequestCommand(int nIndex, BYTE* pProtocol)
{
	NPC_REQUEST_COMMAND *pNpcRequestSync = (NPC_REQUEST_COMMAND *)pProtocol;
	NpcSet.SyncNpc(pNpcRequestSync->ID, Player[nIndex].m_nNetConnectIdx);
}

//-------------------------------------------------------------------------
//	���ܣ��ͻ�����������������ĳ��obj����
//-------------------------------------------------------------------------
void KProtocolProcess::ObjRequestCommand(int nIndex, BYTE* pProtocol)
{
	OBJ_CLIENT_SYNC_ADD	*pObjClientSyncAdd = (OBJ_CLIENT_SYNC_ADD*)pProtocol;
	ObjSet.SyncAdd(pObjClientSyncAdd->m_nID, Player[nIndex].m_nNetConnectIdx);
}

void KProtocolProcess::NpcWalkCommand(int nIndex, BYTE* pProtocol)
{
	NPC_WALK_COMMAND* pNetCommand = (NPC_WALK_COMMAND *)pProtocol;
	int ParamX = pNetCommand->nMpsX;
	int ParamY = pNetCommand->nMpsY;
	if (ParamX < 0)
	{
		ParamX = 0;
	}
	if (ParamY < 0)
	{
		ParamY = 0;
	}

	Npc[Player[nIndex].m_nIndex].SendCommand(do_walk, ParamX, ParamY);

	Npc[Player[nIndex].m_nIndex].m_NowCommand.CmdKind = do_walk;
	Npc[Player[nIndex].m_nIndex].m_NowCommand.Param_X = ParamX;
	Npc[Player[nIndex].m_nIndex].m_NowCommand.Param_Y = ParamY;
	Npc[Player[nIndex].m_nIndex].m_NowCommand.Param_Z = -1;
}

void KProtocolProcess::NpcRunCommand(int nIndex, BYTE* pProtocol)
{
	NPC_RUN_COMMAND* pNetCommand = (NPC_RUN_COMMAND *)pProtocol;
	int ParamX = pNetCommand->nMpsX;
	int ParamY = pNetCommand->nMpsY;
	if (ParamX < 0)
	{
		ParamX = 0;
	}
	if (ParamY < 0)
	{
		ParamY = 0;
	}
	Npc[Player[nIndex].m_nIndex].SendCommand(do_run, ParamX, ParamY);

	Npc[Player[nIndex].m_nIndex].m_NowCommand.CmdKind = do_run;
	Npc[Player[nIndex].m_nIndex].m_NowCommand.Param_X = ParamX;
	Npc[Player[nIndex].m_nIndex].m_NowCommand.Param_Y = ParamY;
	Npc[Player[nIndex].m_nIndex].m_NowCommand.Param_Z = -1;
}


void KProtocolProcess::NpcSkillCommand(int nIndex, BYTE* pProtocol)
{
	NPC_SKILL_COMMAND* pNetCommand = (NPC_SKILL_COMMAND *)pProtocol;
	int ParamX = pNetCommand->nSkillID;
	int ParamY = pNetCommand->nMpsX;
	int ParamZ = pNetCommand->nMpsY;
	Npc[Player[nIndex].m_nIndex].m_nNpcX = pNetCommand->nNpcX;
	Npc[Player[nIndex].m_nIndex].m_nNpcY = pNetCommand->nNpcY;
	Npc[Player[nIndex].m_nIndex].m_nNpcRadius = pNetCommand->nNpcRadius;
	// �����Ϸ��Լ��

	if (ParamX <= 0 || ParamX >= MAX_SKILL )
	{
		return ;
	}

	if (ParamZ < 0) 
		return;

	if (ParamY < 0)
	{
		if (ParamY != -1) 
			return;

		int nNpcIndex = Player[nIndex].FindAroundNpc((DWORD)ParamZ);
		if (nNpcIndex > 0)
		{
			ParamZ = nNpcIndex;
			Npc[Player[nIndex].m_nIndex].SendCommand(do_skill, ParamX, ParamY, ParamZ);
		}
	}
	else
		Npc[Player[nIndex].m_nIndex].SendCommand(do_skill, ParamX, ParamY, ParamZ);

	Npc[Player[nIndex].m_nIndex].m_NowCommand.CmdKind = do_skill;
	Npc[Player[nIndex].m_nIndex].m_NowCommand.Param_X = ParamX;
	Npc[Player[nIndex].m_nIndex].m_NowCommand.Param_Y = ParamY;
	Npc[Player[nIndex].m_nIndex].m_NowCommand.Param_Z = ParamZ;

}

void KProtocolProcess::NpcJumpCommand(int nIndex, BYTE* pProtocol)
{
	NPC_JUMP_COMMAND* pNetCommand = (NPC_JUMP_COMMAND *)pProtocol;
	int ParamX = pNetCommand->nMpsX;
	int ParamY = pNetCommand->nMpsY;
	Npc[Player[nIndex].m_nIndex].SendCommand(do_jump, ParamX, ParamY);

	Npc[Player[nIndex].m_nIndex].m_NowCommand.CmdKind = do_jump;
	Npc[Player[nIndex].m_nIndex].m_NowCommand.Param_X = ParamX;
	Npc[Player[nIndex].m_nIndex].m_NowCommand.Param_Y = ParamY;
	Npc[Player[nIndex].m_nIndex].m_NowCommand.Param_Z = -1;
}

void KProtocolProcess::NpcTalkCommand(int nIndex, BYTE* pProtocol)
{
}

void KProtocolProcess::PlayerTalkCommand(int nIndex, BYTE* pProtocol)
{
	Npc[Player[nIndex].m_nIndex].DoPlayerTalk((char *)pProtocol + 1);
}

void KProtocolProcess::PlayerApplyTeamInfo(int nIndex, BYTE* pProtocol)
{
	Player[nIndex].S2CSendTeamInfo(pProtocol);
}

void KProtocolProcess::PlayerApplyCreateTeam(int nIndex, BYTE* pProtocol)
{
	Player[nIndex].CreateTeam(pProtocol);
}

void KProtocolProcess::PlayerApplyTeamOpenClose(int nIndex, BYTE* pProtocol)
{
	Player[nIndex].SetTeamState(pProtocol);
}

void KProtocolProcess::PlayerApplyAddTeam(int nIndex, BYTE* pProtocol)
{
	Player[nIndex].S2CSendAddTeamInfo(pProtocol);
}

void KProtocolProcess::PlayerAcceptTeamMember(int nIndex, BYTE* pProtocol)
{
	Player[nIndex].AddTeamMember(pProtocol);
}

void KProtocolProcess::PlayerApplyLeaveTeam(int nIndex, BYTE* pProtocol)
{
	Player[nIndex].LeaveTeam(pProtocol);
}

void KProtocolProcess::PlayerApplyTeamKickMember(int nIndex, BYTE* pProtocol)
{
	Player[nIndex].TeamKickOne(pProtocol);
}

void KProtocolProcess::PlayerApplyTeamChangeCaptain(int nIndex, BYTE* pProtocol)
{
	Player[nIndex].TeamChangeCaptain(pProtocol);
}

void KProtocolProcess::PlayerApplyTeamDismiss(int nIndex, BYTE* pProtocol)
{
	Player[nIndex].TeamDismiss(pProtocol);
}

void KProtocolProcess::PlayerApplySetPK(int nIndex, BYTE* pProtocol)
{
	Player[nIndex].SetPK(pProtocol);
}

void KProtocolProcess::PlayerApplyFactionData(int nIndex, BYTE* pProtocol)
{
	Player[nIndex].SendFactionData(pProtocol);
}

void KProtocolProcess::PlayerSendChat(int nIndex, BYTE* pProtocol)
{
	Player[nIndex].ServerSendChat(pProtocol);
}

void KProtocolProcess::PlayerAddBaseAttribute(int nIndex, BYTE* pProtocol)
{
	Player[nIndex].AddBaseAttribute(pProtocol);
}

void KProtocolProcess::PlayerApplyAddSkillPoint(int nIndex, BYTE* pProtocol)
{
	Player[nIndex].AddSkillPoint(pProtocol);
}

void KProtocolProcess::PlayerEatItem(int nIndex, BYTE* pProtocol)
{
	if (Player[nIndex].CheckTrading() || Player[nIndex].CheckShopOpen()  || Player[nIndex].CheckShopBuyOpen())
		return;
	Player[nIndex].EatItem(pProtocol);
}

void KProtocolProcess::PlayerPickUpItem(int nIndex, BYTE* pProtocol)
{
	if (Player[nIndex].CheckTrading() || Player[nIndex].CheckShopOpen()  || Player[nIndex].CheckShopBuyOpen())
		return;
	Player[nIndex].ServerPickUpItem(pProtocol);
}

void KProtocolProcess::PlayerMoveItem(int nIndex, BYTE* pProtocol)
{
	 if (Player[nIndex].CheckShopOpen()  || Player[nIndex].CheckShopBuyOpen())
		return;

	Player[nIndex].ServerMoveItem(pProtocol);
}

void KProtocolProcess::PlayerSellItem(int nIndex, BYTE* pProtocol)
{
	if (Player[nIndex].CheckTrading() || Player[nIndex].CheckShopOpen()  || Player[nIndex].CheckShopBuyOpen())
		return;
	Player[nIndex].SellItem(pProtocol);
}


void KProtocolProcess::PlayerShopPrice(int nIndex, BYTE* pProtocol)
{
	if (Player[nIndex].CheckTrading() || Player[nIndex].CheckShopOpen()  || Player[nIndex].CheckShopBuyOpen())
		return;
	Player[nIndex].ShopPrice(pProtocol);
}


void KProtocolProcess::PlayerShopOpen(int nIndex, BYTE* pProtocol)
{
	if (Player[nIndex].CheckTrading() || Player[nIndex].CheckShopBuyOpen())
		return;
	Player[nIndex].ShopOpen(pProtocol);
}


void KProtocolProcess::PlayerOpenBuyShop(int nIndex, BYTE* pProtocol)
{
	if (Player[nIndex].CheckTrading() || Player[nIndex].CheckShopOpen() || Player[nIndex].CheckShopBuyOpen())
		return;

	Player[nIndex].OpenBuyShop(pProtocol);
}


void KProtocolProcess::PlayerCloseBuyShop(int nIndex, BYTE* pProtocol)
{
	if (Player[nIndex].CheckTrading() || Player[nIndex].CheckShopOpen() || !Player[nIndex].CheckShopBuyOpen())
		return;

	Player[nIndex].CloseBuyShop(pProtocol);
}




void KProtocolProcess::PlayerShopBuyItem(int nIndex, BYTE* pProtocol)
{
	if (Player[nIndex].CheckTrading() || Player[nIndex].CheckShopOpen() || !Player[nIndex].CheckShopBuyOpen())
		return;

	Player[nIndex].ShopBuyItem(pProtocol);
}




void KProtocolProcess::PlayerBlockItem(int nIndex, BYTE* pProtocol)
{
	if (Player[nIndex].CheckTrading() || Player[nIndex].CheckShopOpen() || Player[nIndex].CheckShopBuyOpen())
		return;
	Player[nIndex].BlockItem(pProtocol);
}


void KProtocolProcess::PlayerStaskItem(int nIndex, BYTE* pProtocol)
{
	if (Player[nIndex].CheckTrading() || Player[nIndex].CheckShopOpen() || Player[nIndex].CheckShopBuyOpen())
		return;
	Player[nIndex].StaskItem(pProtocol);
}

void KProtocolProcess::PlayerStringBox(int nIndex, BYTE* pProtocol)
{
	if (Player[nIndex].CheckTrading() || Player[nIndex].CheckShopOpen() || Player[nIndex].CheckShopBuyOpen())
	return;

Player[nIndex].StringBox(pProtocol);
}




void KProtocolProcess::PlayerLoginPWStoreBox(int nIndex, BYTE* pProtocol)
{
	if (Player[nIndex].CheckTrading() || Player[nIndex].CheckShopOpen() || Player[nIndex].CheckShopBuyOpen())
	return;


	Player[nIndex].LoginPWStoreBox(pProtocol);
}




void KProtocolProcess::PlayerPWStoreBox(int nIndex, BYTE* pProtocol)
{
	if (Player[nIndex].CheckTrading() || Player[nIndex].CheckShopOpen() || Player[nIndex].CheckShopBuyOpen())
	return;

	Player[nIndex].PWStoreBox(pProtocol);

}



void KProtocolProcess::PlayerNewPWStoreBox(int nIndex, BYTE* pProtocol)
{
	if (Player[nIndex].CheckTrading() || Player[nIndex].CheckShopOpen() || Player[nIndex].CheckShopBuyOpen())
	return;

	Player[nIndex].NewPWStoreBox(pProtocol);

}







void KProtocolProcess::PlayerBlockStoreBox(int nIndex, BYTE* pProtocol)
{
	if (Player[nIndex].CheckTrading() || Player[nIndex].CheckShopOpen() || Player[nIndex].CheckShopBuyOpen())
	return;

	Player[nIndex].BlockStoreBox(pProtocol);

}





void KProtocolProcess::PlayerCheckItemBox(int nIndex, BYTE* pProtocol)
{

	 if (Player[nIndex].CheckTrading() || Player[nIndex].CheckShopOpen() || Player[nIndex].CheckShopBuyOpen())
	 return;

     Player[nIndex].CheckItemBox(pProtocol);
}

void KProtocolProcess::PlayerSlectCheckBox(int nIndex, BYTE* pProtocol)
{
		if (Player[nIndex].CheckTrading() || Player[nIndex].CheckShopOpen() || Player[nIndex].CheckShopBuyOpen())
		return;

       Player[nIndex].SlectCheckBox(pProtocol);
}

void KProtocolProcess::PlayerButtonPGBox(int nIndex, BYTE* pProtocol)
{

		if (Player[nIndex].CheckTrading() || Player[nIndex].CheckShopOpen() || Player[nIndex].CheckShopBuyOpen())
		return;

        Player[nIndex].ButtonPGBox(pProtocol);
}


void KProtocolProcess::PlayerShopName(int nIndex, BYTE* pProtocol)
{

if (Player[nIndex].CheckTrading() || Player[nIndex].CheckShopOpen() || Player[nIndex].CheckShopBuyOpen())
	return;

Player[nIndex].ShopName(pProtocol);

}




void KProtocolProcess::PlayerUnBlockItem(int nIndex, BYTE* pProtocol)
{
	if (Player[nIndex].CheckTrading() || Player[nIndex].CheckShopOpen() || Player[nIndex].CheckShopBuyOpen())
		return;
	Player[nIndex].UnBlockItem(pProtocol);
}

void KProtocolProcess::PlayerBuyItem(int nIndex, BYTE* pProtocol)
{
	if (Player[nIndex].CheckTrading() || Player[nIndex].CheckShopOpen() || Player[nIndex].CheckShopBuyOpen())
		return;
	Player[nIndex].BuyItem(pProtocol);
}

void KProtocolProcess::PlayerDropItem(int nIndex, BYTE* pProtocol)
{
	if (Player[nIndex].CheckTrading() || Player[nIndex].CheckShopOpen() || Player[nIndex].CheckShopBuyOpen())
		return;
	Player[nIndex].ServerThrowAwayItem(pProtocol);
}

void KProtocolProcess::PlayerSelUI(int nIndex, BYTE* pProtocol)
{
	if (Player[nIndex].CheckTrading() || Player[nIndex].CheckShopOpen() || Player[nIndex].CheckShopBuyOpen())
	return;

	Player[nIndex].ProcessPlayerSelectFromUI(pProtocol);
}

void KProtocolProcess::ChatSetChannel(int nIndex, BYTE* pProtocol)
{
	Player[nIndex].ChatSetTakeChannel(pProtocol);
}

void KProtocolProcess::ChatApplyAddFriend(int nIndex, BYTE* pProtocol)
{
	Player[nIndex].ChatTransmitApplyAddFriend(pProtocol);
}

void KProtocolProcess::ChatAddFriend(int nIndex, BYTE* pProtocol)
{
	Player[nIndex].ChatAddFriend(pProtocol);
}

void KProtocolProcess::ChatRefuseFriend(int nIndex, BYTE* pProtocol)
{
	Player[nIndex].ChatRefuseFriend(pProtocol);
}

void KProtocolProcess::ChatApplyReSendAllFriendName(int nIndex, BYTE* pProtocol)
{
	Player[nIndex].ChatResendAllFriend(pProtocol);
}

void KProtocolProcess::ChatApplySendOneFriendName(int nIndex, BYTE* pProtocol)
{
	Player[nIndex].ChatSendOneFriendData(pProtocol);
}

void KProtocolProcess::ChatDeleteFriend(int nIndex, BYTE* pProtocol)
{
	Player[nIndex].ChatDeleteFriend(pProtocol);
}

void KProtocolProcess::ChatReDeleteFriend(int nIndex, BYTE* pProtocol)
{
	Player[nIndex].ChatRedeleteFriend(pProtocol);
}

void	KProtocolProcess::TradeApplyOpen(int nIndex, BYTE* pProtocol)
{
	Player[nIndex].TradeApplyOpen(pProtocol);
}

void	KProtocolProcess::TradeApplyClose(int nIndex, BYTE* pProtocol)
{
	Player[nIndex].TradeApplyClose(pProtocol);
}

void	KProtocolProcess::TradeApplyStart(int nIndex, BYTE* pProtocol)
{
	Player[nIndex].TradeApplyStart(pProtocol);
}

void	KProtocolProcess::TradeMoveMoney(int nIndex, BYTE* pProtocol)
{
	Player[nIndex].TradeMoveMoney(pProtocol);
}

void	KProtocolProcess::TradeDecision(int nIndex, BYTE* pProtocol)
{
	Player[nIndex].TradeDecision(pProtocol);
}

void	KProtocolProcess::DialogNpc(int nIndex, BYTE * pProtocol)
{
	Player[nIndex].DialogNpc(pProtocol)	;
}

void	KProtocolProcess::TeamInviteAdd(int nIndex, BYTE * pProtocol)
{
	Player[nIndex].m_cTeam.InviteAdd(nIndex, (TEAM_INVITE_ADD_COMMAND*)pProtocol);
}

void	KProtocolProcess::ChangeAuraSkill(int nIndex, BYTE * pProtocol)
{
	SKILL_CHANGEAURASKILL_COMMAND * pCommand = (SKILL_CHANGEAURASKILL_COMMAND*) pProtocol;
	Npc[Player[nIndex].m_nIndex].SetAuraSkill(pCommand->m_nAuraSkill);
}

void	KProtocolProcess::TeamReplyInvite(int nIndex, BYTE * pProtocol)
{
	TEAM_REPLY_INVITE_COMMAND	*pReply = (TEAM_REPLY_INVITE_COMMAND*)pProtocol;
	if (!pProtocol ||
		pReply->m_nIndex <= 0 ||
		pReply->m_nIndex >= MAX_PLAYER ||
		Player[pReply->m_nIndex].m_nIndex <= 0)
		return;
	if (pReply->m_btResult && Player[nIndex].m_cTeam.GetCanTeamFlag() == FALSE)
	{
		SHOW_MSG_SYNC	sMsg;
		sMsg.ProtocolType = s2c_msgshow;
		sMsg.m_wMsgID = enumMSG_ID_CANNOT_ADD_TEAM;
		sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
		g_pServer->PackDataToClient(Player[nIndex].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
		return;
	}
	Player[pReply->m_nIndex].m_cTeam.GetInviteReply(pReply->m_nIndex, nIndex, pReply->m_btResult);
}

//void KProtocolProcess::ReplyPing(int nIndex, BYTE* pProtocol)
//{
//	if (nIndex <= 0 || nIndex >= MAX_PLAYER)
//		return;
//
//	PING_COMMAND PingCmd, *pPingCmd;
//
//	pPingCmd = (PING_COMMAND *)pProtocol;
//
//	PingCmd.ProtocolType = s2c_ping;
//	PingCmd.m_dwTime = pPingCmd->m_dwTime;
//	g_pServer->SendData(Player[nIndex].m_nNetConnectIdx, &PingCmd, sizeof(PING_COMMAND));
//	Player[nIndex].m_uLastPingTime = g_SubWorldSet.GetGameTime();
//}

void KProtocolProcess::NpcSitCommand(int nIndex, BYTE* pProtocol)
{
	NPC_SIT_COMMAND *pSitCmd;

	pSitCmd = (NPC_SIT_COMMAND *)pProtocol;

	if (nIndex <= 0 || nIndex >= MAX_PLAYER)
		return;

	if (Player[nIndex].m_nIndex <= 0 || Player[nIndex].m_nIndex >= MAX_NPC)
		return;

	if (pSitCmd->m_btSitFlag)
		Npc[Player[nIndex].m_nIndex].SendCommand(do_sit);
	else
		Npc[Player[nIndex].m_nIndex].SendCommand(do_stand);
//	if (Npc[Player[nIndex].m_nIndex].m_Doing != do_sit)
//		Npc[Player[nIndex].m_nIndex].SendCommand(do_sit);
//	else
//		Npc[Player[nIndex].m_nIndex].SendCommand(do_stand);
}






void KProtocolProcess::NpcRideCommand(int nIndex, BYTE* pProtocol)
{
	NPC_RIDE_COMMAND *pRideCmd;

	pRideCmd = (NPC_RIDE_COMMAND *)pProtocol;

	if (nIndex <= 0 || nIndex >= MAX_PLAYER)
		return;

	if (Player[nIndex].m_nIndex <= 0 || Player[nIndex].m_nIndex >= MAX_NPC)
		return;

	Npc[Player[nIndex].m_nIndex].SendCommand(do_ride);

}












void KProtocolProcess::ObjMouseClick(int nIndex, BYTE* pProtocol)
{
	if (Player[nIndex].CheckTrading() || Player[nIndex].CheckShopOpen())
		return;

	int		nSubWorldIdx, nRegionIdx, nObjIdx;
	int		nPlayerX, nPlayerY, nObjX, nObjY;
	OBJ_MOUSE_CLICK_SYNC 	*pObj = (OBJ_MOUSE_CLICK_SYNC*)pProtocol;

	nSubWorldIdx = Npc[Player[nIndex].m_nIndex].m_SubWorldIndex;
	nRegionIdx = SubWorld[nSubWorldIdx].FindRegion(pObj->m_dwRegionID);
	if (nRegionIdx < 0)
		return;
	nObjIdx = SubWorld[nSubWorldIdx].m_Region[nRegionIdx].FindObject(pObj->m_nObjID);
	if (nObjIdx <= 0)
		return;

	SubWorld[nSubWorldIdx].Map2Mps(
		Npc[Player[nIndex].m_nIndex].m_RegionIndex,
		Npc[Player[nIndex].m_nIndex].m_MapX,
		Npc[Player[nIndex].m_nIndex].m_MapY,
		Npc[Player[nIndex].m_nIndex].m_OffX,
		Npc[Player[nIndex].m_nIndex].m_OffY,
		&nPlayerX,
		&nPlayerY);
	SubWorld[nSubWorldIdx].Map2Mps(
		nRegionIdx,
		Object[nObjIdx].m_nMapX,
		Object[nObjIdx].m_nMapY,
		Object[nObjIdx].m_nOffX,
		Object[nObjIdx].m_nOffY,
		&nObjX,
		&nObjY);
/*
	Obj_Kind_MapObj = 0,		// ��ͼ�������Ҫ���ڵ�ͼ����
	Obj_Kind_Body,				// npc ��ʬ��
	Obj_Kind_Box,				// ����
	Obj_Kind_Item,				// ���ڵ��ϵ�װ��
	Obj_Kind_Money,				// ���ڵ��ϵ�Ǯ
	Obj_Kind_LoopSound,			// ѭ����Ч
	Obj_Kind_RandSound,			// �����Ч
	Obj_Kind_Light,				// ��Դ��3Dģʽ�з���Ķ�����
	Obj_Kind_Door,				// ����
	Obj_Kind_Trap,				// ����
	Obj_Kind_Prop,				// С���ߣ�������
	Obj_Kind_Num,				// �����������
*/
	switch (Object[nObjIdx].m_nKind)
	{
	case Obj_Kind_Box:
		if (g_GetDistance(nPlayerX, nPlayerY, nObjX, nObjY) > defMAX_EXEC_OBJ_SCRIPT_DISTANCE)
			break;
		if (Object[nObjIdx].m_nState == OBJ_BOX_STATE_CLOSE)
			Object[nObjIdx].ExecScript(nIndex);
		break;
	case Obj_Kind_Door:
		break;
	}
}

void KProtocolProcess::StoreMoneyCommand(int nIndex, BYTE* pProtocol)
{
	STORE_MONEY_COMMAND*	pCommand = (STORE_MONEY_COMMAND *)pProtocol;

	if (pCommand->m_byDir)	// ȡǮ
		Player[nIndex].m_ItemList.ExchangeMoney(room_repository, room_equipment, pCommand->m_dwMoney);
	else					// ��Ǯ
		Player[nIndex].m_ItemList.ExchangeMoney(room_equipment, room_repository, pCommand->m_dwMoney);

}

void KProtocolProcess::NpcReviveCommand(int nIndex, BYTE* pProtocol)
{
//	NPC_REVIVE_COMMAND*		pCommand = (NPC_REVIVE_COMMAND *)pProtocol;

	Player[nIndex].Revive(REMOTE_REVIVE_TYPE);
}

void KProtocolProcess::c2sTradeReplyStart(int nIndex, BYTE* pProtocol)
{
	Player[nIndex].c2sTradeReplyStart(pProtocol);
}

void KProtocolProcess::c2sPKApplyChangeNormalFlag(int nIndex, BYTE* pProtocol)
{
	PK_APPLY_NORMAL_FLAG_COMMAND	*pApply = (PK_APPLY_NORMAL_FLAG_COMMAND*)pProtocol;
	Player[nIndex].m_cPK.SetNormalPKState(pApply->m_btFlag);
}

void KProtocolProcess::c2sPKApplyEnmity(int nIndex, BYTE* pProtocol)
{
	PK_APPLY_ENMITY_COMMAND	*pApply = (PK_APPLY_ENMITY_COMMAND*)pProtocol;
	if (Player[nIndex].m_nIndex && !Npc[Player[nIndex].m_nIndex].m_FightMode)
	{
		SHOW_MSG_SYNC	sMsg;
		sMsg.ProtocolType = s2c_msgshow;
		sMsg.m_wMsgID = enumMSG_ID_PK_ERROR_1;
		sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
		g_pServer->PackDataToClient(Player[nIndex].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
		return;
	}
	if (pApply->m_dwNpcID <= 0 || pApply->m_dwNpcID >= MAX_NPC)
		return;
	int nNpcIdx = Player[nIndex].FindAroundNpc(pApply->m_dwNpcID);
	if (nNpcIdx <= 0)
		return;
	if (Npc[nNpcIdx].m_Kind != kind_player || Npc[nNpcIdx].GetPlayerIdx() <= 0)
		return;

	Player[nIndex].m_cPK.EnmityPKOpen(Npc[nNpcIdx].GetPlayerIdx());
}

#define		defMAX_VIEW_EQUIP_TIME			30
void	KProtocolProcess::c2sViewEquip(int nIndex, BYTE* pProtocol)
{
	if (g_SubWorldSet.GetGameTime() - Player[nIndex].m_nViewEquipTime < defMAX_VIEW_EQUIP_TIME)
		return;
	Player[nIndex].m_nViewEquipTime = g_SubWorldSet.GetGameTime();

	VIEW_EQUIP_COMMAND	*pView = (VIEW_EQUIP_COMMAND*)pProtocol;
	if (pView->m_dwNpcID == Npc[Player[nIndex].m_nIndex].m_dwID)
		return;
	int nPlayerIdx = Player[nIndex].FindAroundPlayer(pView->m_dwNpcID);
	if (nPlayerIdx <= 0)
		return;
	Player[nPlayerIdx].SendEquipItemInfo(nIndex);
}

void KProtocolProcess::LadderQuery(int nIndex, BYTE* pProtocol)
{
	LADDER_QUERY*	pLQ = (LADDER_QUERY *)pProtocol;
	if (nIndex > 0 && nIndex < MAX_PLAYER)
	{
		int lnID = Player[nIndex].m_nNetConnectIdx;
		if (lnID >= 0)
		{
			LADDER_DATA	LadderData;
			LadderData.ProtocolType = s2c_ladderresult;
			LadderData.dwLadderID = pLQ->dwLadderID;
			void* pData = (void *)Ladder.GetTopTen(LadderData.dwLadderID);
			if (pData)
			{
				memcpy(LadderData.StatData, pData, sizeof(LadderData.StatData));
				g_pServer->PackDataToClient(lnID, &LadderData, sizeof(LadderData));
			}
		}
	}
}

void KProtocolProcess::ItemRepair(int nIndex, BYTE* pProtocol)
{
	ITEM_REPAIR	*pIR = (ITEM_REPAIR *)pProtocol;
	if (nIndex > 0 && nIndex < MAX_PLAYER)
	{
		Player[nIndex].RepairItem(pIR->dwItemID);
	}
}

void KProtocolProcess::OpenShopGold(int nIndex, BYTE* pProtocol)
{
	OPEN_SHOP_GOLD	*pIR = (OPEN_SHOP_GOLD *)pProtocol;
	if (nIndex > 0 && nIndex < MAX_PLAYER)
	{
		if (!Npc[Player[nIndex].m_nIndex].m_FightMode)
		{


	    //KPlayerChat::SendSystemInfo(1, nIndex, MESSAGE_SYSTEM_ANNOUCE_HEAD, "Hi�n t�i kh�ng th?s?d�ng. K?tr�n c�c s?m?v�o 9 gi?ng�y 25/1/2013 !", strlen("Hi�n t�i kh�ng th?s?d�ng. K?tr�n c�c s?m?v�o 9 gi?ng�y 25/1/2013 !") );
	
		//return;


		Player[nIndex].ExecuteScript("\\script\\admin\\shopktc.lua","main",0);
			
		//BuySell.OpenSale(nIndex, 23,1);
		//BuySell.OpenSale(nIndex, 26,1);


		}
	}
}





void KProtocolProcess::PlayerOpenTopTKNew(int nIndex, BYTE* pProtocol)
{
	OPEN_TOP_TK_NEW	*pIR = (OPEN_TOP_TK_NEW *)pProtocol;
	if (nIndex > 0 && nIndex < MAX_PLAYER)
	{




	try
	{
	
		bool bExecuteScriptMistake = true;
		KLuaScript * pScript = (KLuaScript* )g_GetScript("\\script\\tongkim\\dieukhientk.lua");;
		if (pScript)
		{

			

			int nTopIndex = 0;
			
			pScript->SafeCallBegin(&nTopIndex);
			if (pScript->CallFunction("OpenTop",0, "d",nIndex));
			{
				bExecuteScriptMistake = false;
			}
			pScript->SafeCallEnd(nTopIndex);

			
		}
		
		

	}
	catch(...)
	{
		printf("Exception Have Caught When Execute Script[%d]!!!!!", g_FileName2Id("\\script\\tongkim\\dieukhientk.lua"));
	}















		
	}
}

void KProtocolProcess::c2sSetImage(int nIndex, BYTE* pProtocol)
{
	SET_IMAGE *pSetImgCmd;

	pSetImgCmd = (SET_IMAGE *)pProtocol;
	if (nIndex <= 0 || nIndex >= MAX_PLAYER)
		return;
	//g_DebugLog("XXX: %d",pSetImgCmd->nNum);
	if (Player[nIndex].m_nIndex <= 0 || Player[nIndex].m_nIndex >= MAX_NPC)
		return;
	Player[nIndex].SetNumImg(pSetImgCmd->nNum);		
}


//UYTHAC
void KProtocolProcess::c2sLiXian(int nIndex, BYTE* pProtocol)
{
	Player[nIndex].ExecuteScript("\\script\\offline.lua","main","");
	printf("TOI DAY NE");
}
//END
void KProtocolProcess::c2sQuayLai(int nIndex, BYTE* pProtocol)
{
	AUTO_QUAYLAI *pAutoQuayLai;
	pAutoQuayLai = (AUTO_QUAYLAI *)pProtocol;
		
		if (nIndex <= 0 || nIndex >= MAX_PLAYER)
		return;

	if (Player[nIndex].m_nIndex <= 0 || Player[nIndex].m_nIndex >= MAX_NPC)
		return;

	Player[nIndex].m_AutoLifePoison = pAutoQuayLai->m_bLifePoison;
	Player[nIndex].m_AutoManaPoison = pAutoQuayLai->m_bManaPoison;
	Player[nIndex].m_AutoAllPoison = pAutoQuayLai->m_bAllPoison;
	Player[nIndex].m_AutoAllPoisonMin = pAutoQuayLai->m_bAllPoisonMin;
	Player[nIndex].m_AutoSuaTrangBi = pAutoQuayLai->m_bSuaTrangBi;
//	Player[nIndex].m_AutoGuiTien = pAutoQuayLai->m_bGuiTien;
	Player[nIndex].m_AutoNhatDoChonLoc = pAutoQuayLai->m_bNhatDoChonLoc;
	
	Player[nIndex].m_AutoTTL = pAutoQuayLai->m_bUseTTL;
	Player[nIndex].m_AutoQuayLai = pAutoQuayLai->m_bQuayLai;
	Player[nIndex].m_AutoNumSelect = pAutoQuayLai->m_NumSelect;
	Player[nIndex].m_AutoRingSelect = pAutoQuayLai->m_RingSelect;
	Player[nIndex].m_AutoWeaSelect = pAutoQuayLai->m_WeaSelect;


	//Player[nIndex].ExecuteScript("\\script\\trolaidiadiemcu.lua","main","");
}


void KProtocolProcess::c2sLocDo(int nIndex, BYTE* pProtocol)
{
	AUTO_LOCDO *pAutoLocDo;
	pAutoLocDo = (AUTO_LOCDO *)pProtocol;
		
		if (nIndex <= 0 || nIndex >= MAX_PLAYER)
		return;

	if (Player[nIndex].m_nIndex <= 0 || Player[nIndex].m_nIndex >= MAX_NPC)
		return;

	Player[nIndex].MangOption[0][1] = pAutoLocDo->m_Option1;
	Player[nIndex].MangOption[1][1] = pAutoLocDo->m_Option2;
	Player[nIndex].MangOption[2][1] = pAutoLocDo->m_Option3;
	Player[nIndex].MangOption[3][1] = pAutoLocDo->m_Option4;
	Player[nIndex].MangOption[4][1] = pAutoLocDo->m_Option5;
	Player[nIndex].MangOption[5][1] = pAutoLocDo->m_Option6;
	Player[nIndex].MangOption[6][1] = pAutoLocDo->m_Option7;
	Player[nIndex].MangOption[7][1] = pAutoLocDo->m_Option8;
	Player[nIndex].MangOption[8][1] = pAutoLocDo->m_Option9;
	Player[nIndex].MangOption[9][1] = pAutoLocDo->m_Option10;
	Player[nIndex].MangOption[10][1] = pAutoLocDo->m_Option11;
	Player[nIndex].MangOption[11][1] = pAutoLocDo->m_Option12;
	Player[nIndex].MangOption[12][1] = pAutoLocDo->m_Option13;
	Player[nIndex].MangOption[13][1] = pAutoLocDo->m_Option14;
	Player[nIndex].MangOption[14][1] = pAutoLocDo->m_Option15;
	Player[nIndex].MangOption[15][1] = pAutoLocDo->m_Option16;
	Player[nIndex].MangOption[16][1] = pAutoLocDo->m_Option17;
	Player[nIndex].MangOption[17][1] = pAutoLocDo->m_Option18;
	Player[nIndex].MangOption[18][1] = pAutoLocDo->m_Option19;
	Player[nIndex].MangOption[19][1] = pAutoLocDo->m_Option20;
	Player[nIndex].MangOption[20][1] = pAutoLocDo->m_Option21;
	Player[nIndex].MangOption[21][1] = pAutoLocDo->m_Option22;
	Player[nIndex].MangOption[22][1] = pAutoLocDo->m_Option23;
	Player[nIndex].MangOption[23][1] = pAutoLocDo->m_Option24;
	Player[nIndex].MangOption[24][1] = pAutoLocDo->m_Option25;
	Player[nIndex].MangOption[25][1] = pAutoLocDo->m_Option26;
	Player[nIndex].MangOption[26][1] = pAutoLocDo->m_Option27;
	Player[nIndex].MangOption[27][1] = pAutoLocDo->m_Option28;
	Player[nIndex].MangOption[28][1] = pAutoLocDo->m_Option29;
	Player[nIndex].MangOption[29][1] = pAutoLocDo->m_Option30;
	Player[nIndex].MangOption[30][1] = pAutoLocDo->m_Option31;
	Player[nIndex].MangOption[31][1] = pAutoLocDo->m_Option32;
	Player[nIndex].MangOption[32][1] = pAutoLocDo->m_Option33;
	Player[nIndex].MangOption[33][1] = pAutoLocDo->m_Option34;
	Player[nIndex].MangOption[34][1] = pAutoLocDo->m_Option35;
	Player[nIndex].MangOption[35][1] = pAutoLocDo->m_Option36;
	Player[nIndex].MangOption[36][1] = pAutoLocDo->m_Option37;
	Player[nIndex].MangOption[37][1] = pAutoLocDo->m_Option38;
	Player[nIndex].MangOption[38][1] = pAutoLocDo->m_Option39;

}


#endif