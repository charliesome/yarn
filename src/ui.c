#include <ncurses.h>
#include <signal.h>
#include <stdlib.h>
#include "kvec.h"
#include "ui.h"
#include "channel.h"

static sig_t
old_sigwinch;

struct ui {
    int width, height;
    kvec_t(channel_t*) channels;
};

static void
curses_init()
{
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
}

static void
curses_cleanup()
{
    endwin();
}

static void
on_sigwinch()
{
}

ui_t*
new_ui()
{
    ui_t* ui = malloc(sizeof(*ui));
    curses_init();
    old_sigwinch = signal(SIGWINCH, on_sigwinch);
    return ui;
}

void
free_ui(ui_t* ui)
{
    signal(SIGWINCH, old_sigwinch);
    curses_cleanup();
    free(ui);
}
