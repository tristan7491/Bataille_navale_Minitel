/**
   Minitel library for Arduino (v0.2) / April 2015
   http://github.com/01010101/Minitel

   By Jerome Saint-Clair aka 01010101
   http://saint-clair.net

   For the Graffiti Research Lab France
   http://graffitiresearchlab.fr

   Based on works by the Tetalab (Fabrice, Renaud, PG & Phil)
   http://tetalab.org
*/

#include "include/Minitel.h"


String accents = "àáâäèéêëìíîïòóôöùúûü";

byte _currentBgColor = BLACK;
byte _currentTextColor = WHITE;
byte _currentMode = TEXT_MODE;
byte _currentVideo = VIDEO_STANDARD;
byte _currentSize = SIZE_NORMAL;
boolean _currentUnderline = false;
boolean _currentBlink = false;
boolean _currentShowCursor = false;
boolean _menuKeyPressed = false;

int _menuKey;
int _specialCharacterKey;
int _characterKey;
int _accentKey;

char _nullChar = '~';

void init(void) {
  useDefaultColors();
  refreshSettings();
}

byte getGraphicChar(String s) {
  byte carac = 32; // caractère pixel

  if (strlen(s) == 6) {
    carac += s[0] == '0' ? 0 : 1;
    carac += s[1] == '0' ? 0 : 2;
    carac += s[2] == '0' ? 0 : 4;
    carac += s[3] == '0' ? 0 : 8;
    carac += s[4] == '0' ? 0 : 16;
    carac += s[5] == '0' ? 0 : 32;
    return carac;
  }
  return 9;
}

void serialprint7(byte b) {
  /*boolean  i = false;
  for (int j = 0; j < 8; j++) {
    if (bitRead(b, j) == 1) {
      i = !i; //calcul de la parité
    }
  }
  if (i) {
    bitWrite(b, 7, 1); //ecriture de la partié
  }
  else {
    bitWrite(b, 7, 0); //ecriture de la partié
  }
  write(b); //ecriture du byte sur le software serial
  */
  printf("%c",b);
}


void graphic(String s) {
  serialprint7(getGraphicChar(s));
}


void textByte(byte b) {
  serialprint7(b);
}


boolean textChar(byte c) {
  byte charByte = getCharByte(c);
  if (isValidChar(charByte)) {
    serialprint7(charByte);
    return true;
  }
  return false;
}

void text(String s, int orientation) {
  for (unsigned int i = 0; i < strlen(s); i++) {
    char c = s[i];
    boolean indent = false;
    if (isAccent(c)) {
      i += 1; // chars with accents take 2 array indexes
      c = s[i];
      indent = printAccentChar(c);
    }
    else {
    
      // TODO Check if c cedil
      indent = textChar(c);
    }
    if (indent && orientation == VERTICAL) {
      moveCursor(LEFT,1);
      moveCursor(DOWN,1);
    }
  }
}

// Characters
/*
  xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx // 0 -> 32
  !"#$%&'()*+,-./0123456789:;<=>?@ // 33 -> 64
  ABCDEFGHIJKLMNOPQRSTUVWXYZ[\] // 65 -> 93
  x // 94 up arrow
  _ // 95 lower pipe associated to underscore
  x // 96 pipe 
  abcdefghijklmnopqrstuvwxyz // 97 -> 122
  // 123 124 125 126 various pipes
*/

// Used to display characters sent from the Arduino
// As a result, not all Minitel supported characters can be sent to/from Arduino
// However, they can be displayed using the specialChar() or graphic functions

byte getCharByte(char c) {
  String characters = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]x_xabcdefghijklmnopqrstuvwxyz";
  return (byte) *strrchr(characters,c);
}


boolean isSerializableKey(void) {
		return (_characterKey >= 33 && _characterKey <= 122 && _characterKey != 94 && _characterKey != 96);
}


void specialChar(byte b) {
  if (isValidChar(b)) {
    serialprint7(25);
    serialprint7(b);
	if (b == 75) {
		serialprint7(99);
	}
  }
}


