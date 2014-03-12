/*************************************************************************
 *
 *    Driver for the Nokia 3310 LCD
 *
 **************************************************************************/
//#include "includes.h"
#include "gpio_lib.h"
#include "lcd3310.h"


//GPIO 3
#define PIN_PH0		SUNXI_GPH(0)
#define PIN_PB3		SUNXI_GPB(3)
#define PIN_PH2		SUNXI_GPH(2)
#define PIN_PB4		SUNXI_GPB(4)
#define PIN_PH7		SUNXI_GPH(7)
#define PIN_PB5		SUNXI_GPB(5)
#define PIN_PH9		SUNXI_GPH(9)
#define PIN_PB6		SUNXI_GPB(6)
#define PIN_PH10	SUNXI_GPH(10)
#define PIN_PB7		SUNXI_GPB(7)
#define PIN_PH11	SUNXI_GPH(11)
#define PIN_PB8		SUNXI_GPB(8)
#define PIN_PH12	SUNXI_GPH(12)
#define PIN_PB10	SUNXI_GPB(10)
#define PIN_PH13	SUNXI_GPH(13)
#define PIN_PB11	SUNXI_GPB(11)
#define PIN_PH14	SUNXI_GPH(14)
#define PIN_PB12	SUNXI_GPB(12)
#define PIN_PH15	SUNXI_GPH(15)
#define PIN_PB13	SUNXI_GPB(13)
#define PIN_PH16	SUNXI_GPH(16)
#define PIN_PB14	SUNXI_GPB(14)
#define PIN_PH17	SUNXI_GPH(17)
#define PIN_PB15	SUNXI_GPB(15)
#define PIN_PH18	SUNXI_GPH(18)
#define PIN_PB16	SUNXI_GPB(16)
#define PIN_PH19	SUNXI_GPH(19)
#define PIN_PB17	SUNXI_GPB(17)
#define PIN_PH20	SUNXI_GPH(20)
#define PIN_PH24	SUNXI_GPH(24)
#define PIN_PH21	SUNXI_GPH(21)
#define PIN_PH25	SUNXI_GPH(25)
#define PIN_PH22	SUNXI_GPH(22)
#define PIN_PH26	SUNXI_GPH(26)
#define PIN_PH23	SUNXI_GPH(23)
#define PIN_PH27	SUNXI_GPH(27)

/* TO IMPLEMENT YOUR VERSION OF THE DRIVER YOU'LL HAVE TO EDIT THIS SECTION ONLY */
/*
# default PINs, BCM GPIO
#pin_CLK   = GPIO.PIN2_6 # Orange = PIN_PE0
#pin_DIN   = GPIO.PIN2_8 # Gelb = PIN_PE1
#pin_DC    = GPIO.PIN2_10 # Gr�n = PIN_PE2
#pin_CS    = GPIO.PIN2_12 # Blau = PIN_PE3
#pin_LIGHT = GPIO.PIN2_14 # Wei� = PIN_PE4
#pin_RST   = GPIO.PIN2_16 # Braun = PIN_PE5


*/




#define PIN_CLK PIN_PE0
#define PIN_MOSI PIN_PE1
#define PIN_DC PIN_PE2
#define PIN_CS PIN_PE3
#define PIN_LIGHT PIN_PE4
#define PIN_RESET PIN_PE5




#define PIN_OUTPUT 1
void LCD_CS_MAKE_OUT(){
    sunxi_gpio_set_cfgpin(PIN_CS, PIN_OUTPUT);	
}
void LCD_DC_MAKE_OUT(){
	sunxi_gpio_set_cfgpin(PIN_DC, PIN_OUTPUT);		
}
void LCD_RES_MAKE_OUT(){		
	sunxi_gpio_set_cfgpin(PIN_RESET, PIN_OUTPUT);
}

void LCD_CS_HIGH(){
	sunxi_gpio_output(PIN_CS, 1);
}
void LCD_CS_LOW(){
	sunxi_gpio_output(PIN_CS, 0);
}

void LCD_RES_HIGH(){
	sunxi_gpio_output(PIN_RESET, 1);
}
void LCD_RES_LOW(){
	sunxi_gpio_output(PIN_RESET, 0);
}

