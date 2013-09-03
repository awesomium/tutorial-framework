#include "view.h"
#include <Awesomium/WebCore.h>
#include <Awesomium/STLHelpers.h>
#include <vector>

class ViewWin;

static bool g_is_initialized = false;
static std::vector<ViewWin*> g_active_views_;
const wchar_t szWindowClass[] = L"ViewWinClass";
const wchar_t szTitle[] = L"Application";
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

using namespace Awesomium;

class ViewWin : public View,
                public WebViewListener::View {
public:
  ViewWin(int width, int height) {
    PlatformInit();

    web_view_ = WebCore::instance()->CreateWebView(width,
                                           height,
                                           0,
                                           Awesomium::kWebViewType_Window);

    web_view_->set_view_listener(this);

    // Create our WinAPI Window
    HINSTANCE hInstance = GetModuleHandle(0);
    hwnd_ = CreateWindow(szWindowClass,
              szTitle,
              WS_OVERLAPPEDWINDOW,
              CW_USEDEFAULT,
              CW_USEDEFAULT,
              width + 20,
              height + 40, 
              NULL,
              NULL,
              hInstance,
              NULL);

    if (!hwnd_)
      exit(-1);

    web_view_->set_parent_window(hwnd_);

    ShowWindow(hwnd_, SW_SHOWNORMAL);
    UpdateWindow(hwnd_);

    SetTimer (hwnd_, 0, 15, NULL );

    g_active_views_.push_back(this);
  }

  virtual ~ViewWin() {
    for (std::vector<ViewWin*>::iterator i = g_active_views_.begin();
        i != g_active_views_.end(); i++) {
      if (*i == this) {
        g_active_views_.erase(i);
        break;
      }
    }

    web_view_->Destroy();
  }

  HWND hwnd() { return hwnd_; }

  static void PlatformInit() {
    if (g_is_initialized)
      return;

    WNDCLASSEX wc;

    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = GetModuleHandle(0);
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = szWindowClass;
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if(!RegisterClassEx(&wc)) {
      exit(-1);
    }

    g_is_initialized = true;
  }

  static ViewWin* GetFromHandle(HWND handle) {
    for (std::vector<ViewWin*>::iterator i = g_active_views_.begin();
        i != g_active_views_.end(); i++) {
      if ((*i)->hwnd() == handle) {
        return *i;
      }
    }

    return NULL;
  }

  // Following methods are inherited from WebViewListener::View

  virtual void OnChangeTitle(Awesomium::WebView* caller,
                             const Awesomium::WebString& title) {
    std::string title_utf8(ToString(title));
    std::wstring title_wide(title_utf8.begin(), title_utf8.end());

    SetWindowText(hwnd_, title_wide.c_str());
  }

  virtual void OnChangeAddressBar(Awesomium::WebView* caller,
    const Awesomium::WebURL& url) { }

  virtual void OnChangeTooltip(Awesomium::WebView* caller,
    const Awesomium::WebString& tooltip) { }

  virtual void OnChangeTargetURL(Awesomium::WebView* caller,
    const Awesomium::WebURL& url) { }

  virtual void OnChangeCursor(Awesomium::WebView* caller,
    Awesomium::Cursor cursor) { }

  virtual void OnChangeFocus(Awesomium::WebView* caller,
    Awesomium::FocusedElementType focused_type) { }

  virtual void OnShowCreatedWebView(Awesomium::WebView* caller,
                                    Awesomium::WebView* new_view,
                                    const Awesomium::WebURL& opener_url,
                                    const Awesomium::WebURL& target_url,
                                    const Awesomium::Rect& initial_pos,
                                    bool is_popup) { }

    virtual void OnAddConsoleMessage(Awesomium::WebView* caller,
                                   const Awesomium::WebString& message,
                                   int line_number,
                                   const Awesomium::WebString& source) { }

 protected:
  HWND hwnd_;
};

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
  ViewWin* view = ViewWin::GetFromHandle(hWnd);

  switch (message) {
  case WM_COMMAND:
    return DefWindowProc(hWnd, message, wParam, lParam);
    break;
  case WM_TIMER:
    break;
  case WM_SIZE:
    view->web_view()->Resize(LOWORD(lParam), HIWORD(lParam));
    break;
  case WM_DESTROY:
    PostQuitMessage(0);
    break;
  case WM_QUIT:
    break;
  default:
    return DefWindowProc(hWnd, message, wParam, lParam);
  }
  return 0;
}

View* View::Create(int width, int height) {
  return new ViewWin(width, height);
}