boolean isValidChar(byte index) {
  if (index >= 32 && index <= 123) {
    return true;
  }
  return false;
}

//
//
// ACCENTS HANDLING
//
//


boolean isAccent(char c) {
  //String accents = "àáâäèéêëìíîïòóôöùúûü";
  if (strchr(accents,c) != NULL) {
    return true;
  }
  return false;
}


boolean printAccentChar(char c) {
    //String accents = "àáâäèéêëìíîïòóôöùúûü";
    
    int index = (accents - strchr(accents,c)) / 2;
    int accentTypeIndex = index % 4;
    printAccent(accentTypeIndex);

	// Check which letter
    int letterIndex = floor(index / 4);
    char letter = getAccentLetter(letterIndex);
    textChar(letter);
    
    return true; // There should be no pb printing accents
}

void printAccent(int index) {
  switch (index) {
    case (0) :
      specialChar(SPE_CHAR_GRAVE);
      break;
    case (1) :
      specialChar(SPE_CHAR_ACUTE);
      break;
    case (2) :
      specialChar(SPE_CHAR_CIRCUMFLEX);
      break;
    default :
      specialChar(SPE_CHAR_UMLAUT);
  }
}

char getAccentLetter(int letterIndex) {
  switch (letterIndex) {
    case (0) :
      return ('a');
      break;
    case (1) :
      return ('e');
      break;
    case (2) :
      return ('i');
      break;
    case (3) :
      return ('o');
      break;
    default :
      return ('u');
  }
}


//
//
// REPEAT CHARACTER
//
//

void repeat(byte n) {
  serialprint7(18);
  serialprint7(64 + n);
}

//
//
// COLOR MANAGEMENT
//
//


void bgColor(byte c) {
  if (c <= 7) {
    serialprint7(27);
    serialprint7(c + 80);
    _currentBgColor = c;
  }
}


void textColor(byte c) {
  if (c <= 7) {
    serialprint7(27);
    serialprint7(c + 64);
    _currentTextColor = c;
  }
}


void useDefaultColors(void) {
  bgColor(BLACK);
  textColor(WHITE);
}

//
//
// MOVING AND POSITIONNING THE CURSOR
//
//

void moveCursorTo(byte x, byte y) {
  serialprint7(31); // Code positionnement de curseur
  serialprint7(64 + y); // coordonnées x (x+64) (x de 1 à 40)
  serialprint7(64 + x); // coordonnées y (y+64) (y de 1 à 24)
  refreshSettings();
}


void moveCursorToLocation(byte location) {
  if (location == HOME || location == LINE_END || location == TOP_LEFT) {
    serialprint7(location);
  }
  else if (location == CENTER || location == TOP_RIGHT || location == BOTTOM_RIGHT || location == BOTTOM_LEFT) {
    if (location == CENTER) {
      moveCursorTo(20, 12);
    }
    else if (location == TOP_RIGHT) {
      moveCursorTo(40, 1);
    }
    else if (location == BOTTOM_RIGHT) {
      moveCursorTo(40, 24);
    }
    else if (location == BOTTOM_LEFT) {
      moveCursorTo(1, 24);
    }
    refreshSettings()  ;
  }
}


void moveCursor(byte dir, int n) {
  if (dir == LEFT || dir == RIGHT || dir == UP || dir == DOWN) {
    for (int i = 0; i < n; i++) {
      serialprint7(dir);
    }
  }
}

//
//
// RESTORING THE CURRENT SETTINGS AS
// SOME COMMANDS SEEM TO RESET THEM
//
//

void refreshSettings(void) {
  // Common parameters
  serialprint7(_currentMode);
  textColor(_currentTextColor);
  bgColor(_currentBgColor); // Only in graphic mode ?
  blink(_currentBlink);
  cursor(_currentShowCursor);
  // Graphic mode specific parameters
  if (_currentMode == GRAPHIC_MODE) {
    pixelate(_currentUnderline);
  }
  // Text mode specific parameters
  if (_currentMode == TEXT_MODE) {
    video(_currentVideo);
    charSize(_currentSize);
  }
}


