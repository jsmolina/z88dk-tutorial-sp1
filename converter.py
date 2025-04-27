import struct
import sys
import argparse

# Perfil de disco por defecto
dsk_creator = b'DSKTOOL.ZXJOGV'
dsk_profiles = {
    'default': {
        'num_tracks': 40,
        'num_sides': 1,
        'num_sectors': 9,
        'sector_size': 512,
        'rw_gap': 0x2a,
        'fmt_gap': 0x52,
        'filler_byte': 0xe5,
    }
}
dsk = dsk_profiles['default']

def show_usage():
    print("Uso: dsktool.py -b <bootloader> -o <output.dsk> -l <loader> [binaries...]")
    sys.exit(1)

def disk_info_block_bytes():
    # Equivalente a pack('C[34]C[14]CCvC[204]', ...)
    header = b"MV - CPCEMU Disk-File\r\nDisk-Info\r\n"
    header = header.ljust(34, b'\x00')
    creator = dsk_creator.ljust(14, b'\x00')
    num_tracks = dsk['num_tracks']
    num_sides = dsk['num_sides']
    track_size = 256 + dsk['num_sectors'] * dsk['sector_size']
    unused = bytes(204)
    return struct.pack(
        f"{len(header)}s{len(creator)}sBBH{len(unused)}s",
        header, creator, num_tracks, num_sides, track_size, unused
    )

def track_info_block_bytes(track, side):
    # Equivalente a pack('C[24]', ...)
    header = b"Track-Info\r\n".ljust(24, b'\x00')
    unused4 = bytes(4)
    unused2 = bytes(2)
    sector_size = dsk['sector_size'] // 256
    tib = struct.pack(
        "24s4sBB2sBBBB",
        header, unused4, track, side, unused2,
        sector_size, dsk['num_sectors'], dsk['rw_gap'], dsk['filler_byte']
    )
    # Sectores
    for sector_id in range(1, dsk['num_sectors'] + 1):
        tib += struct.pack(
            "BBBBBB2s",
            track, side, sector_id, sector_size, 0, 0, bytes(2)
        )
    # Relleno hasta 256 bytes
    tib = tib.ljust(256, b'\x00')
    return tib

def pcw_boot_record():
    # Equivalente a pack("C*", ...)
    fields = [
        0, 0, dsk['num_tracks'], dsk['num_sectors'],
        dsk['sector_size'] // 256, 1, 3, 2,
        dsk['rw_gap'], dsk['fmt_gap']
    ] + [0]*5 + [0]
    return struct.pack("B"*len(fields), *fields)

def build_boot_sector(code_bytes):
    sector_bytes = pcw_boot_record() + code_bytes
    sector_bytes = sector_bytes.ljust(dsk['sector_size'], bytes([dsk['filler_byte']]))
    sector_list = list(sector_bytes)
    cksum = sum(sector_list) % 256
    # Ajustar byte de checksum en posición 0x0F para que el nuevo checksum sea 3
    fiddle_byte = (256 + 3 - cksum) % 256
    sector_list[0x0F] = fiddle_byte
    return bytes(sector_list)

def load_binary(filename):
    with open(filename, "rb") as f:
        return f.read()

def split_binary_in_tracks(binary):
    track_size = dsk['num_sectors'] * dsk['sector_size']
    tracks = []
    offset = 0
    while offset < len(binary):
        end = min(offset + track_size, len(binary))
        track_bytes = binary[offset:end]
        if len(track_bytes) < track_size:
            track_bytes = track_bytes.ljust(track_size, bytes([dsk['filler_byte']]))
        tracks.append(track_bytes)
        offset += track_size
    return tracks

def main():
    print("DSKTOOL - Generate a bootable ZX Spectrum +3 disc image")
    parser = argparse.ArgumentParser()
    parser.add_argument("-b", required=True, help="Bootloader file")
    parser.add_argument("-o", required=True, help="Output DSK file")
    parser.add_argument("-l", required=True, help="Loader file")
    parser.add_argument("binaries", nargs="*")
    args = parser.parse_args()

    bootloader_code = load_binary(args.b)
    loader_code = load_binary(args.l)

    print(f" Bootloader: {args.b}")
    print(f" Bootloader size: {len(bootloader_code)} bytes")
    print(f" Loader: {args.l}")
    print(f" Loader size: {len(loader_code)} bytes")
    print(" Binaries:", ", ".join(args.binaries))
    print(f" Output image: {args.o}")

    with open(args.o, "wb") as dsk_file:
        dsk_file.write(disk_info_block_bytes())
        # Track 0
        dsk_file.write(track_info_block_bytes(0, 0))
        dsk_file.write(build_boot_sector(bootloader_code))
        dsk_file.write(loader_code)
        # Padding
        dsk_file.write(bytes([dsk['filler_byte']] * (dsk['sector_size'] * 8 - len(loader_code))))
        # Data tracks
        data_tracks = []
        tracks_per_bin = {}
        for binfile in args.binaries:
            tracks = split_binary_in_tracks(load_binary(binfile))
            data_tracks.extend(tracks)
            tracks_per_bin[binfile] = len(tracks)
        current_track = 1
        for track_bytes in data_tracks:
            dsk_file.write(track_info_block_bytes(current_track, 0))
            dsk_file.write(track_bytes)
            current_track += 1
        while current_track < 40:
            dsk_file.write(track_info_block_bytes(current_track, 0))
            dsk_file.write(bytes([dsk['filler_byte']] * (dsk['sector_size'] * dsk['num_sectors'])))
            current_track += 1
    print("Tracks per binary:")
    for binfile, num_tracks in tracks_per_bin.items():
        print(f" {binfile}: {num_tracks} track(s)")

if __name__ == "__main__":
    main()
