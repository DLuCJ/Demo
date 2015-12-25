//NOTE: comment out if you want to generate formula tunes or use your own synth
//go do that in othermzk.h
#define USE_SYNTH

#ifdef BLOAT_IS_GROSS
#include "resource.h"
#endif
#include "config.h"
#include <windows.h>
#include "visuals.cpp"

#ifdef USE_SYNTH
#include "mzk.h"    
#else
#include "othermzk.h"
#endif

#include <GL/gl.h>

int win_xres = 1920;
int win_yres = 1080;
int win_xpos = 0;
int win_ypos = 0;
int fullscreen = 1
;

static const PIXELFORMATDESCRIPTOR pfd = {
  sizeof(PIXELFORMATDESCRIPTOR), 1, PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER, PFD_TYPE_RGBA,
  32, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 32, 0, 0, PFD_MAIN_PLANE, 0, 0, 0, 0 };

#ifdef BLOAT_IS_GROSS
static LRESULT onInitDialog(HWND hDlg)
{ 
  HWND Window = CreateWindow("static",0,WS_POPUP,0,0,0,0,0,0,0,0);
  HDC tempdc = GetDC(Window);
  
  // initalize temporary ogl context
  if( !SetPixelFormat(tempdc,ChoosePixelFormat(tempdc,&pfd),&pfd) ) ExitProcess(1);
  wglMakeCurrent(tempdc,wglCreateContext(tempdc));
  
  //get GPU information, select default device
  SendMessage(GetDlgItem(hDlg, IDC_DEVICE), CB_ADDSTRING, 0, (LPARAM)glGetString(GL_RENDERER));
  SendMessage(GetDlgItem(hDlg, IDC_DEVICE), (UINT)CB_SETCURSEL, (WPARAM)0, 0);
  
  //populate resolution list
  SendMessage(GetDlgItem(hDlg, IDC_RESOLUTION), (UINT)CB_RESETCONTENT, (WPARAM)0, 0);
  char * res1= "1920x1080";
  SendMessage(GetDlgItem(hDlg, IDC_RESOLUTION), CB_ADDSTRING, 0, (LPARAM)res1);
  char * res2 = "1280x720";
  SendMessage(GetDlgItem(hDlg, IDC_RESOLUTION), CB_ADDSTRING, 1, (LPARAM)res2);

  //set 1920x1080 as default option, reflecting the way win_xres and win_yres are initialized
  SendMessage(GetDlgItem(hDlg, IDC_RESOLUTION), (UINT)CB_SETCURSEL, (WPARAM)0, 0);
  
  // set fullscreen checkbutton to the default setting
  CheckDlgButton(hDlg, IDC_FULLSCREEN, fullscreen);
  
  // playback device
  UINT num_audio_devices = waveInGetNumDevs();
  
  for (UINT i = 0; i < num_audio_devices; ++i) {
    WAVEINCAPS audio_info;
    MMRESULT mm_result = waveInGetDevCaps(i, &audio_info, sizeof(WAVEINCAPS));
    SendMessage(GetDlgItem(hDlg, IDC_SOUNDCARD), CB_ADDSTRING, 0, (LPARAM)audio_info.szPname);
  }
  
  // select default device
  SendMessage(GetDlgItem(hDlg, IDC_SOUNDCARD), (UINT)CB_SETCURSEL, (WPARAM)0, 0);  
  return (LRESULT)TRUE;
}

static LRESULT onCloseCmd(HWND hDlg, WORD wID)
{
  EndDialog(hDlg, wID);
  return 0;
}

static LRESULT CALLBACK configDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
  switch (message) {
  case WM_INITDIALOG:
    return onInitDialog(hDlg);
    break;

  case WM_COMMAND:
    switch (LOWORD(wParam)) {
    case IDOK:
    case IDCANCEL:
      return onCloseCmd(hDlg, LOWORD(wParam));
      break;

    case IDC_FULLSCREEN:
      if(fullscreen == 1) {
	fullscreen = 0;
      }
      else {
	fullscreen = 1;
      }
      break;
		  
    case IDC_RESOLUTION:
      if((UINT)SendMessage(GetDlgItem(hDlg, IDC_RESOLUTION), (UINT)CB_GETCURSEL, (WPARAM)0, 0)
	 == 1) {
	win_xres = 1280;
	win_yres = 720;
      }

      if((UINT)SendMessage(GetDlgItem(hDlg, IDC_RESOLUTION), (UINT)CB_GETCURSEL, (WPARAM)0, 0)
	 == 0) {
	win_xres = 1920;
	win_yres = 1080;
      }
		  
    }
  }

  return FALSE;
}
#endif

#ifdef __cplusplus
extern "C" 
{
#endif
  int _fltused = 0;
#ifdef __cplusplus
}
#endif
				  
#pragma code_seg(".main") 
void entrypoint(void) 
{
  HINSTANCE hInstance = GetModuleHandle(NULL);
  RECT desktop;
  const HWND hDesktop = GetDesktopWindow();
  GetWindowRect(hDesktop,&desktop);

#ifdef BLOAT_IS_GROSS
  INT_PTR Result = DialogBox(hInstance, MAKEINTRESOURCE(IDD_CONFIG), NULL,(DLGPROC)configDialogProc);
  if (FAILED(Result)){
    MessageBox(NULL, "Could not initialize dialogbox, using default settings.", NULL, MB_OK);
  }
  else{
    if(IDOK != Result) {
      ExitProcess(0);
    }
  }
#endif
  
  if(fullscreen == 1) {
    win_xres = desktop.right;
    win_yres = desktop.bottom;
    DEVMODE screenSettings;
    EnumDisplaySettings(NULL,ENUM_CURRENT_SETTINGS,&screenSettings);
    if(ChangeDisplaySettings(&screenSettings,CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) ExitProcess(1);
  }
  
  win_xpos = (int)(p0d50 * (desktop.right - win_xres)); // 0.5 : p0d50
  win_ypos = (int)(p0d50 * (desktop.bottom - win_yres));

  ShowCursor(FALSE);

  HWND Window = CreateWindow("static",0,WS_POPUP|WS_VISIBLE,win_xpos,win_ypos,win_xres,win_yres,0,0,hInstance,0);

  if(!Window) ExitProcess(1);

  HDC DeviceContext = GetDC(Window);

  //initalize opengl
  if( !SetPixelFormat(DeviceContext,ChoosePixelFormat(DeviceContext,&pfd),&pfd) ) ExitProcess(1);
  wglMakeCurrent(DeviceContext,wglCreateContext(DeviceContext));
  
  InitSound();  
  InitIntro();
  glClear(GL_COLOR_BUFFER_BIT);
  
  long t = 0;
  long dur = timeGetTime();
  do {
    MSG Message;
    PeekMessage(&Message, 0, 0, 0, PM_REMOVE);
    
    waveOutGetPosition(hWaveOut, &MMTime, sizeof(MMTIME));
    t = timeGetTime() - dur;
    intro_do( t , MMTime.u.sample);
    
    wglSwapLayerBuffers( DeviceContext, WGL_SWAP_MAIN_PLANE ); //SwapBuffers( hDC );
    
    //If you press escape or the song ends, everything shuts down
  } while(!GetAsyncKeyState(VK_ESCAPE) && MMTime.u.sample < MAX_SAMPLES);
  
  ExitProcess(0);
}
