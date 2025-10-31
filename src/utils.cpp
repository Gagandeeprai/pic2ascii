#include <sys/ioctl.h>
#include <unistd.h>
#include <iostream>

int getTerminalWidth() {
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {
        std::cerr << "Warning: could not detect terminal width, defaulting to 80\n";
        return 80;
    }
    return w.ws_col; // number of columns in terminal
}
