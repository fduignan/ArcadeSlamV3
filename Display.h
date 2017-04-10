#ifndef __DISPLAY_H
#define __DISPLAY_H
#include <stdint.h>
#include "font5x7.h"

// April 2017: New I/O configuration to reduce wiring complexity
/*
 * DB7 to DB3  :  P1_1 to P1_5 (note reverse order)
 * DB2 to DB0  :  P2_0 to P2_2 (note reverse order)
 * WR to P2_5
 * RS to P2_4
 * CS to P2_3
 * RD to P1_7
 */
#define WR_PIN P2_5
#define RS_PIN P2_4
#define CS_PIN P2_3
#define RD_PIN P1_7
// Assumption: Top left of display is 0,0
// Screen is 240 wide and 320 high 
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 320
// Define a macro to allow easy definition of colours
// Format of colour value: <BGND 1 bit><Red 5 bits><Green 5 bits><Blue 5 bits>
#define RGBToWord( R,  G,  B)  (  ((R&0xf8) << (11-3)) | ((G&0xfc) << (5-2)) | ((B&0xf8)>>3) )
// Define the primary colours
#define BGND RGBToWord(0,0,0)
#define RED RGBToWord(0xff,0,0)
#define BLUE RGBToWord(0,0xff,0)
#define GRN RGBToWord(0,0,0xff)
#define WHI RGBToWord(0xff,0xff,0xff)

class Display {
public:
// It would have been nice to initialize the display in the contructor but this doesn't seem
// to work as system initialization (clocks, watchdog etc) happens AFTER constructor initialization
// A "begin" function must be called during setup to start the display up properly
  Display() {};  
  void begin();
  void putPixel(uint16_t x, uint16_t y, uint16_t colour);
  void putImage(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint16_t *Image);    
  void fillRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t Colour);
  void putText(const char *Text, uint16_t len, uint16_t x, uint16_t y, uint16_t ForeColour, uint16_t BackColour);
  void putNumber(uint16_t Number, uint16_t x, uint16_t y, uint16_t ForeColour, uint16_t BackColour);
  
private:
  void gotoXY(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);
  uint16_t TextBox[FONT_WIDTH*FONT_HEIGHT];
  void output_byte(uint8_t b);
  void Write_Command(uint16_t cmd);  
  void Write_Data(uint16_t data);
  void Write_Command_Data(uint16_t cmd, uint16_t dat);
  inline void WRLow()
  {
    digitalWrite(WR_PIN, LOW);
  }
  inline void WRHigh()
  {
    digitalWrite(WR_PIN, HIGH);
  }
  inline void CommandMode()
  {
    digitalWrite(RS_PIN, LOW);
  }
  inline void DataMode()
  {
    digitalWrite(RS_PIN, HIGH);
  }
  inline void CSLow()
  {
    digitalWrite(CS_PIN, LOW);
  }
  inline void CSHigh()
  {
    digitalWrite(CS_PIN, HIGH);
  }
   inline void RDLow()
  {
    digitalWrite(RD_PIN, LOW);
  }
  inline void RDHigh()
  {
    digitalWrite(RD_PIN, HIGH);
  }
 
};


