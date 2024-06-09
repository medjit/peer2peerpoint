#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

//Tag used for logging
static const char *TAG_NETWORK_MANAGER = "NETWORK_MANAGER";

//Function prototypes
void networkManager_init(void);
static void networkManager_task(void *pvParameter);


//Functions
static void networkManager_task(void *pvParameter)
{
    ESP_LOGI(TAG_NETWORK_MANAGER, "Task created and running.");
    while(true)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}


void networkManager_init(void)
{
    ESP_LOGI(TAG_NETWORK_MANAGER, "Starting...");
    xTaskCreate(&networkManager_task, "networkManager_task", 8048, NULL, 5, NULL);
}

#endif /* NETWORK_MANAGER_H */