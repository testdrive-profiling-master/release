//================================================================================
// Copyright (c) 2013 ~ 2019. HyungKi Jeong(clonextop@gmail.com)
// All rights reserved.
// 
// The 3-Clause BSD License (https://opensource.org/licenses/BSD-3-Clause)
// 
// Redistribution and use in source and binary forms,
// with or without modification, are permitted provided
// that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 
// 3. Neither the name of the copyright holder nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
// BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
// GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
// ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
// OF SUCH DAMAGE.
// 
// Title : TestDrive system
// Rev.  : 11/5/2019 Tue (clonextop@gmail.com)
//================================================================================
#ifndef __TESTDRIVE_H__
#define __TESTDRIVE_H__

#define WIN32_LEAN_AND_MEAN
#include <ObjBase.h>
#include "TestDriveVersion.h"

#ifndef TESTDRIVE_ENTRY
#include <tchar.h>
#include "stdstring.h"
#define CString		CStdString
#define CStringA	CStdStringA
#define CStringW	CStdStringW
#endif//TESTDRIVE_ENTRY

#define	REDIRECT_ARGUMENT(n)			(0xFEED0000|(n))

#ifdef __cplusplus
extern "C" {
#endif

// Semaphore
interface ITDSemaphore{
	STDMETHOD_(int, Down)(void) PURE;
	STDMETHOD_(int, Up)(void) PURE;
	STDMETHOD_(void, Release)(void) PURE;
};

// Object Layout Interface
interface ITDLayout{
	STDMETHOD_(void, SetPosition)(int x, int y) PURE;
	STDMETHOD_(void, SetSize)(int width, int height) PURE;
	STDMETHOD_(void, Move)(int x, int y) PURE;
	STDMETHOD_(void, SetRect)(const RECT* pRc) PURE;
	STDMETHOD_(void, GetRect)(RECT* pRc) PURE;
	STDMETHOD_(int, GetX)(void) PURE;
	STDMETHOD_(int, GetY)(void) PURE;
	STDMETHOD_(int, GetWidth)(void) PURE;
	STDMETHOD_(int, GetHeight)(void) PURE;
	STDMETHOD_(void, SetX)(int x) PURE;
	STDMETHOD_(void, SetY)(int y) PURE;
	STDMETHOD_(void, SetWidth)(int iWidth) PURE;
	STDMETHOD_(void, SetHeight)(int iHeight) PURE;
};

typedef enum{
	TDOBJECT_BUFFER,
	TDOBJECT_CHART,
	TDOBJECT_REPORT,
	TDOBJECT_BUTTON,
	TDOBJECT_HTML,
	TDOBJECT_NONE		= -1
}OBJECT_TYPE;

// TestDrive Object Interface
interface ITDObject{
	// get window layout interface
	STDMETHOD_(ITDLayout*, GetLayout)(void) PURE;
	STDMETHOD_(void, UpdateLayout)(void) PURE;
	STDMETHOD_(OBJECT_TYPE, GetType)(void) PURE;
	STDMETHOD_(void, SetName)(LPCTSTR lpszName) PURE;
	STDMETHOD_(void, SetEnable)(BOOL bEnable = TRUE) PURE;
	STDMETHOD_(void, Show)(BOOL bEnable = TRUE) PURE;
	STDMETHOD_(BOOL, IsVisible)(void) PURE;
	STDMETHOD_(void, Release)(void) PURE;
};

// Buffer interface
interface ITDBufferManager{
	STDMETHOD_(void, OnBufferClick)(DWORD dwID) {};											// set buffer click
	STDMETHOD_(void, OnBufferMouseEvent)(DWORD dwID, DWORD x, DWORD y, DWORD dwFlag) {};	// mouse event, dwFlag(0:none, 0x1:left, 0x2:right, 0x4:on zoom, 0x8:context menu, 0x80000000:pushed)
	STDMETHOD_(BOOL, OnBufferBeforeFileOpen)(DWORD dwID, LPCTSTR sFilename, DWORD dwWidth, DWORD dwHeight) {return TRUE;};	// when file open 'LoadImage' function's called. if you returned false, image will not open. 
};

typedef struct{
	int		x;
	int		y;
}ITDPoint;

typedef struct{
	float	x;
	float	y;
}ITDPointf;

typedef enum{
	ITDDashStyleSolid,          // 0
	ITDDashStyleDash,           // 1
	ITDDashStyleDot,            // 2
	ITDDashStyleDashDot,        // 3
	ITDDashStyleDashDotDot,     // 4
	ITDDashStyleCustom          // 5
}ITDDashStyle;

typedef enum{
	ITDDashCapFlat             = 0,
	ITDDashCapRound            = 2,
	ITDDashCapTriangle         = 3
}ITDDashCap;

typedef enum{
	COLORTYPE_ALPHA,			// A8
	COLORTYPE_LUMINANCE,		// L8
	COLORTYPE_LUMINANCE_ALPHA,	// L8A8
	COLORTYPE_RGB_565,			// R5G6B5
	COLORTYPE_RGBA_4444,		// R4G4B4A4
	COLORTYPE_RGBA_5551,		// R5G5B5A1
	COLORTYPE_RGB_888,			// R8G8B8
	COLORTYPE_ABGR_8888,		// A8B8G8R8
	COLORTYPE_RGBA_8888,		// R8G8B8A8
	COLORTYPE_ARGB_8888,		// A8R8G8B8
	COLORTYPE_SIZE
}COLORFORMAT;

typedef enum{
	IMAGETYPE_PNG,				// *.png
	IMAGETYPE_JPG,				// *.jpg
	IMAGETYPE_TIFF,				// *.tiff
	IMAGETYPE_BMP,				// *.bmp
	IMAGETYPE_GIF,				// *.gif
	IMAGETYPE_AUTO				// auto check
}IMAGETYPE;

typedef enum{
	BUFFER_INTERFACE_ZOOM,
	BUFFER_INTERFACE_ANTIALIASING,
	BUFFER_INTERFACE_LINK,
	BUFFER_INTERFACE_EVENT,
	BUFFER_INTERFACE_LOAD_FROM_FILE,
	BUFFER_INTERFACE_OUTLINE,
	BUFFER_INTERFACE_AUTOFIT,
}BUFFER_INTERFACE;

interface ITDBuffer{
	// get common interface object
	STDMETHOD_(ITDObject*, GetObject)(void) PURE;
	STDMETHOD_(void, SetManager)(ITDBufferManager* pManager, DWORD dwID = 0) PURE;	// set control manager 
	STDMETHOD_(BOOL, Create)(DWORD dwWidth, DWORD dwHeight, COLORFORMAT Type = COLORTYPE_ABGR_8888) PURE;
	STDMETHOD_(BOOL, CreateFromFile)(LPCTSTR sFilename, COLORFORMAT Type = COLORTYPE_ABGR_8888) PURE;
	STDMETHOD_(BOOL, LoadImage)(LPCTSTR sFilename) PURE;
	STDMETHOD_(BOOL, CopyToMemory)(BYTE* pMEM, DWORD dwByteStride = 0) PURE;
	STDMETHOD_(BOOL, CopyFromMemory)(const BYTE* pMEM, DWORD dwByteStride = 0, BOOL bReverse = FALSE) PURE;
	STDMETHOD_(BOOL, CopyFromBuffer)(ITDBuffer* pBuffer, int x = 0, int y = 0, int sx = 0, int sy = 0, int width = 0, int height = 0, int dwidth = 0, int dheight = 0) PURE;
	STDMETHOD_(BOOL, SaveToFile)(LPCTSTR sFilename, IMAGETYPE Type = IMAGETYPE_AUTO, BOOL bStoreAlpha = FALSE) PURE;
	STDMETHOD_(void, ReleaseAll)(void) PURE;
	STDMETHOD_(BOOL, Compare)(ITDBuffer* pBuffer) PURE;
	STDMETHOD_(BOOL, IsInitialize)(void) PURE;
	STDMETHOD_(BOOL, IsExistAlphaChanel)(void) PURE;
	STDMETHOD_(void, Present)(BOOL bImmediate = TRUE) PURE;
	STDMETHOD_(LPVOID, GetPointer)(void) PURE;
	STDMETHOD_(COLORFORMAT, ColorFormat)(void) PURE;
	STDMETHOD_(DWORD, GetBytesStride)(void) PURE;
	STDMETHOD_(DWORD, ColorBitCount)(void) PURE;
	STDMETHOD_(DWORD, Width)(void) PURE;
	STDMETHOD_(DWORD, Height)(void) PURE;
	STDMETHOD_(void, GetRect)(RECT* pRect) PURE;
	STDMETHOD_(BOOL, IsAntialiasing)(void) PURE;
	STDMETHOD_(BOOL, OpenDialog)(void) PURE;
	STDMETHOD_(BOOL, SaveDialog)(void) PURE;
	STDMETHOD_(BOOL, CheckPSNR)(ITDBuffer* pCmpBuffer, double &dPSNR, BOOL &bSame, BOOL bCompareAlpha = FALSE) PURE;
	STDMETHOD_(BOOL, SetViewLink)(ITDBuffer* pBuffer) PURE;
	STDMETHOD_(void, SetEnable)(BUFFER_INTERFACE id, BOOL bEnable = TRUE) PURE;
	STDMETHOD_(void, GetDrawRect)(RECT* pRect) PURE;
	STDMETHOD_(void, SetDrawRect)(const RECT* pRect) PURE;
	STDMETHOD_(HDC, GetDC)(void) PURE;
	// gdi+ : To use this functions, buffer must be valid.
	STDMETHOD_(void, SetPenColor)(BYTE r, BYTE g, BYTE b, BYTE a = 255) PURE;
	STDMETHOD_(void, SetPenDashStyle)(ITDDashStyle style) PURE;
	STDMETHOD_(void, SetPenDashOffset)(float offset) PURE;
	STDMETHOD_(void, SetPenDashCap)(ITDDashCap cap) PURE;
	STDMETHOD_(void, SetPenWidth)(float width) PURE;
	STDMETHOD_(void, SetBrushSolidColor)(BYTE r, BYTE g, BYTE b, BYTE a = 255) PURE;
	STDMETHOD_(void, SetSmoothModeHighQuality)(BOOL bEnable = TRUE) PURE;
	STDMETHOD_(void, UseAlphaChannel)(BOOL bUse = TRUE) PURE;
	STDMETHOD_(void, DrawLine)(float x, float y, float ex, float ey) PURE;
	STDMETHOD_(void, DrawLine)(ITDPoint* pStart, ITDPoint* pEnd) PURE;
	STDMETHOD_(void, DrawLinef)(ITDPointf* pStart, ITDPointf* pEnd) PURE;
	STDMETHOD_(void, DrawLines)(ITDPoint* pPoints, DWORD dwCount) PURE;
	STDMETHOD_(void, DrawLinesf)(ITDPointf* pPoints, DWORD dwCount) PURE;
	STDMETHOD_(void, DrawRectangle)(float x, float y, float width, float height) PURE;
	STDMETHOD_(void, FillRectangle)(float x, float y, float width, float height) PURE;
	STDMETHOD_(void, DrawEllipse)(float x, float y, float width, float height) PURE;
	STDMETHOD_(void, FillEllipse)(float x, float y, float width, float height) PURE;
	STDMETHOD_(void, DrawBezier)(ITDPoint* pPt1, ITDPoint* pPt2, ITDPoint* pPt3, ITDPoint* pPt4) PURE;
	STDMETHOD_(void, DrawBezierf)(ITDPointf* pPt1, ITDPointf* pPt2, ITDPointf* pPt3, ITDPointf* pPt4) PURE;
	STDMETHOD_(void, DrawBeziers)(ITDPoint* pPoints, DWORD dwCount) PURE;
	STDMETHOD_(void, DrawBeziersf)(ITDPointf* pPoints, DWORD dwCount) PURE;
	STDMETHOD_(void, DrawPolygon)(ITDPoint* pPoints, DWORD dwCount) PURE;
	STDMETHOD_(void, DrawPolygonf)(ITDPointf* pPoints, DWORD dwCount) PURE;
	STDMETHOD_(void, FillPolygon)(ITDPoint* pPoints, DWORD dwCount) PURE;
	STDMETHOD_(void, FillPolygonf)(ITDPointf* pPoints, DWORD dwCount) PURE;

};

// Button interface
interface ITDReportManager{
	STDMETHOD_(void, OnReportLink)(DWORD dwID, LPCTSTR lpszString, long cpMin, long cpMax) {};		// On link
};

// Report interface
typedef enum{
	TEXT_STYLE_BOLD			= 0x0001,
	TEXT_STYLE_ITALIC		= 0x0002,
	TEXT_STYLE_UNDERLINE	= 0x0004,
	TEXT_STYLE_STRIKEOUT	= 0x0008,
	TEXT_STYLE_PROTECTED	= 0x0010,
}TEXT_STYLE;

typedef enum{
	TEXT_ALIGN_LEFT		= 1,
	TEXT_ALIGN_RIGHT,
	TEXT_ALIGN_CENTER,
}TEXT_ALIGN;

interface ITDReport{
	STDMETHOD_(ITDObject*, GetObject)(void) PURE;			// get common interface object
	STDMETHOD_(void, SetManager)(ITDReportManager* pManager, DWORD dwID = 0) PURE;	// set control manager
	STDMETHOD_(BOOL, Open)(LPCTSTR lpszFileName, BOOL bAutoUpdate = FALSE) PURE;	// open *.rtf file
	STDMETHOD_(BOOL, Save)(LPCTSTR lpszFileName) PURE;		// save to *.rtf file
	STDMETHOD_(void, Clear)(BOOL bClose = FALSE) PURE;		// clear text. bClose(close opened rtf file.)
	STDMETHOD_(void, AppendText)(LPCTSTR lpszFormat, ...) PURE;			// append text to richedit
	STDMETHOD_(void, SetText)(LPCTSTR lpszFormat, ...) PURE;			// set text to richedit
	STDMETHOD_(void, ReplaceText)(long nStartChar, long nEndChar, LPCTSTR lpszFormat, ...) PURE;		// replace text
	STDMETHOD_(int, GetLineCount)(void) PURE;							// get line count
	STDMETHOD_(void, SetBackgroundColor)(COLORREF crBasckColor) PURE;	// set background color
	STDMETHOD_(BOOL, GetText)(LPTSTR lpszBuffer, DWORD dwSize, long nStartChar = 0, long nEndChar = -1) PURE;	// get selection string
	STDMETHOD_(void, SetStyle)(DWORD dwAdd, DWORD dwRemove =0) PURE;		// set char format effects enable (refer. SEL_BOOLIAN for dwAdd & dwRemove)
	STDMETHOD_(void, SetColor)(COLORREF crTextColor) PURE;			// set selection text color
	STDMETHOD_(void, SetFont)(LPCTSTR sFontName) PURE;				// set selection text font
	STDMETHOD_(void, SetOffset)(LONG yOffset) PURE;					// Character height, in twips (1/1440 of an inch or 1/20 of a printer's point).
	STDMETHOD_(void, SetHeight)(LONG yHeight) PURE;					// Character offset, in twips, from the baseline. If the value of this member is positive, the character is a superscript; if it is negative, the character is a subscript.
	STDMETHOD_(void, SetAlign)(TEXT_ALIGN align) PURE;					// set text alignment
	STDMETHOD_(void, SetLink)(LPCTSTR sLink = NULL) PURE;				// set link
	STDMETHOD_(void, ShowScrollBar)(BOOL bShow) PURE;					// show scroll bar
	STDMETHOD_(void, EnableEdit)(BOOL bEditable) PURE;					// set edit enable
	STDMETHOD_(void, ShowOutline)(BOOL bShow = TRUE) PURE;				// show outline
	STDMETHOD_(void, CalculateHeight)(void) PURE;						// Fit Height to view
	STDMETHOD_(int, GetFirstVisibleLine)(void) PURE;					// Get first visible line
	STDMETHOD_(void, LineScroll)(int iLines, int iChars = 0) PURE;		// Set scroll to Line
	STDMETHOD_(long, GetTextLength)(void) PURE;							// Get text length
	STDMETHOD_(void, ScrollToLastLine)(void) PURE;					// Last line scroll
};

// Chart-Serie interface
interface ITDChartSerie{
	STDMETHOD_(DWORD, GetPointsCount)(void) const PURE;		// Returns the number of points in the series.
	STDMETHOD_(DWORD, GetSerieId)(void) PURE;				// GetSerie ID
	STDMETHOD_(void, SetName)(LPCTSTR NewName) PURE;		// Sets the name of the series, which is displayed in the legend.
	STDMETHOD_(void, SetVisible)(BOOL bVisible) PURE;		// Specifies if the series is visible or not.
	STDMETHOD_(BOOL, IsVisible)(void) const PURE;			// Returns TRUE if the series is visible.
	STDMETHOD_(COLORREF, GetColor)(void) const PURE;		// Returns the color of the series.
	STDMETHOD_(void, SetColor)(COLORREF NewColor) PURE;		// Sets the color of the series.
	STDMETHOD_(COLORREF, GetShadowColor)(void) const PURE;	// eturns the color of the shadow.
	STDMETHOD_(void, SetShadowColor)(COLORREF NewColor) PURE;	// Sets the color of the shadow.
	STDMETHOD_(void, EnableShadow)(BOOL bEnable) PURE;		// Enables or disables the shadow for the series.
	STDMETHOD_(void, SetShadowDepth)(int Depth) PURE;		// Sepcifies the depth (in pixels) of the shadow.
	STDMETHOD_(double, YScreenToValue)(long YScreenCoord) PURE;	// get x value into its relative Y data value.
	STDMETHOD_(double, XScreenToValue)(long XScreenCoord) PURE;	// get y value into its relative X data value.
	STDMETHOD_(void, AddPoint)(double X, double Y) PURE;	// Add point : Line, Point, Surface, Bar serie only
	STDMETHOD_(void, AddPointArray)(double* pX, double* pY, unsigned Count) PURE;	// Add points : Line, Point, Surface, Bar serie only
	STDMETHOD_(void, AddPointRange)(double StartTime, double EndTime, double YValue) PURE;	// Add point : Gantt serie only
	STDMETHOD_(void, AddPointLevel)(double XVal, double Low, double High, double Open, double Close) PURE;	// Add point : Candlestick serie only
	STDMETHOD_(void, AddLabel)(DWORD dwIndex, LPCTSTR lpszLabel) PURE;	// add label on serie index
	STDMETHOD_(void, Clear)(void) PURE;						// clear all serie data
	STDMETHOD_(void, SetWidth)(int iWidth) PURE;			// Set width : bar, gantt, candlestick serie only
	STDMETHOD_(void, SetSmooth)(BOOL bSmooth) PURE;			// Set smooth line : line serie only
};

typedef enum{
	CHART_STYLE_POINT,
	CHART_STYLE_LINE,
	CHART_STYLE_SURFACE,
	CHART_STYLE_BAR,
	CHART_STYLE_CANDLESTICK,
	CHART_STYLE_GANTT
}CHART_STYLE;

// Chart interface
interface ITDChart{
	STDMETHOD_(ITDObject*, GetObject)(void) PURE;			// get common interface object
	STDMETHOD_(int, GetSeriesCount)(void) PURE;				// get series count
	STDMETHOD_(ITDChartSerie*, CreateSerie)(CHART_STYLE style = CHART_STYLE_LINE, BOOL bSecondaryHorizAxis=FALSE, BOOL bSecondaryVertAxis=FALSE) PURE;	// create a serie
	STDMETHOD_(ITDChartSerie*, GetSerie)(DWORD dwSerieID) PURE;		// get a serie
	STDMETHOD_(void, RemoveSerie)(DWORD dwSerieID) PURE;	// remove a serie
	STDMETHOD_(void, RemoveAllSeries)(void) PURE;			// remove all series
};

// Button interface
interface ITDButtonManager{
	STDMETHOD_(void, OnButtonClick)(DWORD dwID) PURE;		// set button check
};

typedef enum{
	BUTTON_STYLE_PUSHBUTTON,
	BUTTON_STYLE_CHECKBOX,
	BUTTON_STYLE_RADIOBUTTON,
}BUTTON_STYLE;

interface ITDButton{
	STDMETHOD_(ITDObject*, GetObject)(void) PURE;			// get common interface object
	STDMETHOD_(void, SetManager)(ITDButtonManager* pManager, DWORD dwID = 0) PURE;	// set control manager
	STDMETHOD_(void, SetText)(LPCTSTR lpszText) PURE;		// button text
	STDMETHOD_(void, SetStyle)(BUTTON_STYLE style) PURE;	// set button style (push button/check box/radio button)
	STDMETHOD_(void, SetCheck)(BOOL bCheck = TRUE) PURE;	// set button check
};

// Html interface
interface ITDHtmlManager{
	STDMETHOD_(LPCTSTR, OnHtmlBeforeNavigate)(DWORD dwID, LPCTSTR lpszURL) PURE;//{return lpszURL;};
	STDMETHOD_(void, OnHtmlDocumentComplete)(DWORD dwID, LPCTSTR lpszURL) PURE;//
	STDMETHOD_(LPCTSTR, OnHtmlWebMessageReceived)(DWORD dwID, LPCTSTR lpszMessage) { return NULL; }	// html2 only
};

// Terminal interface
typedef enum{
	TERMINAL_SETUP_TYPE,					// 'serial', 'ssh'
}TERMINAL_SETUP;

interface ITDTerminalManager{
	STDMETHOD_(LPCTSTR, OnTerminalSetup)(DWORD dwID, TERMINAL_SETUP id) PURE;		// Setup terminal setup sequence, when all setup is completed, 'm_pCommand' will be initialized.
	STDMETHOD_(LPCTSTR, OnTerminalTranslate)(DWORD dwID, LPCTSTR lpszLine) {return NULL;}
	STDMETHOD_(void, OnTerminalQuit)(DWORD dwID) {}
};

interface ITDTerminal{
	// no object.
	STDMETHOD_(void, SetManager)(ITDTerminalManager* pManager, DWORD dwID = 0) PURE;	// set control manager
	STDMETHOD_(void, SendCommand)(LPCTSTR lpszCommand) PURE;
	STDMETHOD_(void, Release)(void) PURE;						// release terminal window
};

typedef enum{
	HTML_BY_ID,
	HTML_BY_NAME,
	HTML_BY_TAG,
}HTML_ENUM;

interface ITDHtml{
	STDMETHOD_(ITDObject*, GetObject)(void) PURE;					// get common interface object
	STDMETHOD_(void, SetManager)(ITDHtmlManager* pManager, DWORD dwID = 0) PURE;		// set control manager 
	STDMETHOD_(void, Navigate)(LPCTSTR lpszURL, LPCTSTR lpszTargetFrame = NULL) PURE;	// navigate
	STDMETHOD_(BOOL, CallJScript)(LPCTSTR lpszScript, ...) PURE;	// call Java script
	STDMETHOD_(int, GetWidth)(void) PURE;							// get page width
	STDMETHOD_(int, GetHeight)(void) PURE;							// get page height
	STDMETHOD_(void, SetBlockNewWindow)(BOOL bBlock = TRUE) PURE;	// blocking new window
	STDMETHOD_(BOOL, PutText)(LPCTSTR lpszName, LPCTSTR lpszObjectID, LPCTSTR lpszText) PURE;	// set text in the form window
	STDMETHOD_(BOOL, GetText)(LPCTSTR lpszName, LPCTSTR lpszObjectID, LPTSTR lpszText, DWORD dwSize) PURE;	// get text in the form window
	STDMETHOD_(BOOL, ClickButton)(LPCTSTR lpszText) PURE;			// click the button in the form window
};

// E-Mail interface
interface ITDMail{
	STDMETHOD_(void, SetSender)(LPCTSTR lpszAddress, LPCTSTR lpszName = NULL) PURE;		// sender's mail address and name
	STDMETHOD_(void, AddRecipient)(LPCTSTR lpszAddress, LPCTSTR lpszName = NULL) PURE;	// recipient's mail address and name
	STDMETHOD_(void, SetSubject)(LPCTSTR lpszSubject) PURE;	// mail title
	STDMETHOD_(void, AddText)(LPCTSTR lpszText) PURE;		// add text of mail body
	STDMETHOD_(void, AddFile)(LPCTSTR lpszFileName) PURE;	// add file
	STDMETHOD_(BOOL, Send)(void) PURE;						// send email operation
	STDMETHOD_(void, Release)(void) PURE;					// release this object : make sure to call this in the end of process
};

// Movie interface
interface ITDMovie{
	// for save
	STDMETHOD_(BOOL, Create)(LPCTSTR lpszFileName, DWORD dwWidth, DWORD dwHeight, DWORD dwFrameRate = 30) PURE;	// Initialize movie file with current system, default frame rate is 30 fps
	STDMETHOD_(BOOL, SetFrame)(PVOID pBuff = NULL) PURE;	// set frame to movie

	// for load
	STDMETHOD_(BOOL, Open)(LPCTSTR lpszFileName, LONG& lFirstFrame, LONG& lNumOfFrames, DWORD* pdwFrameRate = NULL) PURE;	// open
	STDMETHOD_(BOOL, GetFrame)(LONG lFrame, PVOID* pBuff = NULL) PURE;	// get frame from movie

	// common
	STDMETHOD_(ITDBuffer*, Buffer)(void) PURE;				// get buffer
	STDMETHOD_(void, Release)(void) PURE;					// release this object : make sure to call this in the end of process
	
	// option
	STDMETHOD_(void, EnableAlpha)(BOOL bEnable = TRUE) PURE;		// use alpha channel
};


// Serial communication port interface
interface ITDSerialCom{
	enum FlowControl
	{
		NoFlowControl,
		CtsRtsFlowControl,
		CtsDtrFlowControl,
		DsrRtsFlowControl,
		DsrDtrFlowControl,
		XonXoffFlowControl
	};

	enum Parity
	{
		NoParity,
		OddParity,
		EvenParity,
		MarkParity,
		SpaceParity
	};

	enum StopBits
	{
		OneStopBit,
		OnePointFiveStopBits,
		TwoStopBits
	};

	STDMETHOD_(void, Release)(void) PURE;						// release this object : make sure to call this in the end of process
	STDMETHOD_(BOOL, Open)(int nPort, DWORD dwBaud = 115200, Parity parity = NoParity, BYTE DataBits = 8, StopBits stopbits = OneStopBit, FlowControl fc = NoFlowControl, BOOL bOverlapped = FALSE) PURE; // Open communication
	STDMETHOD_(void, Close)(void) PURE;		// close communication
	STDMETHOD_(BOOL, IsOpen)(void) PURE;	// Is communication open?
	STDMETHOD_(DWORD, Read)(void* lpBuf, DWORD dwCount) PURE;
	STDMETHOD_(BOOL, Read)(void* lpBuf, DWORD dwCount, OVERLAPPED& overlapped) PURE;
	STDMETHOD_(DWORD, Write)(const void* lpBuf, DWORD dwCount) PURE;
	STDMETHOD_(BOOL, Write)(const void* lpBuf, DWORD dwCount, OVERLAPPED& overlapped) PURE;
};

//---------------------------------------------------------------------------
interface ITDDocument;
class ITDImplSystem;

typedef struct{
	uint64_t		dwMemorySize;
	uint32_t		UserConfig[1022];
}TESTDRIVE_CONFIG;

// System memory interface
interface ITDMemory{
	typedef enum{
		MEM_DISC_SIMUL,
		MEM_DISC_BINARY,
		MEM_DISC_DWORD,
		MEM_DISC_WORD,
		MEM_DISC_BYTE,
		MEM_DISC_FLOAT,
		MEM_DISC_TEXT,
	} MEM_DISC;

	STDMETHOD_(BOOL, Create)(uint64_t dwByteSize, LPCTSTR lpszName = NULL) PURE;	// create system memory with size
	STDMETHOD_(void, Release)(void) PURE;							// delete system memory
	STDMETHOD_(BOOL, IsInitialized)(void) PURE;						// is system memory existed?
	STDMETHOD_(uint64_t, GetSize)(void) PURE;						// get memory byte size
	STDMETHOD_(BOOL, IsValidAddress)(uint64_t dwAddress) PURE;		// get address validation
	STDMETHOD_(BYTE*, GetPointer)(uint64_t dwAddress = 0, uint64_t dwSize = 0) PURE;		// get pointer from address and size definition
	STDMETHOD_(BOOL, Load)(MEM_DISC disc, LPCTSTR lpszFileName, uint64_t offset) PURE;	// set memory from description file
	STDMETHOD_(BOOL, Save)(MEM_DISC disc, LPCTSTR lpszFileName, uint64_t offset, uint64_t size, DWORD stride = 0) PURE;	// save memory to file
	STDMETHOD_(BOOL, LoadImage)(LPCTSTR lpszFileName, uint64_t dwOffset, COLORFORMAT iColorType, DWORD stride = 0) PURE;	// load memory from image file
	STDMETHOD_(BOOL, StoreImage)(LPCTSTR lpszFileName, uint64_t dwOffset,  DWORD dwWidth, DWORD dwHeight, COLORFORMAT iColorType, BOOL bStoreAlpha = FALSE, DWORD stride = 0) PURE;	// save memory to image file
	STDMETHOD_(TESTDRIVE_CONFIG*, GetConfig)(void) PURE;
	STDMETHOD_(LPCTSTR, GetName)(void) PURE;
};

typedef enum{
	SPEAK_DEFAULT			= 0,
	SPEAK_ASYNC				= 1L << 0,
	SPEAK_PURGEBEFORESPEAK	= 1L << 1,
	SPEAK_IS_FILENAME		= 1L << 2,
	SPEAK_IS_XML			= 1L << 3,
	SPEAK_IS_NOT_XML		= 1L << 4,
	SPEAK_PERSIST_XML		= 1L << 5,
	SPEAK_NLP_SPEAK_PUNC	= 1L << 6,
	SPEAK_NLP_MASK			= SPEAK_NLP_SPEAK_PUNC,
	SPEAK_VOICE_MASK		= SPEAK_ASYNC | SPEAK_PURGEBEFORESPEAK | SPEAK_IS_FILENAME | SPEAK_IS_XML | SPEAK_IS_NOT_XML | SPEAK_NLP_MASK | SPEAK_PERSIST_XML,
	SPEAK_UNUSED_FLAGS		= ~SPEAK_VOICE_MASK
} 	SPEAK_MODE;

typedef const char* (*TD_REDIRECT_LOG)(LPCTSTR lpszLog, int iID);
typedef BOOL (*SEARCH_FILE_FUNCTION)(LPCTSTR sPath, LPVOID pData);

typedef enum{
	TD_INTERFACE_MAIL,
	TD_INTERFACE_MOVIE,
	TD_INTERFACE_SERIAL,
}TD_INTERFACE;

// TestDrive system interface
interface ITDSystem{
	typedef enum {
		TITLE_MAIN,
		TITLE_WORK,
		TITLE_DOC,
		TITLE_MAX
	} TITLE_ID;

	typedef enum {
		PROFILE_ID_INITIALIZE,
		PROFILE_ID_CHECK,
		PROFILE_ID_CLEANUP,
		PROFILE_ID_SW_COMPILE,
		PROFILE_ID_HW_COMPILE,
	} PROFILE_ID;

	typedef enum {
		SYSMSG_INFO,
		SYSMSG_WARNING,
		SYSMSG_ERROR,
	} SYSMSG_ID;

	STDMETHOD_(DWORD, GetVersion)(void) PURE;											// get testdrive version. it must be equal to 'TESTDRIVE_VERSION'
	STDMETHOD_(DWORD, GetLocale)(void) PURE;											// get current locale .ex) LANG_KOREAN, LANG_JAPANESE, LANG_ENGLISH
	STDMETHOD_(void, SetTitle)(TITLE_ID id, LPCTSTR szTitle) PURE;						// set windows title (MAIN/WORK/DOCUMENT)
	STDMETHOD_(void, SetProfile)(PROFILE_ID id, LPCTSTR lpszFileName) PURE;				// set default profile slot. (If lpszFileName is NULL, id's profile slot will be removed.)
	STDMETHOD_(void, ClearLog)(void) PURE;												// clear log output message windows
	STDMETHOD_(void, LogOut)(LPCTSTR szMsg, SYSMSG_ID id = SYSMSG_INFO) PURE;			// put the log message to the output window
	STDMETHOD_(void, LogInfo)(LPCTSTR szFormat, ...) PURE;								// put info log message to the output window
	STDMETHOD_(void, LogWarning)(LPCTSTR szFormat, ...) PURE;							// put warning log message to the output window
	STDMETHOD_(void, LogError)(LPCTSTR szFormat, ...) PURE;								// put error log message to the output window
	STDMETHOD_(BOOL, RunProfile)(LPCTSTR szFileName, BOOL bSystem = FALSE) PURE;		// run *.profile/sp file
	STDMETHOD_(int, ExecuteFile)(LPCTSTR lpszFileName, LPCTSTR lpszArg, BOOL bSystemLog, TD_REDIRECT_LOG func, LPCTSTR lpszWorkDir, ...) PURE;	// run *.exe/com/bat file
	STDMETHOD_(ITDImplSystem*, GetImplementation)(void) PURE;							// get current system application implementation
	STDMETHOD_(ITDDocument*, GetDocument)(LPCTSTR lpszName) PURE;						// get document named as 'lpszName'
	STDMETHOD_(ITDMemory*, GetMemory)(LPCTSTR lpszName = NULL, BOOL bForced = FALSE) PURE;		// get system memory named as 'lpszName', if(lpszName == NULL) get first system memory, if(can't find memory && bForced) return new memory
	STDMETHOD_(double, GetElapsedTime)(LONGLONG& llPrevTime) PURE;						// get elapsed time from llPrevTime
	STDMETHOD_(ITDMail*, CreateMail)(void) PURE;										// create e-mail interface : must call ITDMail::Release when process is terminated
	STDMETHOD_(ITDMovie*, CreateMovieInterface)(void) PURE;								// create movie generator interface : must call ITDMovie::Release when process is terminated
	STDMETHOD_(ITDSerialCom*, CreateSerialComInterface)(void) PURE;						// create serial communication interface : must call ITDSerialCom::Release when process is terminated
	STDMETHOD_(PVOID, CreateInterface)(TD_INTERFACE td_inf) PURE;						// create extended interface

	STDMETHOD_(BOOL, VerilatorCompile)(LPCTSTR lpszFileName, LPCTSTR lpszWorkDir, LPCTSTR lpszOption = NULL) PURE;	// using verilator compiler : default option = "-sv -cc --compiler msvc"
	STDMETHOD_(BOOL, GTKWave)(LPCTSTR lpszCommandLine, LPCTSTR lpszWorkDir = NULL) PURE;// use gtkwave
	STDMETHOD_(BOOL, CppCheck)(LPCTSTR lpszPath, LPCTSTR lpszArgs = NULL) PURE;			// CppCheck
	STDMETHOD_(void, PeekAndPumpMessage)(void) PURE;									// pump message function for long-term execution
	STDMETHOD_(LPCTSTR, GetProjectPath)(void) PURE;										// get current project path
	STDMETHOD_(void, Speak)(LPCTSTR msg, DWORD flags = SPEAK_DEFAULT) PURE;				// TTS speaking
	STDMETHOD_(ITDSemaphore*, CreateSemaphore)(int iInitValue = 1) PURE;				// must be release
	STDMETHOD_(void, SendCommand)(ITDDocument* pDoc, DWORD command, WPARAM wParam = NULL, LPARAM lParam = NULL) PURE;	// must be release
	STDMETHOD_(void, SetForeground)(void) PURE;											// set testdrive program to foreground window
	STDMETHOD_(LPCTSTR, RetrieveFullPath)(LPCTSTR szPath = NULL) PURE;					// change szPath to full path name
	STDMETHOD_(void, TryQuit)(void) PURE;												// try to exit testdrive program
	STDMETHOD_(BOOL, InsertCodeInception)(LPCTSTR lpszTargetFile, LPCTSTR lpszInceptionFile, LPCTSTR lpszTitle = NULL, LPCTSTR lpszAuthor = NULL, LPCTSTR lpszVersion = NULL) PURE;	// insert code inception to source file
	STDMETHOD_(BOOL, ReplaceText)(LPCTSTR lpszTargetFile, LPCTSTR lpszReplaceDescFile) PURE;	// replace strings with replace files
	STDMETHOD_(BOOL, SearchSubPathFile)(LPCTSTR sSearchPath, LPCTSTR sFileName, SEARCH_FILE_FUNCTION SearchFunc, LPVOID pData) PURE;	// search sub-tree path
};

// Property data common interface
typedef enum{
	PROPERTY_TYPE_NONE,
	PROPERTY_TYPE_INT,
	PROPERTY_TYPE_BOOL,
	PROPERTY_TYPE_FLOAT,
	PROPERTY_TYPE_DOUBLE,
	PROPERTY_TYPE_STRING,
	PROPERTY_TYPE_PASSWORD,
	PROPERTY_TYPE_DIRECTORY,
	PROPERTY_TYPE_FILE_PATH,
}PROPERTY_TYPE;

interface ITDPropertyData{
	STDMETHOD_(void, Show)(BOOL bShow = TRUE) PURE;
	STDMETHOD_(void, Enable)(BOOL bEnable = TRUE) PURE;
	STDMETHOD_(void, AllowEdit)(BOOL bAllow = TRUE) PURE;
	STDMETHOD_(BOOL, AddOption)(LPCTSTR lpszOption) PURE;				// if 'lpszOption' is NULL, remove all options of rest.
	STDMETHOD_(int, GetOptionCount)(void) PURE;
	STDMETHOD_(LPCTSTR, GetOption)(int iIndex) PURE;
	STDMETHOD_(LPCTSTR, GetFormat)(void) PURE;
	STDMETHOD_(PROPERTY_TYPE, GetType)(void) PURE;
	STDMETHOD_(DWORD, GetID)(void) PURE;
	STDMETHOD_(LPCTSTR, GetName)(void) PURE;
	STDMETHOD_(DWORD_PTR, GetData)(void) PURE;
	STDMETHOD_(void, UpdateData)(BOOL bUpdate = TRUE) PURE;			// update current data from(TRUE)/to(FALSE) property window
	STDMETHOD_(void, UpdateConfigFile)(BOOL bUpdate = TRUE) PURE;		// update current data from(TRUE)/to(FALSE) "[document_name].ini" file
};

// Document level implementation template
interface ITDImplDocument{
	STDMETHOD_(BOOL, OnCommand)(DWORD command, WPARAM wParam = NULL, LPARAM lParam = NULL) PURE;	// command input from system application or other documentations
	STDMETHOD_(void, OnViewUpdate)(ITDBuffer* pBuffer) PURE;	// from buffer controls
	STDMETHOD_(BOOL, OnPropertyUpdate)(ITDPropertyData* pProperty) PURE;	// from properties window
	STDMETHOD_(void, OnSize)(int width, int height) PURE;		// view window size change
	STDMETHOD_(void, OnShow)(BOOL bShow) PURE;					// show window
	STDMETHOD_(void, Release)(void) PURE;						// release this object
};

class TDImplDocumentBase : public ITDImplDocument{
public:
	TDImplDocumentBase(void){}
	virtual ~TDImplDocumentBase(void){};

	STDMETHOD_(BOOL, OnCommand)(DWORD command, WPARAM wParam = NULL, LPARAM lParam = NULL) {return FALSE;};
	STDMETHOD_(void, OnViewUpdate)(ITDBuffer* pBuffer) {};
	STDMETHOD_(BOOL, OnPropertyUpdate)(ITDPropertyData* pProperty){return FALSE;};
	STDMETHOD_(void, OnSize)(int width, int height) {};
	STDMETHOD_(void, OnShow)(BOOL bShow) {};
	STDMETHOD_(void, Release)(void) {delete this;}
};

#define		TD_EXTERNAL_COMMAND		((DWORD)-1)				// SendMessage(WM_USER,--) from external, it represent on ITDImplDocument::OnCommand(EXTERNAL_COMMAND,--)


// Document level interface
interface ITDDocument{
	STDMETHOD_(LPCTSTR, DocumentTitle)(LPCTSTR lpszTitle = NULL) PURE;	// get/set current document name
	STDMETHOD_(ITDImplDocument*, GetImplementation)(void) PURE;	// get system application implementation
	STDMETHOD_(ITDSystem*, GetSystem)(void) PURE;				// get system control interface

	STDMETHOD_(ITDBuffer*, GetBuffer)(LPCTSTR lpszName) PURE;	// get buffer object
	STDMETHOD_(ITDReport*, GetReport)(LPCTSTR lpszName) PURE;	// get report object
	STDMETHOD_(ITDChart*, GetChart)(LPCTSTR lpszName) PURE;		// get chart object
	STDMETHOD_(ITDButton*, GetButton)(LPCTSTR lpszName) PURE;	// get button object
	STDMETHOD_(ITDHtml*, GetHtml)(LPCTSTR lpszName) PURE;	// get button object
	STDMETHOD_(ITDBuffer*, CreateBuffer)(LPCTSTR lpszName, int x, int y, int width, int height) PURE;		// create buffer object
	STDMETHOD_(ITDReport*, CreateReport)(LPCTSTR lpszName, int x, int y, int width, int height) PURE;		// create report object
	STDMETHOD_(ITDButton*, CreateButton)(LPCTSTR lpszName, int x, int y, int width, int height) PURE;		// create button object
	STDMETHOD_(ITDHtml*, CreateHtml)(LPCTSTR lpszName, int x, int y, int width, int height, BOOL bWebView2 = FALSE) PURE;			// create HTML object

	STDMETHOD_(void, Lock)(void) PURE;		// lock this document and reference count++
	STDMETHOD_(void, UnLock)(void) PURE;	// unlock this document and reference count--
	STDMETHOD_(BOOL, IsLocked)(void) PURE;	// Is reference count zero?
	STDMETHOD_(void, SetAutoClose)(BOOL bClose = TRUE) PURE;	// auto close when reference count is zero
	STDMETHOD_(void, SetDescription)(LPCTSTR lpszDesc) PURE;	// set document description
	STDMETHOD_(int, GetConfigInt)(LPCTSTR lpszKeyName, int iDefaultValue = 0) PURE;	// get integer value from configuration file
	STDMETHOD_(void, SetConfigInt)(LPCTSTR lpszKeyName, int iSetValue) PURE;		// set integer value from configuration file
	STDMETHOD_(void, GetConfigString)(LPCTSTR lpszKeyName, LPTSTR lpszStr, DWORD dwSize, LPCTSTR lpszDefault = _T("")) PURE;	// get string from configuration file
	STDMETHOD_(void, SetConfigString)(LPCTSTR lpszKeyName, LPCTSTR lpszStr) PURE;	// set string from configuration file

	STDMETHOD_(void, ClearAllProperty)(void) PURE;	// clear all property data
	STDMETHOD_(ITDPropertyData*, AddPropertyData)(PROPERTY_TYPE Type, DWORD iID, LPCTSTR lpszName, DWORD_PTR pData, LPCTSTR lpszDesc = NULL) PURE;	// add new property data

	STDMETHOD_(void, SetTimer)(UINT nIDEvent, UINT nElapse) PURE;	// set time event, and it will call the ITDImplDocument::OnCommand
	STDMETHOD_(void, KillTimer)(UINT nIDEvent) PURE;				// kill timer event

	STDMETHOD_(HWND, GetWindowHandle)(void) PURE;				// get document window handle
	STDMETHOD_(void, SetForegroundDocument)(void) PURE;			// set foreground document
	STDMETHOD_(void, AddWatchDogPath)(LPCTSTR lpszPath, DWORD dwID = 0, BOOL bSearchSubDir = FALSE) PURE;	// add watch-dog path
	STDMETHOD_(void, ClearWatchDogPath)(DWORD dwID = -1) PURE;	// if (dwID == -1) clear all watchdog path else clear specific one.

	STDMETHOD_(void, InvalidateLayout)(void) PURE;				// redraw component call 'OnSize' function & invalidate document layout
	STDMETHOD_(void, Show)(BOOL bShow = TRUE) PURE;				// show/hide document
	STDMETHOD_(BOOL, IsVisible)(void) PURE;						// Is document visible ?
};

#ifndef TESTDRIVE_ENTRY
	// TESTDRIVE_DOCUMENT
	__declspec(dllexport) ITDImplDocument* __cdecl RegisterDocument(ITDDocument* pDoc);

	extern ITDSystem*	g_pSystem;

	#define REGISTER_DOCUMENT(DocType)	\
		ITDSystem*	g_pSystem	= NULL; \
		ITDImplDocument* __cdecl RegisterDocument(ITDDocument* pDoc){ \
			g_pSystem	= pDoc->GetSystem();\
			if(g_pSystem->GetVersion() != TESTDRIVE_VERSION) return NULL; \
			return new DocType(pDoc);\
		}

#endif//TESTDRIVE_ENTRY

#ifdef __cplusplus
};
#endif

#endif//__TESTDRIVE_H__
