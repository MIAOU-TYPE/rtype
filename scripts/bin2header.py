#!/usr/bin/env python3
"""
Binary to C/C++ Header Converter
Converts binary files into C arrays for embedding in executables.
"""

import sys
import os


def bin_to_header(input_file: str, output_file: str, var_name: str) -> None:
    """
    Convert a binary file to a C header file with byte array.
    
    Args:
        input_file: Path to input binary file
        output_file: Path to output .cpp file
        var_name: Variable name for the array
    """
    with open(input_file, 'rb') as f:
        data = f.read()
    
    size = len(data)
    
    with open(output_file, 'w') as f:
        f.write(f"// Auto-generated from {os.path.basename(input_file)}\n")
        f.write(f"// Size: {size} bytes\n\n")
        f.write(f"namespace EmbeddedResources {{\n\n")
        f.write(f"extern const unsigned char {var_name}[] = {{\n")
        
        # Write bytes in rows of 12
        for i in range(0, size, 12):
            chunk = data[i:i+12]
            hex_values = ', '.join(f'0x{b:02x}' for b in chunk)
            f.write(f"    {hex_values},\n")
        
        f.write(f"}};\n\n")
        f.write(f"extern const unsigned int {var_name}_size = {size};\n\n")
        f.write(f"}} // namespace EmbeddedResources\n")


def sanitize_var_name(filename: str) -> str:
    """Convert filename to valid C variable name."""
    # Remove extension and replace special chars with underscores
    name = os.path.splitext(os.path.basename(filename))[0]
    name = name.replace('-', '_').replace('.', '_').replace(' ', '_')
    # Append extension without dot
    ext = os.path.splitext(filename)[1][1:]  # Remove leading dot
    return f"{name}_{ext}"


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