void LCD_DC_HIGH()	{
	sunxi_gpio_output(PIN_DC, 1);
}
void LCD_DC_LOW(){
	sunxi_gpio_output(PIN_DC, 0);
}
/*
#define LCD_CS_MAKE_OUT()		LCD3310_CS_DIR |= LCD3310_CS_MASK
#define LCD_CS_HIGH()  			PIN_HIGH(LCD3310_CS_OUT)
#define LCD_CS_LOW()  			PIN_LOW(LCD3310_CS_OUT)

#define LCD_RES_MAKE_OUT()		LCD3310_RST_DIR |= LCD3310_RST_MASK
#define LCD_RES_HIGH()			PIN_HIGH(LCD3310_RST_OUT)
#define LCD_RES_LOW()			PIN_LOW(LCD3310_RST_OUT)

#define LCD_DC_MAKE_OUT()		LCD3310_CD_DIR |= LCD3310_CD_MASK
#define LCD_DC_HIGH()			PIN_HIGH(LCD3310_CD_OUT)
#define LCD_DC_LOW()			PIN_LOW(LCD3310_CD_OUT)
*/
void SEND_BYTE_SPI(unsigned char data) 					
	{	
		int i = 0;
		for(i = 0; i < 8; i++) {		
			if(data & 0x80) {				
				sunxi_gpio_output(PIN_MOSI, 1);
			} else { 						
				sunxi_gpio_output(PIN_MOSI, 0);
			}								
			Delay(10);					
			sunxi_gpio_output(PIN_CLK, 0);		
			Delay(10);						
			sunxi_gpio_output(PIN_CLK, 1);
											
			data <<= 1;						
		}

}
	
	


static void Initialize_SPI(void)
{
	// Initialie SPI Interface pins as GPIOs
	//LCD3310_MOSI_DIR |= LCD3310_MOSI_MASK;
	//LCD3310_SCK_DIR |= LCD3310_SCK_MASK;
	
	
    int result = sunxi_gpio_init();
	sunxi_gpio_set_cfgpin(PIN_CLK, PIN_OUTPUT);	
	sunxi_gpio_set_cfgpin(PIN_MOSI, PIN_OUTPUT);	
	sunxi_gpio_output(PIN_CLK, 1);
	sunxi_gpio_output(PIN_MOSI, 1);	
	//PIN_HIGH(LCD3310_SCK_OUT);
	//PIN_HIGH(LCD3310_MOSI_OUT);
}

/* END OF SECTION */


#define LCD_START_LINE_ADDR	(66-2)

#if LCD_START_LINE_ADDR	> 66
  #error "Invalid LCD starting line address"
#endif

// LCD memory index
unsigned int  LcdMemIdx;

// represent LCD matrix
unsigned char  LcdMemory[LCD_CACHE_SIZE];

