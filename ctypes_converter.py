import re

def convert_etParam_header_to_ctypes(header_file, output_file):
    # Define the pattern for the _PHX_ENUM macro
    phx_enum_pattern = re.compile(r"_PHX_ENUM\(\s*(\w+),\s*(\w+),\s*(\d+)\s*\)")

    with open(header_file, 'r') as f:
        header_content = f.read()

    # Initialize the ctypes output
    ctypes_content = "from ctypes import c_int\n\nclass etParam:\n"

    # Process the lines
    for line in header_content.splitlines():
        # Check for _PHX_ENUM macro usage
        match = phx_enum_pattern.match(line.strip())
        if match:
            enum_group, enum_name, enum_value = match.groups()
            # Calculate the enum value based on the _PHX_ENUM macro definition
            calculated_value = (int(enum_group, 0) | (int(enum_value) << 8))
            ctypes_content += f"    {enum_name} = {calculated_value}\n"
        else:
            # Handle regular enum definitions
            if '=' in line:
                name, value = line.split('=', 1)
                name = name.strip()
                value = value.strip().rstrip(',')
                ctypes_content += f"    {name} = {value}\n"
            elif '}' not in line and '{' not in line and line.strip():
                ctypes_content += f"    {line.strip()} = None\n"

    # Write the ctypes output to the file
    with open(output_file, 'w') as f:
        f.write(ctypes_content)

# Use the function to convert the header file
convert_etParam_header_to_ctypes('etParam.h', 'ctypes_header.py')