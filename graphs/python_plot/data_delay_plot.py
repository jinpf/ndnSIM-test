#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Author: jinpf
# @Date:   2015-05-05 16:24:40
# @Last Modified by:   jinpf
# @Last Modified time: 2015-05-12 11:59:52

# this script shall be used in windows! or modify the font!

import matplotlib.pyplot as plt
from matplotlib.font_manager import FontProperties

def get_data_from_file(filename):
	with open(filename,'r') as f:
		seq = []
		delay = []
		f.readline()
		lines = f.readlines()
		for line in lines:
			data = line.strip('\n').split('\t')
			seq.append(int(data[0]))
			if data[1] == 'not receive':
				delay.append(0.152)
			else:
				delay.append(float(data[1]))
	return (seq, delay)

def plot_data(pull_data, push_data):
	# font = FontProperties(fname=r"c:\windows\fonts\simsun.ttc", size=17) 
	line1, = plt.plot(pull_data[0],pull_data[1],'r.',label=u'pull',linewidth=2)
	line2, = plt.plot(push_data[0],push_data[1],'b+',label=u'push',linewidth=2)
	# plt.legend(handles=[line1,line2])
	plt.ylabel(u'时延(秒)')
	plt.xlabel(u'序列号')
	plt.title(u'数据传输时延')
	plt.show()

def delay_distribute(data_delay, division_value):
	distribute = {}
	for d in data_delay:
		key = round(d,division_value)
		if key in distribute:
			distribute[key] += 1
		else:
			distribute[key] = 1
	delay = distribute.keys()
	delay.sort()
	number = []
	for i in delay:
		number.append(distribute[i])
	return (delay, number)

def plot_each_delay(delay):
	plt.bar(delay[0],delay[1],width = 0.01,\
		align = 'center', color='b')
	plt.show()

def plot_delay_distribute(pull_delay, push_delay):
	bar1 = plt.bar(pull_delay[0],pull_delay[1],width = 0.03,\
		align = 'center', color='r')
	bar2 = plt.bar(push_delay[0],push_delay[1],width = 0.03,\
		align = 'center', color='b')
	# font = FontProperties(fname=r"c:\windows\fonts\simsun.ttc", size=17) 
	plt.legend((bar1[0],bar2[0]),('pull','push'))
	plt.ylabel(u'数据包个数')
	plt.xlabel(u'时延(秒)')
	plt.title(u'不同时延所占比例')
	plt.show()

if __name__ == '__main__':
	datapull = get_data_from_file('app-data-delay-pull.txt')
	datapush = get_data_from_file('app-data-delay-push.txt')
	print max(datapull[1])
	plot_data(datapull, datapush)
	pull_delay = delay_distribute(datapull[1],1)
	push_delay = delay_distribute(datapush[1],3)
	print pull_delay[0],pull_delay[1]
	print push_delay[0],push_delay[1]
	# plot_each_delay(pull_delay)
	plot_delay_distribute(pull_delay, ([0.15],[1000]))