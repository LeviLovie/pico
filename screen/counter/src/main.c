// Include standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

// Include display libraries
#include "OLED_1in3_c.h"
#include "GUI_Paint.h"

// Set up display size
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Set up LED pin
#define LED_PIN 25

// Set up key pins
#define KEY_0 15
#define KEY_1 17

#define BUTTON_READS 28

int counter = 0;

void button_callback(uint gpio, uint32_t events) {
    if (gpio == KEY_0) {
        if (events == GPIO_IRQ_EDGE_FALL) {
            counter--;
        }
    } else if (gpio == KEY_1) {
        if (events == GPIO_IRQ_EDGE_FALL) {
            counter++;
        }
    }
}

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
    gpio_init(KEY_0);
    gpio_set_dir(KEY_0, GPIO_IN);
    gpio_pull_up(KEY_0);
    gpio_init(KEY_1);
    gpio_set_dir(KEY_1, GPIO_IN);
    gpio_pull_up(KEY_1);

    // Init interrupts
    gpio_set_irq_enabled_with_callback(KEY_0, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &button_callback);
    gpio_set_irq_enabled_with_callback(KEY_1, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &button_callback);

    // Init loop
    while (true) {
        // Clear display buffer
        for (int i = 0; i < Imagesize; i++) {
            ImageBuffer[i] = 0x00;
        }

        // Draw counter text
        char buffer[30];
        // printf("Counter: %d\n", counter);
        sprintf(buffer, "Counter: %d", counter);
        Paint_DrawString_EN(0, 0, buffer, &Font16, WHITE, BLACK);

        // Draw ImageBuffer to display
        OLED_1in3_C_Display(ImageBuffer);
    }

    //  In case, while loop has been broken, free memory and exit
    DEV_Module_Exit();
    return 0;
}
