
// PicViewDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PicView.h"
#include "PicViewDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPicViewDlg �Ի���



CPicViewDlg::CPicViewDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PICVIEW_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPicViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER1, m_SCZoom);
}

BEGIN_MESSAGE_MAP(CPicViewDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_FILE, &CPicViewDlg::OnBnClickedButton1)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON_SCREEN, &CPicViewDlg::OnBnClickedButton2)
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_WM_HSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
END_MESSAGE_MAP()

// CPicViewDlg ��Ϣ�������

BOOL CPicViewDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	//��ʼ������BOOL
	m_IsFileOpened = FALSE;
	m_IsFullScreen = FALSE;
	m_IsZoomed = FALSE;
	m_IsDragging = FALSE;
	m_IsReachTop = m_IsReachLeft = m_IsReachRight = m_IsReachBottom = FALSE;
	//��ʼ����������
	int cur_X = GetSystemMetrics(SM_CXFULLSCREEN);
	int cur_Y = GetSystemMetrics(SM_CYFULLSCREEN);
	CRect cur_rect;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &cur_rect, 0);
	cur_Y = cur_rect.bottom;
	m_fullScreen_rect = CRect(0, 0, cur_X, cur_Y);//��ʼ����Ļ����Ϊ������Ļ�Ĵ�С
	GetClientRect(&m_client_rect);//��ʼ���ͻ�������Ϊ��ǰ�ͻ�����С
	//��ʼ������������Ϊ��ǰ�ͻ�����С��ȥ�����߶�
	m_function_rect = CRect(m_client_rect.left, m_client_rect.bottom - ButtonHeight, m_client_rect.Width(), m_client_rect.Height());
	m_draw_rect = CRect(m_client_rect.left, m_client_rect.top, m_client_rect.right, m_client_rect.bottom - ButtonHeight);
	m_origin_rect = m_draw_rect;
	m_actual_rect = m_draw_rect;

	m_filebutton_rect = CRect(m_function_rect.right - ButtonWidth,
		m_function_rect.bottom - ButtonHeight,
		m_function_rect.right, m_function_rect.bottom);

	m_screenbutton_rect = CRect(m_function_rect.left,
		m_function_rect.bottom - ButtonHeight,
		m_function_rect.left + ButtonWidth, m_function_rect.bottom);


	int slider_left = (m_function_rect.right - m_fullScreen_rect.left) / 2 - ButtonWidth * 2;
	int slider_right = (m_function_rect.right - m_fullScreen_rect.left) / 2 + (ButtonWidth * 2);

	m_slider_rect = CRect(slider_left,
		m_function_rect.bottom - ButtonHeight,
		slider_right,
		m_function_rect.bottom);

	//��ʼ��button
	m_BFile = new CButton();
	m_BFullScreen = new CButton();

	m_BFile->Create(L"ѡ���ļ�", BS_PUSHBUTTON,
		m_filebutton_rect,
		this, IDC_BUTTON_FILE);

	m_BFullScreen->Create(L"ȫ��Ԥ��", BS_PUSHBUTTON,
		m_screenbutton_rect,
		this, IDC_BUTTON_SCREEN);

	m_BFile->ShowWindow(SW_SHOW);

	m_SCZoom.SetRange(0, 100, TRUE);
	m_SCZoom.SetPos(0);

	//��ʼ������Point
	m_PointDiff = 0;
	m_OriginPointLT = m_draw_rect.TopLeft();
	m_OriginPointRB = m_draw_rect.BottomRight();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CPicViewDlg::OnPaint()
{
	DrawPic();
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CPicViewDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CPicViewDlg::DrawPic()//����ͼ��
{
	if (m_image.IsNull())
		return;
	//�ػ�ؼ�
	m_IsFileOpened ? m_BFullScreen->ShowWindow(SW_SHOW) : m_BFullScreen->ShowWindow(SW_HIDE);
	if (m_IsFullScreen)
	{
		m_BFile->ShowWindow(SW_HIDE);
		m_BFullScreen->ShowWindow(SW_HIDE);
		m_SCZoom.ShowWindow(SW_HIDE);
	}
	else
	{
		m_BFile->ShowWindow(SW_SHOW);
		m_BFullScreen->ShowWindow(SW_SHOW);
		m_SCZoom.ShowWindow(SW_SHOW);
	}

	CClientDC dc(this);
	CDC memDC;
	CBitmap membitmap;
	CRgn rgn1, rgn2;
	CRect tmpl(m_screenbutton_rect.right,
		m_screenbutton_rect.top,
		m_slider_rect.left,
		m_screenbutton_rect.bottom);
	CRect tmpr(m_slider_rect.right,
		m_slider_rect.top,
		m_filebutton_rect.left,
		m_filebutton_rect.bottom);
	rgn1.CreateRectRgnIndirect(tmpl);
	rgn2.CreateRectRgnIndirect(tmpr);
	rgn1.CombineRgn(&rgn1, &rgn2, RGN_XOR);
	InvalidateRgn(&rgn1, TRUE);
	memDC.CreateCompatibleDC(&dc);
	memDC.SetStretchBltMode(HALFTONE);
	membitmap.CreateCompatibleBitmap(&dc, m_client_rect.Width(), m_client_rect.Height());
	memDC.SelectObject(&membitmap);
	memDC.FillSolidRect(&m_actual_rect, RGB(245, 245, 245));
	memDC.FillSolidRect(&m_function_rect, RGB(245, 245, 245));
	m_image.Draw(memDC, m_draw_rect, m_img_rect);
	dc.BitBlt(0, 0,
		m_actual_rect.Width(),
		m_actual_rect.Height(),
		&memDC,
		0, 0,
		SRCCOPY);
	//�ƺ�
	memDC.DeleteDC();
	membitmap.DeleteObject();
}

void CPicViewDlg::OnBnClickedButton1()//�˺��������ͼ���ļ��Ĵ��Լ�m_img_rect �ĵ���
{
	//���ļ�
	m_PointDiff = 0;
	CString filter(L"ͼ���ļ�|*.jpg;*.png;*.bmp;*.jpeg|JPG�ļ�|*.jpg|PNG�ļ�|*.png|BMP�ļ�|*.bmp|JPEG�ļ�|*.jpeg|ICO�ļ�|*.ico||�����ļ�|*.*");
	CFileDialog FileOpen(TRUE, 0, 0, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter);
	if (IDOK != FileOpen.DoModal())
	{
		MessageBox(L"δ��ͼ��");
		return;
	}
	CString FilePath = FileOpen.GetPathName();
	if (!m_image.IsNull())
		m_image.Destroy();
	HRESULT result = m_image.Load(FilePath);
	if (FAILED(result))
	{
		MessageBox(L"��ͼ��ʧ��");
		return;
	}
	int imgWidth = m_image.GetWidth();
	int imgHeight = m_image.GetHeight();
	m_img_rect = CRect(0, 0, imgWidth, imgHeight);
	m_IsFileOpened = TRUE;
	if (m_ZoomScale != 0)
	{
		m_SCZoom.SetPos(0);
		m_ZoomScale = 0;
	}
	UpdateDrawRect();
	m_OriginPointLT = m_draw_rect.TopLeft();
	m_OriginPointRB = m_draw_rect.BottomRight();
}

BOOL CPicViewDlg::OnEraseBkgnd(CDC* pDC)//����ϵͳ�ı����ػ�
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//InvalidateRect(m_client_rect, FALSE);
	//return TRUE;
	if (!m_IsFullScreen)
		return CDialogEx::OnEraseBkgnd(pDC);
	else
		return TRUE;
}

