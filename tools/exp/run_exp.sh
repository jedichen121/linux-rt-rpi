#!/bin/bash

./pause -t 60 > victim.csv &
BACK_PID=$!

python run_scheduleak.py -n 55 -o attacker_data.csv

wait $BACK_PID
echo "victim task has finished"
