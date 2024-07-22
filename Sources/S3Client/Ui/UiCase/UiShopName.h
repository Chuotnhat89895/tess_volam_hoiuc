// -------------------------------------------------------------------------
//	�ļ���		��	����ȷ�Ͽ�
//	������		��	Wooy(Wu yue)
//	����ʱ��	��	2003-5-28
// -------------------------------------------------------------------------
#ifndef __UiShopName_H__
#define __UiShopName_H__

#include "../elem/wndtext.h"
#include "../elem/wndedit.h"
#include "../elem/wndimage.h"
#include "../elem/wndbutton.h"
#include "../elem/wndlabeledbutton.h"
#include "../../../core/src/gamedatadef.h"



class KUiShopName : protected KWndImage
{
public:
	//----�������ͳһ�Ľӿں���----
	static KUiShopName*	OpenWindow();	
	static KUiShopName*	GetIfVisible();				//�������������ʾ���򷵻�ʵ��ָ��
	static void			CloseWindow(bool bDestroy);		//�رմ���
private:
	KUiShopName();
	~KUiShopName() {}
	void 	Initialize();								//��ʼ��
	void	Show();
	void	Hide();
	void	LoadScheme(const char* pScheme);			//������淽��
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	OnCancel();
	void	OnOk();

private:
	static KUiShopName*	m_pSelf;
	
	KWndEdit32    m_StringEdit;            

    KWndLabeledButton  m_BtnDone, m_BtnCancel;


};


#endif // __UiTradeConfirm_H__