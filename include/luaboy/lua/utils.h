#pragma once

#include <esp_log.h>
#include <esp_err.h>
#include <freertos/semphr.h>

extern "C" {
    #include <lua.h>
    #include <lualib.h>
    #include <lauxlib.h>
}

#include "luaboy/sys/resources.h"
#include "luaboy/lua/api.h"

esp_err_t lua_exec_script(const char* filepath) {
    static const char* TAG = "lua_exec";
    // static bool init = false;
    // static lua_State* L = luaL_newstate();

    // if (!init) {
    //     init = true;
    //     lua_init(L);
    // }

    // if (xSemaphoreTake(xSemaphoreSPI, portMAX_DELAY) == NULL) {
    //     ESP_LOGE(TAG, "Timed out waiting for xSemaphoreSPI");
    //     return ESP_ERR_TIMEOUT;
    // }
    // File file = SD.open(filepath);
    // size_t buffer_size = file.available();
    // char* buffer = (char*) calloc(buffer_size + 1, 1);

    // if (buffer == NULL) {
    //     ESP_LOGE(TAG, "Could not allocate memory for file '%s'. Needed %u bytes", filepath, buffer_size);
    //     xSemaphoreGive(xSemaphoreSPI);
    //     return ESP_ERR_NO_MEM;
    // }

    // size_t n_read = file.readBytes(buffer, buffer_size);
    // xSemaphoreGive(xSemaphoreSPI);

    // if (n_read != buffer_size) {
    //     ESP_LOGE(TAG, "Error reading from card. Expected %u bytes, read %u", buffer_size, n_read);
    //     return ESP_ERR_INVALID_SIZE;
    // }

    // int loadResult = luaL_loadstring(L, buffer);

    // if (loadResult != LUA_OK) {
    //     ESP_LOGE(TAG, "Error loading Lua code: %s", lua_tostring(L, -1));
    //     return ESP_ERR_INVALID_ARG;
    // }

    // int executionResult = lua_pcall(L, 0, 0, 0);
    // if (executionResult != LUA_OK) {
    //     ESP_LOGE(TAG, "Error executing Lua code: %s", lua_tostring(L, -1));
    //     return ESP_ERR_INVALID_STATE;
    // }

    // free(buffer);
    return ESP_OK;
}