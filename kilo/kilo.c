#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <ctype.h>


struct termios orig_termios;

void print_error_and_exit(const char *s) {
    perror(s);
    exit(1);
}

void disasbleRawMode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        print_error_and_exit("tcsetattr");
}


void enableRawMode() {
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) print_error_and_exit("tcgetattr");
    atexit(disasbleRawMode); // register disasbleRawMode, called automatically when program exit.


    struct termios raw = orig_termios;

    // ICRNL: ctrl-m expect it to be read as 13
    // IXON: trun off software flow control
    raw.c_iflag &= ~( BRKINT | ICRNL | INPCK | ISTRIP | IXON);

    // OPOST: trun off all output processing features by turning off the OPOST flag
    raw.c_oflag &= ~(OPOST);

    // sets the character size (CS) to 8 bits per byte
    raw.c_cflag |= (CS8);

    // ICANON: turn off canonical mode
    // ISIG: ture off sending sigs
    // IEXTEN: trun off ctrl-v
    raw.c_lflag &= ~(ECHO | IEXTEN | ICANON | ISIG);

    // sets the minimum number of bytes of input needed before read() can return
    raw.c_cc[VMIN] = 0;

    // sets the maximum amount of time to wait before read() returns
    raw.c_cc[VTIME] = 1;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) print_error_and_exit("tcsetattr");
}


int main(int argc, char const *argv[])
{
    enableRawMode();

   

    // read read 1 byte  from std input into c
    while (1) {
        char c = '\0';
        
        if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN) print_error_and_exit("read");

        // iscntrl: whether a character is a control character
        if (iscntrl(c)) {
            printf("%d\r\n", c);
        } else {
            printf("%d ('%c')\r\n", c, c);
        }
        if (c == 'q') break;
    }
    printf("kilo\n");
    return 0;
}
