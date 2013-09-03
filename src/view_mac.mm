#include "view.h"
#import <Cocoa/Cocoa.h>
#include <Awesomium/WebCore.h>
#include <Awesomium/STLHelpers.h>
#include <vector>

@interface WinDelegate : NSObject<NSWindowDelegate> {
  Awesomium::WebView* webView;
}
@property (assign) Awesomium::WebView* webView;
- (void)windowDidResize:(NSNotification *)notification;
@end

using namespace Awesomium;

class ViewMac : public View,
                public WebViewListener::View {
public:
  ViewMac(int width, int height) {
    web_view_ = WebCore::instance()->CreateWebView(width,
                                                   height,
                                                   0,
                                                   Awesomium::kWebViewType_Window);
    
    web_view_->set_view_listener(this);
    
    window_ = [[[NSWindow alloc] initWithContentRect:NSMakeRect(0, 0, width, height)
                                          styleMask:NSTitledWindowMask | NSResizableWindowMask | NSClosableWindowMask
                                            backing:NSBackingStoreBuffered defer:NO]
                                 autorelease];
    
  
    [window_ cascadeTopLeftFromPoint:NSMakePoint(20,20)];
    [window_ setTitle:@"Application"];
    
    NSView* contentsContainer = [window_ contentView];
    NSView* contentsNativeView = web_view_->window();
    NSRect contentsNativeViewFrame = [contentsContainer frame];
    contentsNativeViewFrame.origin = NSZeroPoint;
    [contentsNativeView setFrame:contentsNativeViewFrame];
    [contentsContainer addSubview:contentsNativeView];
    
    win_delegate_ = [[WinDelegate alloc] init];
    [window_ setDelegate:win_delegate_];
    win_delegate_.webView = web_view_;
    
    [window_ makeKeyAndOrderFront:nil];
  }
  
  virtual ~ViewMac() {
    [window_ setDelegate:nil];
    [win_delegate_ release];
    [window_ release];
    
    web_view_->Destroy();
  }
  
  // Following methods are inherited from WebViewListener::View
  
  virtual void OnChangeTitle(Awesomium::WebView* caller,
                             const Awesomium::WebString& title) {
    std::string title_utf8(ToString(title));
    [window_ setTitle:[[NSString alloc] initWithUTF8String:title_utf8.c_str()]];
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
                    
  virtual void OnAddConsoleMessage(Awesomium::WebView* caller,
                                   const Awesomium::WebString& message,
                                   int line_number,
                                   const Awesomium::WebString& source) { }
  
  virtual void OnShowCreatedWebView(Awesomium::WebView* caller,
                                    Awesomium::WebView* new_view,
                                    const Awesomium::WebURL& opener_url,
                                    const Awesomium::WebURL& target_url,
                                    const Awesomium::Rect& initial_pos,
                                    bool is_popup) { }
  
protected:
  NSWindow* window_;
  WinDelegate* win_delegate_;
};


View* View::Create(int width, int height) {
  return new ViewMac(width, height);
}

@implementation WinDelegate

@synthesize webView;

- (void)windowDidResize:(NSNotification *)notification {
  NSWindow* window = [notification object];
  NSSize windowSize = [[window contentView] frame].size;
  webView->Resize((int)windowSize.width, (int)windowSize.height);
}

@end
