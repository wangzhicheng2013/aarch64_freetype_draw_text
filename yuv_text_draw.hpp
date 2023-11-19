#pragma once
#include "freetype_text_font_mask_draw.hpp"
class yuv_text_draw {
private:
    int color_red_ = 255;
    int color_green_ = 0;
    int color_blue_ = 0;
    int color_y_ = 0;
    int color_u_ = 0;
    int color_v_ = 0;
    int font_size_ = 50;
public:
    static yuv_text_draw & get_instance() {
        static yuv_text_draw instance;
        return instance;
    }
    int init() {
        set_yuv_color();
        return FREETYPE_TEXT_FONT_MASK_DRAW.init();
    }
    inline void set_rgb_color(int r, int g, int b) {
        color_red_ = r;
        color_green_ = g;
        color_blue_ = b;
    }
    inline void set_yuv_color() {
        color_y_ =  0.299  * color_red_ + 0.587  * color_green_ + 0.114  * color_blue_;
        color_u_ = -0.1687 * color_red_ + 0.3313 * color_green_ + 0.5    * color_blue_ + 128;
        color_v_ =  0.5    * color_red_ - 0.4187 * color_green_ - 0.0813 * color_blue_ + 128;
    }
    int add_text_to_uyvy(unsigned char *pic,
				int pic_w,
				int pic_h,
                const wchar_t* str,
				int font_start_x, 
				int font_start_y, 
				int font_zone_width,
				int font_zone_height) {
        unsigned char* font_mask = (unsigned char*)malloc(font_zone_width * font_zone_height);
        if (nullptr == font_mask) {
            return -1;
        }
        int error = FREETYPE_TEXT_FONT_MASK_DRAW.draw(font_mask, str, font_zone_width, font_zone_height, font_size_);
        if (error) {
            free(font_mask);
            return error;
        }
        int i = 0, j = 0;
        int new_i = 0, new_j = 0;
        int n = 0;
        int y_index = 0;
        for (j = 0;j < font_zone_height;j++) {
            for (i = 0;i < font_zone_width;i++) {
				if (0 == font_mask[font_zone_width * j + i]) {
                    continue;
                }
                new_i = i + font_start_x;
                new_j = j + font_start_y;
                if ((new_i >= pic_w) || (new_j >= pic_h)) {
                    return -2;
                }
				n = new_j * pic_w + new_i;
				y_index = (n << 1) - 1;
				pic[y_index] = color_y_;
				if (1 == y_index % 4) {
					pic[y_index - 1] = color_u_;
					pic[y_index + 1] = color_v_;
				}
            }
		}
        free(font_mask);
        return 0;
    }
    int add_text_to_yuyv(unsigned char *pic,
				int pic_w,
				int pic_h,
                const wchar_t* str,
				int font_start_x, 
				int font_start_y, 
				int font_zone_width,
				int font_zone_height) {
        unsigned char* font_mask = (unsigned char*)malloc(font_zone_width * font_zone_height);
        if (nullptr == font_mask) {
            return -1;
        }
        int error = FREETYPE_TEXT_FONT_MASK_DRAW.draw(font_mask, str, font_zone_width, font_zone_height, font_size_);
        if (error) {
            free(font_mask);
            return error;
        }
        int i = 0, j = 0;
        int new_i = 0, new_j = 0;
        int n = 0;
        int y_index = 0;
        for (j = 0;j < font_zone_height;j++) {
            for (i = 0;i < font_zone_width;i++) {
				if (0 == font_mask[font_zone_width * j + i]) {
                    continue;
                }
                new_i = i + font_start_x;
                new_j = j + font_start_y;
                if ((new_i >= pic_w) || (new_j >= pic_h)) {
                    return -2;
                }
				n = new_j * pic_w + new_i;
				y_index = (n << 1);
				pic[y_index] = color_y_;
				if (0 == (y_index & 0x03)) {
					pic[y_index + 1] = color_u_;
					pic[y_index + 3] = color_v_;
				}
            }
		}
        free(font_mask);
        return 0;
    }
private:
    yuv_text_draw() = default;
    virtual ~yuv_text_draw() {
    }
};
#define YUV_TEXT_DRAW yuv_text_draw::get_instance()