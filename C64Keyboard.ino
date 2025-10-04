/*

    Arduino Commodore C64 Keyboard as an extended ASCII keyboard 
    Laurent FAVARD

    Based on DJ Sures (Synthiam.com) (c)2019, Chris Garrett 

    About Arduino Pro Micro as Arduino Leonardo
    https://learn.sparkfun.com/tutorials/pro-micro--fio-v3-hookup-guide/all


    Select Board: "Leonardo"
    Windows: Serial port: COM7, or...COM5 depend of the board
*/

//  C64 KeyBoard            Arduino Pro Micro      				Socket 24 pins
//  Pin           			Pin logical (Not DIL socket)
//  (Rows=)			
//			
//   20           			2 - SDA
//   19           			3 - SCL
//   18           			4 - A6
//   17           			5
//   16           			6 - A7
//   15           			7 -
//   14           			8 - A8
//   13           			9 - A9

//  (Columns)
//   12           			10 - A10
//   11           			16 - MOSI
//   10           			14 - MISO
//   9            			15 - SCLK
//   8            			18 - A0
//   7            			19 - A1
//   6            			20 - A2
//   5            			21 - A3
//   4- 5V        			Not connected
//   3- /RESTORE
//   2- NO PIN  
//   1- GND       			GND


#define     _DEBUG                  _DEBUG

#define		COMMODORE_SHIFT_LEFT 	17
#define		COMMODORE_SHIFT_RIGHT 	64
#define		COMMODORE_CTRL       	72

#define     KEYS_COUNT              80
#define     KEYS_ROW_COUNT          8
#define     KEYS_COL_COUNT          10

#define     K_NULL                  0x00
#define     K_BS                    0x08
#define     K_RET                   0x0D
#define     K_ESC                   0x1B
#define     K_DEL                   0x7F

#define     K_COMMODORE             0x80
#define     K_RUNSTOP               0x81
#define     K_CLRHOME               0x82
#define     K_F1                    0x83
#define     K_F2                    0x84
#define     K_F3                    0x85
#define     K_F4                    0x86
#define     K_F5                    0x87
#define     K_F6                    0x88
#define     K_F7                    0x89
#define     K_F8                    0x90
#define     K_PI                    0x91

int       	lastKeyState[KEYS_COUNT];
long      	lastDebounceTime[KEYS_COUNT];
int       	debounceDelay = 50;

int       	RowPinMap[KEYS_ROW_COUNT] = {9, 3, 4, 5, 6, 7, 8, 2};
int       	ColPinMap[KEYS_COL_COUNT] = {10, 16, 14, 21, 18, 19, 20, 15, 1, 0};

typedef struct {
	uint16_t  	stdChar;
	uint16_t	shiftedChar;
    uint16_t	ctrlChar;
    uint16_t	shiftedCtrlChar;
} KeyCharMap;

