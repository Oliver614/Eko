

# stitchImages.py

# pip install Pillow

import sys
from PIL import Image

imagesArray = ['Knob0005'+'.png'+str('%04d'%image) for image in range(2, 256+1)]
# Example: 'Rotating Knob Animation 256f 128x150/0001.png' to 'Rotating Knob Animation 256f 128x150/0256.png'
images = [Image.open(x) for x in imagesArray]

widths, heights = zip(*(i.size for i in images))

def stitchXRight():
    total_width = sum(widths)
    max_height = max(heights)
    new_im = Image.new('RGBA', (total_width, max_height))
    x_offset = 0
    for im in images:
        new_im.paste(im, (x_offset, 0))
        x_offset += im.size[0]
        print('Add Image "%s"..'%im.filename)
    return new_im

def stitchYDown():
    total_width = max(widths)
    max_height = sum(heights)
    new_im = Image.new('RGBA', (total_width, max_height))
    y_offset = 0
    for im in images:
        new_im.paste(im, (0, y_offset))
        y_offset += im.size[1]
        print('Add Image "%s"..'%im.filename)
    return new_im

stitchYDown().save('bigKnob.png')
