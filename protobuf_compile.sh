#!/bin/bash

# Define source and destination directories
SRC_DIR="./src/protobuf/squemes/robocup_ssl"
DST_DIR="./src/protobuf"

# Ensure the destination directory exists
mkdir -p "$DST_DIR"

# Compile the ProtoBuf file
protoc -I="$SRC_DIR" --cpp_out="$DST_DIR" "$SRC_DIR/messages_robocup_ssl_detection.proto"
protoc -I="$SRC_DIR" --cpp_out="$DST_DIR" "$SRC_DIR/messages_robocup_ssl_wrapper.proto"
protoc -I="$SRC_DIR" --cpp_out="$DST_DIR" "$SRC_DIR/messages_robocup_ssl_geometry.proto"

SRC_DIR="./src/protobuf/squemes/grsim"
protoc -I="$SRC_DIR" --cpp_out="$DST_DIR" "$SRC_DIR/grSim_Commands.proto"
protoc -I="$SRC_DIR" --cpp_out="$DST_DIR" "$SRC_DIR/grSim_Packet.proto"
protoc -I="$SRC_DIR" --cpp_out="$DST_DIR" "$SRC_DIR/grSim_Replacement.proto"
echo "Compilation complete. Output in $DST_DIR."pro