MAX_DUMP_LEN = 16384

from PIL import Image
import tqdm

import math
import serial
import struct
import sys
import zlib

start_address = int(sys.argv[2], 16) # Force hex by specifying that it's a base-16 number. Remove the , 16 to remove this behavior.
bytes_to_dump = int(sys.argv[3])
filename_to_save_to = sys.argv[4]

def to_be_unsigned_int(val):
    return struct.pack('>I', val)

def from_be_unsigned_int(inpt):
    return struct.unpack('>I', inpt)[0]

def from_unsigned_char(inpt):
    return struct.unpack('B', inpt)[0]

with serial.Serial(sys.argv[1], 115200, timeout = 2) as ser:
    request_length = MAX_DUMP_LEN # How many bytes per request.
    n_blocks = math.ceil(bytes_to_dump / request_length)
    with open(filename_to_save_to, 'wb') as f:
        for i in tqdm.tqdm(range(n_blocks)):
            def get_data(fetch_bytes, addr, n_tries = 0):
                if n_tries > 10:
                    print('Too many retries.')
                    return None
                ser.write(to_be_unsigned_int(addr))
                ser.write(to_be_unsigned_int(fetch_bytes))

                calculator_response = ser.read(1)
                calculator_response_handled = from_unsigned_char(calculator_response)
                is_accepted = calculator_response_handled  == 1
                if is_accepted:
                    # Get the checksum and data.
                    expected_checksum = from_be_unsigned_int(ser.read(4))
                    read_data = ser.read(fetch_bytes)
                    actual_checksum = zlib.crc32(read_data)
                    if actual_checksum == expected_checksum:
                        print('Checksums match: %X' % expected_checksum)
                        return read_data
                    else:
                        print('ERROR: checksum mismatch expected: %X but got %X' % (expected_checksum, actual_checksum))
                        # Try splitting it up into half.
                        fb_half = fetch_bytes // 2
                        if (fetch_bytes % fb_half) == 0:
                            raw_tmp = b''
                            for i in range(2):
                                half_data = get_data(fb_half, addr + (fb_half * i), n_tries + 1)
                                if half_data is None:
                                    print('Wrote unknown starting at', addr + (fb_half * i), 'with a length of', fb_half)
                                    half_data = b'U' * fb_half
                                raw_tmp += half_data
                            return raw_tmp
                        else:
                            raw_tmp = get_data(fetch_bytes, addr, n_tries + 1)
                            if raw_tmp is None:
                                print('Wrote unknown starting at', addr, 'with a length of', fetch_bytes)
                                return b'U' * fetch_bytes # U stands for unknown.
                            else:
                                return raw_tmp

                else:
                    print('Requested rejected by the calculator.', calculator_response, calculator_response_handled)
            start_byte = i * request_length
            end_byte = min(start_byte + request_length, bytes_to_dump)
            req_len = end_byte - start_byte
            res = get_data(req_len, start_address + start_byte)
            f.write(res)
