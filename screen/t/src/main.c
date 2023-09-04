// Include standard libraries
#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

// Include display libraries
#include "OLED_1in3_c.h"
#include "GUI_Paint.h"

// Set up display size
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Set up LED pin
#define LED_PIN 25

int main() {
    // Init Wi-Fi (for LED) and turn on LED
    stdio_init_all();
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 1);
    
    // Init OLED display
    if (DEV_Module_Init() != 0) {
        printf("Unable to init OLED\r\n");
        return -1;
    }
    OLED_1in3_C_Init();
    OLED_1in3_C_Clear();
    
    // Init display buffer
    printf("Initing Image buffer...\n");
    UBYTE *ImaeBuffer;
    UWORD Imagesize = ((SCREEN_WIDTH % 8 == 0)? (SCREEN_WIDTH / 8): (SCREEN_WIDTH / 8 + 1)) * SCREEN_HEIGHT;
    if((ImaeBuffer = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        return -1;
    }
    // Clear display buffer
    for (int i = 0; i < Imagesize; i++) {
        ImaeBuffer[i] = 0x00;
    }
    Paint_NewImage(ImaeBuffer, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0xff);

    // Init temerature sensor
    adc_init();
    adc_set_temp_sensor_enabled(true);
    adc_select_input(4);

    // Make update loop
    while (true) {
        // Clear display buffer
        for (int i = 0; i < Imagesize; i++) {
            ImaeBuffer[i] = 0x00;
        }
        // Calculate temperature in Celsius
        uint16_t raw = adc_read();
        const float conversion_factor = 3.3f / (1<<12);
        float result = raw * conversion_factor;
        float temp = 27 - (result -0.706)/0.001721;
        // Draw text
        char buffer[30];
        sprintf(buffer, "Temperature of CPU is: %.2fC", temp);
        Paint_DrawString_EN(0, 0, buffer, &Font12, WHITE, BLACK);
        OLED_1in3_C_Display(ImaeBuffer);
        // Wait 250ms
        DEV_Delay_ms(250);
    }

    //  In case, while loop has been broken, free memory and exit
    DEV_Module_Exit();
    return 0;
}