KeyCharMap keysCharMap[KEYS_COUNT] = {

    // Std          Shifted         Ctrl
    {K_DEL,         K_DEL,          K_NULL,         K_NULL},                    //  0-  INST DEL 	
    {K_RET,         K_NULL,         K_NULL,         K_NULL},                    //  1-  RETURN 	
    {K_NULL,        K_NULL,         K_NULL,         K_NULL},                    //  2-  CURSOR LEFT/RIGHT
    {K_F7,          K_NULL,         K_F8,           K_NULL},                    //  3-  F7
    {K_F1,          K_NULL,         K_F2,           K_NULL},                    //  4-  F1
    {K_F3,          K_NULL,         K_F4,           K_NULL},	                //  5-  F3
    {K_F5,          K_NULL,         K_F6,           K_NULL},	                //  6-  F5
    {K_NULL,        K_NULL,         K_NULL,         K_NULL},                    //  7-  CURSOR UP/DOWN
    {K_NULL,        K_NULL,         K_NULL,         K_NULL},	                //  8-  Null
    {K_NULL,        K_NULL,         K_NULL,         K_NULL},                    //  9-  Null


    {'3',           '#',            K_NULL,         K_NULL},                    //  10- 3 #
    {'w',           'W',            K_NULL,         K_NULL},	                //  11- W
    {'a',           'A',            K_NULL,         K_NULL},	                //  12- A
    {'4',           '$',            K_NULL,         K_NULL},	                //  13- 4 $
    {'z',           'Z',            K_NULL,         K_NULL},	                //  14- Z
    {'s',           'S',            K_NULL,         K_NULL},	                //  15- S
    {'e',           'E',            K_NULL,         K_NULL},	                //  19- E
    {K_NULL,        K_NULL,         K_NULL,         K_NULL},  	                //  17- LSHFT (17)
    {'2',           K_NULL,         K_NULL,         K_NULL},	                //  18- Joy2Down
    {'*',           K_NULL,         K_NULL,         K_NULL},                    //  19- Joy1Down


    {'5',           '%',            K_NULL,         K_NULL},                    //  20- 5 %                             
    {'r',           'R',            K_NULL,         K_NULL},                    //  21- R 	
    {'d',           'D',            K_NULL,         K_NULL},                    //  22- D  	
    {'6',           '&',            K_NULL,         '|'},                       //  23- 6 & 	
    {'c',           'C',            K_NULL,         K_NULL},                    //  24- C 	
    {'f',           'F',            K_NULL,         K_NULL},                    //  25- F  	
    {'t',           'T',            K_NULL,         K_NULL},                    //  26- T  	
    {'x',           'X',            K_NULL,         K_NULL},                    //  27- X  		
    {'4',           '4',            K_NULL,         K_NULL},                    //  28- Joy2Left	
    {'-',           '-',            K_NULL,         K_NULL},                    //  29- Joy1Left 

    
    {'7',           '\'',           K_NULL,         K_NULL},                    //  30- 7  	
    {'y',           'Y',            K_NULL,         K_NULL},                    //  31- Y  	
    {'g',           'G',            K_NULL,         K_NULL},                    //  32- G  	
    {'8',           '(',            K_NULL,         K_NULL},                    //  33- 8 (  	
    {'b',           'B',            K_NULL,         K_NULL},                    //  34- B  	
    {'h',           'H',            K_NULL,         K_NULL},                    //  35- H  	
    {'u',           'U',            K_NULL,         K_NULL},                    //  36- U  	
    {'v',           'V',            K_NULL,         K_NULL},                    //  37- V  		
    {'6',           K_NULL,         K_NULL,         K_NULL},                    //  38- Joy2Right  	
    {'+',           K_NULL,         K_NULL,         K_NULL},                    //  39- Joy1Right  

    
    {'9',           ')',            K_NULL,         '^'},	                    //  40- 9 ) 
    {'i',           'I',            K_NULL,         K_NULL},	                //  41- I  
    {'j',           'J',            K_NULL,         K_NULL},	                //  52- J  
    {'0',           '0',            K_NULL,         K_NULL},	                //  43- 0 Zero  
    {'m',           'M',            K_NULL,         K_NULL},	                //  44- M  
    {'k',           'K',            K_NULL,         K_NULL},	                //  45- K  
    {'o',           'O',            K_NULL,         K_NULL},	                //  46- O  
    {'n',           'N',            K_NULL,         K_NULL},		            //  47- N    
    {'5',           K_NULL,         K_NULL,         K_NULL},	                //  48- Joy2F1    
    {K_NULL,        K_NULL,         K_NULL,         K_NULL},                    //  49- Joy1F1  

    
    {'+',           K_NULL,         K_NULL,         K_NULL},                    //  50- +     
    {'p',           'P',            K_NULL,         K_NULL},                    //  51- P  
    {'l',           'L',            K_NULL,         K_NULL},                    //  52- L
    {'-',            K_NULL,        K_NULL,         K_NULL},                    //  53- -
    {'.',           '>',            K_NULL,         K_NULL},                    //  54- . >
    {':',           '[',            K_NULL,         K_NULL},                    //  55- : [
    {'@',            K_NULL,        K_NULL,         K_NULL},                    //  56- @
    {',',           '<',            K_NULL,         K_NULL},                    //  57- , <	
    {'3',           K_NULL,         K_NULL,         K_NULL},                    //  58- Joy2F2
    {'1',           K_NULL,         K_NULL,         K_NULL},                    //  59- Joy1F2

    
    {'$',           K_NULL,         K_NULL,         K_NULL},                    //  60- POUND (60)
    {'*',           K_NULL,         K_NULL,         K_NULL},                    //  61- *
    {';',           ']',            K_NULL,         K_NULL},                    //  62- ; ]  
    {K_CLRHOME,     K_NULL,         K_NULL,         K_NULL},                    //  63- CLR HOME (63) 
    {K_NULL,        K_NULL,         K_NULL,         K_NULL},                    //  64- RSHFT (64)  
    {'=',           '=',            K_NULL,         K_NULL},                    //  65- =   	    
    {K_PI,          K_NULL,         K_NULL,         K_NULL},                    //  66- ARROW ALONE UP/PI
    {'/',           '?',            K_NULL,         '\\'},                      //  67- ? /	    
    {K_NULL,        K_NULL,         K_NULL,         K_NULL},                    //  68- Restore	    
    {K_NULL,        K_NULL,         K_NULL,         K_NULL},                    //  69- Null

    
    {'1',           '!',            K_NULL,         K_NULL},                    //  70- 1 !
    {K_ESC,         K_NULL,         K_NULL,         K_NULL},                    //  71- ARROW ALONE TO LEFT
    {K_NULL,        K_NULL,         K_NULL,         K_NULL},                    //  72- CTRL (72)    	
    {'2',           '"',            K_NULL,         '~'},                       //  73- 2 "
    {' ',           K_NULL,         K_NULL,         K_NULL},                    //  74- SPC
    {K_COMMODORE,   K_NULL,         K_NULL,         K_NULL},                    //  75- C= COMMODE KEY (75)              
    {'q',           'Q',            K_NULL,         K_NULL},                    //  76- Q (76)	                
    {K_RUNSTOP,     K_NULL,         K_NULL,         K_NULL},                    //  77- RUN STOP              
    {'8',           K_NULL,         K_NULL,         K_NULL},                    //  78- Joy2Up
    {'\\',          K_NULL,         K_NULL,         K_NULL}                     //  79- Joy1Up
};

