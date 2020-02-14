#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <ctype.h>


#define CTRL_KEY(k) ((k) & 0x1f)


/*** terminal ***/

struct termios orig_termios;

void print_error_and_exit(const char *s) {
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);
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

char editorReadKey() {
    int nread;
    char c;

    // read read 1 byte from std input into c
    while ((nread = read(STDIN_FILENO, &c, 1)) != 1)
    {
        if (nread == -1 && errno != EAGAIN) print_error_and_exit("read");
    }
    
    return c;
}

/*** input ***/


void editorProcessKeypress() {
    char c = editorReadKey();

    switch (c)
    {
    case CTRL_KEY('q'):
        write(STDOUT_FILENO, "\x1b[2J", 4);
        write(STDOUT_FILENO, "\x1b[H", 3);
        exit(0);
        break;
    
    default:
        break;
    }
}

/*** ouput ***/

// drawing each row of the buffer of text being edited
void editorDrawRows() {
    for (size_t y = 0; y < 24; y++)
    {
        write(STDOUT_FILENO, "~\r\n", 3);
    }
}

void editorRefreshScreen() {
    // \x1b: escape character 1 byte
    // <esc>[2J will clear the entire screen
    // write 4 bytes to terminal
    write(STDOUT_FILENO, "\x1b[2J", 4);

    // \x1b[H: reposition the cursor to top-left pos in terminal
    write(STDOUT_FILENO, "\x1b[H", 3);

    editorDrawRows();

    write(STDOUT_FILENO, "\x1b[H", 3);
}


/*** init ***/

int main(int argc, char const *argv[])
{
    enableRawMode();
    
    while (1) {
        editorRefreshScreen();
        editorProcessKeypress();
    }
    printf("kilo\n");
    return 0;
}