const unsigned char  FontLookup [][5] =
{
    { 0x00, 0x00, 0x00, 0x00, 0x00 },   // sp
    { 0x00, 0x00, 0x2f, 0x00, 0x00 },   // !
    { 0x00, 0x07, 0x00, 0x07, 0x00 },   // "
    { 0x14, 0x7f, 0x14, 0x7f, 0x14 },   // #
    { 0x24, 0x2a, 0x7f, 0x2a, 0x12 },   // $
    { 0xc4, 0xc8, 0x10, 0x26, 0x46 },   // %
    { 0x36, 0x49, 0x55, 0x22, 0x50 },   // &
    { 0x00, 0x05, 0x03, 0x00, 0x00 },   // '
    { 0x00, 0x1c, 0x22, 0x41, 0x00 },   // (
    { 0x00, 0x41, 0x22, 0x1c, 0x00 },   // )
    { 0x14, 0x08, 0x3E, 0x08, 0x14 },   // *
    { 0x08, 0x08, 0x3E, 0x08, 0x08 },   // +
    { 0x00, 0x00, 0x50, 0x30, 0x00 },   // ,
    { 0x10, 0x10, 0x10, 0x10, 0x10 },   // -
    { 0x00, 0x60, 0x60, 0x00, 0x00 },   // .
    { 0x20, 0x10, 0x08, 0x04, 0x02 },   // /
    { 0x3E, 0x51, 0x49, 0x45, 0x3E },   // 0
    { 0x00, 0x42, 0x7F, 0x40, 0x00 },   // 1
    { 0x42, 0x61, 0x51, 0x49, 0x46 },   // 2
    { 0x21, 0x41, 0x45, 0x4B, 0x31 },   // 3
    { 0x18, 0x14, 0x12, 0x7F, 0x10 },   // 4
    { 0x27, 0x45, 0x45, 0x45, 0x39 },   // 5
    { 0x3C, 0x4A, 0x49, 0x49, 0x30 },   // 6
    { 0x01, 0x71, 0x09, 0x05, 0x03 },   // 7
    { 0x36, 0x49, 0x49, 0x49, 0x36 },   // 8
    { 0x06, 0x49, 0x49, 0x29, 0x1E },   // 9
    { 0x00, 0x36, 0x36, 0x00, 0x00 },   // :
    { 0x00, 0x56, 0x36, 0x00, 0x00 },   // ;
    { 0x08, 0x14, 0x22, 0x41, 0x00 },   // <
    { 0x14, 0x14, 0x14, 0x14, 0x14 },   // =
    { 0x00, 0x41, 0x22, 0x14, 0x08 },   // >
    { 0x02, 0x01, 0x51, 0x09, 0x06 },   // ?
    { 0x32, 0x49, 0x59, 0x51, 0x3E },   // @
    { 0x7E, 0x11, 0x11, 0x11, 0x7E },   // A
    { 0x7F, 0x49, 0x49, 0x49, 0x36 },   // B
    { 0x3E, 0x41, 0x41, 0x41, 0x22 },   // C
    { 0x7F, 0x41, 0x41, 0x22, 0x1C },   // D
    { 0x7F, 0x49, 0x49, 0x49, 0x41 },   // E
    { 0x7F, 0x09, 0x09, 0x09, 0x01 },   // F
    { 0x3E, 0x41, 0x49, 0x49, 0x7A },   // G
    { 0x7F, 0x08, 0x08, 0x08, 0x7F },   // H
    { 0x00, 0x41, 0x7F, 0x41, 0x00 },   // I
    { 0x20, 0x40, 0x41, 0x3F, 0x01 },   // J
    { 0x7F, 0x08, 0x14, 0x22, 0x41 },   // K
    { 0x7F, 0x40, 0x40, 0x40, 0x40 },   // L
    { 0x7F, 0x02, 0x0C, 0x02, 0x7F },   // M
    { 0x7F, 0x04, 0x08, 0x10, 0x7F },   // N
    { 0x3E, 0x41, 0x41, 0x41, 0x3E },   // O
    { 0x7F, 0x09, 0x09, 0x09, 0x06 },   // P
    { 0x3E, 0x41, 0x51, 0x21, 0x5E },   // Q
    { 0x7F, 0x09, 0x19, 0x29, 0x46 },   // R
    { 0x46, 0x49, 0x49, 0x49, 0x31 },   // S
    { 0x01, 0x01, 0x7F, 0x01, 0x01 },   // T
    { 0x3F, 0x40, 0x40, 0x40, 0x3F },   // U
    { 0x1F, 0x20, 0x40, 0x20, 0x1F },   // V
    { 0x3F, 0x40, 0x38, 0x40, 0x3F },   // W
    { 0x63, 0x14, 0x08, 0x14, 0x63 },   // X
    { 0x07, 0x08, 0x70, 0x08, 0x07 },   // Y
    { 0x61, 0x51, 0x49, 0x45, 0x43 },   // Z
    { 0x00, 0x7F, 0x41, 0x41, 0x00 },   // [
	{ 0x55, 0x2A, 0x55, 0x2A, 0x55 },   // 55
    { 0x00, 0x41, 0x41, 0x7F, 0x00 },   // ]
    { 0x04, 0x02, 0x01, 0x02, 0x04 },   // ^
    { 0x40, 0x40, 0x40, 0x40, 0x40 },   // _
    { 0x00, 0x01, 0x02, 0x04, 0x00 },   // '
    { 0x20, 0x54, 0x54, 0x54, 0x78 },   // a
    { 0x7F, 0x48, 0x44, 0x44, 0x38 },   // b
    { 0x38, 0x44, 0x44, 0x44, 0x20 },   // c
    { 0x38, 0x44, 0x44, 0x48, 0x7F },   // d
    { 0x38, 0x54, 0x54, 0x54, 0x18 },   // e
    { 0x08, 0x7E, 0x09, 0x01, 0x02 },   // f
    { 0x0C, 0x52, 0x52, 0x52, 0x3E },   // g
    { 0x7F, 0x08, 0x04, 0x04, 0x78 },   // h
    { 0x00, 0x44, 0x7D, 0x40, 0x00 },   // i
    { 0x20, 0x40, 0x44, 0x3D, 0x00 },   // j
    { 0x7F, 0x10, 0x28, 0x44, 0x00 },   // k
    { 0x00, 0x41, 0x7F, 0x40, 0x00 },   // l
    { 0x7C, 0x04, 0x18, 0x04, 0x78 },   // m
    { 0x7C, 0x08, 0x04, 0x04, 0x78 },   // n
    { 0x38, 0x44, 0x44, 0x44, 0x38 },   // o
    { 0x7C, 0x14, 0x14, 0x14, 0x08 },   // p
    { 0x08, 0x14, 0x14, 0x18, 0x7C },   // q
    { 0x7C, 0x08, 0x04, 0x04, 0x08 },   // r
    { 0x48, 0x54, 0x54, 0x54, 0x20 },   // s
    { 0x04, 0x3F, 0x44, 0x40, 0x20 },   // t
    { 0x3C, 0x40, 0x40, 0x20, 0x7C },   // u
    { 0x1C, 0x20, 0x40, 0x20, 0x1C },   // v
    { 0x3C, 0x40, 0x30, 0x40, 0x3C },   // w
    { 0x44, 0x28, 0x10, 0x28, 0x44 },   // x
    { 0x0C, 0x50, 0x50, 0x50, 0x3C },   // y
    { 0x44, 0x64, 0x54, 0x4C, 0x44 },   // z
    { 0x08, 0x6C, 0x6A, 0x19, 0x08 },   // { (gramotevichka)
    { 0x0C, 0x12, 0x24, 0x12, 0x0C },   // | (sarce)
    { 0x7E, 0x7E, 0x7E, 0x7E, 0x7E },    // kvadratche
};


