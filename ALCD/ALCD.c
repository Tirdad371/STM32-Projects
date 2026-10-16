#include "ALCD.h"

#include "main.h"      // HAL, GPIO, SystemCoreClock
#include <stdio.h>     // snprintf, vsnprintf
#include <stdarg.h>    // va_list
#include <inttypes.h>  // PRId32

//-------------------------------------command-----------------------------------
#define ALCD_CMD_CLEAR_DISPLAY				0x01
#define ALCD_CMD_CLEAR_DISPLAY_TIMEOUT		4000 //us

#define ALCD_CMD_RETURN_HOME				0x02
#define ALCD_CMD_RETURN_HOME_TIMEOUT		4000 //us

#define ALCD_CMD_ENTRY_MODE_SET				0x04
#define ALCD_CMD_ENTRY_MODE_SET_TIMEOUT		75 //us

#define ALCD_CMD_DISPLAY_CONTROL			0x08	//ON OFF
#define ALCD_CMD_DISPLAY_CONTROL_TIMEOUT	75 //us

#define ALCD_CMD_SHIFT						0x10
#define ALCD_CMD_SHIFT_TIMEOUT				75 //us

#define ALCD_CMD_FUNCTION_SET				0x20
#define ALCD_CMD_FUNCTION_SET_TIMEOUT		75 //us

#define ALCD_CMD_SET_CGRAM					0x40
#define ALCD_CMD_SET_CGRAM_TIMEOUT			75 //us

#define ALCD_CMD_SET_DDRAM					0x80
#define ALCD_CMD_SET_DDRAM_TIMEOUT			75 //us

#define ALCD_CMD_WRITE_DATA					75 //us



//-------------------------------------COMMANDS-----------------------------------

//-------------------------------------FLAGS--------------------------------------
#define ALCD_FLAG_ENTRY_MODE_INCREMENT              0x02
#define ALCD_FLAG_ENTRY_MODE_DECREMENT              0x00
#define ALCD_FLAG_ENTRY_MODE_SHIFT                  0x01

#define ALCD_FLAG_DISPLAY_CONTROL_ON                0x04
#define ALCD_FLAG_DISPLAY_CONTROL_OFF               0x00
#define ALCD_FLAG_DISPLAY_CONTROL_CURSOR_ON         0x02
#define ALCD_FLAG_DISPLAY_CONTROL_CURSOR_OFF        0x00
#define ALCD_FLAG_DISPLAY_CONTROL_BLINK_ON          0x01
#define ALCD_FLAG_DISPLAY_CONTROL_BLINK_OFF         0x00

#define ALCD_FLAG_FUNCTION_SET_8BIT                 0x10
#define ALCD_FLAG_FUNCTION_SET_4BIT                 0x00
#define ALCD_FLAG_FUNCTION_SET_2LINE                0x08
#define ALCD_FLAG_FUNCTION_SET_1LINE                0x00
#define ALCD_FLAG_FUNCTION_SET_5_10_DOT             0x04
#define ALCD_FLAG_FUNCTION_SET_5_8_DOT              0x00


//-------------------------------------FLAGS--------------------------------------



typedef enum{
	ALCD_RS_State_Command = 0,
	ALCD_RS_State_Data = 1,
}ALCD_RS_State;

typedef union{
	uint8_t Value;
	struct{
		uint8_t D0 : 1;
		uint8_t D1 : 1;
		uint8_t D2 : 1;
		uint8_t D3 : 1;
		uint8_t D4 : 1;
		uint8_t D5 : 1;
		uint8_t D6 : 1;
		uint8_t D7 : 1;
	};
}ALCD_Byte;

typedef struct{
	int8_t	X;
	int8_t	Y;
	int8_t Rows;
	int8_t Columns;
}ALCD_Handle;




static ALCD_Handle alcdHandle = {0};


void ALCD_write(ALCD_RS_State rs, uint8_t val, uint32_t timeout);
void ALCD_write4Bit(ALCD_RS_State rs, uint8_t val);
void ALCD_trigger();




/* USER CODE BEGIN 4 */

