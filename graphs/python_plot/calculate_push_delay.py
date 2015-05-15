#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Author: jinpf
# @Date:   2015-05-15 08:40:03
# @Last Modified by:   jinpf
# @Last Modified time: 2015-05-15 15:40:46

import random

def p_random(percent):
	x = int(random.uniform(1,100))
	if x > int(percent*100):
		return 0
	else:
		return 1

def record_G_C(fname):
	with open(fname, 'r') as f:
		PGdata = {}
		Cdata = {}
		f.readline()
		records = f.readlines()
		for record in records:
			data = record.strip('\n').split('\t')
			if data[3] == 'P_GData':
				PGdata[int(data[5])] = float(data[0])
			if data[3] == 'C_Data':
				Cdata[int(data[5])] = float(data[0])
		return (PGdata,Cdata)

def find_lost(fname):
	with open(fname, 'r') as f:
		f.readline()
		records = f.readlines()
		lost = [int(record.strip('\n').split('\t')[0]) for record in records \
		if record.strip('\n').split('\t')[1] == 'not receive']
		return lost

def lost_delay_calculate(lostseq, record, percent):
	lost_delay = {}
	for seq in lostseq:
		nseq = seq + 1
		while nseq not in record[1]:
			nseq += 1
		lost_delay[seq] = record[1][nseq] - record[0][seq] + 0.3 + \
		0.3 * p_random(1-(percent*1.0/100)**2)
	# print lost_delay
	return lost_delay
	# print (sum(lost_delay.values()) + 0.15*(1000-len(lost_delay)))/1000

def push_lost_delay_calculate(percent):
	record = record_G_C('app-delays-trace-push-lost-'+ str(percent) +'.txt')
	lostseq = find_lost('app-data-delay-push-lost-' + str(percent) + '.txt')
	return lost_delay_calculate(lostseq,record,percent)

if __name__ == '__main__':
	print push_lost_delay_calculate(5)