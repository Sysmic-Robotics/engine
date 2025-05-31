#!/bin/bash

# Define source and destination directories
SRC_DIR="./src/protobuf/protos"
DST_DIR="./src/protobuf"

# Use the protoc version 3.6.1 compiled by the project
PROTOC="./build/project_protobuf-prefix/bin/protoc"

# Check if protoc exists, if not build it first
if [ ! -f "$PROTOC" ]; then
    echo "protoc not found. Building protobuf first..."
    cmake --build build --target project_protobuf
    if [ ! -f "$PROTOC" ]; then
        echo "Error: Failed to build protoc"
        exit 1
    fi
fi

# Ensure the destination directory exists
mkdir -p "$DST_DIR"

# Compile grSim proto files
"$PROTOC" -I="$SRC_DIR" --cpp_out="$DST_DIR" "$SRC_DIR/grSim_Commands.proto"
"$PROTOC" -I="$SRC_DIR" --cpp_out="$DST_DIR" "$SRC_DIR/grSim_Packet.proto"
"$PROTOC" -I="$SRC_DIR" --cpp_out="$DST_DIR" "$SRC_DIR/grSim_Replacement.proto"

# Compile SSL Vision proto files
"$PROTOC" -I="$SRC_DIR" --cpp_out="$DST_DIR" "$SRC_DIR/ssl_vision_detection.proto"
"$PROTOC" -I="$SRC_DIR" --cpp_out="$DST_DIR" "$SRC_DIR/ssl_vision_geometry.proto"
"$PROTOC" -I="$SRC_DIR" --cpp_out="$DST_DIR" "$SRC_DIR/ssl_vision_wrapper.proto"

echo "Compilation complete. Output in $DST_DIR."