#include "gd32vf103.h"
#include "systick.h"
#include "lcd/lcd.h"
#include "pict.inc"

void _init();


void LCD_ShowPict(void)
{
    int i;
    LCD_Address_Set(0,0,159,79);
    for(i=0;i<25600;i+=2)
    {
        LCD_WR_DATA8(gimp_image.pixel_data[i+1]);
        LCD_WR_DATA8(gimp_image.pixel_data[i]);
    }
}

int main(void) {
    _init();
    Lcd_Init();			// init OLED
//    LCD_Clear(RED);
    BACK_COLOR=WHITE;
    LCD_ShowPict();
    /* Configure the LED pins */
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOC);
    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_1 | GPIO_PIN_2);
    gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13);

    gpio_bit_set(GPIOA, GPIO_PIN_1 | GPIO_PIN_2);
    gpio_bit_set(GPIOC, GPIO_PIN_13);

    for (int i = 0; 1; i++) {
        if (i & 1) {
            gpio_bit_reset(GPIOA, GPIO_PIN_1);
        } else {
            gpio_bit_set(GPIOA, GPIO_PIN_1);
        }
        if (i & 2) {
            gpio_bit_reset(GPIOA, GPIO_PIN_2);
        } else {
            gpio_bit_set(GPIOA, GPIO_PIN_2);
        }
        if (i & 4) {
            gpio_bit_reset(GPIOC, GPIO_PIN_13);
        } else {
            gpio_bit_set(GPIOC, GPIO_PIN_13);
        }
        delay_1ms(500);

    }
}
