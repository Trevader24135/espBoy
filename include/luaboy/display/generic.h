#pragma once

#include <vector>
#include <esp_log.h>
#include <math.h>
#include <set>
#include <vector>

#include "luaboy/sys/err.h"
#include "luaboy/display/color.h"
#include "luaboy/display/font.h"


struct point_t {
    size_t x;
    size_t y;

    bool operator==(const point_t& other) const {
        return this->x == other.x && this->y == other.y;
    };

    bool operator<(const point_t& other) const {
        if (this->x < other.x )
            return true;
        else if (this->x > other.x)
            return false;
        return this->y < other.y;
    };
};

class Display {
    private:
        static inline const char* TAG = "Display";

    protected:
        std::vector<std::vector<Color>> pixbuf;
        

    public:
        size_t width, height;
        static inline Color BLACK = Color(0.0, 0.0, 0.0, 1.0);
        static inline Color WHITE = Color(1.0, 1.0, 1.0, 1.0);

        luaboy_err_t clear() {
            for (auto& row: this->pixbuf) {
                for (auto& pix: row) {
                    pix.clear();
                }
            }

            for (int y = 0; y < this->height; y++) {
                for (int x = 0; x < this->width; x++) {
                    pixbuf[y][x].clear();
                }
            }
            return LUABOY_OK;
        }

        luaboy_err_t resize(size_t width, size_t height) {
            this->height = height;
            this->width = width;

            this->pixbuf.resize(this->height);
            for (auto& row: this->pixbuf) {
                row.resize(this->width);
            }
            this->clear();
            return LUABOY_OK;
        };

        luaboy_err_t set_pixel(size_t x, size_t y, const Color& color) {
            if (x >= this->width || y >= this->height) {
                ESP_LOGE(this->TAG, "Tried writing out of bounds (size: (%zu, %zu), tried: (%zu, %zu))", this->width, this->height, x, y);
                return LUABOY_BOUNDS_ERR;
            }

            ESP_LOGD(this->TAG, "Setting pixel: (%zu, %zu) to color (%.1f, %.1f, %.1f)", x, y, color.red, color.green, color.blue);

            this->pixbuf[y][x] += color;
            return LUABOY_OK;
        }

        luaboy_err_t draw_line(size_t startx, size_t starty, size_t endx, size_t endy, const Color& color) {
            std::set<point_t> points;
            ESP_LOGD(this->TAG, "Drawing Line from (%zu, %zu) to (%zu, %zu)", startx, starty, endx, endy);

            for (float i = 0; i < 1; i += 0.01) {
                points.insert(
                    point_t{
                        .x = (size_t)((startx * (1.0 - i)) + (endx * i)),
                        .y = (size_t)((starty * (1.0 - i)) + (endy * i))
                    }
                );
            }
            for (auto& point: points) {
                luaboy_err_t err = this->set_pixel(point.x, point.y, color);
                if (err != LUABOY_OK) {
                    return err;
                }
            }
            return LUABOY_OK;
        }

        luaboy_err_t draw_poly(const std::vector<point_t>& corners, const Color& color) {
            std::set<point_t> points;

            point_t curr = corners[0];

            for (auto& point: corners) {
                if (point == corners[0])
                    continue;

                this->draw_line(curr.x, curr.y, point.x, point.y, color);
                curr = point;
            }

            this->draw_line(curr.x, curr.y, corners[0].x, corners[0].y, color);

            return LUABOY_OK;
        }

        luaboy_err_t draw_arc(size_t xc, size_t yc, const float radius, const float angle1, const float angle2, const Color& color) {
            std::set<point_t> points;

            for (float i = angle1; i < angle2; i += 0.0628) {
                points.insert(
                    point_t{
                        .x = (size_t)(radius * cos(i) * 0.999) + xc,
                        .y = (size_t)(radius * sin(i) * 0.999) + yc
                    }
                );
            }

            for (auto& point: points) {
                this->set_pixel(point.x, point.y, color);
            }
            return LUABOY_OK;
        }

        luaboy_err_t print(size_t x, size_t y, const Color& color, const char* str) {
            for (const uint8_t* c = (uint8_t*) str; *c != 0x00; c++) {
                if (*c == '\n') {
                    x = 0;
                    y += 8;
                }
                else {
                    for (uint8_t col = 0; col < 6; col++) {
                        for (uint8_t row = 0; row < 8; row++) {
                            if (Font_6x8::ascii[*c][col] & (1 << row)) {
                                this->set_pixel(x + col, y + row, color);
                            }
                        }
                    }
                    x += 6;
                    if (x + 6 > width) {
                        x = 0;
                        y += 8;
                    }
                }
            }
            return LUABOY_OK;
        }

        luaboy_err_t printf(size_t x, size_t y, const Color& color, const char* fmt, ...) {
            char buffer[64] = {0};

            va_list argptr;
            va_start(argptr, fmt);
            vsnprintf(buffer, sizeof(buffer), fmt, argptr);
            va_end(argptr);

            print(x, y, color, buffer);
            return LUABOY_OK;
        }

        virtual luaboy_err_t flush() const = 0;
};
