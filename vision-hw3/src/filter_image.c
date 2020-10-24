#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"
#define TWOPI 6.2831853

void l1_normalize(image im)
{
    int i, j, k;
    float factor = 0;
    float pixel;
    for (k = 0; k < im.c; ++k)
    {
        for (j = 0; j < im.h; ++j)
        {
            for (i = 0; i < im.w; ++i)
            {
                factor += get_pixel(im, i, j, k);
            }
        }
    }
    for (k = 0; k < im.c; ++k)
    {
        for (j = 0; j < im.h; ++j)
        {
            for (i = 0; i < im.w; ++i)
            {
                pixel = get_pixel(im, i, j, k);
                set_pixel(im, i, j, k, pixel / factor);
            }
        }
    }
}

image make_box_filter(int w)
{
    image box_filter = make_image(w, w, 1);
    int i, j, k;
    for (k = 0; k < box_filter.c; ++k)
    {
        for (j = 0; j < box_filter.h; ++j)
        {
            for (i = 0; i < box_filter.w; ++i)
            {
                set_pixel(box_filter, i, j, k, 1.0);
            }
        }
    }
    l1_normalize(box_filter);

    return box_filter;
}

image convolve_image(image im, image filter, int preserve)
{
    int w, h, c, f_w, f_h, x_im, y_im;
    assert(im.c == filter.c || filter.c == 1);
    //Note that filter's size can't be even, i.e 1x1, 3x3, 5x5 etc
    assert(filter.w % 2 && filter.h % 2);
    int numChannels, filter_size;
    float q, im_pixel, fil_pixel;

    filter_size = (filter.w - 1) / 2;

    if (im.c == filter.c)
    {
        if (preserve == 1)
        {
            numChannels = im.c;
        }
        else
        {
            numChannels = 1;
        }
    }
    else if (filter.c == 1)
    {
        if (preserve == 1)
        {
            numChannels = im.c;
        }
        else
        {
            numChannels = 1;
        }
    }
    image conv_img = make_image(im.w, im.h, numChannels);
    for (w = 0; w < im.w; w++)
    {
        for (h = 0; h < im.h; h++)
        {
            q = 0;
            for (c = 0; c < im.c; c++)
            {
                if (numChannels > 1)
                {
                    q = 0;
                }
                for (f_w = 0; f_w < filter.w; f_w++)
                {
                    x_im = w - filter_size + f_w;
                    for (f_h = 0; f_h < filter.h; f_h++)
                    {
                        y_im = h - filter_size + f_h;
                        im_pixel = get_pixel(im, x_im, y_im, c);
                        if (filter.c > 1)
                        {
                            fil_pixel = get_pixel(filter, f_w, f_h, c);
                        }
                        else
                        {
                            fil_pixel = get_pixel(filter, f_w, f_h, 0);
                        }
                        q = q + im_pixel * fil_pixel;
                    }
                }
                if (numChannels > 1)
                {
                    set_pixel(conv_img, w, h, c, q);
                }
                else
                {
                    set_pixel(conv_img, w, h, 0, q);
                }
            }
        }
    }
    return conv_img;
}

image make_highpass_filter()
{
    image hp_filter = make_image(3, 3, 1);
    set_pixel(hp_filter, 0, 0, 0, 0);
    set_pixel(hp_filter, 1, 0, 0, -1);
    set_pixel(hp_filter, 2, 0, 0, 0);
    set_pixel(hp_filter, 0, 1, 0, -1);
    set_pixel(hp_filter, 1, 1, 0, 4);
    set_pixel(hp_filter, 2, 1, 0, -1);
    set_pixel(hp_filter, 0, 2, 0, 0);
    set_pixel(hp_filter, 1, 2, 0, -1);
    set_pixel(hp_filter, 2, 2, 0, 0);
    return hp_filter;
}

image make_sharpen_filter()
{
    image sp_filter = make_image(3, 3, 1);
    set_pixel(sp_filter, 0, 0, 0, 0);
    set_pixel(sp_filter, 1, 0, 0, -1);
    set_pixel(sp_filter, 2, 0, 0, 0);
    set_pixel(sp_filter, 0, 1, 0, -1);
    set_pixel(sp_filter, 1, 1, 0, 5);
    set_pixel(sp_filter, 2, 1, 0, -1);
    set_pixel(sp_filter, 0, 2, 0, 0);
    set_pixel(sp_filter, 1, 2, 0, -1);
    set_pixel(sp_filter, 2, 2, 0, 0);
    return sp_filter;
}

image make_emboss_filter()
{
    image eb_filter = make_image(3, 3, 1);
    set_pixel(eb_filter, 0, 0, 0, -2);
    set_pixel(eb_filter, 1, 0, 0, -1);
    set_pixel(eb_filter, 2, 0, 0, 0);
    set_pixel(eb_filter, 0, 1, 0, -1);
    set_pixel(eb_filter, 1, 1, 0, 1);
    set_pixel(eb_filter, 2, 1, 0, 1);
    set_pixel(eb_filter, 0, 2, 0, 0);
    set_pixel(eb_filter, 1, 2, 0, 1);
    set_pixel(eb_filter, 2, 2, 0, 2);
    return eb_filter;
}

// Question 2.2.1: Which of these filters should we use preserve when we run our convolution and which ones should we not? Why?
// Answer: TODO

// Question 2.2.2: Do we have to do any post-processing for the above filters? Which ones and why?
// Answer: TODO

