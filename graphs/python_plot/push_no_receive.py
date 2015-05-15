#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Author: jinpf
# @Date:   2015-05-13 21:46:19
# @Last Modified by:   jinpf
# @Last Modified time: 2015-05-13 21:58:53

def no_receive_percentage(fname):
	with open(fname, 'r') as f:
		f.readline()
		records = f.readlines()
		lost = [record.strip('\n').split('\t')[0] for record in records \
		if record.strip('\n').split('\t')[1] == 'not receive']
		return len(lost)*1.0/len(records)

if __name__ == '__main__':
	print no_receive_percentage('app-data-delay-push-lost-15.txt')
		