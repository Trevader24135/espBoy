#pragma once

#include <Arduino.h>
#include <freertos/semphr.h>
#include <SD.h>
#include <tomlcpp.hpp>
#include <WiFi.h>

#include "luaboy/sys/resources.h"

esp_err_t get_wifi_credentials(char* ssid, size_t ssid_buf_size, char* pass, size_t pass_buf_size) {
    static const char* TAG = "get_wifi_credentials";
    if (!xSemaphoreTake(xSemaphoreSPI, portMAX_DELAY)) {
        ESP_LOGE(TAG, "Timed out waiting for xSemaphoreSPI");
        return ESP_ERR_TIMEOUT;
    }

    File file = SD.open("/wifi.tml");
    size_t buffer_size = file.available();
    char* buffer = (char*) calloc(buffer_size + 1, 1);

    if (buffer == NULL) {
        ESP_LOGE(TAG, "Could not allocate memory for file '/wifi.tml'. Needed %u bytes", buffer_size);
        xSemaphoreGive(xSemaphoreSPI);
        return ESP_ERR_NO_MEM;
    }

    size_t n_read = file.readBytes(buffer, buffer_size);
    xSemaphoreGive(xSemaphoreSPI);

    if (n_read != buffer_size) {
        ESP_LOGE(TAG, "Error reading from card. Expected %u bytes, read %u", buffer_size, n_read);
        return ESP_ERR_INVALID_SIZE;
    }

    toml::Result wifi_conf = toml::parse(buffer);
    if (!wifi_conf.table) {
        ESP_LOGE(TAG, "Error reading from card. Expected %u bytes, read %u", buffer_size, n_read);
        return ESP_ERR_INVALID_ARG;
    }

    auto [has_ssid, toml_ssid] = wifi_conf.table->getString("ssid");
    if (!has_ssid) {
        ESP_LOGE(TAG, "/wifi.toml has no top-level key 'ssid', which should contain a string.");
        return ESP_ERR_INVALID_ARG;
    }

    auto [has_pass, toml_pass] = wifi_conf.table->getString("pass");
    if (!has_pass) {
        ESP_LOGE(TAG, "/wifi.toml has no top-level key 'pass', which should contain a string.");
        return ESP_ERR_INVALID_ARG;
    }

    strncpy(ssid, toml_ssid.c_str(), ssid_buf_size);
    strncpy(pass, toml_pass.c_str(), pass_buf_size);
    ESP_LOGI(TAG, "Got Wifi credentials from SD card. SSID: %s PASS: %s", ssid, pass);

    return ESP_OK;
}