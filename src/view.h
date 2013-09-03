#ifndef TUTORIAL_VIEW_H_
#define TUTORIAL_VIEW_H_

namespace Awesomium {
class WebView;
}

// Common class that implements a windowed WebView, handles all input/display,
// and abstracts away all the platform-specific details.
class View {
 public:
  virtual ~View() {}

  // Platform-specific constructor
  static View* Create(int width, int height);

  // Get the associated WebView
  Awesomium::WebView* web_view() { return web_view_; }

 protected:
  View() { }

  Awesomium::WebView* web_view_;
};

#endif  // TUTORIAL_VIEW_H_
