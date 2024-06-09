#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "led_strip.h"
#include "sdkconfig.h"
#include "OBBMonitor/OBBMonitor.h"
#include "OBStatusLeds/OBStatusLeds.h"
#include "NetworkManager/NetworkManager.h"
#include "httpFileServer/httpFileServer.h"


void app_main(void)
{
    obbMonitor_init();
    obStatusLeds_init();
    networkManager_init();
    httpFileServer_init();

    while (1) {

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}