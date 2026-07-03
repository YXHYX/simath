import struct

def parse_tdf_file(file_path):
    with open(file_path, "rb") as f:
        file_data = f.read()
    
    # Flexible check for the font file signature anywhere in the first 30 bytes
    if b"TheDraw FONTS" not in file_data[:50] and b"TheDraw fonts" not in file_data[:50]:
        print("[Warning] Standard signature not found in header, attempting to parse anyway...")
    
    fonts = {}
    file_len = len(file_data)
    
    # Scan for the font block magic delimiter: 0x55, 0xAA, 0x00, 0xFF
    pointer = 0
    while pointer < file_len - 4:
        if file_data[pointer:pointer+4] == b"\x55\xAA\x00\xFF":
            font_start = pointer
            
            # Extract font name length and the name itself
            name_len = file_data[font_start + 4]
            # Name resides in a 12-byte block from offset +5 to +17
            font_name_bytes = file_data[font_start + 5 : font_start + 17]
            font_name = font_name_bytes[:name_len].decode('ascii', errors='ignore').strip()
            
            if not font_name:
                font_name = f"Unnamed_Font_{font_start}"
                
            font_type = file_data[font_start + 21]  # 0 = Outline, 1 = Block, 2 = Color
            spacing = file_data[font_start + 22]
            block_size = struct.unpack_from("<H", file_data, font_start + 23)[0]
            
            # The 94 short integers (offsets) start at font_start + 25
            offsets_start = font_start + 25
            char_offsets = []
            for i in range(94):
                offset = struct.unpack_from("<H", file_data, offsets_start + (i * 2))[0]
                char_offsets.append(offset)
                
            # Base data offset relative to font segment start
            char_data_start = font_start + 213
            font_chars = {}
            
            # Loop through the printable ASCII range (33 '!' to 126 '~')
            for idx, offset in enumerate(char_offsets):
                ascii_code = 33 + idx
                char_symbol = chr(ascii_code)
                
                # 0xFFFF means the character is unmapped/empty
                if offset == 0xFFFF:
                    continue
                    
                data_ptr = char_data_start + offset
                matrix = []
                current_row = []
                
                while data_ptr < file_len:
                    byte1 = file_data[data_ptr]
                    
                    if font_type == 2:  # Color Font (2 bytes per grid tile)
                        if byte1 == 0x00:  # End of character entry
                            if current_row:
                                matrix.append(current_row)
                            break
                        elif byte1 == 0x0D:  # Carriage Return (New Row)
                            matrix.append(current_row)
                            current_row = []
                            data_ptr += 1
                        else:
                            char_byte = byte1
                            attr_byte = file_data[data_ptr + 1] if data_ptr + 1 < file_len else 0
                            
                            # Safely map to IBM Code Page 437 character layout
                            char_str = bytes([char_byte]).decode('cp437', errors='replace')
                            
                            current_row.append({
                                'char': char_str,
                                'fg': attr_byte % 16,
                                'bg': (attr_byte // 16) % 8
                            })
                            data_ptr += 2
                    else:  # Outline/Block Font (1 byte per grid tile)
                        if byte1 == 0x00:  # End of character
                            if current_row:
                                matrix.append(current_row)
                            break
                        elif byte1 == 0x0D:  # New Row
                            matrix.append(current_row)
                            current_row = []
                            data_ptr += 1
                        else:
                            char_str = bytes([byte1]).decode('cp437', errors='replace')
                            current_row.append({'char': char_str, 'fg': None, 'bg': None})
                            data_ptr += 1
                
                if matrix:
                    font_chars[char_symbol] = matrix
            
            fonts[font_name] = {
                'type': ['Outline', 'Block', 'Color'][font_type] if font_type < 3 else 'Unknown',
                'spacing': spacing,
                'characters': font_chars
            }
            
            # Skip past the entire current font block data segment to look for the next one
            pointer = char_data_start + block_size
        else:
            pointer += 1
            
    return fonts

# Execution Context
if __name__ == "__main__":
    try:
        # Load the uploaded file asset
        font_library = parse_tdf_file("ANOMALIX.TDF")
        
        if not font_library:
            print("Error: No fonts could be extracted. Please verify the file integrity.")
        else:
            print(f"Successfully processed file! Found {len(font_library)} fonts:")
            for name, details in font_library.items():
                print(f" -> Name: '{name}' | Type: {details['type']} | Encoded Characters: {len(details['characters'])}")
            
            # Sample Printout: Pull the letter 'A' or 'B' from the first detected schema
            sample_font = list(font_library.keys())[0]
            print(f"\n--- Visualizing letter 'B' from [{sample_font}] ---")
            
            chars_dict = font_library[sample_font]['characters']
            target_char = 'j' if 'j' in chars_dict else list(chars_dict.keys())[0]
            
            for row in chars_dict[target_char]:
                # Combine the cell matrix characters to display the full shape
                print("".join([cell['char'] for cell in row]))
                
    except FileNotFoundError:
        print("Error: Could not locate '208X.TDF'. Make sure it is in the working directory.")