//
//
// SHOW / HIDE CURSOR
//
//


void cursor(boolean b) {
  if (b) {
    serialprint7(CURSOR_SHOW);
  }
  else {
    serialprint7(CURSOR_HIDE);
  }
  _currentShowCursor = b;
}

//
//
// CLEANING SYSTEM
//
//


void clearScreen(void) {
  serialprint7(CLEARSCREEN);
  refreshSettings();
}

//
//
// TEXT OR GRAPHIC MODE SELECTION
//
//

void mode(byte mode) {
  if (mode == GRAPHIC_MODE || mode == TEXT_MODE) {
    _currentMode = mode;
    refreshSettings();
  }
}

void graphicMode(void) {
  mode(GRAPHIC_MODE);
}

void textMode(void) {
  mode(TEXT_MODE);
}

//
//
//
//
//


void blink(boolean b) {
  serialprint7(27);
  if (b) {
    serialprint7(BLINK_ON);
  }
  else {
    serialprint7(BLINK_OFF);
  }
  _currentBlink = b;
}


void charSize(byte type) {
  if (type == SIZE_NORMAL || type == SIZE_DOUBLE_HEIGHT || type == SIZE_DOUBLE_WIDTH || type == SIZE_DOUBLE) {
    serialprint7(27);
    serialprint7(type);
    _currentSize = type;
  }
}


void incrustation(boolean b) {
  serialprint7(27);
  if (b) {
    serialprint7(INCRUSTATION_ON);
  }
  else {
    serialprint7(INCRUSTATION_OFF);
  }
}


void pixelate(boolean b) {
  serialprint7(27);
  if (b) {
    serialprint7(UNDERLINE_ON);
  }
  else {
    serialprint7(UNDERLINE_OFF);
  }
  _currentUnderline = b;
}


void lineMask(boolean b) {
  serialprint7(27);
  if (b) {
    serialprint7(LINE_MASK_ON);
  }
  else {
    serialprint7(LINE_MASK_OFF);
  }
}

void video(byte v) {
  if (v == VIDEO_INVERT || v == VIDEO_STANDARD || v == VIDEO_TRANSPARENT) {
    serialprint7(27);
    serialprint7(v);
    _currentVideo = v;
  }
}

void standardVideo(void) {
  video(VIDEO_STANDARD);
}

void invertVideo(void) {
  video(VIDEO_INVERT);
}

void transparentVideo(void) {
  video(VIDEO_TRANSPARENT);
}


void setMaxSpeed(void) {
  /*
    serialprint7(27);
    serialprint7(SPEED_4800);
  */
}

//
//
// SOUND
//
//

// Less than 200ms isn't taken into account
/*void bip(unsigned long duration) {
  unsigned long beginTime = millis();
  while (millis() < beginTime + duration) {
    serialprint7(27);
    serialprint7(BIP);
    delay(100);
  }
}*/

//
//
// KEYSTROKES ANALYSIS AND LOGGING
//
//

/*
	Read and decode keyboard input and store values in according variables
	_specialCharacterKey if a special character Jeu G2, schema 2.8 p103
	_characterKey if a normal character Jeu G0, schema 2.5 p 100
	_menuKey is a menu key
*/

