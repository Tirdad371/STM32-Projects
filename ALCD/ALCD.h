#ifndef _ALCD_H_
#define _ALCD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

//------------------------------------------------------------------------------------------

#define ALCD_RS_GPIO	GPIOD
#define ALCD_RS_PIN		GPIO_PIN_11

#define ALCD_RW_GPIO	GPIOD
#define ALCD_RW_PIN		GPIO_PIN_10

#define ALCD_EN_GPIO	GPIOD
#define ALCD_EN_PIN		GPIO_PIN_12

#define ALCD_D4_GPIO	GPIOD
#define ALCD_D4_PIN		GPIO_PIN_1

#define ALCD_D5_GPIO	GPIOD
#define ALCD_D5_PIN		GPIO_PIN_2

#define ALCD_D6_GPIO	GPIOD
#define ALCD_D6_PIN		GPIO_PIN_3

#define ALCD_D7_GPIO	GPIOD
#define ALCD_D7_PIN		GPIO_PIN_4

#define ALCD_CLK_ENABLE()					__HAL_RCC_GPIOD_CLK_ENABLE(); \
											__HAL_RCC_GPIOC_CLK_ENABLE();

#define ALCD_Delay_us(US)					__ALCD_Delay_us((US))
#define ALCD_Delay_ms(MS)					__ALCD_Delay_ms((MS))

#define ALCD_High_Time						20 // us
#define ALCD_Low_Time						20 // us

#define ALCD_PRINTF_BUFF_SIZE	40


void __ALCD_Delay_us(uint32_t us);
void __ALCD_Delay_ms(uint32_t ms);

void ALCD_init(uint8_t col, uint8_t row);
void ALCD_initPins(void);


void ALCD_putChar(char c);
void ALCD_puts(const char* str);

void ALCD_goto(uint8_t x, uint8_t y);

void ALCD_putCharXY(uint8_t x, uint8_t y, char c);
void ALCD_putsXY(uint8_t x, uint8_t y, const char * str);

void ALCD_putNum(int32_t num);

void ALCD_printf(const char* fmt, ...);

void ALCD_Clear(void);



//------------------------------------------------------------------------------------------


#ifdef	__cplusplus
};
#endif



#endif		// _ALCD_H_
