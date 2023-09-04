#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

int generateRandomNumber() {
    srand(time(NULL));
    return (rand() % 225) + 25;
}

void blink(int time_off, int time_on) {
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
    sleep_ms(time_off);
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
    sleep_ms(time_on);
}

int main() {
    stdio_init_all();
    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed");
        return -1;
    }

    unsigned seed = time(0);
    srand(seed);

    while (true) {
        int blink_off = generateRandomNumber();
        int blink_on = generateRandomNumber();
        blink(blink_off, blink_on);
        printf("Blinking for %dms with %dms off\n", blink_on, blink_off);
    }
}