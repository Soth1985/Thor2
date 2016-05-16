#include <Thor/Framework/ThWinSystemWindow.h>

namespace Thor
{

THOR_REG_TYPE(ThWinSystemWindow, THOR_TYPELIST_1(ThiSystemWindow));
//----------------------------------------------------------------------------------------
//
//					WndProc
//
//----------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	ThWinSystemWindow* pWindow = 0;

	if (msg == WM_NCCREATE)
	{
		pWindow = (ThWinSystemWindow*)((LPCREATESTRUCT) lParam)->lpCreateParams;
#ifdef THOR_X64
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pWindow);
#else
		SetWindowLong (hwnd, GWL_USERDATA, (long) pWindow);
#endif
		//If you process any messages that are sent before CreateWindowEx returns
		//the HWND, you need something in the place of your HWND member.
		pWindow->SetHwnd(hwnd);
	}
	else
	{
#ifdef THOR_X64
		pWindow = (ThWinSystemWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
#else
		pWindow = (ThWinSystemWindow*)GetWindowLong(hwnd, GWL_USERDATA);
#endif
	}

	if(pWindow)
		return pWindow->ProcessMessage (msg, wParam, lParam);
	else
		return DefWindowProc(hwnd, msg, wParam, lParam);
}
//----------------------------------------------------------------------------------------
//
//					GetAccelerators
//
//----------------------------------------------------------------------------------------
ThFlags32 GetAccelerators()
{
	ThFlags32 result;

	if( GetAsyncKeyState(VK_LSHIFT) )
		result.SetFlag(true, eAcceleratorKey::LShift);

	if( GetAsyncKeyState(VK_RSHIFT) )
		result.SetFlag(true, eAcceleratorKey::RShift);

	if( GetAsyncKeyState(VK_LCONTROL) )
		result.SetFlag(true, eAcceleratorKey::LCtrl);

	if( GetAsyncKeyState(VK_RCONTROL) )
		result.SetFlag(true, eAcceleratorKey::LCtrl);

	if( GetAsyncKeyState(VK_LMENU) )
		result.SetFlag(true, eAcceleratorKey::LAlt);

	if( GetAsyncKeyState(VK_RMENU) )
		result.SetFlag(true, eAcceleratorKey::RAlt);
	
	return result;
}
//----------------------------------------------------------------------------------------
//
//					ThWinSystemWindow
//
//----------------------------------------------------------------------------------------
ThWinSystemWindow::ThWinSystemWindow()
	:
