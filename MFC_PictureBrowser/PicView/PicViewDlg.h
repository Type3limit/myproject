
// PicViewDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"


// CPicViewDlg 对话框
class CPicViewDlg : public CDialogEx
{
//常量数据
	const UINT ButtonWidth = 100;//按键宽度
	const UINT ButtonHeight = 50;//按键高度
//成员
	CImage m_image;//存储图像资源对象
	CRect  m_img_rect;//当前图片大小矩形
	CRect m_fullScreen_rect;//当前屏幕大小矩形
	CRect  m_client_rect;//当前客户端大小矩形
	CRect  m_function_rect;//当前功能(按键)区大小矩形
	CRect m_actual_rect;//实际可用于绘图的矩形
	CRect  m_draw_rect; //当前绘制区大小矩形
	CRect m_origin_rect;//原Draw_Rect大小，作为放大图片的参考矩形
	CRect m_filebutton_rect;//选择文件按钮的矩形
	CRect m_screenbutton_rect;//全屏按钮矩形
	CRect m_slider_rect;//滑块矩形

	WINDOWPLACEMENT m_wp_old;//保存旧窗口布局，用于从全屏状态下恢复

	BOOL m_IsFileOpened;//是否已经打开了图像文件
	BOOL m_IsFullScreen;//是否进入全屏显示状态
	BOOL m_IsZoomed;//是否放缩
	BOOL m_IsDragging;//是否正在拖拽

	BOOL m_IsReachTop;//是否触及顶部
	BOOL m_IsReachLeft;//是否触及左侧
	BOOL m_IsReachRight;//是否触及右侧
	BOOL m_IsReachBottom;//是否触及底部

	BOOL m_IsTrack;//是否跟踪鼠标，用于检测鼠标是否移出当前窗口

	INT m_ZoomScale;//记录当前放缩比例，整数单位，具体计算在放缩图片函数中进行
	CPoint m_OriginPoint;//记录上一次的鼠标位置
	CPoint m_OriginPointLT;//记录移动前左上的点位
	CPoint m_OriginPointRB;//记录移动前右下的点位
	CPoint m_PointDiff;//记录偏移量
	//界面相关成员
	CButton *m_BFile;//打开文件按钮
	CButton *m_BFullScreen;//全屏按钮
	CSliderCtrl m_SCZoom;	// 控制图片放缩


// 构造
public:
	CPicViewDlg(CWnd* pParent = NULL);	// 标准构造函数
	void DrawPic();//绘制图像文件
	void UpdateDrawRect();//根据不同情况调整Draw_Rect
	void MoveButton();//在图像文件打开后，移动按键至功能区矩形
	void ZoomPic();//放大图片
	virtual BOOL PreTranslateMessage(MSG *pMsg);//用于阻截ESC按键消息
	virtual void OnGetMinMaxInfo(MINMAXINFO* lpMMI);//用于限制窗口的最大宽窄度
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PICVIEW_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
