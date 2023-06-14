#pragma once

struct RGBColor {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

class Color {
    private:
        const float og_red, og_green, og_blue, og_alpha;
    public:
        float red, green, blue, alpha;

        Color() :
        og_red(0.0), og_green(0.0), og_blue(0.0), og_alpha(1.0),
        red(0.0), green(0.0), blue(0.0), alpha(1.0) {}

        Color(float red, float green, float blue, float alpha = 1.0) :
        og_red(red), og_green(green), og_blue(blue), og_alpha(alpha),
        red(red), green(green), blue(blue), alpha(alpha) {}

        Color& operator=(const Color &other) {
            this->red = other.red;
            this->green = other.green;
            this->blue = other.blue;
            return *this;
        }

        Color& operator+=(const Color &other) {
            this->red = (this->red * (1.0 - other.alpha)) + (other.red * other.alpha);
            this->green = (this->green * (1.0 - other.alpha)) + (other.green * other.alpha);
            this->blue = (this->blue * (1.0 - other.alpha)) + (other.blue * other.alpha);
            return *this;
        }

        bool operator!=(const Color &other) {
            float dr = std::abs(this->red - other.red);
            float dg = std::abs(this->green - other.green);
            float db = std::abs(this->blue - other.blue);
            if (dr == NAN || dg == NAN || db == NAN)
                return true;
            else if (dr > 0.01 || dg > 0.01 || db > 0.01)
                return true;
            return false;
        }

        Color as_bw() const {
            float avg = (this->red + this->green + this->blue) / 3;
            return Color(avg, avg, avg, this->alpha);
        }

        RGBColor as_rgb() const {
            return RGBColor{
                .red = (uint8_t)(this->red * 255),
                .green = (uint8_t)(this->green * 255),
                .blue = (uint8_t)(this->blue * 255),
            };
        }

        Color* to_bw() {
            float avg = (this->red + this->green + this->blue) / 3;
            this->red = avg;
            this->green = avg;
            this->blue = avg;
            return this;
        }

        Color* clear() {
            this->red = this->og_red;
            this->green = this->og_green;
            this->blue = this->og_blue;
            this->alpha = this->og_alpha;
            return this;
        }
};
