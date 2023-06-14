#pragma once

#include <esp_log.h>
#include <Stream.h>
#include <HardwareSerial.h>
#include <sstream>
#include <string>

#include "luaboy/display/generic.h"

static void put_color(std::ostringstream* buf, const Color* color) {
    auto rgb = color->as_rgb();
    *buf << "\x1b[48;2;";
    *buf << (int) rgb.red;
    *buf << ";";
    *buf << (int) rgb.green;
    *buf << ";";
    *buf << (int) rgb.blue;
    *buf << "m ";
}

class DisplayUart : public Display {
    private:
        static inline const char* TAG = "DisplayUart";

    protected:
        HardwareSerial* stream;

    public:
        // Note that this is potentially memory unsafe, because if `stream` goes
        // out of scope and is destroyed, this reference will be invalid. A
        // smart man would use a smart pointer (or Rust), but I'm lazy, and PIO
        // doesn't support Rust.
        DisplayUart(HardwareSerial* stream, size_t width, size_t height):
        stream(stream) {
            this->resize(width, height);
        }

        virtual luaboy_err_t flush() const {
            std::ostringstream buf;

            for (auto& row: this->pixbuf) {
                Color prev = Color(NAN, NAN, NAN);
                put_color(&buf, &row[0]);
                for (auto& pixel: row) {
                    if (prev != pixel) {
                        put_color(&buf, &pixel);
                    }
                    else {
                        buf << " ";
                    }
                    prev = pixel;
                }
                buf << "\x1b[0m\n";
            }

            this->stream->write(buf.str().c_str());
            this->stream->flush();
            return LUABOY_OK;
        }
};
