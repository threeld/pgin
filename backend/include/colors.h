// colors.h
#ifndef COLORS_H
#define COLORS_H

// Define ANSI color codes as macros
#define RESET   "\033[0m"   // resets the colour and font style to the default
#define BOLD    "\033[1m"
#define UNDERLINE "\033[4m"
#define BLACK   "\033[30m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define BGBLUE  "\033[44m"

// Some extra colours
#define BOLD_RED    "\033[1;31m"
#define BG_GREEN    "\033[42m"

#endif // COLORS_H

