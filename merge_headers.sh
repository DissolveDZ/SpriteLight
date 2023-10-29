#!/bin/bash

# Source directory for the header files
SOURCE_DIR="SpriteLight/engine_include"

# Output directory for the merged header
OUTPUT_DIR="include"
OUTPUT_DIR_SPRITELIGHT="SpriteLight/include"

# Output file name
OUTPUT_FILE="engine.h"

# Create an empty output file in the source directory
> "$SOURCE_DIR/$OUTPUT_FILE"

# Add `#pragma once` at the beginning of the output file
echo "#pragma once" > "$SOURCE_DIR/$OUTPUT_FILE"

# Extract the include order from SpriteLight.h
INCLUDE_ORDER=$(sed -n -e '/#include/s/#include "\(.*\)"/\1/p' "$SOURCE_DIR/SpriteLight.h")

# Loop through the header files in the specified order, appending them with newlines
for header in $INCLUDE_ORDER; do
    cat "$SOURCE_DIR/$header" >> "$SOURCE_DIR/$OUTPUT_FILE"
    echo -e "\n" >> "$SOURCE_DIR/$OUTPUT_FILE" # Add a newline after the header
done

# Copy the merged header to the output directories
cp "$SOURCE_DIR/$OUTPUT_FILE" "$OUTPUT_DIR/$OUTPUT_FILE"
cp "$SOURCE_DIR/$OUTPUT_FILE" "$OUTPUT_DIR_SPRITELIGHT/$OUTPUT_FILE"
