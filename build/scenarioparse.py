
print("uint8_t map[25][16] = {")
with open('pacmansce.csv', 'r') as f:
    for linenum, row in enumerate(f):
        if(linenum == 0):
            continue
        if(linenum == 25):
            break

        resultcols = []
        cols = row.split(',')
        for i in range(1, len(cols) - 1, 2):
            x = (int(cols[i]) << 4) | int(cols[i+1])

            resultcols.append(str(x))

        print("{" + (','.join(resultcols)) + "},")

print("};")
# compress x = (number1 << 4) | number2
# decompress number1 = (x & 0b11110000) >> 4, number2 = (x & 0b00001111)