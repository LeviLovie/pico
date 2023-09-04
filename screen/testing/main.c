#include <stdlib.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#include "EPD_Test.h"
// #include "LCD_1in14.h" // Not needed for OLED screens
#include "OLED_1in3_c.h"
#include "ImageData.h"
#include "GUI_Paint.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

int main() {
    stdio_init_all();

    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed");
        return -1;
    }
    if (DEV_Module_Init() != 0) {
        printf("Unable to init OLED\r\n");
        return -1;
    }
    OLED_1in3_C_Init();
    OLED_1in3_C_Clear();
    
    printf("Initing Image buffer...\n");
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);

    UBYTE *Image;
    UWORD Imagesize = ((SCREEN_WIDTH % 8 == 0)? (SCREEN_WIDTH / 8): (SCREEN_WIDTH / 8 + 1)) * SCREEN_HEIGHT;
    if((Image = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        return -1;
    }
    for (int i = 0; i < Imagesize; i++) {
        Image[i] = 0x00;
    }
    Paint_NewImage(Image, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0xff);

    printf("Starting update loop...\n");
    while (true) {
        printf("Updating screen...\n");
        Paint_DrawString_EN(0, 0, "Hello, world!", &Font12, WHITE, BLACK);
        OLED_1in3_C_Display(Image);
        DEV_Delay_ms(250);
    }

    DEV_Module_Exit();
}