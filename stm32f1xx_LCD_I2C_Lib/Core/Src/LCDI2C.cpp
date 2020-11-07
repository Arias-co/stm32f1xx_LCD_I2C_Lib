/*
 * LCDI2C.cpp
 *
 *  Created on: Nov 6, 2020
 *      Author: arielarias
 */

#include "LCDI2C.h"

LCD_I2C::LCD_I2C(I2C_HandleTypeDef *i2cx, uint8_t addr)
{
    // TODO Auto-generated constructor stub
    hi2cx = i2cx;
    i2cDeviceAddr = addr << 1;
}


/*Private functions------------------------------------------------*/
/**
*@brief: Lcd i2c device check.
*@retval: none
*/
void LCD_I2C::i2cDeviceCheck(void)
{
    /* Checks if target device is ready for communication. */
    /* 3 is number of trials, 1000ms is timeout */
    HAL_Delay(10);
    while (HAL_I2C_IsDeviceReady(hi2cx, i2cDeviceAddr, 3, 1000) != HAL_OK)
    {

    }
}

/**
*@brief: Send commands to lcd.
*@retval: none
*/
void LCD_I2C::Set_Command(uint8_t cmd)
{
    data_M = cmd & Mask_Data;        //Most significant bit
    data_L = (cmd << 4) & Mask_Data; //Least significant bit

    //For backlight On/off
    data_M |= data_BL;
    data_L |= data_BL;

    data[0] = data_M | LCD_E;  //Enable E pin, RS=0
    data[1] = data_M;          //Disable E pin, RS=0
    data[2] = data_L | LCD_E;
  data[3] = data_L;

    HAL_I2C_Master_Transmit(hi2cx, i2cDeviceAddr, (uint8_t*)data, 4, 200);
}

/**
*@brief: Write data to lcd.
*@retval: none
*/
void LCD_I2C::Write_Data(uint8_t datax)
{
    data_M = datax & Mask_Data;        //Most significant bit
    data_L = (datax << 4) & Mask_Data; //Least significant bit

    //For backlight On/off
    data_M |= data_BL;
    data_L |= data_BL;

    data[0] = data_M | LCD_E|LCD_RS;  //Enable E pin, RS=1
    data[1] = data_M | LCD_RS;        //Disable E pin, RS=1
    data[2] = data_L | LCD_E|LCD_RS;
  data[3] = data_L | LCD_RS;

    HAL_I2C_Master_Transmit(hi2cx, i2cDeviceAddr, (uint8_t*)data, 4, 200);
}

/**
*@brief: Clear lcd display.
*@retval: none
*/
void LCD_I2C::Clear(void)
{
    Set_Command(LCD_CLEAR_DISPLAY);
    HAL_Delay(10);
    str_len = 0;
    line_pos = 1;
}

/**
*@brief: Set lcd cursor position.
*@param: line_x: line no, chr_x: character no.
*@retval: none
*/
void LCD_I2C::SetCursor(int line_x, int chr_x)
{
  line_pos = line_x; //hold line position.

    if(((line_x >=1 && line_x <= line_MAX) && (chr_x >=1 && chr_x <= chr_MAX)))
    {
        Set_Command(LCD_SET_DDRAMADDR | Cursor_Data[line_x - 1][chr_x - 1]);
    }
}

/**
*@brief: Send string data to lcd.
*@param: str[]: string array, mode: str slide/noslide.
*@retval: none
*/
void LCD_I2C::Send_String(char *str, uint8_t mode)
{
    char *buffer[BFR_MAX];
    static uint8_t i[4] = {chr_MAX,chr_MAX,chr_MAX,chr_MAX}; //i follows the ch position while sliding.
  static uint8_t c[4] = {0, 0, 0, 0}; //c follows the each ch of the str buffer while sliding.
  static uint8_t ch_len = 0; //follow the string lenght.
  str_len = 0;


    switch(mode)
    {
        case STR_NOSLIDE:

            while (*str)
            {
                Write_Data (*str);
                str++;
                str_len++;
                if(str_len == chr_MAX)
                {
                    SetCursor(line_pos + 1, 1);
                    str_len = 0;
                }
            }

            break;

        case STR_SLIDE:

          for(int a = 0; a < BFR_MAX; a++)
          buffer[a]=str++;

            ch_len = strlen(*buffer);

            SetCursor(line_pos, i[line_pos - 1]);

            for(int k = c[line_pos - 1];k < ch_len; k++)
            Write_Data (*buffer[k]);

            i[line_pos - 1]--;

            if(i[line_pos -1] == 0)
            {
                i[line_pos - 1] = 1;
                c[line_pos - 1]++;
        if(c[line_pos - 1] == ch_len)
                    {
                        i[line_pos - 1] = chr_MAX;
                        c[line_pos - 1] = 0;
                        ch_len = 0;
                    }
            }

            break;
    }
}

/**
*@brief: write funcion virtual
*@param: *ch: "string + %f", value: float data variable
*@retval: none
*/
void LCD_I2C::write(uint8_t * text)
{
    Send_String((char *)text, STR_NOSLIDE);
}

/**
*@brief: Backlight control
*@param: light_state: BL on/off
*@retval: none
*/
void LCD_I2C::BackLight(uint8_t light_state)
{
    if(light_state == LCD_BL_ON)
    {
    data_BL = LCD_BL_ON;
        Write_Data(0x20); //Empty character
    }
    else if (light_state == LCD_BL_OFF)
    {
        data_BL = LCD_BL_OFF;
        Write_Data(0x20);
    }
}

/**
*@brief: Lcd initiliazing settings.
*@retval: none
*/
void LCD_I2C::Init(void)
{
    i2cDeviceCheck();
    Set_Command(LCD_CLEAR_DISPLAY);
    HAL_Delay(200);
    Set_Command(LCD_RETURN_HOME);
    HAL_Delay(5);
    Set_Command(LCD_FUNCTION_SET|MODE_4B|MODE_2L|MODE_5X8_DOTS);
    HAL_Delay(5);
    Set_Command(LCD_DISPLAY_CONTROL|DISPLAY_ON|CURSOR_OFF|BLINK_OFF);
    HAL_Delay(5);
    Set_Command(LCD_SET_DDRAMADDR);
    HAL_Delay(200);
}

/*************************END OF FILE*****************************/


LCD_I2C::~LCD_I2C()
{
    // TODO Auto-generated destructor stub
}