void ALCD_init(uint8_t col, uint8_t row)
{
	ALCD_initPins();

  // Wait 40ms for rise up
  ALCD_Delay_ms(100);
  // Send 4Bit command: 8Bit Interface
  ALCD_write4Bit(
    ALCD_RS_State_Command,
    ALCD_CMD_FUNCTION_SET | ALCD_FLAG_FUNCTION_SET_8BIT
  );
  // Wait 5ms
  ALCD_Delay_ms(50);
  // Send 4Bit command : 8Bit Interface
  ALCD_write4Bit(
    ALCD_RS_State_Command,
    ALCD_CMD_FUNCTION_SET | ALCD_FLAG_FUNCTION_SET_8BIT
  );
  // Wait 100us
  ALCD_Delay_us(100);
  // Send 4Bit command : 8Bit Interface
  ALCD_write4Bit(
    ALCD_RS_State_Command,
    ALCD_CMD_FUNCTION_SET | ALCD_FLAG_FUNCTION_SET_8BIT
  );
  // Send 4Bit command : 4Bit Interface
  ALCD_write4Bit(
    ALCD_RS_State_Command,
    ALCD_CMD_FUNCTION_SET | ALCD_FLAG_FUNCTION_SET_4BIT
  );

  // Send command: 4Bit Interface, 2Line, 5x8 dot
  ALCD_write(
    ALCD_RS_State_Command,
    ALCD_CMD_FUNCTION_SET | ALCD_FLAG_FUNCTION_SET_4BIT |
    ALCD_FLAG_FUNCTION_SET_2LINE | ALCD_FLAG_FUNCTION_SET_5_8_DOT,
    ALCD_CMD_FUNCTION_SET_TIMEOUT
  );
  // Send command: Display off
  ALCD_write(
    ALCD_RS_State_Command,
    ALCD_CMD_DISPLAY_CONTROL | ALCD_FLAG_DISPLAY_CONTROL_OFF,
    ALCD_CMD_DISPLAY_CONTROL_TIMEOUT
  );
  // Send command: Display clear
  ALCD_write(
    ALCD_RS_State_Command,
    ALCD_CMD_CLEAR_DISPLAY,
    ALCD_CMD_CLEAR_DISPLAY_TIMEOUT
  );

  // Display On
  ALCD_write(
    ALCD_RS_State_Command,
    ALCD_CMD_DISPLAY_CONTROL | ALCD_FLAG_DISPLAY_CONTROL_ON |
    ALCD_FLAG_DISPLAY_CONTROL_CURSOR_OFF | ALCD_FLAG_DISPLAY_CONTROL_BLINK_ON,
    ALCD_CMD_DISPLAY_CONTROL_TIMEOUT
  );

  alcdHandle.X = 0;
  alcdHandle.Y = 0;
  alcdHandle.Rows = row;
  alcdHandle.Columns = col;


}

void ALCD_initPins(void)
{

	GPIO_InitTypeDef init;

  // Clock Enable
  ALCD_CLK_ENABLE();

  init.Mode = GPIO_MODE_OUTPUT_PP;
  init.Pull = GPIO_NOPULL;
  init.Speed = GPIO_SPEED_HIGH;
  // init RS
  init.Pin = ALCD_RS_PIN;
  HAL_GPIO_Init(ALCD_RS_GPIO, &init);
  // init RW
  init.Pin = ALCD_RW_PIN;
  HAL_GPIO_Init(ALCD_RW_GPIO, &init);
  // init EN
  init.Pin = ALCD_EN_PIN;
  HAL_GPIO_Init(ALCD_EN_GPIO, &init);
  // init D4
  init.Pin = ALCD_D4_PIN;
  HAL_GPIO_Init(ALCD_D4_GPIO, &init);
  // init D5
  init.Pin = ALCD_D5_PIN;
  HAL_GPIO_Init(ALCD_D5_GPIO, &init);
  // init D6
  init.Pin = ALCD_D6_PIN;
  HAL_GPIO_Init(ALCD_D6_GPIO, &init);
  // init D7
  init.Pin = ALCD_D7_PIN;
  HAL_GPIO_Init(ALCD_D7_GPIO, &init);

}

void ALCD_write(ALCD_RS_State rs, uint8_t val, uint32_t timeout)
{
	ALCD_Byte byte = {val};

	//RS Low
	HAL_GPIO_WritePin(ALCD_RS_GPIO, ALCD_RS_PIN, (GPIO_PinState) rs);
	//RW Low
	HAL_GPIO_WritePin(ALCD_RW_GPIO, ALCD_RW_PIN, GPIO_PIN_RESET);
	//Write High Nibble (4x bit)
	HAL_GPIO_WritePin(ALCD_D7_GPIO, ALCD_D7_PIN, byte.D7);
	HAL_GPIO_WritePin(ALCD_D6_GPIO, ALCD_D6_PIN, byte.D6);
	HAL_GPIO_WritePin(ALCD_D5_GPIO, ALCD_D5_PIN, byte.D5);
	HAL_GPIO_WritePin(ALCD_D4_GPIO, ALCD_D4_PIN, byte.D4);
	//Trigger
	ALCD_trigger();
	//Write Low Nible
	HAL_GPIO_WritePin(ALCD_D7_GPIO, ALCD_D7_PIN, byte.D3);
	HAL_GPIO_WritePin(ALCD_D6_GPIO, ALCD_D6_PIN, byte.D2);
	HAL_GPIO_WritePin(ALCD_D5_GPIO, ALCD_D5_PIN, byte.D1);
	HAL_GPIO_WritePin(ALCD_D4_GPIO, ALCD_D4_PIN, byte.D0);
	//Trigger
	ALCD_trigger();

	//wait
	ALCD_Delay_us(timeout);

}

