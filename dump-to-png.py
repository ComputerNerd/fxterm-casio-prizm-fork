from PIL import Image
import tqdm

import math
import struct
import sys
import os

output_fname = sys.argv[2]
output_fname_base, output_fname_ext = os.path.splitext(output_fname)

with open(sys.argv[1], 'rb') as f:
    raw_data_all = f.read()

bytes_per_block = 1327104
for i in range(0, len(raw_data_all), bytes_per_block):
    raw_data = raw_data_all[i : i + bytes_per_block]

    image_size = (384, math.ceil(len(raw_data) / 768))
    print(image_size)
    img = Image.new('RGB', image_size)
    pixel_data = struct.unpack('>%dH' % (len(raw_data) // 2, ), raw_data)
    for k, v in tqdm.tqdm(enumerate(pixel_data)):
        r = (v & 31) * 255 // 31
        g = ((v >> 5) & 63) * 255 // 63
        b = ((v >> 11) & 31) * 255 // 31


        xy_tuple = (k % 384, k // 384)
        try:
            img.putpixel(xy_tuple, (r, g, b))
        except:
            print('Exception', xy_tuple)
            break

    idx = i // bytes_per_block
    img.save('%s_%d%s' % (output_fname_base, idx, output_fname_ext))

