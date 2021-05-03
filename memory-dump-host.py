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
                        return get_data(n_tries + 1)
                else:
                    print('Requested rejected by the calculator.', calculator_response, calculator_response_handled)
            start_byte = i * request_length
            end_byte = min(start_byte + request_length, bytes_to_dump)
            req_len = end_byte - start_byte
            res = get_data(req_len, start_address + start_byte)
            if res is None:
                print('Wrote unknown starting at', start_byte, 'with a length of', req_len)
                f.write(b'U' * req_len) # U for unknown.
            else:
                f.write(res)
