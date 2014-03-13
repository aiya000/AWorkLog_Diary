#include "Select.h"
#include <ncurses.h>

Select::Select(){
	initscr();
	keypad(stdscr, true);
	noecho();
	cbreak();
}
Select::~Select(){ this->close(); }
void Select::close(){ endwin(); }

void Select::run(){
	const int SCR_HEIGHT = [](){ int y,x; getmaxyx(stdscr,y,x); return x; }();
	const int SCR_WIDTH  = [](){ int y,x; getmaxyx(stdscr,y,x); return y; }();
	WINDOW* main = newwin(SCR_WIDTH, SCR_HEIGHT, 0, 0);
	box(main, ACS_VLINE, ACS_HLINE);  wrefresh(main);

	auto draw = [&](){
		touchwin(main);  wrefresh(main);
		mvwaddstr(main, (SCR_HEIGHT/3)*2, 1, "q : close");
		refresh();
	};


	bool loopFlag = true;
	while(loopFlag){
		draw();
		switch(getch()){
		case KEY_DOWN:
		case 'j':
			break;
		case KEY_UP:
		case 'k':
			break;
		case 'q':
			loopFlag = false;
			break;
		}
	}

	delwin(main);
}