m_Hwnd(0),
m_Cursor(0)
{
	//Get current module handle
	HMODULE thisModule;
	GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCSTR)this, &thisModule);
	//Register window class
	static ThString className("ThorSystemWindow");
	WNDCLASSEX wndClass;
	//Register the class if it does not already exist
	if( !GetClassInfoEx(thisModule, className.c_str(), &wndClass) )
	{
		memset(&wndClass, 0, sizeof WNDCLASSEX);
		wndClass.cbSize = sizeof WNDCLASSEX;
		wndClass.lpfnWndProc = WndProc;
		wndClass.hInstance = thisModule;
		wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wndClass.lpszClassName = className.c_str();
		wndClass.hIcon = LoadIcon(0, MAKEINTRESOURCE(IDI_WINLOGO) );
		wndClass.hIconSm = LoadIcon(0, MAKEINTRESOURCE(IDI_WINLOGO) );
		m_Cursor = wndClass.hCursor = LoadCursor(0, MAKEINTRESOURCE(IDC_ARROW) );
		wndClass.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);

		if( !RegisterClassEx(&wndClass) )
			throw TheFailedToCreateWindow();
	}

	m_Hwnd = CreateWindowEx(0, className.c_str(), "", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 300, 300, 0, 0, thisModule, this);
	::SetCursor(m_Cursor);
}
//----------------------------------------------------------------------------------------
ThWinSystemWindow::~ThWinSystemWindow()
{
	DestroyWindow(m_Hwnd);
}
//----------------------------------------------------------------------------------------
void ThWinSystemWindow::SetHwnd(HWND hwnd)
{
	m_Hwnd = hwnd;
}
//----------------------------------------------------------------------------------------
LRESULT ThWinSystemWindow::ProcessMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch(msg)
	{	
	case WM_LBUTTONDOWN:
		{
			md_OnMouseButtonDown(eMouseButton::Left, GetAccelerators(), (ThF32)GET_X_LPARAM(lParam), (ThF32)GET_Y_LPARAM(lParam));
			break;
		}
	case WM_LBUTTONUP:
		{
			md_OnMouseButtonUp(eMouseButton::Left, GetAccelerators(), (ThF32)GET_X_LPARAM(lParam), (ThF32)GET_Y_LPARAM(lParam));
			break;
		}
	case WM_RBUTTONDOWN:
		{
			md_OnMouseButtonDown(eMouseButton::Right, GetAccelerators(), (ThF32)GET_X_LPARAM(lParam), (ThF32)GET_Y_LPARAM(lParam));
			break;
		}
	case WM_RBUTTONUP:
		{
			md_OnMouseButtonUp(eMouseButton::Right, GetAccelerators(), (ThF32)GET_X_LPARAM(lParam), (ThF32)GET_Y_LPARAM(lParam));
			break;
		}
	case WM_MBUTTONDOWN:
		{
			md_OnMouseButtonDown(eMouseButton::Middle, GetAccelerators(), (ThF32)GET_X_LPARAM(lParam), (ThF32)GET_Y_LPARAM(lParam));
			break;
		}		
	case WM_MBUTTONUP:
		{
			md_OnMouseButtonUp(eMouseButton::Middle, GetAccelerators(), (ThF32)GET_X_LPARAM(lParam), (ThF32)GET_Y_LPARAM(lParam));
			break;
		}
	case WM_MOUSEMOVE:
		{
			md_OnMouseMove(GetAccelerators(), (ThF32)GET_X_LPARAM(lParam), (ThF32)GET_Y_LPARAM(lParam));
			break;
		}
	case WM_MOUSEWHEEL:
		{
			md_OnMouseWheel(GetAccelerators(), (ThF32)GET_WHEEL_DELTA_WPARAM(wParam), (ThF32)GET_X_LPARAM(lParam), (ThF32)GET_Y_LPARAM(lParam));
			break;
		}
	case WM_SIZE:
		{
			md_OnSize((ThF32)GET_X_LPARAM(lParam), (ThF32)GET_Y_LPARAM(lParam));
			break;
		}
	case WM_CREATE:
		{
			md_OnCreate();
			break;
		}
	case WM_DESTROY:
		{
			md_OnDestroy();
			break;
		}
	case WM_PAINT:
		{
			hdc = BeginPaint(m_Hwnd, &ps);
			md_OnPaint();
			EndPaint(m_Hwnd, &ps);
			break;
		}	
	case WM_SETCURSOR: 
		{
			if(m_Cursor != NULL)
				::SetCursor(m_Cursor); 
			break; 
		}
	default:
		return DefWindowProc (m_Hwnd, msg, wParam, lParam);
	}

	return 0;	
}
//----------------------------------------------------------------------------------------
void ThWinSystemWindow::Show()
{
	ShowWindow(m_Hwnd, SW_SHOW);
}
//----------------------------------------------------------------------------------------
void ThWinSystemWindow::Hide()
{
	ShowWindow(m_Hwnd, SW_HIDE);
}
//----------------------------------------------------------------------------------------
void ThWinSystemWindow::Minimize()
{
	ShowWindow(m_Hwnd, SW_MINIMIZE);
}
//----------------------------------------------------------------------------------------
void ThWinSystemWindow::Maximize()
{
	ShowWindow(m_Hwnd, SW_MAXIMIZE);
}
//----------------------------------------------------------------------------------------
void ThWinSystemWindow::ShowCursor()
{
	::ShowCursor(TRUE);
}
//----------------------------------------------------------------------------------------
void ThWinSystemWindow::HideCursor()
{
	::ShowCursor(FALSE);
}
//----------------------------------------------------------------------------------------
void ThWinSystemWindow::SetCursor(const ThString& cursor)
{
	m_Cursor = LoadCursor( GetModuleHandle(0), cursor.c_str() );

	if(m_Cursor != NULL)
	{
		::SetCursor(m_Cursor);
	}
}
//----------------------------------------------------------------------------------------
void ThWinSystemWindow::SetIcon(const ThString& icon)
{
	m_Icon = LoadIcon( GetModuleHandle(0), icon.c_str() );

	if(m_Icon != NULL)
	{
		SendMessage( m_Hwnd, WM_SETICON, ICON_BIG, (LPARAM)m_Icon );
		SendMessage( m_Hwnd, WM_SETICON, ICON_SMALL, (LPARAM)m_Icon );
	}
}
//----------------------------------------------------------------------------------------
void ThWinSystemWindow::SetStyle(eWindowStyle::Val style)
{
	switch(style)
	{
	case eWindowStyle::CanvasOnly:
		{
			SetWindowLong(m_Hwnd, GWL_EXSTYLE, WS_EX_TOPMOST );
			SetWindowLong(m_Hwnd, GWL_STYLE, WS_POPUP );
			break;
		}
	case eWindowStyle::HasBorderAndCaption:
		{
			SetWindowLong(m_Hwnd, GWL_EXSTYLE, 0 );
			SetWindowLong(m_Hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW );
			break;
		}
	}
}
//----------------------------------------------------------------------------------------
void ThWinSystemWindow::SetClientArea(ThF32 width, ThF32 height)
{
	RECT rcClient, rcWindow;
	POINT ptDiff;
	GetClientRect(m_Hwnd, &rcClient);
	GetWindowRect(m_Hwnd, &rcWindow);
	ptDiff.x = (rcWindow.right - rcWindow.left) - rcClient.right;
	ptDiff.y = (rcWindow.bottom - rcWindow.top) - rcClient.bottom;
	MoveWindow(m_Hwnd, rcWindow.left, rcWindow.top, (int)width + ptDiff.x, (int)height + ptDiff.y, TRUE);
}
//----------------------------------------------------------------------------------------
void ThWinSystemWindow::SetWindowPos(ThF32 x, ThF32 y)
{
	::SetWindowPos(m_Hwnd, HWND_TOP, (int)x, (int)y, 0, 0, SWP_NOSIZE);
}
//----------------------------------------------------------------------------------------
void ThWinSystemWindow::SetCaption(const ThString& caption)
{
	SetWindowText(m_Hwnd, caption.c_str() );
}
//----------------------------------------------------------------------------------------
ThF32 ThWinSystemWindow::GetClientWidth()const
{
	RECT rcClient;
	GetClientRect(m_Hwnd, &rcClient);
	return ThF32( rcClient.right - rcClient.left );
}
//----------------------------------------------------------------------------------------
ThF32 ThWinSystemWindow::GetClientHeight()const
{
	RECT rcClient;
	GetClientRect(m_Hwnd, &rcClient);
	return ThF32( rcClient.bottom - rcClient.top );
}
//----------------------------------------------------------------------------------------
ThF32 ThWinSystemWindow::GetWidth()const
{
	RECT rcWindow;	
	GetWindowRect(m_Hwnd, &rcWindow);
	return ThF32( rcWindow.right - rcWindow.left );
}
//----------------------------------------------------------------------------------------
ThF32 ThWinSystemWindow::GetHeight()const
{
	RECT rcWindow;	
	GetWindowRect(m_Hwnd, &rcWindow);
	return ThF32( rcWindow.bottom - rcWindow.top );
}
//----------------------------------------------------------------------------------------
void* ThWinSystemWindow::GetWindowHandle()const
{
	const HWND* ptr = &m_Hwnd;
	return (void*)(ptr);
}
//----------------------------------------------------------------------------------------
ThiType* ThWinSystemWindow::GetType()const
{
	return Thor::TypeOf<ThWinSystemWindow>();
}

}
