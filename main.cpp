#include <string>
std::string cls =  "My class",wn="Window",f = "fnt";
#include "res.h"


#include <sstream>
#include <vector>
HFONT font;

HDC hd;
HINSTANCE winHinst;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK DialogProc( HWND   hwndDlg, UINT   uMsg, WPARAM wParam, LPARAM lParam);
char _x[5],_y[5],_r[5];
INT_PTR CALLBACK DialogProcR( HWND   hwndDlg, UINT   uMsg, WPARAM wParam, LPARAM lParam);
int char_hight=16,char_lenght=8,win_wight=100,win_hight=60;
void bresenham(int x,int y);
HWND handle;
void clear();
#define SGN(a) (a<0)?-1:1
void radius(int r);







int WINAPI WinMain(HINSTANCE hThisInstance,HINSTANCE hPrevInstance,LPSTR lpszArgument,int nCmdShow)
{
    MSG msg;
    winHinst = hThisInstance;
    WNDCLASSEX wcs = WNDCLASSEX();
    wcs.cbSize = sizeof(WNDCLASSEX);
    wcs.lpfnWndProc = WndProc;
    wcs.hInstance = winHinst;
    wcs.hIcon = LoadIcon(NULL,IDI_APPLICATION);
    wcs.hCursor = LoadCursor(NULL,IDC_ARROW);
    wcs.lpszMenuName = MAKEINTRESOURCE(MY_MENU);
    wcs.hIconSm = LoadIcon(NULL,IDI_APPLICATION);
    wcs.lpszClassName = cls.c_str();
    if(!RegisterClassEx(&wcs)) {
        return 0;
    }
    DWORD winStyle = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_VISIBLE | CW_USEDEFAULT;
    RECT winSize;
    winSize.left = winSize.top=0;
    winSize.right = win_wight*char_lenght;
    winSize.bottom = win_hight*char_hight;
    AdjustWindowRect(&winSize,winStyle,FALSE);
    int winX = winSize.right-winSize.left;
    int winY = winSize.bottom-winSize.top;
    handle = CreateWindowEx(WS_EX_CLIENTEDGE,cls.c_str(),wn.c_str(),winStyle,CW_USEDEFAULT,CW_USEDEFAULT,winX,winY,NULL,NULL,winHinst,NULL);
    if(handle == NULL) {
        return 0;
    }
    ShowWindow (handle, nCmdShow);
    font = CreateFont(char_hight,char_lenght,0,0,FW_NORMAL,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,PROOF_QUALITY,FF_MODERN,f.c_str());
    hd = GetDC(handle);
    SelectObject(hd, font);
    SetBkColor(hd,BLACK);
    std::string p((unsigned int) win_wight, ' ');
    SetTextColor(hd,BLACK);
    for (int i=0;i<win_hight;++i) {
        TextOut(hd,0,i*char_hight,p.c_str(),p.size());
    }
    ShowWindow(handle,SW_SHOW);
    while (GetMessage (&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}




LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_COMMAND:
            switch(wParam) {
                case ENTER:
                    CreateDialog(winHinst, MAKEINTRESOURCE(DLG_NAME), hWnd, DialogProc);
                    break;
                case CLEAR:
                    clear();
                    break;
                case R:
                    CreateDialog(winHinst, MAKEINTRESOURCE(DLG_R), hWnd, DialogProcR);
                    break;
                default:
                    break;
            }
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        case WM_LBUTTONDOWN:
            POINT p;
            GetCursorPos(&p);
            int x1,y1;
            x1 = static_cast<int>(p.x);
            y1 =  static_cast<int>(p.y);
            WINDOWINFO wf;
            GetWindowInfo(handle,&wf);
            x1-=wf.rcClient.left;
            y1-=wf.rcClient.top;
            x1/=char_lenght;
            y1/=char_hight;
            bresenham(x1,y1);
            break;
        case WM_CREATE:
            HMENU menu,subMenu;
            menu = CreateMenu();
            subMenu = CreatePopupMenu();
            AppendMenu(subMenu,MF_STRING,ENTER,"&Enter");
            AppendMenu(subMenu,MF_STRING,CLEAR,"&Clear");
            AppendMenu(subMenu,MF_STRING,R,"&R");
            AppendMenu(menu,MF_STRING|MF_POPUP,(UINT)subMenu,"&Menu");
            SetMenu(hWnd,menu);
            break;
        default:
            return DefWindowProc (hWnd, message, wParam, lParam);
    }
    return 0;
}