image make_gaussian_filter(float sigma)
{
    int ker_size;
    // make kernel size of the next odd integer after sigma*6
    ker_size = ceil(sigma * 6);
    if (!(ker_size % 2))
    {
        ker_size++;
    }
    image gaussian_filter = make_image(ker_size, ker_size, 1);
    int w, h, x, y, center;
    float gaussian;
    center = (ker_size - 1) / 2;
    for (w = 0; w < gaussian_filter.w; w++)
    {
        for (h = 0; h < gaussian_filter.h; h++)
        {
            x = w - center;
            y = h - center;
            gaussian = 1 / (2 * M_PI * sigma * sigma) * expf(-(x * x + y * y) / (2 * sigma * sigma));
            set_pixel(gaussian_filter, w, h, 0, gaussian);
        }
    }
    return gaussian_filter;
}

image add_image(image a, image b)
{
    // TODO
    assert(a.w == b.w);
    assert(a.h == b.h);
    assert(a.c == b.c);
    int w, h, c;
    image im_sum = make_image(a.w, a.h, a.c);
    for (w = 0; w < a.w; w++)
    {
        for (h = 0; h < a.h; h++)
        {
            for (c = 0; c < a.c; c++)
            {
                set_pixel(im_sum, w, h, c, get_pixel(a, w, h, c) + get_pixel(b, w, h, c));
            }
        }
    }
    return im_sum;
}

image sub_image(image a, image b)
{
    // TODO
    assert(a.w == b.w);
    assert(a.h == b.h);
    assert(a.c == b.c);
    int w, h, c;
    image im_sub = make_image(a.w, a.h, a.c);
    for (w = 0; w < a.w; w++)
    {
        for (h = 0; h < a.h; h++)
        {
            for (c = 0; c < a.c; c++)
            {
                set_pixel(im_sub, w, h, c, get_pixel(a, w, h, c) - get_pixel(b, w, h, c));
            }
        }
    }
    return im_sub;
}

image make_gx_filter()
{
    // TODO
    image gx_filter = make_image(3, 3, 1);
    set_pixel(gx_filter, 0, 0, 0, -1);
    set_pixel(gx_filter, 1, 0, 0, 0);
    set_pixel(gx_filter, 2, 0, 0, 1);
    set_pixel(gx_filter, 0, 1, 0, -2);
    set_pixel(gx_filter, 1, 1, 0, 0);
    set_pixel(gx_filter, 2, 1, 0, 2);
    set_pixel(gx_filter, 0, 2, 0, -1);
    set_pixel(gx_filter, 1, 2, 0, 0);
    set_pixel(gx_filter, 2, 2, 0, 1);
    return gx_filter;
}

image make_gy_filter()
{
    // TODO
    image gy_filter = make_image(3, 3, 1);
    set_pixel(gy_filter, 0, 0, 0, -1);
    set_pixel(gy_filter, 1, 0, 0, -2);
    set_pixel(gy_filter, 2, 0, 0, -1);
    set_pixel(gy_filter, 0, 1, 0, 0);
    set_pixel(gy_filter, 1, 1, 0, 0);
    set_pixel(gy_filter, 2, 1, 0, 0);
    set_pixel(gy_filter, 0, 2, 0, 1);
    set_pixel(gy_filter, 1, 2, 0, 2);
    set_pixel(gy_filter, 2, 2, 0, 1);
    return gy_filter;
}

void feature_normalize(image im)
{
    // TODO
    int h, w, c;
    float pixel, maxv, minv;
    maxv = -INFINITY;
    minv = +INFINITY;
    for (c = 0; c < im.c; ++c)
    {
        for (h = 0; h < im.h; ++h)
        {
            for (w = 0; w < im.w; ++w)
            {
                pixel = get_pixel(im, w, h, c);
                if (pixel < minv)
                {
                    minv = pixel;
                }
                if (pixel > maxv)
                {
                    maxv = pixel;
                }
            }
        }
    }
    float range = maxv - minv;
    for (c = 0; c < im.c; ++c)
    {
        for (h = 0; h < im.h; ++h)
        {
            for (w = 0; w < im.w; ++w)
            {
                if (range != 0)
                {
                    pixel = get_pixel(im, w, h, c);
                    set_pixel(im, w, h, c, (pixel - minv) / range);
                }
                else
                {
                    set_pixel(im, w, h, c, 0);
                }
            }
        }
    }
}

image *sobel_image(image im)
{
    image *mag_and_direct = calloc(2, sizeof(image));

    mag_and_direct[0] = make_image(im.w, im.h, 1);
    mag_and_direct[1] = make_image(im.w, im.h, 1);

    image gx_filter = make_gx_filter();
    image gy_filter = make_gy_filter();

    image gx_image = convolve_image(im, gx_filter, 0);
    image gy_image = convolve_image(im, gy_filter, 0);

    int w, h;
    float x, y;
    for (w = 0; w < im.w; w++)
    {
        
        for (h = 0; h < im.h; h++)
        {
            
            x = get_pixel(gx_image, w, h, 0);
            y = get_pixel(gy_image, w, h, 0);
            
            set_pixel(mag_and_direct[0], w, h, 0, sqrtf(x * x + y * y));
            set_pixel(mag_and_direct[1], w, h, 0, atan2(y,x));
        }
    }
    return mag_and_direct;
}

image colorize_sobel(image im)
{
    image *sobel = sobel_image(im);
    image color_sobel = make_image(im.w, im.h, im.c);
    feature_normalize(sobel[0]);
    feature_normalize(sobel[1]);
    float S, V, H;
    for (int w=0; w<im.w; w++){
        for (int h=0; h<im.h; h++){
            S = get_pixel(sobel[0], w, h, 0);
            V = get_pixel(sobel[0], w, h, 0);
            H = get_pixel(sobel[1], w, h, 0);

            set_pixel(color_sobel, w, h, 0, H);
            set_pixel(color_sobel, w, h, 1, S);
            set_pixel(color_sobel, w, h, 2, V);
            
        }
    }
    hsv_to_rgb(color_sobel);

    return color_sobel;
}
