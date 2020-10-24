#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"

float get_pixel(image im, int x, int y, int c)
{
    // Return the pixel value at columx x, row y, channel c
    // Padding (Clamp strategy))
    if(x>=im.w)
    {
        x = im.w-1;
    }
    if(x<=0)
    {
        x = 0;
    }
    if (y>=im.h)
    {
        y = im.h-1;
    }
    if(y<=0)
    {
        y = 0;
    }
    if (c>=im.c)
    {
        c = im.c-1;
    }
    if(c<=0)
    {
        c = 0;
    }
    
    return im.data[x + im.w*y + im.w*im.h*c];
}

void set_pixel(image im, int x, int y, int c, float v)
{
    // Set the pixel value of column x, row y, channel c to v
    im.data[x + im.w*y + im.w*im.h*c] = v;
}

image copy_image(image im)
{
    int i,j,k;
    int index=0;
    image copy = make_image(im.w, im.h, im.c);
    for(k = 0; k < im.c; ++k)
    {
        for(j = 0; j < im.h; ++j)
        {
            for(i = 0; i < im.w; ++i)
            {
                index = i + im.w*j + im.w*im.h*k;
                copy.data[index] = im.data[index];
            }
        }
    }
    return copy;
}

image rgb_to_grayscale(image im)
{
    int i,j;
    int index_r=0;
    int index_g=0;
    int index_b=0;
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);
    for(j = 0; j < im.h; ++j)
    {
        for(i = 0; i < im.w; ++i)
        {
            index_r = i + im.w*j;
            index_g = i + im.w*j + im.w*im.h*1;
            index_b = i + im.w*j + im.w*im.h*2;
            gray.data[index_r] = im.data[index_r]*0.299 + im.data[index_g]*0.587 + im.data[index_b]*0.114;
        }
    }
    return gray;
}

void shift_image(image im, int c, float v)
{
    // TODO Fill this in
    int i,j;
    int index=0;
    for(j = 0; j < im.h; ++j)
    {
        for(i = 0; i < im.w; ++i)
        {
            index = i + im.w*j + im.w*im.h*c;
            im.data[index] = im.data[index]+v;
        }
    }
}

void clamp_image(image im)
{
    int i,j,k;
    int index=0;
    for(k = 0; k < im.c; ++k)
    {
        for(j = 0; j < im.h; ++j)
        {
            for(i = 0; i < im.w; ++i)
            {
                index = i + im.w*j + im.w*im.h*k;
                if(im.data[index]<0)
                {
                    im.data[index] = 0;
                }
                if(im.data[index]>1)
                {
                    im.data[index] = 1;
                }
            }
        }
    }
}


// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

void rgb_to_hsv(image im)
{
    int i,j;
    int index_r=0;
    int index_g=0;
    int index_b=0;
    float V=0;
    float m=0;
    float S=0;
    float dH=0;
    float H=0;
    float C=0;
    for(j = 0; j < im.h; ++j)
    {
        for(i = 0; i < im.w; ++i)
        {
            index_r = i + im.w*j;
            index_g = i + im.w*j + im.w*im.h*1;
            index_b = i + im.w*j + im.w*im.h*2;
            //Calculating Value
            V = three_way_max(im.data[index_r], im.data[index_g], im.data[index_b]);
            m = three_way_min(im.data[index_r], im.data[index_g], im.data[index_b]);
            C = V-m;

            //Calculating Saturation
            if(V==0)
            {
                S = 0;
            }
            else
            {
                S = (V-m)/V;
            }
            
            //Calculating Hue
            if(C==0)
            {
                H=0;
            }
            else
            {
                if(V==im.data[index_r])
                {
                    dH = (im.data[index_g] - im.data[index_b])/C;
                    H = (dH<0) ? dH/6 + 1 : dH/6;
                }
                else if (V==im.data[index_g])
                {
                    dH = (im.data[index_b] - im.data[index_r])/C + 2;
                    H = (dH<0) ? dH/6 + 1 : dH/6;
                }
                else if (V==im.data[index_b])
                {
                    dH = (im.data[index_r] - im.data[index_g])/C + 4;
                    H = (dH<0) ? dH/6 + 1 : dH/6;
                }
                
                
            }
            //while(H<0){H++;}
            //while(H>1){H--;}
            im.data[index_r] = H;
            im.data[index_g] = S;
            im.data[index_b] = V;
        }
    }
}

void hsv_to_rgb(image im)
{   
    // Here red - H, green - S, blue - V
    int index_r=0;
    int index_g=0;
    int index_b=0;
    int i,j;
    float C, dH, X, m, tmp;
    for(j = 0; j < im.h; ++j)
    {
        for(i = 0; i < im.w; ++i)
        {
            index_r = i + im.w*j;
            index_g = i + im.w*j + im.w*im.h*1;
            index_b = i + im.w*j + im.w*im.h*2;
            C = im.data[index_b] * im.data[index_g];
            m=im.data[index_b]-C;
            if(C==0)
            {
                tmp = im.data[index_b];
                im.data[index_r] = tmp;
                im.data[index_g] = tmp;
                im.data[index_b] = tmp;
            }
            else
            {
                dH = im.data[index_r]*6;

                X = C*(1 - fabs(fmod(dH, 2) - 1));

                if(dH>=0 && dH <=1)
                {
                    im.data[index_r] = C+m;
                    im.data[index_g] = X+m;
                    im.data[index_b] = 0+m;
                }
                else if(dH>=1 && dH <=2)
                {
                    im.data[index_r] = X+m;
                    im.data[index_g] = C+m;
                    im.data[index_b] = 0+m;
                }
                else if(dH>=2 && dH <=3)
                {
                    im.data[index_r] = 0+m;
                    im.data[index_g] = C+m;
                    im.data[index_b] = X+m;
                }
                else if(dH>=3 && dH <=4)
                {
                    im.data[index_r] = 0+m;
                    im.data[index_g] = X+m;
                    im.data[index_b] = C+m;
                }
                else if(dH>=4 && dH <=5)
                {
                    im.data[index_r] = X+m;
                    im.data[index_g] = 0+m;
                    im.data[index_b] = C+m;
                }
                else if(dH>=5 && dH <=6)
                {
                    im.data[index_r] = C+m;
                    im.data[index_g] = 0+m;
                    im.data[index_b] = X+m;
                }
            }

        }
    }
}
