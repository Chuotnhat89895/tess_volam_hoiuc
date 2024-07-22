/*****************************************************************************************
//	����--״̬����
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-9-2
*****************************************************************************************/
#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/WndMessage.h"
#include "../elem/wnds.h"
#include "UiParadeItem.h"
#include "../ShortcutKey.h"
#include "../UiSoundSetting.h"
#include "../../../core/src/coreshell.h"
#include "../UiBase.h"

extern iCoreShell*		g_pCoreShell;
#define IMAGE_PLAYER	"\\Settings\\AvatarPlayer.ini"
#define	SCHEME_INI		"UiParadeItem.ini"

KUiParadeItem* KUiParadeItem::m_pSelf = NULL;

// -------------------------------------------------------------------------
// ---> �����ؼ���UIEP_*�Լ��ɽ�����Ʒ�����͵Ķ�Ӧ��ϵ
static struct UE_CTRL_MAP
{
	int				nPosition;
	const char*		pIniSection;
}CtrlItemMap[_ITEM_COUNT] =
{
	{ UIEP_HEAD,		"Cap"		},	//װ��-ñ��
	{ UIEP_HAND,		"Weapon"	},	//װ��-����
	{ UIEP_NECK,		"Necklace"	},	//װ��-����
	{ UIEP_FINESSE,		"Bangle"	},	//װ��-����
	{ UIEP_BODY,		"Cloth"		},	//װ��-�·�
	{ UIEP_WAIST,		"Sash"		},	//װ��-����
	{ UIEP_FINGER1,		"Ring1"		},	//װ��-��ָ
	{ UIEP_FINGER2,		"Ring2"		},	//װ��-��ָ
	{ UIEP_WAIST_DECOR,	"Pendant"	},	//װ��-��׺
	{ UIEP_FOOT,		"Shoes"		},	//װ��-Ь��
	{ UIEP_HORSE,		"Horse"		},	//װ��-��
	{ UIEP_MASK,		"Mask"		}	// Mask EP chu ko phai PE
};


//--------------------------------------------------------------------------
//	���ܣ��������������ʾ���򷵻�ʵ��ָ��
//--------------------------------------------------------------------------
KUiParadeItem* KUiParadeItem::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}

//--------------------------------------------------------------------------
//	���ܣ��򿪴��ڣ�����Ψһ��һ�������ʵ��
//--------------------------------------------------------------------------
KUiParadeItem* KUiParadeItem::OpenWindow(KUiPlayerItem* pDest)
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiParadeItem;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
		m_pSelf->UpdateData(pDest);
		
		char Scheme[256];
		g_UiBase.GetCurSchemePath(Scheme, 256);
		LoadScheme(Scheme);

		m_pSelf->BringToTop();
		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		m_pSelf->Show();
	}
	return m_pSelf;
}

//--------------------------------------------------------------------------
//	���ܣ��رմ��ڣ�ͬʱ����ѡ���Ƿ�ɾ������ʵ��
//--------------------------------------------------------------------------
void KUiParadeItem::CloseWindow(bool bDestroy)
{
	if (m_pSelf)
	{
		if (bDestroy == false)
			m_pSelf->Hide();
		else
		{
			m_pSelf->Destroy();
			m_pSelf = NULL;
		}

		if (g_pCoreShell)
			g_pCoreShell->OperationRequest(GOI_VIEW_PLAYERITEM_END, 0, 0);
	}
}

//--------------------------------------------------------------------------
//	���ܣ���ʼ��
//--------------------------------------------------------------------------
void KUiParadeItem::Initialize()
{
	AddChild(&m_Name);
	AddChild(&m_Title);
	AddChild(&m_Repute);
	AddChild(&m_Pk);
	AddChild(&m_Pd);
	AddChild(&m_Xh);
	AddChild(&m_Cs);
	AddChild(&m_Qh);
	AddChild(&m_Avatar);
//	AddChild(&m_MateName);
	for (int i = 0; i < _ITEM_COUNT; i ++)
	{
		m_EquipBox[i].SetObjectGenre(CGOG_ITEM);
		AddChild(&m_EquipBox[i]);
		m_EquipBox[i].SetContainerId((int)UOC_EQUIPTMENT);
	}

	AddChild(&m_Close);

	Wnd_AddWindow(this);
}

//--------------------------------------------------------------------------
//	���ܣ����봰�ڵĽ��淽��
//--------------------------------------------------------------------------
void KUiParadeItem::LoadScheme(const char* pScheme)
{
	if (m_pSelf)
	{
		char		Buff[128];
		KIniFile	Ini;
		sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI);
		if (Ini.Load(Buff))
			m_pSelf->LoadScheme(&Ini);	
	}
}

