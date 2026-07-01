#!/usr/bin/env bash

set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
SKETCH_NAME="DubaiCity"
BUILD_ROOT_DIR="$ROOT_DIR/.arduino-cli-build"
BUILD_SKETCH_DIR="$BUILD_ROOT_DIR/$SKETCH_NAME"
DEFAULT_PORT="/dev/cu.usbmodem14601"
DEFAULT_FQBN="esp32:esp32:esp32c6:CDCOnBoot=cdc"
DEFAULT_BAUD="115200"

usage() {
  cat <<'EOF'
Usage:
  ./scripts/arduino-cli.sh compile
  ./scripts/arduino-cli.sh upload
  ./scripts/arduino-cli.sh both
  ./scripts/arduino-cli.sh monitor
  ./scripts/arduino-cli.sh board-list

Optional environment variables:
  PORT=/dev/cu.usbmodem14601
  FQBN=esp32:esp32:esp32c6:CDCOnBoot=cdc
  BAUD=115200

Examples:
  ./scripts/arduino-cli.sh compile
  PORT=/dev/cu.usbmodem14601 ./scripts/arduino-cli.sh upload
  FQBN=esp32:esp32:esp32c6:CDCOnBoot=cdc ./scripts/arduino-cli.sh both
  BAUD=115200 ./scripts/arduino-cli.sh monitor
EOF
}

require_tool() {
  if ! command -v arduino-cli >/dev/null 2>&1; then
    echo "arduino-cli wurde nicht gefunden."
    echo "Installiere es z. B. mit: brew install arduino-cli"
    exit 1
  fi
}

prepare_sketch_dir() {
  rm -rf "$BUILD_ROOT_DIR"
  mkdir -p "$BUILD_SKETCH_DIR"
  cp "$ROOT_DIR/$SKETCH_NAME.ino" "$BUILD_SKETCH_DIR/$SKETCH_NAME.ino"
}

compile_sketch() {
  prepare_sketch_dir
  arduino-cli compile --fqbn "${FQBN:-$DEFAULT_FQBN}" "$BUILD_SKETCH_DIR"
}

upload_sketch() {
  prepare_sketch_dir
  arduino-cli upload \
    -p "${PORT:-$DEFAULT_PORT}" \
    --fqbn "${FQBN:-$DEFAULT_FQBN}" \
    "$BUILD_SKETCH_DIR"
}

monitor_port() {
  arduino-cli monitor \
    -p "${PORT:-$DEFAULT_PORT}" \
    --config "baudrate=${BAUD:-$DEFAULT_BAUD}"
}

main() {
  require_tool

  case "${1:-}" in
    compile)
      compile_sketch
      ;;
    upload)
      upload_sketch
      ;;
    both)
      compile_sketch
      upload_sketch
      ;;
    monitor)
      monitor_port
      ;;
    board-list)
      arduino-cli board list
      ;;
    *)
      usage
      exit 1
      ;;
  esac
}

main "${1:-}"