void readKey(void) {

	_menuKey = -1;
	_specialCharacterKey = -1;
	_characterKey = -1;


  byte b = 255;
  //b =  read();

  // Menu keys start with 147 + another number
  if (b == 147) {

	_accentKey = -1; // Drop previously set accent
	xtimer_usleep(50); // Wait a bit
    //_menuKey = read(); // Read the next byte
    
  }
  
  // Shift or Ctrl key with GP2 character set start with 153
  else if (b == 153) {
	 _accentKey = -1; // Drop previously set accent
	 
	  xtimer_usleep(50); // Wait a bit
      //b = read(); // Read the next byte

	  if (b == 65 || b == 66 || b == 72 || b == 195) { // Accent key
	  	_accentKey = b % 128;
	  	return;	
	  }
	  
	  else {
	  	_specialCharacterKey = b % 128;
	  
		  if (b == 75) { // Special case for the ç
		  	xtimer_usleep(50); // Wait a bit
	  		//b = read(); // Read the next byte
		  	if (b == 99) {
		  		_specialCharacterKey = 75; // Implicit cedil with implicit c
	  		}
		  }
	  }
  }
  
  // Non prefixed keys
  else if (b != 255) {
	_characterKey = b % 128;
	
	// If an accent key was pressed before check if character can have this an accent
	if (_characterKey ==  97 || _characterKey ==  101 || _characterKey == 105 || _characterKey == 111 || _characterKey == 117 ) {
	
		// Remove accents if not supported by this letter
		if (_characterKey == 97 && _accentKey == SPE_CHAR_ACUTE ) { // a
			_accentKey = -1;
		}
		else if (_characterKey == 105 && (_accentKey == SPE_CHAR_GRAVE || _accentKey == SPE_CHAR_ACUTE )) { // i
			_accentKey = -1;
		}
		else if (_characterKey == 111 && (_accentKey == SPE_CHAR_GRAVE || _accentKey == SPE_CHAR_ACUTE )) { // o
			_accentKey = -1;
		}
		else if (_characterKey == 117 && _accentKey == SPE_CHAR_ACUTE ) { // u
			_accentKey = -1;
		}
	}
	else {
		_accentKey = -1;
	}
  }
}


//
//
// KEYS GETTERS
//
//

boolean keyTyped(void) {
	return isMenuKey() || isCharacterKey() || isSpecialCharacterKey() || accentKeyStored() ;
}

boolean isMenuKey(void) {
	return _menuKey != -1;
}

int getMenuKey(void) {
	return _menuKey;
}

boolean isSpecialCharacterKey(void) {
	return _specialCharacterKey != -1;
}

int getSpecialCharacterKey(void) {
	return _specialCharacterKey;
}

boolean isCharacterKey(void) {
	return _characterKey != -1;
}

char getCharacterKey(void) {
	return _characterKey;
}

boolean accentKeyStored(void) {
	return _accentKey != -1;
}

int getAccentKey(void) {
	return _accentKey;
}

//
//
// DRAWING FUNCTIONS
//
//

void rect_char(char c, int x, int y, int w, int h) {
  byte b = getCharByte(c);
  rect(b, x, y, w, h);
}

void rect(byte c, int x, int y, int w, int h) {
  moveCursorTo(x, y);
  textByte(c);
  repeat(w);
  moveCursorTo(x, y + 1);
  for (int i = 0; i < h - 2; i++) {
    textByte(c);
    moveCursor(DOWN,1);
    moveCursor(LEFT,1);
  }
  moveCursorTo(x + w, y + 1);
  for (int i = 0; i < h - 2; i++) {
    textByte(c);
    moveCursor(DOWN,1);
    moveCursor(LEFT,1);
  }
  moveCursorTo(x, y + h - 1);
  textByte(c);
  repeat(w);
}

void spiral(int x, int y, int siz, int c) {
  int curSiz = 1;
  // Center
  moveCursorTo(x, y);
  specialChar(c);
  x++;
  // Spiral
  for (int i = 0; i < siz; i++) {
    for (int j = 0; j < curSiz; j++) {
	  moveCursorTo(x, y);
      specialChar(c);
      y++;
    }
    curSiz++;
    for (int j = 0; j < curSiz; j++) {
	  moveCursorTo(x, y);
      specialChar(c);
      x--;
    }
    for (int j = 0; j < curSiz; j++) {
	  moveCursorTo(x, y);
      specialChar(c);
      y--;
    }
    curSiz++;
    for (int j = 0; j < curSiz; j++) {
	  moveCursorTo(x, y);
      specialChar(c);
      x++;
    }
  }
}

