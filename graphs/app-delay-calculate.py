## this script handle ndn app log, and calculate the
#  delay between producer generate data and consumer 
#  received the generated data, and record the delay
#  data in app-data-delay.txt.txt


def ReadAndRecord():
    with open('app-delays-trace.txt', 'r') as f:      
        # producer set
        Pset = set()
        # consumer set
        Cset = set()
        # 2 dimension dict InforbySeq = {seq:{node:time}}
        InforbySeq = {}     
        # skip head
        f.readline()
        record = f.readline()
        while record != '':
            Datas = record.strip('\n').split('\t')
            # 0:time 1:node 2:appid 3:type 4:Name 5:seq 6:hop
            if 'P' in Datas[3]:
                Pset.add(Datas[1])
            if 'C' in Datas[3]: 
                Cset.add(Datas[1])
            if Datas[3] in ('P_GData', 'C_Data'):
                if Datas[5] not in InforbySeq:
                    InforbySeq[Datas[5]] = {}
                InforbySeq[Datas[5]][Datas[1]] = Datas[0]          
            record = f.readline()
        return (InforbySeq, Pset, Cset)    

def CalculateDelay(record):
    # record[0]:InforbySeq record[1]:Pset record[2]:Cset
    # 2 dimension dict delay = {seq:{node:delaytime}}
    delay = {}
    for seq in record[0]:
        if seq not in delay:
            delay[seq] = {}
        for p in record[1]:
            for c in record[2]:
                if c in record[0][seq]:
                    delay[seq][c] = str( float(record[0][seq][c]) - \
                    float(record[0][seq][p]) )
                else:
                    delay[seq][c] = 'not receive'
    return delay

def WriteDelayInfile(delay, Ctuple):
    with open('app-data-delay.txt','w') as f:
        Cstr = ''
        for c in Ctuple:
            Cstr += c + '\t'
        f.write("seq\t" + Cstr + '\n')
        for seq in sorted(delay, key = lambda x:int(x)):
            Wstr = seq
            for c in Ctuple:
                Wstr += '\t' + delay[seq][c]
            f.write(Wstr + '\n')

if __name__ == '__main__' :
    record = ReadAndRecord()
    delay = CalculateDelay(record)
    Ctuple = tuple(sorted(record[2]))
    WriteDelayInfile(delay, Ctuple)