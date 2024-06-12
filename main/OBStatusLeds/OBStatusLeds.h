#ifndef OB_STATUS_LEDS_H
#define OB_STATUS_LEDS_H

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"


//TODO: change to actual board GPIOs
#define GREEN_STATUS_LED_GPIO   2   //ESP32dev oboard led pin
#define RED_STATUS_LED_GPIO     15   //ESP32cam onboard flash led pin

//Tag used for logging
static const char *TAG_OB_STATUS_LEDS = "OB_STATUS_LEDS";

//Define enum with all possible status that can be represented by LEDs
typedef enum
{
    OBSTATUSLEDS_INIT,
    OBSTATUSLEDS_OK,
    OBSTATUSLEDS_ERROR
} obStatusLeds_status_t;

//Define variable to keep status
static obStatusLeds_status_t _status = OBSTATUSLEDS_INIT;

//Function prototypes
void obStatusLeds_init(void);
void obStatusLeds_setStatus(obStatusLeds_status_t status);
static void obStatusLeds_task(void *pvParameter);


//Functions

/*
    Task function handle led behaviour
*/
static void obStatusLeds_task(void *pvParameter)
{
    ESP_LOGI(TAG_OB_STATUS_LEDS, "Task created and running.");
    while(true)
    {
        gpio_set_level(GREEN_STATUS_LED_GPIO,0);
        gpio_set_level(RED_STATUS_LED_GPIO,0);
        vTaskDelay(700 / portTICK_PERIOD_MS);
        gpio_set_level(GREEN_STATUS_LED_GPIO,1);
        vTaskDelay(50 / portTICK_PERIOD_MS);

        gpio_set_level(GREEN_STATUS_LED_GPIO,0);
        gpio_set_level(RED_STATUS_LED_GPIO,0);
        vTaskDelay(700 / portTICK_PERIOD_MS);
        gpio_set_level(RED_STATUS_LED_GPIO,1);
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}

/*
    Init function to prepare GPIOS and create taks
*/
void obStatusLeds_init(void)
{
    ESP_LOGI(TAG_OB_STATUS_LEDS, "Configure Red and Green LEDs GPIOs.");
    gpio_reset_pin(GREEN_STATUS_LED_GPIO);
    gpio_reset_pin(RED_STATUS_LED_GPIO);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(GREEN_STATUS_LED_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_direction(RED_STATUS_LED_GPIO, GPIO_MODE_OUTPUT);

    xTaskCreate(&obStatusLeds_task, "obStatusLeds_task", 2048, NULL, 5, NULL);
}

/*
    setStatus function to set status of the LEDs //Called from other components.
*/
void obStatusLeds_setStatus(obStatusLeds_status_t status)
{
    ESP_LOGI(TAG_OB_STATUS_LEDS, "Set status to %d\n", status);
    _status = status;
}


#endif /* OB_STATUS_LEDS_H */