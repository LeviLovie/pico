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

bool KEY_0_PRESSED = false;
bool KEY_1_PRESSED = false;

struct Transform {
    int x;
    int y;
    int direction;
};

void button_callback(uint gpio, uint32_t events) {
    if (gpio == KEY_0) {
        KEY_0_PRESSED = true;
    } else if (gpio == KEY_1) {
        KEY_1_PRESSED = true;
    }
}

int gen_rand_num(int max) {
    return rand() % max;
}

int main() {
    // Init Wi-Fi (for LED) and turn on LED
    stdio_init_all();
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 1);

    // Set seed for RNG
    srand(time(NULL));
    
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
    gpio_set_irq_enabled_with_callback(KEY_0, GPIO_IRQ_EDGE_FALL, true, &button_callback);
    gpio_set_irq_enabled_with_callback(KEY_1, GPIO_IRQ_EDGE_FALL, true, &button_callback);

    // Init loop
    struct Transform* elements = NULL;
    size_t elements_size = 0;
    bool apple_on_snake = true;
    int apple_x = 0;
    int apple_y = 0;
    while (apple_on_snake) {
        apple_on_snake = false;
        apple_x = gen_rand_num(SCREEN_WIDTH / 4 - 1);
        apple_y = gen_rand_num(SCREEN_HEIGHT / 4 - 1);
        for (int i = 0; i < elements_size; i++) {
            if (elements[i].x == apple_x && elements[i].y == apple_y) {
                apple_on_snake = true;
            }
        }
    }
    int state = 1;
    int timer = 0;
    while (true) {
        for (int i = 0; i < Imagesize; i++) {
            ImageBuffer[i] = 0x00;
        }

        if (state == 0) { // Game
            // Update
            if (KEY_0_PRESSED || KEY_1_PRESSED) {
                if (KEY_0_PRESSED) {
                    elements[elements_size - 1].direction -= 1;
                    KEY_0_PRESSED = false;
                } else if (KEY_1_PRESSED) {
                    elements[elements_size - 1].direction += 1;
                    KEY_1_PRESSED = false;
                }
            }
            if (elements[elements_size - 1].direction < 0) {
                elements[elements_size - 1].direction = 3;
            } else if (elements[elements_size - 1].direction > 3) {
                elements[elements_size - 1].direction = 0;
            }
            if (elements[elements_size - 1].direction == 0) {
                elements[elements_size - 1].x += 1;
                for (int i = 0; i < elements_size - 1; i++) {
                    if (elements[elements_size - 1].x == elements[i].x && elements[elements_size - 1].y == elements[i].y) {
                        state = 2;
                        timer = 0;
                    }
                }
            } else if (elements[elements_size - 1].direction == 1) {
                elements[elements_size - 1].y += 1;
                for (int i = 0; i < elements_size - 1; i++) {
                    if (elements[elements_size - 1].x == elements[i].x && elements[elements_size - 1].y == elements[i].y) {
                        state = 2;
                        timer = 0;
                    }
                }
            } else if (elements[elements_size - 1].direction == 2) {
                elements[elements_size - 1].x -= 1;
                for (int i = 0; i < elements_size - 1; i++) {
                    if (elements[elements_size - 1].x == elements[i].x && elements[elements_size - 1].y == elements[i].y) {
                        state = 2;
                        timer = 0;
                    }
                }
            } else if (elements[elements_size - 1].direction == 3) {
                elements[elements_size - 1].y -= 1;
                for (int i = 0; i < elements_size - 1; i++) {
                    if (elements[elements_size - 1].x == elements[i].x && elements[elements_size - 1].y == elements[i].y) {
                        state = 2;
                        timer = 0;
                    }
                }
            }
            if (elements[elements_size - 1].x < 0) {
                elements[elements_size - 1].x = SCREEN_WIDTH / 4 - 1;
            } else if (elements[elements_size - 1].x >= SCREEN_WIDTH / 4) {
                elements[elements_size - 1].x = 0;
            }
            if (elements[elements_size - 1].y < 0) {
                elements[elements_size - 1].y = SCREEN_HEIGHT / 4 - 1;
            } else if (elements[elements_size - 1].y >= SCREEN_HEIGHT / 4) {
                elements[elements_size - 1].y = 0;
            }
            for (int i = 0; i < elements_size - 1; i++) {
                elements[i].x = elements[i + 1].x;
                elements[i].y = elements[i + 1].y;
                elements[i].direction = elements[i + 1].direction;
            }
            if (elements[elements_size - 1].x == apple_x && elements[elements_size - 1].y == apple_y) {
                elements_size++;
                elements = (struct Transform*)realloc(elements, elements_size * sizeof(struct Transform));
                elements[elements_size - 1].x = elements[elements_size - 2].x;
                elements[elements_size - 1].y = elements[elements_size - 2].y;
                elements[elements_size - 1].direction = elements[elements_size - 2].direction;
                bool apple_on_snake = true;
                while (apple_on_snake) {
                    apple_on_snake = false;
                    apple_x = gen_rand_num(SCREEN_WIDTH / 4 - 1);
                    apple_y = gen_rand_num(SCREEN_HEIGHT / 4 - 1);
                    for (int i = 0; i < elements_size; i++) {
                        if (elements[i].x == apple_x && elements[i].y == apple_y) {
                            apple_on_snake = true;
                        }
                    }
                }
            }

            // Draws
            for (int i = 0; i < elements_size; i++) {
                Paint_DrawRectangle(
                    elements[i].x * 4,
                    elements[i].y * 4,
                    elements[i].x * 4 + 4,
                    elements[i].y * 4 + 4,
                    WHITE,
                    DOT_PIXEL_1X1,
                    DRAW_FILL_FULL
                );
            }
            Paint_DrawRectangle(
                apple_x * 4,
                apple_y * 4,
                apple_x * 4 + 4,
                apple_y * 4 + 4,
                WHITE,
                DOT_PIXEL_1X1,
                DRAW_FILL_FULL
            );

            if (elements_size == 50) {
                state = 3;
            }
        } else if (state == 1) {
            Paint_DrawString_EN((SCREEN_WIDTH / 2) - (5 * 8.5), 10, "Snake", &Font24, WHITE, BLACK);
            Paint_DrawString_EN((SCREEN_WIDTH / 2) - (16 * 3.5), SCREEN_HEIGHT - 12 - 5, "Press any button", &Font12, WHITE, BLACK);

            elements = NULL;
            elements_size = 0;
            for (int i = 0; i < 5; i++) {
                elements_size++;
                elements = (struct Transform*)realloc(elements, elements_size * sizeof(struct Transform));
                elements[elements_size - 1].x = i + 10;
                elements[elements_size - 1].y = 10;
                elements[elements_size - 1].direction = 0;
            }
            bool apple_on_snake = true;
            while (apple_on_snake) {
                apple_on_snake = false;
                apple_x = gen_rand_num(SCREEN_WIDTH / 4 - 1);
                apple_y = gen_rand_num(SCREEN_HEIGHT / 4 - 1);
                for (int i = 0; i < elements_size; i++) {
                    if (elements[i].x == apple_x && elements[i].y == apple_y) {
                        apple_on_snake = true;
                    }
                }
            }

            if (KEY_0_PRESSED || KEY_1_PRESSED) {
                state = 0;
                KEY_0_PRESSED = false;
                KEY_1_PRESSED = false;
            }
        } else if (state == 2) {
            Paint_DrawString_EN((SCREEN_WIDTH / 2) - (9 * 5.75), 10, "Game over", &Font16, WHITE, BLACK);
            KEY_0_PRESSED = false;
            KEY_1_PRESSED = false;
            timer += 1;
            if (timer > 30) {
                state = 1;
            }
        } else if (state == 3) {
            Paint_DrawString_EN((SCREEN_WIDTH / 2) - (7 * 6.75), 8, "Ya won!", &Font20, WHITE, BLACK);
            Paint_DrawString_EN((SCREEN_WIDTH / 2) - (8 * 6.75), 8 + 20, "So cool!", &Font20, WHITE, BLACK);
            Paint_DrawString_EN((SCREEN_WIDTH / 2) - (16 * 3.5), SCREEN_HEIGHT - 12 - 5, "Press any button", &Font12, WHITE, BLACK);
            if (KEY_0_PRESSED || KEY_1_PRESSED) {
                state = 1;
                KEY_0_PRESSED = false;
                KEY_1_PRESSED = false;
            }
        }

        sleep_ms(100);

        // Draw ImageBuffer to display
        OLED_1in3_C_Display(ImageBuffer);
    }

    //  In case, while loop has been broken, free memory and exit
    DEV_Module_Exit();
    return 0;
}
