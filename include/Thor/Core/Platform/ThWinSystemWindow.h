#pragma once

#include <Thor/Framework/ThiSystemWindow.h>
#include <Thor/Framework/FrameworkFwdInternal.h>
#include <windows.h>
#include <windowsx.h>

namespace Thor
{
//----------------------------------------------------------------------------------------
//
//					ThWinSystemWindow
//
//----------------------------------------------------------------------------------------
class ThWinSystemWindow:public ThiSystemWindow
{
public:
	ThWinSystemWindow();

	~ThWinSystemWindow();

	virtual void		Show();
	virtual void		Hide();
	virtual void		Minimize();
	virtual void		Maximize();
	virtual void		ShowCursor();
	virtual void		HideCursor();
	virtual void		SetCursor(const ThString& cursor);
	virtual void		SetIcon(const ThString& icon);
	virtual void		SetStyle(eWindowStyle::Val style);
	virtual void		SetClientArea(ThF32 width, ThF32 height);
	virtual void		SetWindowPos(ThF32 x, ThF32 y);
	virtual void		SetCaption(const ThString& caption);
	virtual ThF32		GetClientWidth()const;
	virtual ThF32		GetClientHeight()const;
	virtual ThF32		GetWidth()const;
	virtual ThF32		GetHeight()const;
	virtual void*		GetWindowHandle()const;

	virtual ThiType*	GetType()const;
	void				SetHwnd(HWND hwnd);
	LRESULT				ProcessMessage(UINT msg, WPARAM wParam, LPARAM lParam);

private:
	HWND		m_Hwnd;
	HCURSOR		m_Cursor;
	HICON		m_Icon;
};

}