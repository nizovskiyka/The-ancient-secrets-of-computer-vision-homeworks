from uwimg import *
im = load_image("data/dog.jpg")
res = sobel_image(im)
mag = res[0]
feature_normalize(mag)
save_image(mag, "magnitude")
grad = res[1]
feature_normalize(grad)
save_image(grad, "gradient")