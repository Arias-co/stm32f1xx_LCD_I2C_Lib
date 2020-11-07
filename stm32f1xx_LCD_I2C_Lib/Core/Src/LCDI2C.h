/*
 * LCDI2C.h
 *
 *  Created on: Nov 6, 2020
 *      Author: arielarias
 */

#ifndef SRC_LCDI2C_H_
#define SRC_LCDI2C_H_

#include <string.h>
#include "stdio.h"
#include "stm32f1xx_hal.h"
#include "print.h"



#define LCD_16x2
//#define LCD_16x4
//#define LCD_20x2
//#define LCD_20x4




#ifdef LCD_16x2
const uint8_t line_MAX =  2;
const uint8_t chr_MAX  = 16;
const uint8_t Cursor_Data[line_MAX][chr_MAX] = {
  {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F}, //1. line DDRAM address
  {0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F}, //2. line DDRAM address
};
#endif

#ifdef LCD_16x4
const uint8_t line_MAX = 4;
const uint8_t chr_MAX = 16;

const uint8_t Cursor_Data[line_MAX][chr_MAX] = {
  {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F}, //1. line DDRAM address
  {0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F}, //2. line DDRAM address
  {0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F}, //3. line DDRAM address
  {0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x5B,0x5C,0x5D,0x5E,0x5F}, //4. line DDRAM address
};
#endif

#ifdef LCD_20x2
const uint8_t line_MAX = 2;
const uint8_t chr_MAX = 20;

const uint8_t Cursor_Data[line_MAX][chr_MAX] = {
  {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,0x11,0x12,0x13}, //1. line DDRAM address
  {0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,0x50,0x51,0x52,0x53}, //2. line DDRAM address
};
#endif

#ifdef LCD_20x4
const uint8_t line_MAX = 4;
const uint8_t chr_MAX = 20;

const uint8_t Cursor_Data[line_MAX][chr_MAX] = {
  {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,0x11,0x12,0x13}, //1. line DDRAM address
  {0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,0x50,0x51,0x52,0x53}, //2. line DDRAM address
  {0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27}, //3. line DDRAM address
  {0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x5B,0x5C,0x5D,0x5E,0x5F,0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67}, //4. line DDRAM address
};
#endif

/*Defines and variables------------------------------------------*/
//LCD instructions
#define LCD_CLEAR_DISPLAY           0x01
#define LCD_RETURN_HOME             0x02
#define LCD_ENTRYMODE_SET           0x04
#define LCD_DISPLAY_CONTROL         0x08
#define LCD_CURSORDISPLAY_SHIFT     0x10
#define LCD_FUNCTION_SET            0x20
#define LCD_SET_CGRAMADDR           0x40
#define LCD_SET_DDRAMADDR           0x80
//#define   LCD_READ_BUSYFLAG           0x100


//Read busy flag sub-options
//typedef enum {
//  BUSY_FLAG_ON  = 0x01,
//  BUSY_FLAG_OFF = 0x00
//  } lcd_read_BusyFlag_t;

//LCD backlight, RS, RW, E definitions for i2c module
//P7............P0 is data that will be sent to lcd through i2c module(PCF8574);
//P0 : RS
//P1 : RW
//P2 : E
//P3 : BT --> backlight pin activation
//P4 : D4
//P5 : D5
//P6 : D6
//P7 : D7
#define LCD_RS     0x01
#define LCD_RW     0x02
#define LCD_E      0x04
#define LCD_BL_ON  0x08 //Backlight on
#define LCD_BL_OFF 0x00 //Backlight off

//Define lcd string mode;
#define STR_NOSLIDE 0x00
#define STR_SLIDE 0x01

//Define lcd parameters
//typedef struct {
//  uint8_t i2cAddr;
//  uint8_t X;
//  uint8_t Y;
//  uint8_t Data_Lenght;
//  uint8_t Display_Line;
//  uint8_t Char_Font;
//} lcd_set_device_t;





/*Defines and variables--------------------------------------------*/

class LCD_I2C: public Print
{

private:


    I2C_HandleTypeDef *hi2cx;

    uint8_t i2cDeviceAddr; //* LCD i2c module default address, change i2c device address here, default is 0x4E.


    uint8_t Mask_Data = 0xf0; //Select upper bits.
    uint8_t data[4], data_M, data_L, data_BL;
    uint8_t line_pos = 1; //hold line position, default is 1. line.
    uint8_t str_len = 0; //follow the string lenght.
    const uint8_t BFR_MAX = 100; //*Change it if you want to use a different value.




public:


    typedef enum {
        ENTRY_INCREMENT       = 0x02,
        ENTRY_DECREMENT       = 0x00,
        ENTRY_DISPLAY_SHIFT   = 0x01,
        ENTRY_DISPLAY_NOSHIFT = 0x00
    } lcd_EntryMode_set_t;

    //Display control sub-options
    typedef enum {
        DISPLAY_ON   = 0x04,
        DISPLAY_OFF  = 0x00,
        CURSOR_ON    = 0x02,
        CURSOR_OFF   = 0x00,
        BLINK_ON  = 0x01,
      BLINK_OFF = 0x00
    } lcd_display_control_t;

    //Cursor or display shift sub-options
    typedef enum {
        DISPLAY_SHIFT  = 0x08,
        CURSOR_SHIFT   = 0x00,
        SHIFT_TO_RIGHT = 0x04,
        SHIFT_TO_LEFT  = 0x00
    } lcd_CursorDisplay_shift_t;

    //Function set sub-options
    typedef enum {
        MODE_8B        = 0x10,
        MODE_4B        = 0x00,
        MODE_2L        = 0x08,
        MODE_1L        = 0x00,
        MODE_5X10_DOTS = 0x04,
        MODE_5X8_DOTS  = 0x00
    } lcd_function_set_t;

    LCD_I2C(I2C_HandleTypeDef *i2cx, uint8_t addr = 0x27 );


    /*Private functions----------------------------------------------*/
    /**
    *@brief: Lcd i2c device check.
    *@retval: none
    */
    void i2cDeviceCheck(void);

    /**
    *@brief: Lcd initiliazing settings.
    *@retval: none
    */
    void Init(void);

    /**
    *@brief: Clear lcd display.
    *@retval: none
    */
    void Clear(void);

    ///**
    //*@brief: LCD display returns to home position.
    //*@retval: none
    //*/
    //void LCD_Return_Home(void);

    /**
    *@brief: Set lcd cursor position.
    *@param: line_x: line no, chr_x: character no.
    *@retval: none
    */
    void SetCursor(int line_x, int chr_x);

    ///**
    //*@bief: Read busy flag (BF) indicating internal operation is being performed.
    //*@retval: BF
    //*/
    //HAL_StatusTypeDef LCD_Read_BusyFlag(void);

    /**
    *@brief: Backlight control
    *@param: light_state: on/off
    *@retval: none
    */
    void BackLight(uint8_t light_state);

    /**
    *@brief: Send commands to lcd.
    *@retval: none
    */
    void Set_Command(uint8_t cmd);

    /**
    *@brief: Write data.
    *@retval: none
    */
    void Write_Data(uint8_t datax);

    /**
    *@brief: Send string data.
    *@param: *str: string pointer, mode: str slide/noslide.
    *@retval: none
    */
    void Send_String(char *str, uint8_t mode);

    /**
    *@brief: Print value, ch to lcd.
    *@param: value: uint8_t
    *@retval: none
    */
    //void print(char const *ch, float value);

    virtual void write(uint8_t * text);

    virtual ~LCD_I2C();
};

#endif /* SRC_LCDI2C_H_ */
