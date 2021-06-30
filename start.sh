#!/bin/bash
echo "Starting display service..."
if ! screen -list | grep -q 'arctic-display'; then # make sure we don't start too many instances
    screen -dmS arctic-display bash -c "cd services/display && ./server.py"
    echo "Started display service."
    sleep 2
else
    echo "Display service already running."
fi
echo "Starting launcher..."
cd launcher
python3 launcher.py