void Display::begin()
{
/* Wiring
    CS  P1.3
    RS  P1.4
    WR  P1.5
    RD  P1.6
    RST P1.7
    PORT2 = D0 to D7
*/

  pinMode(WR_PIN,OUTPUT);
  pinMode(RS_PIN,OUTPUT);
  pinMode(CS_PIN,OUTPUT);
  pinMode(RD_PIN,OUTPUT);
  pinMode(P1_1,OUTPUT);
  pinMode(P1_2,OUTPUT);
  pinMode(P1_3,OUTPUT);
  pinMode(P1_4,OUTPUT);
  pinMode(P1_5,OUTPUT);
  pinMode(P2_0,OUTPUT);
  pinMode(P2_1,OUTPUT);
  pinMode(P2_2,OUTPUT);
  RDHigh();
  CSHigh();
  WRHigh();
  delay(100);
  CSLow();
  delay(10);    
  // Power control startup sequence
  Write_Command_Data(0x11, 0x2004);
  Write_Command_Data(0x13, 0xCC00);
  Write_Command_Data(0x15, 0x2600);
  Write_Command_Data(0x14, 0x252A);
  Write_Command_Data(0x12, 0x0033);
  Write_Command_Data(0x13, 0xCC04);
  Write_Command_Data(0x13, 0xCC06);
  Write_Command_Data(0x13, 0xCC4F);
  Write_Command_Data(0x13, 0x674F);
  Write_Command_Data(0x11, 0x2003);
  
  // Gamma control
  Write_Command_Data(0x30, 0x2609);
  Write_Command_Data(0x31, 0x242C);
  Write_Command_Data(0x32, 0x1F23);
  Write_Command_Data(0x33, 0x2425);
  Write_Command_Data(0x34, 0x2226);
  Write_Command_Data(0x35, 0x2523);
  Write_Command_Data(0x36, 0x1C1A);
  Write_Command_Data(0x37, 0x131D);
  Write_Command_Data(0x38, 0x0B11);
  Write_Command_Data(0x39, 0x1210);
  Write_Command_Data(0x3A, 0x1315);
  Write_Command_Data(0x3B, 0x3619);
  Write_Command_Data(0x3C, 0x0D00);
  Write_Command_Data(0x3D, 0x000D);
  
  Write_Command_Data(0x16, 0x0007); // power up
  Write_Command_Data(0x02, 0x0013); // LCD waveform control
  Write_Command_Data(0x03, 0x0003); // Interface mode
  Write_Command_Data(0x01, 0x0127); // driver output control
  Write_Command_Data(0x08, 0x0303); // Blank period
  Write_Command_Data(0x0A, 0x000B); // Frame cycle control
  Write_Command_Data(0x0B, 0x0003); // 
  Write_Command_Data(0x0C, 0x0000); // External interface control
  Write_Command_Data(0x41, 0x0000); // Vertical screen control
  Write_Command_Data(0x50, 0x0000); // MDDI
  Write_Command_Data(0x60, 0x0005); // MTP Init
  Write_Command_Data(0x70, 0x000B); // Timing
  Write_Command_Data(0x71, 0x0000); 
  Write_Command_Data(0x78, 0x0000);
  Write_Command_Data(0x7A, 0x0000);
  Write_Command_Data(0x79, 0x0007);
  Write_Command_Data(0x07, 0x0051); // Display control 
  Write_Command_Data(0x07, 0x0053);
  Write_Command_Data(0x79, 0x0000); // Panel signal control
  Write_Command(0x22);

  fillRectangle(0,0,240,320,0);

}
void Display::putPixel(uint16_t x, uint16_t y, uint16_t colour)
{
  gotoXY(x,y,x+1,y+1);
  Write_Data(colour >> 8);
  Write_Data(colour & 0xff);
}
void Display::putImage(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint16_t *Image)
{
  uint16_t Colour;
  gotoXY(x,y,x+width-1,y+height-1);
  for (y = 0; y < height; y++)
  {
    for (x=0; x < width; x++)
    {
      Colour = *(Image++);                        
      Write_Data(Colour);
    }
  }
}
void Display::fillRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t Colour)
{
  gotoXY(x,y,x+width-1,y+height-1);
  for (y = 0; y < height; y++)
    for (x=0; x < width; x++)
    {
      Write_Data(Colour);
    }
}
void Display::putText(const char *Text, uint16_t len, uint16_t x, uint16_t y, uint16_t ForeColour, uint16_t BackColour)
{
  uint8_t Index=0;
  uint8_t Row,Col;
  const uint8_t *CharacterCode = 0;

  for (Index = 0; Index < len; Index++)
  {        
    CharacterCode = &Font5x7[FONT_WIDTH*(Text[Index]-32)]; 
    Col=0;
    while (Col < FONT_WIDTH)
    {
      Row = 0;
      while (Row < FONT_HEIGHT)
      {
        if (CharacterCode[Col] & (1 << Row))
        {
          TextBox[(Row*FONT_WIDTH)+Col]=ForeColour;                    
        }
        else
        {
          TextBox[(Row*FONT_WIDTH)+Col]=BackColour;                    
        }                                
        Row++;                
      }            
      Col++;
    }
    putImage(x,y,FONT_WIDTH,FONT_HEIGHT,(const uint16_t *)TextBox);
    x = x + FONT_HEIGHT + 2;                
  }
}
void Display::putNumber(uint16_t Number, uint16_t x, uint16_t y, uint16_t ForeColour, uint16_t BackColour)
{
  char Buffer[5]; // Maximum value = 65535
  Buffer[4] = Number % 10 + '0';
  Number = Number / 10;
  Buffer[3] = Number % 10 + '0';
  Number = Number / 10;
  Buffer[2] = Number % 10 + '0';
  Number = Number / 10;  
  Buffer[1] = Number % 10 + '0';
  Number = Number / 10; 
  Buffer[0] = Number % 10 + '0';  
  putText(Buffer,5,x,y,ForeColour,BackColour);
}
void Display::output_byte(uint8_t b)
{
  P1OUT &=0xc1;
  P2OUT &=0xf8;
  if (b)
  { // only write out bits if b is non zero to save time
    P1OUT |= reverse_bits[ (b >> 3) ] >> 2;
    P2OUT |= reverse_bits[ (b & 0x7) ] >> 5;
  }
}
void Display::Write_Command(uint16_t cmd)
{
  CommandMode();
  output_byte(cmd >> 8);
  WRLow();
  WRHigh();
  output_byte(cmd & 0xff);
  WRLow();
  WRHigh();
}
void Display::Write_Data(uint16_t data)
{
  DataMode();
  output_byte(data >> 8);
  WRLow();  
  WRHigh();   
  output_byte(data & 0xff);
  WRLow();  
  WRHigh();
}
void Display::Write_Command_Data(uint16_t cmd, uint16_t dat)
{
  Write_Command(cmd);
  Write_Data(dat);
}
void Display::gotoXY(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{  
  // open up an area for drawing on the display
  Write_Command_Data(0x46, (x2 << 8) | x1);
  Write_Command_Data(0x47, y2);
  Write_Command_Data(0x48, y1);
  Write_Command_Data(0x20, x1);
  Write_Command_Data(0x21, y1);
  Write_Command(0x22);
}

Display Screen;
#endif

