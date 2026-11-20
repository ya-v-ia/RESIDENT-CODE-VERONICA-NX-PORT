/* host_fatal on Switch: there are no message boxes, so the system's error dialog (the error
   applet) shows the message until the player closes it. Its own file: <switch.h> clashes with
   the PS2 shim's names and SDL's. */
#include <switch.h>
#include <stdio.h>
#include <sys/stat.h>

void sdl3c_fatal_screen(const char* msg)
{
#ifdef __SWITCH2__
    /* On Switch 2 (kaiHEN), errorApplicationShow() can hang the applet session.
       Write the error to SD and display it on the libounce text console instead. */
    mkdir("sdmc:/switch", 0777);
    mkdir("sdmc:/switch/recvx", 0777);
    FILE* f = fopen("sdmc:/switch/recvx/fatal.txt", "w");
    if (f) {
        fprintf(f, "Code: Veronica X fatal error:\n%s\n", msg ? msg : "(null)");
        fclose(f);
    }
    consoleInit(NULL);
    printf("\n\n  Code: Veronica X (Switch 2) - Fatal Error\n\n");
    printf("  %s\n\n", msg ? msg : "(null)");
    printf("  Saved to: sdmc:/switch/recvx/fatal.txt\n");
    printf("  Log:      sdmc:/switch/recvx/recvx.log\n\n");
    printf("  Press + or B to exit to hbmenu.\n");
    consoleUpdate(NULL);

    padConfigureInput(1, HidNpadStyleSet_NpadStandard);
    PadState pad;
    padInitializeDefault(&pad);
    while (appletMainLoop()) {
        padUpdate(&pad);
        u64 kDown = padGetButtonsDown(&pad);
        if (kDown & (HidNpadButton_Plus | HidNpadButton_B | HidNpadButton_A))
            break;
        consoleUpdate(NULL);
    }
    consoleExit(NULL);
#else
    ErrorApplicationConfig c;
    char text[1800];
    snprintf(text, sizeof(text), "Code: Veronica X: %s\n\nLog: sdmc:/switch/recvx/recvx.log", msg);
    if (R_SUCCEEDED(errorApplicationCreate(&c, text, NULL))) errorApplicationShow(&c);
#endif
}
