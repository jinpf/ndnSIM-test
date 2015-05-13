def print_average(fname):
    with open(fname,'r') as f:
        f.readline()
        records = f.readlines()
        delay = [float(record.strip('\n').split('\t')[1]) for record in records]
        print sum(delay)/len(delay)

if __name__ == '__main__':
    print_average('app-data-delay-pull-lost-5.txt')
    print_average('app-data-delay-pull-lost-10.txt')
    print_average('app-data-delay-pull-lost-15.txt')
