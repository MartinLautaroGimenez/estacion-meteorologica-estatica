#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define LedOnBoard 2

void app_main() {
    printf("Hola :v\n");
    gpio_set_direction(LedOnBoard, GPIO_MODE_OUTPUT);

    for(;;){
        if(gpio_set_level(LedOnBoard, 1) == ESP_OK){
            print("exito");
        }
        printf("ESP32 con ESP IDF 0w0\n");
        vTaskDelay(1000/portTICK_PERIOD_MS);
        gpio_set_level(LedOnBoard, 0);
        printf("Ta interesante el Free RTOS\n");
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}