#!/bin/bash

./pause -t 60 > victim_win.csv &
BACK_PID=$!

python run_scheduleak.py -n 55 -o attacker_data_win.csv

wait $BACK_PID
echo "victim task has finished"
