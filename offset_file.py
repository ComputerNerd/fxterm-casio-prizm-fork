import sys
in_fname = sys.argv[1]
out_fname = sys.argv[2]
start_byte = int(sys.argv[3])
if len(sys.argv) > 4:
    length = int(sys.argv[4])
else:
    length = None


if in_fname == out_fname:
    print('Filenames must not match.')
else:
    with open(in_fname, 'rb') as f:
        all_dat = f.read()

    with open(out_fname, 'wb') as f:
        if length is None:
            f.write(all_dat[start_byte : ])
        else:
            f.write(all_dat[start_byte : start_byte + length])
