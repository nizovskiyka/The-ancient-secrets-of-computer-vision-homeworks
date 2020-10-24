#include <math.h>
#include "image.h"

float nn_interpolate(image im, float x, float y, int c)
{
    return get_pixel(im, roundf(x), roundf(y), c);
}

image nn_resize(image im, int w, int h)
{
    int i,j,k;
    float h_scaling_factor, w_scaling_factor, h_displacement_factor, w_displacement_factor;
    image resized_image = make_image(w, h, im.c);

    w_scaling_factor = (float)im.w/w;
    h_scaling_factor = (float)im.h/h;
    w_displacement_factor = -0.5 + 0.5*w_scaling_factor;
    h_displacement_factor = -0.5 + 0.5*h_scaling_factor;
    

    for(k = 0; k < resized_image.c; ++k)
    {
        for(j = 0; j < resized_image.h; ++j)
        {
            for(i = 0; i < resized_image.w; ++i)
            {
                set_pixel(resized_image, i, j, k, nn_interpolate(im, (i*w_scaling_factor) + w_displacement_factor, (j*h_scaling_factor) + h_displacement_factor, k));
            }
        }
    }
    return resized_image;
}

float bilinear_interpolate(image im, float x, float y, int c)
{
    float x_floor, y_floor;
    x_floor = floor(x);
    y_floor = floor(y);
    float d1,d2,d3,d4,res1,res2,res3,res4;
    d1=x-x_floor;
    d2=x_floor+1-x;
    d3=y_floor+1-y;
    d4=y-y_floor;
    res1 = get_pixel(im, x_floor, y_floor+1, c)*d2*d4;
    res2 = get_pixel(im, x_floor+1, y_floor+1, c)*d1*d4;
    res3 = get_pixel(im, x_floor, y_floor, c)*d2*d3;
    res4 = get_pixel(im, x_floor+1, y_floor, c)*d1*d3;
    return res1+res2+res3+res4;
}



image bilinear_resize(image im, int w, int h)
{
    int i,j,k;
    float h_scaling_factor, w_scaling_factor, h_displacement_factor, w_displacement_factor;
    image resized_image = make_image(w, h, im.c);

    w_scaling_factor = (float)im.w/w;
    h_scaling_factor = (float)im.h/h;
    w_displacement_factor = -0.5 + 0.5*w_scaling_factor;
    h_displacement_factor = -0.5 + 0.5*h_scaling_factor;
    

    for(k = 0; k < resized_image.c; ++k)
    {
        for(j = 0; j < resized_image.h; ++j)
        {
            for(i = 0; i < resized_image.w; ++i)
            {
                set_pixel(resized_image, i, j, k, bilinear_interpolate(im, (i*w_scaling_factor) + w_displacement_factor, (j*h_scaling_factor) + h_displacement_factor, k));
            }
        }
    }
    return resized_image;
}