void ALCD_write4Bit(ALCD_RS_State rs, uint8_t val)
{
	ALCD_Byte byte = {val};

	//RS Low
	HAL_GPIO_WritePin(ALCD_RS_GPIO, ALCD_RS_PIN, GPIO_PIN_RESET);
	//RW Low
	HAL_GPIO_WritePin(ALCD_RW_GPIO, ALCD_RW_PIN, GPIO_PIN_RESET);
	//Write High Nibble (4x bit)
	HAL_GPIO_WritePin(ALCD_D7_GPIO, ALCD_D7_PIN, byte.D7);
	HAL_GPIO_WritePin(ALCD_D6_GPIO, ALCD_D6_PIN, byte.D6);
	HAL_GPIO_WritePin(ALCD_D5_GPIO, ALCD_D5_PIN, byte.D5);
	HAL_GPIO_WritePin(ALCD_D4_GPIO, ALCD_D4_PIN, byte.D4);
	//Trigger
	ALCD_trigger();


}

void ALCD_trigger(void)
{
	//Set High
	HAL_GPIO_WritePin(ALCD_EN_GPIO, ALCD_EN_PIN, GPIO_PIN_SET);
	ALCD_Delay_us(ALCD_High_Time);
	//Set Low
	HAL_GPIO_WritePin(ALCD_EN_GPIO, ALCD_EN_PIN, GPIO_PIN_RESET);
	ALCD_Delay_us(ALCD_Low_Time);
}

void ALCD_putChar(char c){
	ALCD_write(ALCD_RS_State_Data, c, ALCD_CMD_WRITE_DATA);
	if (++alcdHandle.X >= alcdHandle.Columns) {
		alcdHandle.X = 0;
		if (++alcdHandle.Y >= alcdHandle.Rows) {
			alcdHandle.Y = 0;
		}
		ALCD_goto(alcdHandle.X, alcdHandle.Y);
	}


}
void ALCD_puts(const char* str)
{
	while(*str != '\0')
	{
		ALCD_putChar(*str++);
	}
}

void ALCD_goto(uint8_t x, uint8_t y){
	static const uint8_t Line_ADDR[2] = {0x00, 0x40,};

	if(x >= alcdHandle.Columns)
	{
		x = 0;
		y ++;
		if (y >= alcdHandle.Rows) {
			y = 0;
		}
	}

	ALCD_write(	ALCD_RS_State_Command,
				ALCD_CMD_SET_DDRAM | (Line_ADDR[y] + x),
				ALCD_CMD_SET_DDRAM_TIMEOUT);

	alcdHandle.X = x;
	alcdHandle.Y = y;
}

void ALCD_putCharXY(uint8_t x, uint8_t y, char c)
{
	ALCD_goto(x, y);
	ALCD_putChar(c);
}
void ALCD_putsXY(uint8_t x, uint8_t y, const char * str){
	ALCD_goto(x,y);
	ALCD_puts(str);
}

void ALCD_putNum(int32_t num){
	char temp[12];
	snprintf(temp, sizeof(temp) - 1, "%d", num);
	ALCD_puts(temp);
}

void ALCD_printf(const char* fmt, ...)
{
	char temp[40];
	va_list args;
	va_start(args, fmt);
	vsnprintf(temp, sizeof(temp) - 1, fmt, args);
	va_end(args);
	ALCD_puts(temp);
}

void ALCD_Clear(void)
{
	ALCD_write(
				ALCD_RS_State_Command,
				ALCD_CMD_CLEAR_DISPLAY,
				ALCD_CMD_CLEAR_DISPLAY_TIMEOUT
	  );
}

//-----------------------------------DELAY---------------------------------------------
void __ALCD_Delay_us(uint32_t us)
{
  if (us == 0) return;
  us = SystemCoreClock / 3000000 * us;

  __asm volatile (
    "1:              \n"
    "subs %0, %0, #1 \n"
    "bne 1b          \n"
    : "+r" (us)
    :
    : "cc"
  );
}

void __ALCD_Delay_ms(uint32_t ms)
{
  if (ms == 0) return;
  ms = SystemCoreClock / 3000 * ms;

  __asm volatile (
    "1:              \n"
    "subs %0, %0, #1 \n"
    "bne 1b          \n"
    : "+r" (ms)
    :
    : "cc"
  );
}
//-----------------------------------DELAY---------------------------------------------

/* USER CODE END 4 */













