#ifndef MINITEL_H
#define MINITEL_H
/**
 * Minitel library for Arduino (v0.2) / April 2015
 * http://github.com/01010101/Minitel
 *
 * By Jerome Saint-Clair aka 01010101
 * http://saint-clair.net
 * 
 * For the Graffiti Research Lab France
 * http://graffitiresearchlab.fr
 * 
 * Based on works by the Tetalab (Fabrice, Renaud, PG & Phil)
 * http://tetalab.org
 */


#include "xtimer.h"
#include <math.h>
#include <string.h>

typedef int boolean;
typedef unsigned char byte;
typedef char* String;

// Boolean values
#define false 0
#define true 1
#define FALSE 0
#define TRUE 1

// Modes

#define GRAPHIC_MODE 14
#define TEXT_MODE 15

#define HORIZONTAL 0
#define VERTICAL 1

// Cursor moves
#define LEFT 8
#define RIGHT 9
#define DOWN 10
#define UP 11

// Cursor positionning
#define HOME 13
#define LINE_END 24

#define TOP_LEFT 1
#define TOP_RIGHT 2
#define BOTTOM_LEFT 3
#define BOTTOM_RIGHT 4
#define CENTER 5

// Cursor visibility
// Preceeded by 27
#define CURSOR_SHOW 17
#define CURSOR_HIDE 20

// Clear screen
#define CLEARSCREEN 12

// Font type
// Preceeded by 27
// DOUBLE-HEIGHT and DOUBLE may not work on certain Minitels
#define SIZE_NORMAL 76
#define SIZE_DOUBLE_HEIGHT 77
#define SIZE_DOUBLE_WIDTH 78
#define SIZE_DOUBLE 79


// Colors (+80 for background, +64 for text)
// Preceeded by 27
// 64->71 txt color black, red, green, yellow, blue, magenta, cyan, white
// 80->87 bg color black, red, green, yellow, blue, magenta, cyan, white
#define BLACK 0
#define RED 1
#define GREEN 2
#define YELLOW 3
#define MAGENTA 4
#define BLUE 5
#define CYAN 6
#define WHITE 7

// Blink
// Preceeded by 27
#define BLINK_ON 72
#define BLINK_OFF 73

// Incrustation
// Preceeded by 27
#define INCRUSTATION_ON 75
#define INCRUSTATION_OFF 74

// Underline
#define UNDERLINE_ON 90
#define UNDERLINE_OFF 89

// Underline
#define LINE_MASK_ON 88
#define LINE_MASK_OFF 95

// Video mode
#define VIDEO_INVERT 93
#define VIDEO_STANDARD 92
#define VIDEO_TRANSPARENT 94

// Speeds
#define SPEED_75 0
#define SPEED_300 1
#define SPEED_4800 100
#define SPEED_9600 111 // ??? Minitel 2 ???

// Bip
#define BIP 7

// Accents
// #define ACCUTE 65
// #define GRAVE 66
// #define CIRCUMFLEX 67
// #define UMLAUT 72

#define SUB_ARTICLE_SEPARATOR 31;

// Preceeded by 25
#define SPE_CHAR_POUND 35
#define SPE_CHAR_DOLLAR 36
#define SPE_CHAR_HASHTAG 38
#define SPE_CHAR_PARAGRAPH 39
#define SPE_CHAR_ARROW_LEFT 44
#define SPE_CHAR_ARROW_UP 45
#define SPE_CHAR_ARROW_RIGHT 46
#define SPE_CHAR_ARROW_DOWN 47
#define SPE_CHAR_DEGREE 48
#define SPE_CHAR_MINUS_PLUS 49
#define SPE_CHAR_DIVIDE 56
#define SPE_CHAR_1_4 60 // output only ?
#define SPE_CHAR_1_2 61 // output only ?
#define SPE_CHAR_3_4 62 // output only ?
#define SPE_CHAR_GRAVE 65
#define SPE_CHAR_ACUTE 66
#define SPE_CHAR_CIRCUMFLEX 67
#define SPE_CHAR_UMLAUT 72
#define SPE_CHAR_CEDIL 75
#define SPE_CHAR_UPPER_OE 106
#define SPE_CHAR_LOWER_OE 122
#define SPE_CHAR_BETA 123

// Non Arduino characters
#define SPE_CHAR_ARROW_UP2 94
#define SPE_CHAR_PIPE_BOTTOM 95
#define SPE_CHAR_PIPE_MIDDLE 96
#define SPE_CHAR_PIPE_LEFT 123
#define SPE_CHAR_PIPE_CENTER 124
#define SPE_CHAR_PIPE_RIGHT 125
#define SPE_CHAR_PIPE_TOP 126

// TODO Escape character handling ???

#define SOMMAIRE 198
#define ANNULATION 197
#define RETOUR 66
#define REPETITION 195
#define GUIDE 68
#define CORRECTION 71
#define SUITE 72
#define ENVOI 65


void init(void);
void mode(byte mode);
void cursor(boolean b);
void blink(boolean b);
void pixelate(boolean b);
void video(byte v);
void incrustation(boolean b);
void lineMask(boolean b);

boolean isValidChar(byte index);
boolean isSpecialChar(byte c);

boolean isAccent(char c);
boolean printAccentChar(char c);
void printAccent(int index);
char getAccentLetter(int letterIndex);

void refreshSettings(void);

void serialprint7(byte b);

byte getGraphicChar(String s);
void graphic(String s);

void textByte(byte b);
boolean textChar(byte c);
void text(String s, int orientation);
byte getCharByte(char c);
void specialChar(byte c);
void semiGraphic(byte b);

void repeat(byte n);

void bgColor(byte c);
void textColor(byte c);
void useDefaultColors(void);

void moveCursorTo(byte x, byte y);
void moveCursorToLocation(byte location);
void moveCursor(byte dir, int n);

void clearScreen(void);

void graphicMode(void);
void textMode(void);

void charSize(byte type);

void standardVideo(void);
void invertVideo(void);
void transparentVideo(void);

void setMaxSpeed(void);

void bip(unsigned long duration);

void rect_char(char c, int x, int y, int w, int h);
void rect(byte c, int x, int y, int w, int h);
void spiral(int x, int y, int siz, int c);

void readKey(void);
boolean keyTyped(void);

boolean isMenuKey(void);
int getMenuKey(void);

boolean isSpecialCharacterKey(void);
int getSpecialCharacterKey(void);

boolean accentKeyStored(void);
int getAccentKey(void);

boolean isCharacterKey(void);
boolean isSerializableKey(void);
char getCharacterKey(void);

#endif
