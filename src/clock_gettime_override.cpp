#include <dlfcn.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <thread>
#include <iostream>
#include <thread>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <unistd.h>
#include <signal.h>

using namespace std;

// *** commented code below is a placeholder for enabling time delay on hotkey. I am
// *** desperately stuck. Problem is: wine launches a shitload of processes; but I
// *** can't do global keyboard grab in all of them. I need to determine the one
// *** where I want to delay time, and only grab hotkey there. And… the "constructor"
// *** function below never gets ran for any process with "exe" postfix!

// extern char *program_invocation_name;

// const char app_name[] = "winecfg.exe";

// void __attribute__((constructor)) hotkey_handler() {
//     return; // TODO
//     if (strcmp(app_name, program_invocation_name))
//         return;
//     Display*    dpy     = XOpenDisplay(0);
//     Window      root    = DefaultRootWindow(dpy);
//     XEvent      ev;

//     unsigned int    modifiers       = 0; // ControlMask | ShiftMask;
//     int             keycode         = XKeysymToKeycode(dpy,XK_F8);
//     Window          grab_window     = root;
//     Bool            owner_events    = False;
//     int             pointer_mode    = GrabModeAsync;
//     int             keyboard_mode   = GrabModeAsync;

//     XGrabKey(dpy, keycode, AnyModifier, grab_window, owner_events, pointer_mode,
//              keyboard_mode);

//     XSelectInput(dpy, root, KeyPressMask );
//     while(true) {
//         XNextEvent(dpy, &ev);
//         switch(ev.type)
//         {
//             case KeyPress:
//                 if ((ev.xkey.state & (ShiftMask | ControlMask | Mod1Mask | Mod4Mask)) == modifiers) {
//                     // todo: change time flow here
//                     XUngrabKey(dpy,keycode,modifiers,grab_window);
//                 }

//             default:
//                 break;
//         }
//     }

//     XCloseDisplay(dpy);
// }

extern "C"
int clock_gettime(clockid_t clk_id, struct timespec *tp) {
    using clock_gettime_ptr = int (*)(clockid_t, struct timespec*);
    const static clock_gettime_ptr real_clock_gettime
        = (clock_gettime_ptr) dlsym(RTLD_NEXT, "clock_gettime");
    int ret = real_clock_gettime(clk_id, tp);
    if (ret != -1) {
        const short AMOUNT_TO_SLOW = 4;
        const time_t NSEC_FRAC = 1000000000 / AMOUNT_TO_SLOW;
        const long nsec_part = NSEC_FRAC * (tp->tv_sec % AMOUNT_TO_SLOW);
        tp->tv_nsec = tp->tv_nsec / AMOUNT_TO_SLOW + nsec_part;
        tp->tv_sec = tp->tv_sec / AMOUNT_TO_SLOW;
    }
    return ret;
}
