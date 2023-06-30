#pragma once

#include <esp_log.h>

#include "luaboy/lua/luaboy.hpp"

int lua_print(lua_State* L) {
    static const char* TAG = "lua_print";
    int nargs = lua_gettop(L);

    for (int i = 1; i <= nargs; i++) {
        const char* message = lua_tostring(L, i); // Get the argument as a string
        ESP_LOGI(TAG, "%s", message); // Print the argument
    }

    return 0;
}

void lua_init(lua_State* L) {
    luaL_openlibs(L);
    lua_register(L, "print", lua_print);
}
