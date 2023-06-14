#pragma once

#include <Arduino.h>
#include <esp_log.h>
#include <WiFi.h>

#include "luaboy/sys/wifi.h"

namespace Tasks {
    void vTaskWiFiManager(void* args) {
        static const char* TAG = "Task-WiFi";
        char ssid[64], password[64];

        ESP_LOGI(TAG, "initializing WiFi");
        WiFi.mode(WIFI_STA);

        while (true) {
            switch (WiFi.status()) {
                case WL_IDLE_STATUS:
                    ESP_LOGW(TAG, "WiFi is idling. attempting to connect.");
                    get_wifi_credentials(ssid, sizeof(ssid) / sizeof(ssid[0]), password, sizeof(ssid) / sizeof(password[0]));
                    WiFi.begin(ssid, password);
                    vTaskDelay(pdMS_TO_TICKS(10000));
                    break;
                case WL_NO_SSID_AVAIL:
                    ESP_LOGI(TAG, "WiFi SSID not found. Retrying.");
                    get_wifi_credentials(ssid, sizeof(ssid) / sizeof(ssid[0]), password, sizeof(ssid) / sizeof(password[0]));
                    WiFi.begin(ssid, password);
                    vTaskDelay(pdMS_TO_TICKS(10000));
                    break;
                case WL_SCAN_COMPLETED:
                    ESP_LOGI(TAG, "WiFi scan completed");
                    break;
                case WL_CONNECTED:
                    ESP_LOGI(TAG, "IP Address: %s", WiFi.localIP().toString().c_str());
                    vTaskDelay(pdMS_TO_TICKS(60000));
                    break;
                case WL_CONNECT_FAILED:
                    ESP_LOGE(TAG, "WiFi failed to connect. attempting to connect.");
                    get_wifi_credentials(ssid, sizeof(ssid) / sizeof(ssid[0]), password, sizeof(ssid) / sizeof(password[0]));
                    WiFi.begin(ssid, password);
                    vTaskDelay(pdMS_TO_TICKS(10000));
                    break;
                case WL_CONNECTION_LOST:
                    ESP_LOGE(TAG, "WiFi connection lost. attempting to connect.");
                    get_wifi_credentials(ssid, sizeof(ssid) / sizeof(ssid[0]), password, sizeof(ssid) / sizeof(password[0]));
                    WiFi.begin(ssid, password);
                    vTaskDelay(pdMS_TO_TICKS(10000));
                    break;
                case WL_DISCONNECTED:
                    ESP_LOGE(TAG, "WiFi disconnected. attempting to connect.");
                    get_wifi_credentials(ssid, sizeof(ssid) / sizeof(ssid[0]), password, sizeof(ssid) / sizeof(password[0]));
                    WiFi.begin(ssid, password);
                    vTaskDelay(pdMS_TO_TICKS(10000));
                    break;
                case WL_NO_SHIELD:
                    ESP_LOGE(TAG, "How did I get here? Wifi error no shield.");
                    vTaskDelay(pdMS_TO_TICKS(1000));
                    break;
            }
        }
        vTaskDelete(NULL);
    }
}