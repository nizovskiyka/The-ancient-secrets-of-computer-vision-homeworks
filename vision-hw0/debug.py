from uwimg import *
'''Test debug'''

# im = load_image("data/dots.png")

EPS = .005

def within_eps(a, b):
    return a-EPS<b and b<a+EPS

# 4. Shift Image
im = load_image("data/dog.jpg")
c = load_image("data/dog.jpg")
shift_image(c, 0, .1)
print("Bad")
print(within_eps(im.data[0], c.data[0]))
print(im.data[0])
print(c.data[0])
print("Good")
print(within_eps(im.data[im.w*im.h+13] + .1,  c.data[im.w*im.h + 13]))
print(im.data[im.w*im.h + 13])
print(c.data[im.w*im.h + 13])