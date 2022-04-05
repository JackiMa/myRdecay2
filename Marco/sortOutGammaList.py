'''
description: 
param {*} crystalName
return {*} void
description: 对OutGamma进行排序
'''

import sys

crystalName = sys.argv[1] # 获取传入参数
fileName = "CoinOutGamma_" + crystalName + ".csv"

datacsv = []
with open(fileName,'r') as f:
    for line in f:
        datacsv.append(line.split("\n")[0].split(","))

datacsv.sort(key = lambda x:float(x[0]),reverse=0)
with open(fileName,'w') as f:
    for line in datacsv:
        f.write("%s,%s,%s\n" %(line[0],line[1],line[2]))