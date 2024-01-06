# Simple script to convert file passed in argument to bin file
import sys

if len(sys.argv) < 2:
    print("Too few arguments. Insert a filename to convert!")

file_to_convert = sys.argv[1]
buffer          = ""
result_file     = file_to_convert.replace(".html", ".bin")

buffer = open(file_to_convert, "r").read()

with open(result_file, "wb") as bin_file:
    for c in buffer:
        if c not in ['\n', '\t', '\r']:
            bin_file.write(bytes(c, 'utf-8'))

print(f"> Binary file successfully created, size: {len(buffer)}")