void CPicViewDlg::OnBnClickedButton2()
{
	if (!m_IsFullScreen)
	{
		m_IsFullScreen = TRUE;
		WINDOWPLACEMENT wp_new;
		int sWidth = GetSystemMetrics(SM_CXSCREEN);
		int sHeight = GetSystemMetrics(SM_CYSCREEN);
		CRect Window_Rect;
		::GetWindowRect(::GetDesktopWindow(), &Window_Rect);
		GetWindowPlacement(&m_wp_old);
		CRect Client_Rect;
		RepositionBars(0, 0xffff, AFX_IDW_PANE_FIRST, reposQuery, &Client_Rect);
		ClientToScreen(&Client_Rect);
		::AdjustWindowRectEx(&Window_Rect, GetStyle(), FALSE, GetExStyle());
		wp_new.length = sizeof(WINDOWPLACEMENT);
		wp_new.showCmd = SW_SHOWNORMAL;
		wp_new.rcNormalPosition = Window_Rect;
		m_client_rect = Window_Rect;
		SetWindowPlacement(&wp_new);
		UpdateDrawRect();
	}
}

BOOL CPicViewDlg::PreTranslateMessage(MSG *pMsg)
{
	if (pMsg->message == WM_KEYDOWN&&m_IsFullScreen)
	{
		if (pMsg->wParam == VK_ESCAPE)
		{
			m_IsFullScreen = FALSE;
			SetWindowPlacement(&m_wp_old);
			UpdateDrawRect();
			return TRUE;
		}
		else
			return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CPicViewDlg::UpdateDrawRect()
{
	GetClientRect(&m_client_rect);
	if (!m_IsFullScreen&&m_IsFileOpened)
	{

		m_function_rect = CRect(m_client_rect.left, m_client_rect.bottom - ButtonHeight, m_client_rect.right, m_client_rect.bottom);
		m_actual_rect = CRect(m_client_rect.left, m_client_rect.top, m_client_rect.Width(), m_client_rect.Height() - ButtonHeight);
	}
	else if (m_IsFullScreen)
		m_actual_rect = m_client_rect;
	else
		return;
	

	
	int img_width = m_img_rect.Width();
	int img_height = m_img_rect.Height();

	int actual_width = m_actual_rect.Width();
	int actual_height = m_actual_rect.Height();

	float fscaleimgHW = static_cast<float>(img_height) / static_cast<float>(img_width);
	float fscaleimgWH = static_cast<float>(img_width) / static_cast<float>(img_height);

	int cur_W = 0, cur_H = 0;
	float fscale = 1.0;
	if (img_width > actual_width&& img_height <= actual_height)//ͼ���ȴ��ڿͻ�����ͼ���С�ڵ��ڿͻ���
	{
		cur_W = actual_width;
		cur_H = static_cast<int>(cur_W * fscaleimgHW);
		m_draw_rect = CRect((actual_width - cur_W) / 2,
			(actual_height - cur_H) / 2,
			(actual_width + cur_W) / 2,
			(actual_height + cur_H) / 2);
	}
	else if (img_width <= actual_width && img_height > actual_height)//ͼ����С�ڵ��ڿͻ�����ͼ����ڿͻ���
	{
		cur_H = actual_height;
		cur_W = static_cast<int>(cur_H * fscaleimgWH);
		m_draw_rect = CRect((actual_width - cur_W) / 2,
			(actual_height - cur_H) / 2,
			(actual_width + cur_W) / 2,
			(actual_height + cur_H) / 2);
	}
	else if (img_width <= actual_width && img_height < actual_height)//ͼ����С�ڵ��ڿͻ�����ͼ���С�ڿͻ���
	{
		m_draw_rect = CRect((actual_width - img_width) / 2,
			(actual_height - img_height) / 2,
			(actual_width + img_width) / 2,
			(actual_height + img_height) / 2);
	}
	else//ͼ�����ڿͻ�����ͼ��ߴ��ڿͻ���
	{

		float fscaleCWH = static_cast<float>(m_actual_rect.Width()) / static_cast<float>(m_actual_rect.Height());
		float fscaleIWH = static_cast<float>(m_img_rect.Width()) / static_cast<float>(m_img_rect.Height());
		bool Choose = (fscaleCWH - fscaleIWH) > 0 ? false : true;
		if (Choose)
		{

			fscaleimgHW - fscaleimgWH > 0 ? cur_H = actual_height : cur_W = actual_width;
			fscaleimgHW - fscaleimgWH > 0 ? cur_W = static_cast<int>(cur_H * fscaleimgWH) : cur_H = static_cast<int>(cur_W * fscaleimgHW);
		}
		else
		{
			fscaleimgHW - fscaleimgWH > 0 ? cur_W = actual_width : cur_H = actual_height;
			fscaleimgHW - fscaleimgWH > 0 ? cur_H = static_cast<int>(cur_W * fscaleimgHW) : cur_W = static_cast<int>(cur_H * fscaleimgWH);
		}
		m_draw_rect = CRect((actual_width - cur_W) / 2,
			(actual_height - cur_H) / 2,
			(actual_width + cur_W) / 2,
			(actual_height + cur_H) / 2);
	}
	m_origin_rect = m_draw_rect;
	MoveButton();
	if (m_IsZoomed)
		ZoomPic();
	SendMessage(WM_PAINT);
}

void CPicViewDlg::MoveButton()
{
	if (!m_IsFullScreen)
	{

		m_filebutton_rect = CRect(m_function_rect.right - ButtonWidth,
			m_function_rect.bottom - ButtonHeight,
			m_function_rect.right, m_function_rect.bottom);

		m_BFile->MoveWindow(m_filebutton_rect);

		m_screenbutton_rect = CRect(m_function_rect.left,
			m_function_rect.bottom - ButtonHeight,
			m_function_rect.left + ButtonWidth, m_function_rect.bottom);

		m_BFullScreen->MoveWindow(m_screenbutton_rect);

		int slider_left = (m_function_rect.right - m_fullScreen_rect.left) / 2 - ButtonWidth * 2;
		int slider_right = (m_function_rect.right - m_fullScreen_rect.left) / 2 + (ButtonWidth * 2);

		m_slider_rect = CRect(slider_left,
			m_function_rect.bottom - ButtonHeight,
			slider_right,
			m_function_rect.bottom);

		m_SCZoom.MoveWindow(m_slider_rect);
	}
}

void CPicViewDlg::OnSize(UINT nType, int cx, int cy)
{
	UpdateDrawRect();
	CDialogEx::OnSize(nType, cx, cy);
	// TODO: �ڴ˴������Ϣ����������
}

void CPicViewDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	if (m_IsFileOpened)
	{
		lpMMI->ptMinTrackSize.x = 800;
		lpMMI->ptMinTrackSize.y = 600;
	}
	CDialog::OnGetMinMaxInfo(lpMMI);
}

void CPicViewDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (pScrollBar == (CScrollBar*)&m_SCZoom)
	{
		m_ZoomScale = m_SCZoom.GetPos();
		UpdateData(FALSE);
		if (!m_IsZoomed)
		{
			m_SCZoom.SetPos(0);
		}
		m_IsZoomed = TRUE;
		ZoomPic();
	}
	else
		CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CPicViewDlg::ZoomPic()//����ͼƬ
{
	if (m_ZoomScale == 0)
	{
		m_IsZoomed = FALSE;
		m_PointDiff = 0;
		UpdateDrawRect();
	}
	else
	{
		CRect rect;

		//��ͼʵ�ַ���ͼƬʱ����ԭͼƬ����ק���λ�ã�����ʱδ�ɹ�  2019/12/19 15��25
		if ((m_PointDiff.x > 0 || m_PointDiff.y > 0)&&(m_ZoomScale>10))
		{
		 	rect = CRect(
				m_origin_rect.TopLeft() + m_PointDiff,
				m_origin_rect.BottomRight() + m_PointDiff);
		}
		else 
			rect = m_origin_rect;
		float cur_Scale = static_cast<float>(1.0 + m_ZoomScale * 0.01);
		int draw_width = static_cast<int>(rect.Width()*cur_Scale);
		int draw_height = static_cast<int>(rect.Height()*cur_Scale);
		int diffX = (draw_width - rect.Width()) / 2;
		int diffY = (draw_height - rect.Height()) / 2;
		int cur_left = rect.left - diffX;
		int cur_top = rect.top - diffY;
		int cur_right = rect.right + diffX;
		int cur_bottom = rect.bottom + diffY;
		m_draw_rect = CRect(cur_left, cur_top, cur_right, cur_bottom);
		m_OriginPointLT = m_draw_rect.TopLeft();
		m_OriginPointRB = m_draw_rect.BottomRight();
		SendMessage(WM_PAINT);
	}
}

void CPicViewDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (PtInRect(m_actual_rect, point) && m_IsZoomed)
	{
		m_IsDragging = TRUE;
		m_OriginPoint = point;
	}
	else
	{
		m_IsDragging = FALSE;
		CDialogEx::OnLButtonDown(nFlags, point);
	}
}

void CPicViewDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_IsDragging = FALSE;
	CDialogEx::OnLButtonUp(nFlags, point);
}

void CPicViewDlg::OnMouseMove(UINT nFlags, CPoint point)
{

	if (!m_IsTrack)
	{
		TRACKMOUSEEVENT MEvent;
		MEvent.cbSize = sizeof(TRACKMOUSEEVENT);
		MEvent.dwFlags = TME_LEAVE;
		MEvent.hwndTrack = this->m_hWnd;
		if (::_TrackMouseEvent(&MEvent))
		{
			m_IsTrack = TRUE;
		}
	}

	m_IsReachLeft = (m_draw_rect.left >= m_actual_rect.left ? TRUE : FALSE);
	m_IsReachRight = (m_draw_rect.right <= m_actual_rect.right ? TRUE : FALSE);
	m_IsReachTop = (m_draw_rect.top >= m_actual_rect.top ? TRUE : FALSE);
	m_IsReachBottom = (m_draw_rect.bottom <= m_actual_rect.bottom ? TRUE : FALSE);

	if (m_IsFullScreen && m_IsReachLeft &&m_IsReachRight &&m_IsReachTop&&m_IsReachBottom)
		m_IsDragging = FALSE;

	if (!PtInRect(m_actual_rect, point))
		m_IsDragging = FALSE;

	if (m_IsDragging == TRUE)
	{
		CPoint point_buffer = point - m_OriginPoint;
	    CPoint LT = m_OriginPointLT + m_PointDiff;
		CPoint RB = m_OriginPointRB + m_PointDiff;
		if (m_IsReachTop && (point_buffer.y > 0) && (LT.y > m_actual_rect.top))
		{
			m_PointDiff.y -= (LT.y - m_actual_rect.top);
			point_buffer.y = 0;
		}
		if (m_IsReachLeft && (point_buffer.x > 0) && (LT.x > m_actual_rect.left))
		{
			m_PointDiff.x -= (LT.x - m_actual_rect.left);
			point_buffer.x = 0;
		}

		if (m_IsReachRight && (point_buffer.x < 0) && (RB.x < m_actual_rect.right))
		{
			m_PointDiff.x += (m_actual_rect.right - RB.x);
			point_buffer.x = 0;

		}
		if (m_IsReachBottom && (point_buffer.y < 0) && (RB.y < m_actual_rect.bottom))
		{
			m_PointDiff.y += (m_actual_rect.bottom - RB.y);
			point_buffer.y = 0;
		}
		m_PointDiff += point_buffer;
		 LT = m_OriginPointLT + m_PointDiff;
		 RB = m_OriginPointRB + m_PointDiff;
		m_draw_rect = CRect(LT, RB);
		m_OriginPoint = point;
		if(LT.x<=m_actual_rect.left&&
			LT.y<=m_actual_rect.top&&
			RB.x>=m_actual_rect.right&&
			RB.y>=m_actual_rect.bottom)
		SendMessage(WM_PAINT);
	}
	else
	{
		CDialogEx::OnMouseMove(nFlags, point);
	}
}

void CPicViewDlg::OnMouseLeave()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_IsDragging = FALSE;
	m_IsTrack = FALSE;
	CDialogEx::OnMouseLeave();
}
