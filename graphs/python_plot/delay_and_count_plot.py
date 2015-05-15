#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Author: jinpf
# @Date:   2015-05-15 16:31:15
# @Last Modified by:   jinpf
# @Last Modified time: 2015-05-15 21:07:02

import matplotlib.pyplot as plt
from matplotlib.font_manager import FontProperties

def plot_delay(pull_delay, push_delay):
	w = 0.25 
	bar1 = plt.bar([x-w-0.05 for x in range(1,5)],pull_delay,color = 'r', width = w)
	bar2 = plt.bar([x+0.05 for x in range(1,5)],push_delay,color = 'b', width = w)
	plt.legend((bar1[0],bar2[0]),('pull','push'))
	font = FontProperties(fname=r"c:\windows\fonts\simsun.ttc", size=17) 
	plt.ylabel(u'数据平均接收延迟(秒)', fontproperties=font)
	plt.xlabel(u'链路丢包率', fontproperties=font)
	plt.title(u'不同丢包率下数据接收平均时延', fontproperties=font)
	plt.xticks([x for x in range(1,5)], ('0', '5%', '10%', '15%'))
	plt.axis([0.3, 4.9, 0, 0.6])
	plt.show()

def plot_count(pull_count, push_count):
	w = 0.25 
	bar1 = plt.bar([x-w-0.05 for x in range(1,5)],pull_count,color = 'r', width = w)
	bar2 = plt.bar([x+0.05 for x in range(1,5)],push_count,color = 'b', width = w)
	plt.legend((bar1[0],bar2[0]),('pull','push'))
	font = FontProperties(fname=r"c:\windows\fonts\simsun.ttc", size=17) 
	plt.ylabel(u'数据包总数', fontproperties=font)
	plt.xlabel(u'链路丢包率', fontproperties=font)
	plt.title(u'不同丢包率下链路传输数据包总数', fontproperties=font)
	plt.xticks([x for x in range(1,5)], ('0', '5%', '10%', '15%'))
	plt.axis([0.3, 4.9, 0, 6000])
	plt.show()

if __name__ == '__main__':
	plot_delay([0.367039315,0.4151854646,0.4766644646,0.5673401546],[0.152536578,0.2372487,0.3482715916,0.4458923516])
	plot_count([5190, 5265, 5358, 5564], [1001, 1063, 1125, 1231])