#!/bin/bash
echo "Starting display service..."
if ! screen -list | grep -q 'arctic-display'; then # make sure we don't start too many instances
    screen -dmS arctic-display bash -c "cd services/display && ./server.py"
    echo "Started display service."
else
    echo "Display service already running."
fi

echo "Waiting to let services finish starting..."
sleep 2
echo "Starting launcher..."
cd launcher
python3 launcher.py