// simple delays
void Delay(volatile unsigned long a) { while (a!=0) a--; }

void Delayc(volatile unsigned char a) { while (a!=0) a--; }

/****************************************************************************/
/*  Init LCD Controler                                                      */
/*  Function : LCDInit                                                      */
/*      Parameters                                                          */
/*          Input   :  Nothing                                              */
/*          Output  :  Nothing                                              */
/****************************************************************************/
void LCDInit(void)
{  	
	// Initialie SPI Interface
    Initialize_SPI();
	
	// set pin directions
	LCD_CS_MAKE_OUT();
	LCD_CS_HIGH();
	LCD_DC_MAKE_OUT();
	LCD_RES_MAKE_OUT();
	
	// Toggle reset pin
	LCD_RES_LOW();
	Delay(1000);
	LCD_RES_HIGH();		
	Delay(1000);	
	
	// Send sequence of command
	LCDSend( 0x21, SEND_CMD );  // LCD Extended Commands.
	LCDSend( 0xC8, SEND_CMD );  // Set LCD Vop (Contrast). 0xC8
	LCDSend( 0x04 | !!(LCD_START_LINE_ADDR&(1u<<6)), SEND_CMD );  // Set Temp S6 for start line
	LCDSend( 0x40 | (LCD_START_LINE_ADDR & ((1u<<6)-1)), SEND_CMD );  // Set Temp S[5:0] for start line
	//LCDSend( 0x13, SEND_CMD );  // LCD bias mode 1:48.
	LCDSend( 0x12, SEND_CMD );  // LCD bias mode 1:68.
	LCDSend( 0x20, SEND_CMD );  // LCD Standard Commands, Horizontal addressing mode.
	//LCDSend( 0x22, SEND_CMD );  // LCD Standard Commands, Vertical addressing mode.
	LCDSend( 0x08, SEND_CMD );  // LCD blank
	LCDSend( 0x0C, SEND_CMD );  // LCD in normal mode.
	
	// Clear and Update
	LCDClear();
	LCDUpdate();
}

/****************************************************************************/
/*  Reset LCD 	                                                            */
/*  Function : LCDReset                                                     */
/*      Parameters                                                          */
/*          Input   :  Resets the LCD module		                        */
/*          Output  :  Nothing                                              */
/****************************************************************************/
void LCDReset(void)
{

	// Close SPI module - optional
	// NOT DONE
	
	LCD_RES_LOW();
}

