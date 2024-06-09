#ifndef ONBOARD_BATTERY_MONITOR_H
#define ONBOARD_BATTERY_MONITOR_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

//Tag used for logging
static const char *TAG_OB_BATTERY_MONITOR = "OB_BATTERY_MONITOR";

//Function prototypes
void obbMonitor_init(void);
static void obbMonitor_task(void *pvParameter);

//Functions
static void obbMonitortask(void *pvParameter)
{
    ESP_LOGI(TAG_OB_BATTERY_MONITOR, "Task created and running.");
    while(true)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void obbMonitor_init(void)
{
    ESP_LOGI(TAG_OB_BATTERY_MONITOR, "Configure Battery and PS monitoring GPIOs.");
    xTaskCreate(&obbMonitortask, "obbMonitortask", 8048, NULL, 5, NULL);
}

#endif /* ONBOARD_BATTERY_MONITOR_H */