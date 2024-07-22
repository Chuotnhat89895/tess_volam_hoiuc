/*****************************************************************************************
//	Chuc nang: Choi tu dong
//	Copyright : Ken Nguyen 2013
//	Author	:   Ken Nguyen (thienthanden2)
//	CreateTime:	12-05-2013
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once

#include "../elem/wndpage.h"
#include "../elem/wndbutton.h"
#include "../elem/wndtext.h"
#include "../../../core/src/gamedatadef.h"
#include "../Elem/WndMessageListBox.h"
#include "../Elem/WndScrollBar.h"
#include "../Elem/WndEdit.h"
#include "../Elem/WndPureTextBtn.h"
#include "../Elem/WndLabeledButton.h"
#include "../../../core/src/gamedatadef.h"
#include "../elem/WndList.h"

class KUiTreoMay : public KWndPage
{
public:
	void	Initialize();
	void	LoadScheme();
	void	Breathe();
	void	Active(BOOL bAct); //ham kich hoat

	KWndText32			m_tLifePoison;
	KWndText32			m_tManaPoison;
	KWndText32			m_tAllPoison;
	KWndText32			m_tAllPoisonMin;
	
	KWndEdit32			m_eLifePoison;
	KWndEdit32			m_eManaPoison;
	KWndEdit32			m_eAllPoison;
	KWndEdit32			m_eAllPoisonMin;
	
	KWndText32			m_tSuaTrangBi;
	KWndLabeledButton	m_tbSuaTrangBi;
	BOOL				m_bSuaTrangBi;
	
	KWndText32			m_tGuiTien;
	KWndLabeledButton	m_tbGuiTien;
	BOOL				m_bGuiTien;
	
	KWndText32			m_tBanTrangBi;
	KWndLabeledButton	m_tbBanTrangBi;
	BOOL				m_bBanTrangBi;
	
	KWndText32			m_tQuayLai;
	KWndLabeledButton	m_tbQuayLai;
	BOOL				m_bQuayLai;
	

BOOL				m_bIsActived;		//duoc kich hoat hay chua?
BOOL                m_Start;	

	
KWndLabeledButton   m_bStart;	
KWndLabeledButton   m_bClose;
private:
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	static KUiTreoMay*		m_pTreoMaySelf;
private:

};


class KUiNhatItem : public KWndPage
{
public:
	void	Initialize();
	void	LoadScheme();
	void	Breathe();
	void	Active(BOOL bAct); //ham kich hoat
	
	
	KWndText32			m_tMoneyObj;
	KWndText32			m_tItemObj;
	KWndText32			m_tPropObj;
	KWndText32			m_tItemSelect;
	KWndText32			m_tNumSelect;
	KWndText32			m_tRingSelect;
	KWndText32			m_tWeaSelect;
	
	KWndEdit32			m_eNumSelect;
	
	KWndLabeledButton			m_tbMoneyObj;
	KWndLabeledButton			m_tbItemObj;
	KWndLabeledButton			m_tbPropObj;
	KWndLabeledButton			m_tbItemSelect;
	KWndLabeledButton			m_tbRingSelect;
	KWndLabeledButton			m_tbWeaSelect;
	
	
	BOOL				m_bMoneyObj;
	BOOL				m_bItemObj;
	BOOL				m_bPropObj;
	BOOL				m_bItemSelect;
	BOOL				m_bRingSelect;
	BOOL				m_bWeaSelect;
	
	KWndLabeledButton   m_bStart;	
	KWndLabeledButton   m_bClose;
private:
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	static KUiNhatItem*		m_pNhatItemSelf;
private:





BOOL					m_bIsActived;		//duoc kich hoat hay chua?
BOOL                m_Start;	
};


class KUiFighting : public KWndPage
{
public:
	void	Initialize();
	void	LoadScheme();
	void	Breathe();
	void	Active(BOOL bAct); //ham kich hoat

	KWndText32		    m_tLife;
	KWndText32		    m_tNgaMyBuff;
KWndText32		    m_tMana;
KWndText32		    m_tReturn;
KWndText32		    m_tAtack;
KWndText32		    m_tRange;
KWndText32		    m_tNAtack;
KWndText32		    m_tNQuanhDiem;
KWndText32		    m_tNTheoSau;
KWndText32		    m_tNThongBao;

KWndEdit32			m_eLife;
KWndEdit32			m_eNgaMyBuff;
KWndEdit32		    m_eMana;
KWndEdit32		    m_eReturn;
KWndEdit32		    m_eAtack;
KWndEdit32		    m_eRange;
KWndEdit32		    m_eNAtack;

	KWndLabeledButton			m_tbQuanhDiem;
	KWndLabeledButton			m_tbTheoSau;
	BOOL				m_bQuanhDiem;
	BOOL				m_bTargetPlayer;
	BOOL				m_bTheoSau;
	
KWndLabeledButton   m_bStart;	
KWndLabeledButton   m_bClose;
private:
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	static KUiFighting*		m_pFightSelf;
private:





BOOL					m_bIsActived;		//duoc kich hoat hay chua?
BOOL                m_Start;	
};

class KUiPickUp : public KWndPage
{
public:
	void	Initialize();
	void	LoadScheme();
	void	Breathe();
	void	Active(BOOL bAct); //ham kich hoat

	KWndText32			m_tPlayer;
	KWndText32			m_tMissle;
	KWndText32			m_tbHorse;
	KWndText32			m_tTeam;
	KWndText32			m_tReturnPos;
	KWndText32			m_tTeamAuto;
	KWndText32			m_tInviteAuto;
	KWndText32			m_tAcceptAuto;
	KWndText32			m_tCancelAuto;
	
	KWndLabeledButton			m_tHidePlayer;
	KWndLabeledButton			m_tHideMissle;
	KWndLabeledButton			m_tHorse;
	KWndLabeledButton			m_tbInviteAuto;
	KWndLabeledButton			m_tbAcceptAuto;
	KWndLabeledButton			m_tbCancelAuto;

	BOOL				m_bHidePlayer;
	BOOL				m_bHideMissle;
	BOOL				m_bHorse;
	BOOL				m_bInviteAuto;
	BOOL				m_bAcceptAuto;
	BOOL				m_bCancelAuto;
	
private:
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	BOOL					m_bIsActived;		//duoc kich hoat hay chua?

	

};

class KUiAutoI : public KWndPageSet
{
public:
	static KUiAutoI* OpenWindow();
	static KUiAutoI* ActiveAuto();		
	static KUiAutoI* GetIfVisible();	
	static BOOL GetIfActive();
	static void			CloseWindow(bool bDestroy);		//关闭窗口
	static void			BlockAuto();		//关闭窗口
	static void			LoadScheme();//载入界面方案
	void			Breathe();
	static KUiAutoI*			LoadPrivateSetting(KIniFile* pFile);
	static int			SavePrivateSetting(KIniFile* pFile);
	
	
private:
	KUiAutoI() {}
	~KUiAutoI() {}
	void	Initialize();
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	PaintWindow();
private:
	static KUiAutoI* m_pSelf;
private:
	KWndButton			m_Close;
	KWndLabeledButton			m_Active;
	KWndPureTextBtn		m_TActiveText;
	KWndPureTextBtn			m_FightText;
	KWndPureTextBtn			m_PickText;
	KWndPureTextBtn			m_NhatItemText;
	KWndPureTextBtn			m_TreoMayText;
	BOOL		m_ActiveText;
	KUiFighting			m_FightPad;	//Bang~ Chien' dau'
	KUiPickUp			m_PickPad;//Bang~ Ho Tro
	KUiNhatItem			m_NhatItemPad;//Bang~ nhat do`
	KUiTreoMay			m_TreoMayPad;//Bang~ treo may
	
	//--------------------------------------------------------
	KWndButton			m_FightingBtn;	//nut chien' dau'
	KWndButton			m_PickUpBtn;	//nut ho tro
	KWndButton			m_NhatItemBtn;	//nut nhat do`
	KWndButton			m_TreoMayBtn;	//nut nhat do`
	KWndList		m_NearbyList;
	KUiPlayerItem*	m_pNearbyPlayersList;
	KUiPlayerTeam	m_Info;
	//--------------------------------------------------------
	char				m_szTitle[24]; //ten auto
};
