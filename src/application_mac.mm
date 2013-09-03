#include "application.h"
#include "view.h"
#import <Cocoa/Cocoa.h>
#include <Awesomium/WebCore.h>
#include <string>

@interface AppDelegate : NSObject<NSApplicationDelegate> {
  NSTimer *timer;
}
- (void)startUpdateTimer;
- (void)applicationWillTerminate:(NSNotification *)aNotification;
- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication;
- (void)updateTimer:(NSTimer *)timer;
@end

using namespace Awesomium;

class ApplicationMac : public Application {
  NSAutoreleasePool* pool;
  NSApplication* app;
  AppDelegate* appDelegate;
public:
  ApplicationMac() {
    listener_ = NULL;
    web_core_ = NULL;
  }
  
  virtual ~ApplicationMac() {
    [NSApp setDelegate:nil];
    [appDelegate release];
    [pool release];
    
    if (listener())
      listener()->OnShutdown();
    
    if (web_core_)
      web_core_->Shutdown();
  }
  
  virtual void Run() {
    Load();
    
    [NSApp activateIgnoringOtherApps:YES];
    [appDelegate startUpdateTimer];
    [NSApp run];
  }
  
  virtual void Quit() {
    [app terminate:app];
  }
  
  virtual void Load() {
    pool = [[NSAutoreleasePool alloc] init];
    app = [NSApplication sharedApplication];
    [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
    appDelegate = [[AppDelegate alloc] init];
    [NSApp setDelegate:appDelegate];
    
    web_core_ = WebCore::Initialize(WebConfig());
    
    if (listener())
      listener()->OnLoaded();
  }
  
  virtual View* CreateView(int width, int height) {
    return View::Create(width, height);
  }
  
  virtual void DestroyView(View* view) {
    delete view;
  }
  
  virtual void ShowMessage(const char* message) {
    NSAlert *alert = [[[NSAlert alloc] init] autorelease];
    [alert setMessageText:[[NSString alloc] initWithUTF8String:message]];
    [alert runModal];
  }
};

Application* Application::Create() {
  return new ApplicationMac();
}

@implementation AppDelegate

- (void)startUpdateTimer {
	timer = [NSTimer timerWithTimeInterval:(1.0f/60.0f)
                                  target:self
                                selector:@selector(updateTimer:)
                                userInfo:nil
                                 repeats:YES];
	[[NSRunLoop currentRunLoop] addTimer:timer forMode:NSDefaultRunLoopMode];
	[[NSRunLoop currentRunLoop] addTimer:timer forMode:NSEventTrackingRunLoopMode];
}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
  if (Awesomium::WebCore::instance())
    Awesomium::WebCore::instance()->Shutdown();
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication {
  return YES;
}

- (void)updateTimer:(NSTimer *)timer {
	if (Awesomium::WebCore::instance())
    Awesomium::WebCore::instance()->Update();
}

@end
