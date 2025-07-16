#!/bin/bash

ENGINE="/home/gerson/Desktop/engine/build/src/engine"
LUA_SCRIPT="/home/gerson/Desktop/strategy/main.lua"

echo "[Watchdog] Starting engine watchdog..."

while true; do
  echo "[Watchdog] Launching engine..."
  "$ENGINE" "$LUA_SCRIPT"
  EXIT_CODE=$?

  if [ $EXIT_CODE -ne 0 ]; then
    echo "[Watchdog] Engine crashed with exit code $EXIT_CODE. Restarting in 2 seconds..."
    sleep 2
  else
    echo "[Watchdog] Engine exited normally with code $EXIT_CODE. Stopping watchdog."
    break
  fi
done
