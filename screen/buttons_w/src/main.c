// Include standard libraries
#include <stdlib.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

// Include display libraries
#include "OLED_1in3_c.h"
#include "GUI_Paint.h"

// Set up display size
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Set up key pins
#define KEY_0 15
#define KEY_1 17

int main() {
    // Init Wi-Fi (for LED) and turn on LED
    stdio_init_all();
    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed");
        return -1;
    }
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
    
    // Init OLED display
    if (DEV_Module_Init() != 0) {
        printf("Unable to init OLED\r\n");
        return -1;
    }
    OLED_1in3_C_Init();
    OLED_1in3_C_Clear();
    
    // Init display buffer
    printf("Initing Image buffer...\n");
    UBYTE *ImageBuffer;
    UWORD Imagesize = ((SCREEN_WIDTH % 8 == 0)? (SCREEN_WIDTH / 8): (SCREEN_WIDTH / 8 + 1)) * SCREEN_HEIGHT;
    if((ImageBuffer = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        return -1;
    }
    // Clear display buffer
    for (int i = 0; i < Imagesize; i++) {
        ImageBuffer[i] = 0x00;
    }
    Paint_NewImage(ImageBuffer, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0xff);

    // Set up keys
    DEV_GPIO_Mode(KEY_0, 0);
    DEV_GPIO_Mode(KEY_1, 0);
    // Make update loop
    while(true) {
        for (int i = 0; i < Imagesize; i++) {
            ImageBuffer[i] = 0x00;
        }

        if(DEV_Digital_Read(KEY_0) == 0) {
            Paint_DrawRectangle(2, 2, 16, 16, WHITE, DOT_PIXEL_2X2, DRAW_FILL_FULL);
        } else {
            Paint_DrawRectangle(2, 2, 16, 16, BLACK, DOT_PIXEL_2X2, DRAW_FILL_EMPTY);
        }
            
        if(DEV_Digital_Read(KEY_1) == 0) {
            Paint_DrawRectangle(2, SCREEN_HEIGHT - 16, 16, SCREEN_HEIGHT - 2, WHITE, DOT_PIXEL_2X2, DRAW_FILL_FULL);
        } else {
            Paint_DrawRectangle(2, SCREEN_HEIGHT - 16, 16, SCREEN_HEIGHT - 2, BLACK, DOT_PIXEL_2X2, DRAW_FILL_EMPTY);
        }

        OLED_1in3_C_Display(ImageBuffer);
        sleep_ms(100);
    }

    //  In case, while loop has been broken, free memory and exit
    DEV_Module_Exit();
    return 0;
}
