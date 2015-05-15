#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Author: jinpf
# @Date:   2015-05-15 14:27:57
# @Last Modified by:   jinpf
# @Last Modified time: 2015-05-15 14:47:27

def delay_average(fname):
	with open(fname) as f:
		f.readline()
		records = f.readlines()
		return sum([float(record.strip('\n').split('\t')[1]) for record in records])/len(records)

if __name__ == '__main__':
	print delay_average('app-data-delay-pull-lost-15.txt')