INT_PTR CALLBACK DialogProc( HWND   hwndDlg, UINT   uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg) {
        case WM_COMMAND:
            switch(wParam) {
                case BT_OK:{
                    GetDlgItemText(hwndDlg, EDIT_X, _x, 5);
                    GetDlgItemText(hwndDlg, EDIT_Y, _y, 5);
                    int x = atoi(_x), y = atoi(_y);
                    UpdateWindow(handle);
                    bresenham(x, y);
                    EndDialog(hwndDlg, TRUE);
                    break;
            }
                default:
                    break;
                }
            break;
        case WM_CLOSE:
            EndDialog(hwndDlg, FALSE);
            break;
        case WM_DESTROY:
            DestroyWindow(hwndDlg);
            break;
        default:
            return FALSE;
    }
    return TRUE;
}

INT_PTR CALLBACK DialogProcR( HWND   hwndDlg, UINT   uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg) {
        case WM_COMMAND:
            switch(wParam) {
                case BT_OK:{
                    GetDlgItemText(hwndDlg, EDIT_R, _r, 5);
                    int r = atoi(_r);
                    radius(r);
                    EndDialog(hwndDlg, TRUE);
                    break;
            }
                default:
                    break;
            }
            break;
        case WM_CLOSE:
            EndDialog(hwndDlg, FALSE);
            break;
        case WM_DESTROY:
            DestroyWindow(hwndDlg);
            break;
        default:
            return FALSE;
    }
    return TRUE;
}

void bresenham(int x,int y)
{
    UpdateWindow(handle);
    int dy = y-win_hight/2;
    int dx = x-win_wight/2;
    int deltax,deltay,x1=win_wight/2,y1=win_hight/2;
    deltax = (dx==0)? 0:SGN(dx);
    deltay = (dy==0)? 0:SGN(dy);
    int ax = 2*abs(dx),ay = 2*abs(dy);
    SetTextColor(hd,YELLOW);
    char ch = '+';
    int t = 0;
    if(ax==ay) {
        while(x1!=x) {

            x1+=deltax;
            y1+=deltay;
            TextOut(hd,x1*char_lenght,y1*char_hight,&ch,1);
        }
    }
    else if(ax>ay) {
        while( x1 != x ) {

            if(t>0) {
                y1+=deltay;
                t-=ax;
            }
            x1 += deltax;
            t += ay;
            TextOut(hd,x1*char_lenght,y1*char_hight,&ch,1);
        }
    }
    else {
        while( y1 != y ) {
            if( t > 0 ) {
                x1 += deltax;
                t -= ay;
            }
            y1 += deltay;
            t += ax;
            TextOut(hd,x1*char_lenght,y1*char_hight,&ch,1);
        }
    }
}

void clear()
{
    std::string p((unsigned int) win_wight, ' ');
    SetTextColor(hd,BLACK);
    for (int i=0;i<win_hight;++i) {
        TextOut(hd,0,i*char_hight,p.c_str(),p.size());
    }
}



void radius(int r)
{
    int t_x = win_wight/2,t_y=win_hight/2;
    int x = 0,y = 0-r;
    for (;x<r;++x) {
        if(x*x+y*y<r*r) {
            bresenham(t_x+x,t_y+y);

        }
        else {
            --x;
            ++y;
        }
    }
}