//������淽��
void KUiParadeItem::LoadScheme(class KIniFile* pIni)
{
	if (g_pCoreShell->GetGameData(GDI_PLAYER_IS_MALE, 0, m_Dest.uId))
		Init(pIni, "Male");
	else
		Init(pIni, "Female");

	m_Face    .Init(pIni, "Face");
	m_Name    .Init(pIni, "Name");
	m_Title   .Init(pIni, "Title");
	m_Repute  .Init(pIni, "Repute");
	m_Pk      .Init(pIni, "PKValue");
	m_Pd      .Init(pIni, "Pd");
	m_Xh      .Init(pIni, "Xh");
	m_Cs      .Init(pIni, "Cs");
	m_Qh      .Init(pIni, "Qh");
	m_Close	 .Init(pIni, "Close");
//	m_MateName	 .Init(pIni, "MateName");

	for (int i = 0; i < _ITEM_COUNT; i ++)
	{
		m_EquipBox[i].Init(pIni, CtrlItemMap[i].pIniSection);
	}
}

//--------------------------------------------------------------------------
//	���ܣ����ں���
//--------------------------------------------------------------------------
int KUiParadeItem::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int nRet = 0;
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)(KWndWindow*)&m_Close)
			Hide();
		break;
	default:
		nRet = KWndShowAnimate::WndProc(uMsg, uParam, nParam);
	}
	return nRet;
}

//--------------------------------------------------------------------------
//	���ܣ����»������ݣ������Ȳ��ױ����ݣ�
//--------------------------------------------------------------------------
void KUiParadeItem::UpdateBaseData(KUiPlayerItem* pDest)
{
	if (!g_pCoreShell)
		return;

	KUiPlayerBaseInfo	Info;
	memset(&Info, 0, sizeof(KUiPlayerBaseInfo));
	g_pCoreShell->GetGameData(GDI_PLAYER_BASE_INFO, (int)&Info, pDest->uId);
//	g_pCoreShell->OperationRequest(GOI_GET_MATE_NAME,pDest->uId,0);
	m_Name  .SetText(Info.Name);
	m_Title .SetText(Info.Title);
	m_Repute.SetIntText(Info.nRepute);
	m_Pk    .SetIntText(Info.nPk);
	m_Pd    .SetIntText(Info.nPd);
	m_Xh    .SetIntText(Info.nXh);

	char cCs[32] = "";
	sprintf(cCs,"Tr�ng sinh %d",Info.nCs);

	m_Cs    .SetText(cCs);

	
	// chan dung
	KIniFile Ini;
	int Left = 0, Top = 0;
	char sz_Temp[30];
	char ImageNpc[100];
	int nNumIcon = Info.nNumImg;
	if (Ini.Load(IMAGE_PLAYER))
	{
		sprintf(sz_Temp, "%d_Position", nNumIcon);
		Ini.GetInteger2("AvatarInfo", sz_Temp, &Left, &Top);
		m_Avatar.SetPosition(Left,Top);
		sprintf(sz_Temp, "%d_Image", nNumIcon);
		Ini.GetString("AvatarInfo", sz_Temp, "", ImageNpc, sizeof(ImageNpc));
		m_Avatar.SetImage(ISI_T_SPR, ImageNpc);	
	}
	// end
}
//void KUiParadeItem::UpdateMateName(char* szName)
//{
//	char str[2] = "";
//	if (m_pSelf)
//	{
//		if (strcmp(szName,str) == 0)
//		 m_pSelf->m_MateName.SetText("Ch�a c�");
//		else
//		 m_pSelf->m_MateName.SetText(szName);	
//	}
//}
//--------------------------------------------------------------------------
//	���ܣ���������
//--------------------------------------------------------------------------
void KUiParadeItem::UpdateData(KUiPlayerItem* pDest)
{
	if (pDest == NULL)
		return;

	UpdateBaseData(pDest);

	UpdateAllEquips(pDest);

	m_Dest = *pDest;
}

void KUiParadeItem::UpdateAllEquips(KUiPlayerItem* pDest)
{
	if (!g_pCoreShell)
		return;

	KUiObjAtRegion	Equips[_ITEM_COUNT];
	int nCount = g_pCoreShell->GetGameData(GDI_PARADE_EQUIPMENT, (unsigned int)&Equips, 0);
	int	i;
	for (i = 0; i < _ITEM_COUNT; i++)
		m_EquipBox[i].Celar();
	for (i = 0; i < nCount; i++)
	{
		if (Equips[i].Obj.uGenre != CGOG_NOTHING)
			UpdateEquip(&Equips[i], true);
	}
}

//--------------------------------------------------------------------------
//	���ܣ�װ���仯����
//--------------------------------------------------------------------------
void KUiParadeItem::UpdateEquip(KUiObjAtRegion* pEquip, int bAdd)
{
	if (pEquip)
	{
		for (int i = 0; i < _ITEM_COUNT; i++)
		{
			if (CtrlItemMap[i].nPosition == pEquip->Region.v)
			{
				if (bAdd)
					m_EquipBox[i].HoldObject(pEquip->Obj.uGenre, pEquip->Obj.uId,
						pEquip->Region.Width, pEquip->Region.Height);
				else
					m_EquipBox[i].HoldObject(CGOG_NOTHING, 0, 0, 0);
				break;
			}
		}
	}
}