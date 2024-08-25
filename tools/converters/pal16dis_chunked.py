from argparse import ArgumentParser
import os, struct


def convert_binary_to_vtx_c_source(src_path, dest_path):
    with open(src_path, "rb") as binary_file:
        # Obtain file size, iterate in 0x20 byte chunks
        file_size = binary_file.seek(0, 2)
        for i in range(0, file_size, 0x20):
            # Split the dest_path into multiple files depending on how many chunks there are
            name, ext = os.path.splitext(dest_path)
            file_name = f"{name}_{i}{ext}"
            with open(file_name, "w") as c_file:
                # Iterate each chunk and parse out the data
                for j in range(0, 0x20, 2):
                    chunk = binary_file.read(2)  # 2 bytes for each 16-bit palette entry

                    if len(chunk) < 2:
                        break

                    pal = struct.unpack(">H", chunk[0:2])  # 2 bytes

                    c_file.write(f"{hex(pal[0])}, ")


def main():
    parser = ArgumentParser(
        description="Converts a binary file to a number of u16 palettes"
    )
    parser.add_argument("src_path", type=str, help="Binary source file path")
    parser.add_argument("dest_path", type=str, help="Destination C include file path")

    args = parser.parse_args()
    convert_binary_to_vtx_c_source(args.src_path, args.dest_path)


if __name__ == "__main__":
    main()
