#include "application.h"
#include "view.h"
#include <Awesomium/WebCore.h>
#include <Awesomium/STLHelpers.h>
#ifdef _WIN32
#include <Windows.h>
#endif

using namespace Awesomium;

class TutorialApp : public Application::Listener {
  Application* app_;
  View* view_;
 public:
  TutorialApp() 
    : app_(Application::Create()),
      view_(0) {
    app_->set_listener(this);
  }

  virtual ~TutorialApp() {
    if (view_)
      app_->DestroyView(view_);
    if (app_)
      delete app_;
  }

  void Run() {
    app_->Run();
  }

  // Inherited from Application::Listener
  virtual void OnLoaded() {
    view_ = View::Create(800, 600);
    view_->web_view()->LoadURL(WebURL(WSLit("http://www.google.com")));
  }

  // Inherited from Application::Listener
  virtual void OnUpdate() {
  }

  // Inherited from Application::Listener
  virtual void OnShutdown() {
  }
};

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE, wchar_t*, 
  int nCmdShow) {

  TutorialApp app;
  app.Run();

  return 0;
}
