#include <Arduino.h>
#include <esp_log.h>
#include <freertos/semphr.h>
#include <SD.h>

#include "luaboy/display/uart.h"
#include "luaboy/lua/api.h"
#include "luaboy/lua/luaboy.hpp"
#include "luaboy/sys/resources.h"
#include "luaboy/tasks/WiFiManager.h"
#include "luaboy/display/lua_bindings.h"

void vTaskTest(void* arg) {
    static const char TAG[] = "Test";
    lua_State* L = luaL_newstate();
    lua_init(L);

    DisplayUart display(&Serial, 80, 15);
    lua_bind_display(L, &display);

    while (true) {
        luaL_dostring(
            L,
            "print(\"lua print!\")\n"
            "display_clear(display)\n"
            "display_draw_line(display, 10, 1, 50, 9, {0, 1, 0})\n"
            "display_draw_arc(display, 10, 5, 4.0, 0.0, 6.28, {0, 0, 1})\n"
            "display_draw_arc(display, 40, 5, 3.0, -3.14, 3.14, {0, 0, 1})\n"
            "display_draw_arc(display, 70, 5, 6.0,  3.14, 9.42, {0, 0, 1})\n"
            "local points = {\n"
            "  {75, 7},\n"
            "  {60, 1},\n"
            "  {60, 13}\n"
            "}\n"
            "display_draw_poly(display, points, {1, 0, 0, 0.5})\n"
            "display_print(display, 2, 5, {1, 0, 1, 0.25}, string.format(\"Test! %d\", 123))\n"
            "display_flush(display)\n"
        );
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    lua_close(L);
    vTaskDelete(NULL);
}

extern "C" void app_main() {
    static const char* TAG = "app_main";

    ESP_LOGI(TAG, "initializing Arduino");
    initArduino();

    Serial.begin(921600);

    ESP_LOGI(TAG, "Creating task vTaskTest");
    xTaskCreate(
        vTaskTest,
        "Test Task",
        8192,
        NULL,
        1,
        NULL
    );

    ESP_LOGD(TAG, "app_main completed");
}