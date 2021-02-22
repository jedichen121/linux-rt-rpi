import csv
import matplotlib.pyplot as plt
from numpy import loadtxt

offset_truth_list = []
offset_truth_list_win = []

with open('victim.csv', 'r') as file:
    reader = csv.reader(file, delimiter = '\t')
    for row in reader:
        offset_truth_list.append(int(row[0]))

with open('victim_win.csv', 'r') as file:
    reader = csv.reader(file, delimiter = '\t')
    for row in reader:
        offset_truth_list_win.append(int(row[0]))

offset_truth = sum(offset_truth_list) / len(offset_truth_list)
offset_truth_win = sum(offset_truth_list_win) / len(offset_truth_list_win)

print(offset_truth)
print(offset_truth_win)

offset_attacker_list = []
exec_time_attacker_list = []

with open('attacker_data.csv', 'r') as file:
    reader = csv.reader(file, delimiter = '\t')
    for row in reader:
        offset_attacker_list.append(abs(int(row[0]) - offset_truth) / offset_truth * 100)
        exec_time_attacker_list.append(int(row[2]))

# print(offset_attacker_list)

offset_attacker_list_win = []
exec_time_attacker_list_win = []

with open('attacker_data_win.csv', 'r') as file:
    reader = csv.reader(file, delimiter = '\t')
    for row in reader:
        offset_attacker_list_win.append(abs(int(row[0]) - offset_truth_win) / offset_truth_win * 100)
        exec_time_attacker_list_win.append(int(row[2]))

# print(offset_attacker_list_win)
plt.rcParams.update({'font.size': 14})

plt.figure(figsize=(8,4))
# plt.subplot(211)
plt.plot(offset_attacker_list_win[1:51], 'bo', label='with SchedGuard')
plt.plot(offset_attacker_list[1:51], 'rx', label='without SchedGuard')
plt.legend(loc="lower right")
plt.ylim(0.012, 0.045)
# plt.xlabel('(a)')
plt.ylabel('inference error (%)')
plt.title('Inference error on inital offset')
plt.savefig('inital.pdf', bbox_inches='tight')

# plot execution time for both cases
plt.figure(figsize=(8,4))
plt.plot(exec_time_attacker_list_win[1:51], 'bo', label='with SchedGuard')
plt.plot(exec_time_attacker_list[1:51], 'rx', label='without SchedGuard')
plt.legend(loc="upper right")
plt.ylim(17500, 44000)
# plt.xlabel('(a)')
plt.ylabel('time (us)')
plt.title('Inferred best case execution time')
plt.savefig('exe_time.pdf', bbox_inches='tight')  