#!/bin/bash
echo "Starting display service..."
if ! screen -list | grep -q 'arctic-display'; then # make sure we don't start too many instances
    screen -dmS arctic-display bash -c "cd subarctic/display && python3 ./server.py"
    sleep 1
    if ! screen -list | grep -q 'arctic-display'; then # make sure we don't start too many instances
		echo "Failed to start display service!"
		exit -1
	else
		echo "Started display service!"
	fi
else
    echo "Display service already running."
fi

echo "Starting input service..."
if ! screen -list | grep -q 'arctic-input'; then # make sure we don't start too many instances
    screen -dmS arctic-input bash -c "cd subarctic/input && python3 ./server.py"
    sleep 1
    if ! screen -list | grep -q 'arctic-input'; then # make sure we don't start too many instances
		echo "Failed to start input service!"
		exit -1
	else
		echo "Started input service!"
	fi
else
    echo "Input service already running."
fi

echo "Waiting to let services finish starting..."
sleep 2
echo "Starting launcher..."
cd frost
python3 launcher.py
