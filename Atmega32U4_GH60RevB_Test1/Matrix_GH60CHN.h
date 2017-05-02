#ifndef MATRIX_GH60CHN_H
#define MATRIX_GH60CHN_H

#define ROWS  5
#define COLS  14

//lrow B6 F5 F6 F7 F4 B2
//row D0 D1 D2 D3 D5
//col F0 F1 E6 C7 C6 B7 D4 B0 B1 B5 B4 D7 D6 B3
static uint8_t rowPins[ROWS]={5,6,7,8,23};
static uint8_t colPins[COLS]={21,20,24,10,9,4,22,0,1,14,13,12,11,3};
static  uint8_t RowLED[ROWS+1]={15,18,17,16,19,2};
static  uint8_t ColLED[COLS]={21,20,24,10,9,4,22,0,1,14,13,12,11,3};

 static uint8_t hexaKeys[ROWS][COLS] = {
{KEY_ESC,       KEY_1,KEY_2,KEY_3,KEY_4,KEY_5,KEY_6,KEY_7,KEY_8,KEY_9,KEY_0,        KEY_MINUS,     KEY_EQUAL,      KEY_BACKSPACE},
{KEY_TAB  ,     KEY_Q,KEY_W,KEY_E,KEY_R,KEY_T,KEY_Y,KEY_U,KEY_I,KEY_O,KEY_P,        KEY_LEFT_BRACE,KEY_RIGHT_BRACE,KEY_BACKSLASH},
{KEY_CAPS_LOCK, KEY_A,KEY_S,KEY_D,KEY_F,KEY_G,KEY_H,KEY_J,KEY_K,KEY_L,KEY_SEMICOLON,KEY_QUOTE,     0x00,           KEY_ENTER},
{KEY_LEFT_SHIFT,0x00 ,KEY_Z,KEY_X,KEY_C,KEY_V,KEY_B,KEY_N,KEY_M,KEY_COMMA,KEY_PERIOD,KEY_SLASH,    0x00,           KEY_RIGHT_SHIFT},
{KEY_LEFT_CTRL,KEY_LEFT_GUI,KEY_LEFT_ALT ,0x00,0x00,KEY_SPACE,0x00,0x00,0x00,0x00,KEY_RIGHT_ALT,KEY_RIGHT_GUI,0x00, KEY_RIGHT_CTRL  }
 						};
 static uint8_t hexaKeys2[ROWS][COLS] = {
{KEY_TILDE,     KEY_F1,KEY_F2,KEY_F3,KEY_F4,KEY_F5,KEY_F6,KEY_F7,KEY_F8,KEY_F9,KEY_F10,KEY_F11,KEY_F12,      KEY_DELETE},
{KEY_TAB  ,     KEY_Q,KEY_UP,KEY_E,KEY_R,KEY_T,KEY_Y,KEY_U,KEY_I,KEY_O,KEY_P,        KEY_LEFT_BRACE,KEY_RIGHT_BRACE,KEY_BACKSLASH},
{KEY_CAPS_LOCK, KEY_LEFT,KEY_DOWN,KEY_RIGHT,KEY_F,KEY_G,KEY_H,KEY_J,KEY_K,KEY_L,KEY_SEMICOLON,KEY_QUOTE,     0x00,           KEY_ENTER},
{KEY_LEFT_SHIFT,0x00 ,KEY_Z,KEY_X,KEY_C,KEY_V,KEY_B,KEY_N,KEY_M,KEY_COMMA,KEY_PERIOD,KEY_SLASH,    0x00,           KEY_RIGHT_SHIFT},
{KEY_LEFT_CTRL,KEY_LEFT_GUI,KEY_LEFT_ALT ,0x00,0x00,KEY_SPACE,0x00,0x00,0x00,0x00,KEY_RIGHT_ALT,KEY_RIGHT_GUI,0x00, KEY_RIGHT_CTRL  }
 						};
 static uint8_t keytype[ROWS][COLS] = {
		 {0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10},
		 {0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10},
		 {0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x10},
		 {0x20,0x00,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x20},
		 {0x20,0x20,0x20,0x00,0x00,0x10,0x00,0x00,0x00,0x00,0x20,0x20,0x40,0x20}
 };
 /*
0		PIN_B0
1		PIN_B1
2		PIN_B2
3		PIN_B3
4		PIN_B7
5		PIN_D0
6		PIN_D1
7		PIN_D2
8		PIN_D3
9		PIN_C6
10		PIN_C7
11		PIN_D6
12		PIN_D7
13		PIN_B4
14		PIN_B5
15		PIN_B6
16		PIN_F7
17		PIN_F6
18		PIN_F5
19		PIN_F4
20		PIN_F1
21		PIN_F0
22		PIN_D4
23		PIN_D5
24		PIN_E6
  *
  */
  #endif