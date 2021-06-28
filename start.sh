#!/bin/bash
echo "Starting arcticOS..."
if ! screen -list | grep -q 'arctic-display'; then # make sure we don't start too many instances
    screen -dmS arctic-display ./start-displayserver.sh
    echo "Started display server."
    sleep 2
else
    echo "Display server already running."
fi
echo "Starting launcher..."
cd launcher
python3 launcher.py