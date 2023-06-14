#pragma once

#include <esp_log.h>
#include <lua.hpp>
#include <vector>

#include "luaboy/display/color.h"
#include "luaboy/display/generic.h"
#include "luaboy/sys/err.h"

static int lua_panic(lua_State* L) {
    const char* errorMsg = lua_tostring(L, -1);
    ESP_LOGE("LUA_ERROR", "Lua panic: %s", errorMsg);
    return 0; // Return 0 to terminate Lua
}

static Color get_color(lua_State* L, int idx) {
    size_t len = lua_rawlen(L, idx);
    Color color;

    lua_rawgeti(L, idx, 1);
    color.red = lua_tonumber(L, -1);
    lua_rawgeti(L, idx, 2);
    color.green = lua_tonumber(L, -1);
    lua_rawgeti(L, idx, 3);
    color.blue = lua_tonumber(L, -1);

    if (len > 3) {
        lua_rawgeti(L, idx, 4);
        color.alpha = lua_tonumber(L, -1);
        lua_pop(L, 4);
    } else {
        lua_pop(L, 3);
    }

    return color;
}

template <typename T>
static int display_clear(lua_State* L) {
    static_assert(std::is_base_of<Display, T>::value, "Derived must be a subclass of Display");

    T* display = (T*) lua_touserdata(L, 1);

    luaboy_err_t err = display->clear();

    if (err != LUABOY_OK) {
        luaL_error(L, "Error clearing! %d", err);
    }

    lua_pop(L, 1);
    return 0;
}

template <typename T>
static int display_resize(lua_State* L) {
    static_assert(std::is_base_of<Display, T>::value, "Derived must be a subclass of Display");

    T* display = (T*) lua_touserdata(L, 1);
    size_t width = lua_tointeger(L, 2);
    size_t height = lua_tointeger(L, 3);

    luaboy_err_t err = display->resize(width, height);

    if (err != LUABOY_OK) {
        luaL_error(L, "Error resizing! %d", err);
    }

    lua_pop(L, 3);
    return 0;
}

template <typename T>
static int display_set_pixel(lua_State* L) {
    static_assert(std::is_base_of<Display, T>::value, "Derived must be a subclass of Display");

    T* display = (T*) lua_touserdata(L, 1);
    size_t startx = lua_tointeger(L, 2);
    size_t starty = lua_tointeger(L, 3);
    Color color =  get_color(L, 4);

    luaboy_err_t err = display->set_pixel(startx, starty, color);

    if (err != LUABOY_OK) {
        luaL_error(L, "Error setting pixel! %d", err);
    }

    lua_pop(L, 4);
    return 0;
}

template <typename T>
static int display_draw_line(lua_State* L) {
    static_assert(std::is_base_of<Display, T>::value, "Derived must be a subclass of Display");

    T* display = (T*) lua_touserdata(L, 1);
    size_t startx = lua_tointeger(L, 2);
    size_t starty = lua_tointeger(L, 3);
    size_t endx = lua_tointeger(L, 4);
    size_t endy = lua_tointeger(L, 5);
    Color color =  get_color(L, 6);

    luaboy_err_t err = display->draw_line(startx, starty, endx, endy, color);

    if (err != LUABOY_OK) {
        luaL_error(L, "Error drawing line! %d", err);
    }

    lua_pop(L, 6);
    return 0;
}

template<typename T>
static int display_draw_arc(lua_State* L) {
    static_assert(std::is_base_of<Display, T>::value, "Derived must be a subclass of Display");

    T* display = (T*) lua_touserdata(L, 1);
    size_t xc = lua_tointeger(L, 2);
    size_t yc = lua_tointeger(L, 3);
    float radius = lua_tonumber(L, 4);
    float angle1 = lua_tonumber(L, 5);
    float angle2 = lua_tonumber(L, 6);
    Color color =  get_color(L, 7);

    luaboy_err_t err = display->draw_arc(xc, yc, radius, angle1, angle2, color);

    if (err != LUABOY_OK) {
        luaL_error(L, "Error drawing arc! %d", err);
    }

    lua_pop(L, 7);
    return 0;
}

template<typename T>
static int display_draw_poly(lua_State* L) {
    static_assert(std::is_base_of<Display, T>::value, "Derived must be a subclass of Display");

    T* display = (T*) lua_touserdata(L, 1);

    size_t n_corners = lua_rawlen(L, 2);
    std::vector<point_t> corners;

    for (size_t i = 1; i <= n_corners; i++) {
        lua_rawgeti(L, 2, i);  // Retrieve the i-th element from the table

        // Extract the fields and convert them to the struct type
        point_t point;
        lua_rawgeti(L, -1, 1);
        point.x = lua_tointeger(L, -1);

        lua_rawgeti(L, -2, 2);
        point.y = lua_tointeger(L, -1);

        lua_pop(L, 3);  // Pop the field values and the element table

        corners.push_back(point);  // Process the extracted point
    }

    Color color =  get_color(L, 3);

    luaboy_err_t err = display->draw_poly(corners, color);

    if (err != LUABOY_OK) {
        luaL_error(L, "Error drawing arc! %d", err);
    }

    lua_pop(L, 3);
    return 0;
}

template<typename T>
static int display_print(lua_State* L) {
    static_assert(std::is_base_of<Display, T>::value, "Derived must be a subclass of Display");

    T* display = (T*) lua_touserdata(L, 1);
    size_t x = lua_tointeger(L, 2);
    size_t y = lua_tointeger(L, 3);
    Color color =  get_color(L, 4);
    const char* str = lua_tostring(L, 5);

    luaboy_err_t err = display->print(x, y, color, str);

    if (err != LUABOY_OK) {
        luaL_error(L, "Error flushing display! %d", err);
    }

    lua_pop(L, 1);
    return 0;
}

template<typename T>
static int display_flush(lua_State* L) {
    static_assert(std::is_base_of<Display, T>::value, "Derived must be a subclass of Display");

    T* display = (T*) lua_touserdata(L, 1);

    luaboy_err_t err = display->flush();

    if (err != LUABOY_OK) {
        luaL_error(L, "Error flushing display! %d", err);
    }

    lua_pop(L, 1);
    return 0;
}

template <typename T>
luaboy_err_t lua_bind_display(lua_State* L, T* display) {
    static_assert(std::is_base_of<Display, T>::value, "Derived must be a subclass of Display");

    lua_atpanic(L, lua_panic);
    lua_pushlightuserdata(L, display);
    lua_setglobal(L, "display");

    lua_register(L, "display_clear", display_clear<T>);
    lua_register(L, "display_resize", display_resize<T>);
    lua_register(L, "display_set_pixel", display_set_pixel<T>);
    lua_register(L, "display_draw_line", display_draw_line<T>);
    lua_register(L, "display_draw_arc", display_draw_arc<T>);
    lua_register(L, "display_draw_poly", display_draw_poly<T>);
    lua_register(L, "display_print", display_print<T>);
    lua_register(L, "display_flush", display_flush<T>);

    return LUABOY_OK;
}