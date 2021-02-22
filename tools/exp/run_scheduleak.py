import csv
import string
import argparse
import subprocess


# cmd = ['./print_test']
cmd = ['./scheduleak', '-d', '1000000', '-v', '100000', '-p', '330000', '-c', '100000']
data = []

def get_args():
    parser = argparse.ArgumentParser(description='Scheduling simulations.')       
    parser.add_argument('-n', '--num', type=int, default=10,
                    help='Number of cycles to run Scheduleak algorithm.')
    parser.add_argument('-o', '--output', type=str, default="attacker_data.csv",
                    help='Output file name.')

    return parser.parse_args()



def main():
    args = get_args()

    for i in range(args.num):
        result = subprocess.run(cmd, stdout=subprocess.PIPE).stdout.decode('utf-8')
        result = result.translate(str.maketrans(" \t,;.?!-:@[](){}_*/", " "*19)).split()
        if "Result" in result:
            index = result.index("Result") + 2

            # result has the following form
            # [..., 'Inference', 'Result', '=', '88069', '108450', '=>', '20381', 'us']
            # initial, finish time, runtime
            data.append([result[index], result[index+1], result[index+3]])

    file = open(args.output, 'w', newline ='') 
    
    # writing the data into the file 
    with file:
        write = csv.writer(file, delimiter='\t')
        write.writerows(data)

    print("%d Scheduleak attacks finished" % (args.num))

if __name__ == "__main__":
    main()
