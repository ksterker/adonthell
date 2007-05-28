/*   SDLMain.m - main entry point for our Cocoa-ized SDL app
       Initial Version: Darrell Walisser <dwaliss1@purdue.edu>
       Non-NIB-Code & other changes: Max Horn <max@quendi.de>

    Feel free to customize this file to suit your needs
*/

#import "SDL.h"
#import "osx.h"
#import <sys/param.h> /* for MAXPATHLEN */
#import <unistd.h>

#ifdef USE_LIBTOOL
/* exported names for libltdl */
#define main_init _sdl_LTX_main_init
#endif

/* Portions of CPS.h */
typedef struct CPSProcessSerNum
{
	UInt32		lo;
	UInt32		hi;
} CPSProcessSerNum;

extern "C" {
    OSErr	CPSGetCurrentProcess( CPSProcessSerNum *psn);
    OSErr 	CPSEnableForegroundOperation( CPSProcessSerNum *psn, UInt32 _arg2, UInt32 _arg3, UInt32 _arg4, UInt32 _arg5);
    OSErr	CPSSetFrontProcess( CPSProcessSerNum *psn);
}

/* An internal Apple class used to setup Apple menus */
@interface NSAppleMenuController:NSObject {}
- (void)controlMenu:(NSMenu *)aMenu;
@end

@interface SDLApplication : NSApplication
@end

@implementation SDLApplication
/* Invoked from the Quit menu item */
- (void)terminate:(id)sender
{
    /* Post a SDL_QUIT event */
    SDL_Event event;
    event.type = SDL_QUIT;
    SDL_PushEvent(&event);
}
@end


/* The main class of the application, the application's delegate */
@implementation SDLMain

- init: (adonthell::app*) theApp
{
    Application = theApp;
}

static NSString *getApplicationName(void)
{
    NSDictionary *dict;
    NSString *appName = 0;

    /* Determine the application name */
    dict = (NSDictionary *)CFBundleGetInfoDictionary(CFBundleGetMainBundle());
    if (dict)
        appName = [dict objectForKey: @"CFBundleName"];
    
    if (![appName length])
        appName = [[NSProcessInfo processInfo] processName];

    return appName;
}

static void setApplicationMenu(void)
{
    /* warning: this code is very odd */
    NSMenu *appleMenu;
    NSMenuItem *menuItem;
    NSString *title;
    NSString *appName;
    
    appName = getApplicationName();
    appleMenu = [[NSMenu alloc] initWithTitle:@""];
    
    /* Add menu items */
    title = [@"About " stringByAppendingString:appName];
    [appleMenu addItemWithTitle:title action:@selector(orderFrontStandardAboutPanel:) keyEquivalent:@""];

    [appleMenu addItem:[NSMenuItem separatorItem]];

    title = [@"Hide " stringByAppendingString:appName];
    [appleMenu addItemWithTitle:title action:@selector(hide:) keyEquivalent:@"h"];

    menuItem = (NSMenuItem *)[appleMenu addItemWithTitle:@"Hide Others" action:@selector(hideOtherApplications:) keyEquivalent:@"h"];
    [menuItem setKeyEquivalentModifierMask:(NSAlternateKeyMask|NSCommandKeyMask)];

    [appleMenu addItemWithTitle:@"Show All" action:@selector(unhideAllApplications:) keyEquivalent:@""];

    [appleMenu addItem:[NSMenuItem separatorItem]];

    title = [@"Quit " stringByAppendingString:appName];
    [appleMenu addItemWithTitle:title action:@selector(terminate:) keyEquivalent:@"q"];

    
    /* Put menu into the menubar */
    menuItem = [[NSMenuItem alloc] initWithTitle:@"" action:nil keyEquivalent:@""];
    [menuItem setSubmenu:appleMenu];
    [[NSApp mainMenu] addItem:menuItem];

    /* Tell the application object that this is now the application menu */
    [NSApp setAppleMenu:appleMenu];

    /* Finally give up our references to the objects */
    [appleMenu release];
    [menuItem release];
}

/* Create a window menu */
static void setupWindowMenu(void)
{
    NSMenu      *windowMenu;
    NSMenuItem  *windowMenuItem;
    NSMenuItem  *menuItem;

    windowMenu = [[NSMenu alloc] initWithTitle:@"Window"];
    
    /* "Minimize" item */
    menuItem = [[NSMenuItem alloc] initWithTitle:@"Minimize" action:@selector(performMiniaturize:) keyEquivalent:@"m"];
    [windowMenu addItem:menuItem];
    [menuItem release];
    
    /* Put menu into the menubar */
    windowMenuItem = [[NSMenuItem alloc] initWithTitle:@"Window" action:nil keyEquivalent:@""];
    [windowMenuItem setSubmenu:windowMenu];
    [[NSApp mainMenu] addItem:windowMenuItem];
    
    /* Tell the application object that this is now the window menu */
    [NSApp setWindowsMenu:windowMenu];

    /* Finally give up our references to the objects */
    [windowMenu release];
    [windowMenuItem release];
}

/* Replacement for NSApplicationMain */
void CustomApplicationMain (adonthell::app *theApp)
{
    NSAutoreleasePool	*pool = [[NSAutoreleasePool alloc] init];
    SDLMain				*sdlMain;

    /* Ensure the application object is initialised */
    [SDLApplication sharedApplication];

    {
        CPSProcessSerNum PSN;
        /* Tell the dock about us */
        if (!CPSGetCurrentProcess(&PSN))
            if (!CPSEnableForegroundOperation(&PSN,0x03,0x3C,0x2C,0x1103))
                if (!CPSSetFrontProcess(&PSN))
                    [SDLApplication sharedApplication];
    }

    /* Set up the menubar */
    [NSApp setMainMenu:[[NSMenu alloc] init]];
    setApplicationMenu();
    setupWindowMenu();
    
    /* Create SDLMain and make it the app delegate */
    sdlMain = [[SDLMain alloc] init:theApp];
    [NSApp setDelegate:sdlMain];
    
    /* Start the main event loop */
    [NSApp run];
    
    [sdlMain release];
    [pool release];
}

/* Called when the internal event loop has just started running */
- (void) applicationDidFinishLaunching: (NSNotification *) note
{
    int status = Application->main ();
    Application->cleanup ();
    exit (status);
}
@end

extern "C" {

/* Init */
int main_init (adonthell::app *theApp)
{
    CustomApplicationMain (theApp);
    return 0;
}

}