void setup() {

    Serial.begin(115200);
    delay(1000);

#ifdef  _DEBUG
    Serial.println("Commodore 64 keyboard");
    Serial.println("FAVARD Laurent");
#endif

    for (int i = 0; i < 80; i++) {
        lastKeyState[i] = false;
    }

    for (int Row = 0; Row < KEYS_ROW_COUNT; Row++) {
        pinMode(RowPinMap[Row], INPUT_PULLUP);
    }

    for (int Col = 0; Col < KEYS_COL_COUNT; Col++) {
        pinMode(ColPinMap[Col], INPUT_PULLUP);
    }
}

void loop() {

    for (int row = 0; row < KEYS_ROW_COUNT; row++) {

        int rowPin = RowPinMap[row];

        pinMode(rowPin, OUTPUT);
        digitalWrite(rowPin, LOW);

        for (int col = 0; col < KEYS_COL_COUNT; col++) {

            int  keyPos    = col + (row * 10);
            int  outChar   = NULL;
            bool isKeyDown = !digitalRead(ColPinMap[col]);

            if (millis() >= lastDebounceTime[keyPos] + debounceDelay) {

                // If no SHIFT key is pressed, normal keys
				if (!lastKeyState[COMMODORE_SHIFT_LEFT] && !lastKeyState[COMMODORE_SHIFT_RIGHT] && !lastKeyState[COMMODORE_CTRL]) {
					outChar   = keysCharMap[keyPos].stdChar;
				}
				// If SHIFT ONLY key is pressed, modify some keys
				else if((lastKeyState[COMMODORE_SHIFT_LEFT] || lastKeyState[COMMODORE_SHIFT_RIGHT]) && !lastKeyState[COMMODORE_CTRL]) {
					outChar   = keysCharMap[keyPos].shiftedChar;
				}
                // If CTRL key is pressed, modify some keys
				else if(lastKeyState[COMMODORE_CTRL] && !(lastKeyState[COMMODORE_SHIFT_LEFT] || lastKeyState[COMMODORE_SHIFT_RIGHT]) ) {
					outChar   = keysCharMap[keyPos].ctrlChar;
				}
                else if(lastKeyState[COMMODORE_CTRL] && (lastKeyState[COMMODORE_SHIFT_LEFT] || lastKeyState[COMMODORE_SHIFT_RIGHT]) ) {
					outChar   = keysCharMap[keyPos].shiftedCtrlChar;
				}

                if (isKeyDown && !lastKeyState[keyPos]) {

					lastKeyState[keyPos] = true;
					lastDebounceTime[keyPos] = millis();

                    if(outChar) {
#ifdef  _DEBUG                        
                        serialPrintDebug(col, row, keyPos, outChar);
#else
                        Serial.write(outChar);
#endif
                    }
                }

                if (!isKeyDown && lastKeyState[keyPos]) {

                    lastKeyState[keyPos] = false;
                    lastDebounceTime[keyPos] = millis();
                }
            }
        }

        digitalWrite(rowPin, HIGH);
        delay(1);
        pinMode(rowPin, INPUT_PULLUP);
    }
}

/*
    Display the key pressed with column, row, position inside keysCharMap array, Hexa code and printale char.
*/
void serialPrintDebug(int column, int row, int keyPos, unsigned outChar) {

    Serial.print("col: ");
    Serial.print(column);
    Serial.print(", row: ");
    Serial.print(row);
    Serial.print(", Position: ");
    Serial.print(keyPos);

    Serial.print(", Char: $");
    Serial.print(outChar, HEX);

    Serial.print(", ");                       
    Serial.write(outChar);
    
    Serial.println();
}
