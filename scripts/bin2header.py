#!/usr/bin/env python3
"""
Binary to C/C++ Header Converter
Converts binary files into C arrays for embedding in executables.
"""

import sys
import os


def bin_to_header(input_file: str, output_file: str, var_name: str) -> None:
    try:
        with open(input_file, 'rb') as f:
            data = f.read()
    except IOError as e:
        print(f"Error: Failed to read input file '{input_file}': {e}")
        sys.exit(1)
    except Exception as e:
        print(f"Error: Unexpected error reading '{input_file}': {e}")
        sys.exit(1)
    
    size = len(data)
    
    try:
        with open(output_file, 'w') as f:
            f.write(f"// Auto-generated from {os.path.basename(input_file)}\n")
            f.write(f"// Size: {size} bytes\n\n")
            f.write(f"namespace EmbeddedResources {{\n\n")
            f.write(f"extern const unsigned char {var_name}[] = {{\n")
            
            for i in range(0, size, 12):
                chunk = data[i:i+12]
                hex_values = ', '.join(f'0x{b:02x}' for b in chunk)
                f.write(f"    {hex_values},\n")
            
            f.write(f"}};\n\n")
            f.write(f"extern const unsigned int {var_name}_size = {size};\n\n")
            f.write(f"}} // namespace EmbeddedResources\n")
    except IOError as e:
        print(f"Error: Failed to write output file '{output_file}': {e}")
        sys.exit(1)
    except Exception as e:
        print(f"Error: Unexpected error writing '{output_file}': {e}")
        sys.exit(1)


def sanitize_var_name(filename: str) -> str:
    name = os.path.splitext(os.path.basename(filename))[0]
    name = name.replace('-', '_').replace('.', '_').replace(' ', '_')
    ext = os.path.splitext(filename)[1][1:]
    if ext:
        return f"{name}_{ext}"
    return name


if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: bin2header.py <input_file> <output_file>")
        sys.exit(1)
    
    input_path = sys.argv[1]
    output_path = sys.argv[2]
    
    if not os.path.exists(input_path):
        print(f"Error: Input file '{input_path}' not found")
        sys.exit(1)
    
    var_name = sanitize_var_name(input_path)
    bin_to_header(input_path, output_path, var_name)
    print(f"Generated {output_path} with variable '{var_name}'")