/****************************************************************************/
/*  Update LCD                                                              */
/*  Function : LCDUpdate                                                    */
/*      Parameters                                                          */
/*          Input   :  sends buffer data to the LCD                         */
/*          Output  :  Nothing                                              */
/****************************************************************************/
void LCDUpdate ( void )
{
	int x,y;
	
	for(y=0; y<48 / 8; y++) {
		LCDSend(0x80, SEND_CMD );
		LCDSend(0x40 | y, SEND_CMD );	
		for(x=0; x < 84; x++) {
			LCDSend( LcdMemory[y * 84 + x], SEND_CHR );
		}	
	}
}
/****************************************************************************/
/*  Send to LCD                                                             */
/*  Function : LCDSend                                                      */
/*      Parameters                                                          */
/*          Input   :  data and  SEND_CHR or SEND_CMD                       */
/*          Output  :  Nothing                                              */
/****************************************************************************/
void LCDSend(unsigned char data, unsigned char cd)
{
	LCD_CS_LOW();

	if(cd == SEND_CHR) {
    	LCD_DC_HIGH();
  	}
  	else {
    	LCD_DC_LOW();
  	}

	// send data over SPI
	SEND_BYTE_SPI(data);
		
	LCD_CS_HIGH();
}


/****************************************************************************/
/*  Clear LCD                                                               */
/*  Function : LCDClear                                                     */
/*      Parameters                                                          */
/*          Input   :  Nothing                                              */
/*          Output  :  Nothing                                              */
/****************************************************************************/
void LCDClear(void)
{
	int i;
	
	// loop all cashe array
	for (i=0; i<LCD_CACHE_SIZE; i++) {
		LcdMemory[i] = 0x00;
	}
}

/****************************************************************************/
/*  Write char at x position on y row                                       */
/*  Function : LCDChrXY                                                     */
/*      Parameters                                                          */
/*          Input   :  pos, row, char                                       */
/*          Output  :  Nothing                                              */
/****************************************************************************/
void LCDChrXY (unsigned char x, unsigned char y, unsigned char ch )
{
    unsigned int    index   = 0;
    unsigned int    offset  = 0;
    unsigned int    i       = 0;

    // check for out off range
    if ( x > LCD_X_RES ) return;
    if ( y > LCD_Y_RES ) return;

	index=(unsigned int)x*5+(unsigned int)y*84;

    for ( i = 0; i < 5; i++ )
    {
	  offset = FontLookup[ch - 32][i];
	  LcdMemory[index] = offset;
      index++;
    }

}

/****************************************************************************/
/*  Write negative char at x position on y row                              */
/*  Function : LCDChrXYInverse                                              */
/*      Parameters                                                          */
/*          Input   :  pos, row, char                                       */
/*          Output  :  Nothing                                              */
/****************************************************************************/
void LCDChrXYInverse (unsigned char x, unsigned char y, unsigned char ch )
{
	unsigned int    index   = 0;
    unsigned int    i       = 0;

    // check for out off range
    if ( x > LCD_X_RES ) return;
    if ( y > LCD_Y_RES ) return;

	index=(unsigned int)x*5+(unsigned int)y*84;

    for ( i = 0; i < 5; i++ )
    {
      LcdMemory[index] = ~(FontLookup[ch - 32][i]);
      index++;
    }

}

/****************************************************************************/
/*  Set LCD Contrast                                                        */
/*  Function : LcdContrast                                                  */
/*      Parameters                                                          */
/*          Input   :  contrast                                             */
/*          Output  :  Nothing                                              */
/****************************************************************************/
void LCDContrast(unsigned char contrast) {

    //  LCD Extended Commands.
    LCDSend( 0x21, SEND_CMD );

    // Set LCD Vop (Contrast).
    LCDSend( 0x80 | contrast, SEND_CMD );

    //  LCD Standard Commands, horizontal addressing mode.
    LCDSend( 0x20, SEND_CMD );
}


/****************************************************************************/
/*  Send string to LCD                                                      */
/*  Function : LCDStr                                                       */
/*      Parameters                                                          */
/*          Input   :  row, text, inversion                                 */
/*          Output  :  Nothing                                              */
/****************************************************************************/
void LCDStr(unsigned char row, const unsigned char *dataPtr, unsigned char inv ) {

	// variable for X coordinate
	unsigned char x = 0;
	
	// loop to the and of string
	while ( *dataPtr ) {
		if(inv) {
			LCDChrXYInverse(x, row, *dataPtr);
		} else {
			LCDChrXY( x, row, *dataPtr);
		}	
		x++;
		dataPtr++;
	}
	
	LCDUpdate();
}


