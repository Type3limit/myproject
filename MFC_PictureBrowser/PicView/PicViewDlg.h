
// PicViewDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"


// CPicViewDlg �Ի���
class CPicViewDlg : public CDialogEx
{
//��������
	const UINT ButtonWidth = 100;//�������
	const UINT ButtonHeight = 50;//�����߶�
//��Ա
	CImage m_image;//�洢ͼ����Դ����
	CRect  m_img_rect;//��ǰͼƬ��С����
	CRect m_fullScreen_rect;//��ǰ��Ļ��С����
	CRect  m_client_rect;//��ǰ�ͻ��˴�С����
	CRect  m_function_rect;//��ǰ����(����)����С����
	CRect m_actual_rect;//ʵ�ʿ����ڻ�ͼ�ľ���
	CRect  m_draw_rect; //��ǰ��������С����
	CRect m_origin_rect;//ԭDraw_Rect��С����Ϊ�Ŵ�ͼƬ�Ĳο�����
	CRect m_filebutton_rect;//ѡ���ļ���ť�ľ���
	CRect m_screenbutton_rect;//ȫ����ť����
	CRect m_slider_rect;//�������

	WINDOWPLACEMENT m_wp_old;//����ɴ��ڲ��֣����ڴ�ȫ��״̬�»ָ�

	BOOL m_IsFileOpened;//�Ƿ��Ѿ�����ͼ���ļ�
	BOOL m_IsFullScreen;//�Ƿ����ȫ����ʾ״̬
	BOOL m_IsZoomed;//�Ƿ����
	BOOL m_IsDragging;//�Ƿ�������ק

	BOOL m_IsReachTop;//�Ƿ񴥼�����
	BOOL m_IsReachLeft;//�Ƿ񴥼����
	BOOL m_IsReachRight;//�Ƿ񴥼��Ҳ�
	BOOL m_IsReachBottom;//�Ƿ񴥼��ײ�

	BOOL m_IsTrack;//�Ƿ������꣬���ڼ������Ƿ��Ƴ���ǰ����

	INT m_ZoomScale;//��¼��ǰ����������������λ����������ڷ���ͼƬ�����н���
	CPoint m_OriginPoint;//��¼��һ�ε����λ��
	CPoint m_OriginPointLT;//��¼�ƶ�ǰ���ϵĵ�λ
	CPoint m_OriginPointRB;//��¼�ƶ�ǰ���µĵ�λ
	CPoint m_PointDiff;//��¼ƫ����
	//������س�Ա
	CButton *m_BFile;//���ļ���ť
	CButton *m_BFullScreen;//ȫ����ť
	CSliderCtrl m_SCZoom;	// ����ͼƬ����


// ����
public:
	CPicViewDlg(CWnd* pParent = NULL);	// ��׼���캯��
	void DrawPic();//����ͼ���ļ�
	void UpdateDrawRect();//���ݲ�ͬ�������Draw_Rect
	void MoveButton();//��ͼ���ļ��򿪺��ƶ�����������������
	void ZoomPic();//�Ŵ�ͼƬ
	virtual BOOL PreTranslateMessage(MSG *pMsg);//�������ESC������Ϣ
	virtual void OnGetMinMaxInfo(MINMAXINFO* lpMMI);//�������ƴ��ڵ�����խ��
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PICVIEW_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnBnClickedButton1();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedButton2();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
};
