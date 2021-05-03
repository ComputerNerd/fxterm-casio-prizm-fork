from PIL import Image
import tqdm

import math
import struct
import sys

output_fname = sys.argv[2]

with open(sys.argv[1], 'rb') as f:
    raw_data = f.read()

img = Image.new('RGB', (384, math.ceil(len(raw_data) // 768)))
pixel_data = struct.unpack('>%dH' % (len(raw_data) // 2, ), raw_data)
for k, v in tqdm.tqdm(enumerate(pixel_data)):
    r = (v & 31) * 255 // 31
    g = ((v >> 5) & 63) * 255 // 63
    b = ((v >> 11) & 31) * 255 // 31
    img.putpixel((k % 384, k // 384), (r, g, b))
img.save(output_fname)

