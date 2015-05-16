#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Author: jinpf
# @Date:   2015-05-06 10:05:27
# @Last Modified by:   jinpf
# @Last Modified time: 2015-05-11 16:43:53

import matplotlib.pyplot as plt
from matplotlib.font_manager import FontProperties

def read_and_count(filename,interval=1):
	with open(filename,'r') as f:
		line = f.readline()
		time_count = {}
		while line != '':
			line = f.readline()
			record = line.strip('\n').split('\t')
			if record[3] in ['P_Interest','C_Data']:
				if record[5] == '1000' and record[3] == 'C_Data':
					return sorted(time_count.iteritems(), key = lambda x:x[0])
				t = int(round((float(record[0]))))/interval*interval
				if t in time_count:
					time_count[t] += 1
				else:
					time_count[t] = 1

def re_load(data):
	t = []
	c = []
	for i in data:
		t.append(i[0])
		c.append(i[1])
	return (t,c)

def plot_time_count(pull_data,push_data):
	font = FontProperties(fname=r"c:\windows\fonts\simsun.ttc", size=17) 
	line1, = plt.plot(pull_data[0],pull_data[1],'r.',label=u'pull',linewidth=1)
	line2, = plt.plot(push_data[0],push_data[1],'b+',label=u'push',linewidth=1)
	plt.legend(handles=[line1,line2])
	plt.ylabel(u'链路包个数', fontproperties=font)
	plt.xlabel(u'时间(每3秒)', fontproperties=font)
	plt.title(u'链路包个数', fontproperties=font)
	plt.axis([0,1000,0,8])
	plt.show()


if __name__ == '__main__':
	pull_count=read_and_count('app-delays-trace-pull.txt')
	push_count=read_and_count('app-delays-trace-push.txt')
	plot_time_count(re_load(pull_count),re_load(push_count))