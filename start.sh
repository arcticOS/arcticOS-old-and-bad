#!/bin/bash
echo "Starting display service..."
if ! screen -list | grep -q 'arctic-display'; then # make sure we don't start too many instances
    screen -dmS arctic-display bash -c "cd services/display && ./server.py"
    echo "Started display service."
else
    echo "Display service already running."
fi

echo "Starting input service..."
if ! screen -list | grep -q 'arctic-input'; then # make sure we don't start too many instances
    screen -dmS arctic-input bash -c "cd services/input && ./server.py"
    echo "Started input service."
else
    echo "Input service already running."
fi

echo "Waiting to let services finish starting..."
sleep 2
echo "Starting launcher..."
cd launcher
python3 launcher.py