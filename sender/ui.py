# -*- coding: utf-8 -*-
"""
Created on Thu May 27 20:57:29 2021

@author: p1y2p
"""
import tkinter as tk
import os
from subprocess import Popen,PIPE
#os.chdir("./sender")
os.system("make")
root=tk.Tk()
root.geometry('800x600')
root.wm_title("发送端")
#IP地址
ipLabel=tk.Label(root,text="IP:")
ipLabel.place(x=20,y=30,width=100,height=40)
ipAddr=tk.StringVar()
ipEntry=tk.Entry(root,textvariable=ipAddr)
ipAddr.set('192.168.31.128')
ipEntry.place(x=120,y=30,width=400,height=40)
#端口
portLabel=tk.Label(root,text="端口:")
portLabel.place(x=20,y=90,width=100,height=40)
portNum=tk.StringVar()
portEntry=tk.Entry(root,textvariable=portNum)
portNum.set('8888')
portEntry.place(x=120,y=90,width=400,height=40)
#文件列表
snd_list=tk.Listbox(root)
sb=tk.Scrollbar(snd_list)
sb.pack(side=tk.RIGHT,fill=tk.Y)
snd_list['yscrollcommand']=sb.set
snd_list.place(x=120,y=240,height=200,width=400)
sb.config(command=snd_list.yview)
#文件地址
fileLabel=tk.Label(root,text="文件地址:")
fileLabel.place(x=20,y=150,width=100,height=40)
filePath=tk.StringVar()
fileEntry=tk.Entry(root,textvariable=filePath)
filePath.set('')
fileEntry.place(x=120,y=150,width=400,height=40)
#添加文件
def addFile():
    global filePath
    snd_list.insert(tk.END,filePath.get())
    print(snd_list.get(0,snd_list.size()-1))
    filePath.set('')
#打开文件
def openFile(filePath):
    another=tk.Toplevel()
    another.geometry('400x300')
    with open(filePath) as file_obj:
        content = file_obj.read()
    lb = tk.Label(another, text=content,width=400,height=10,anchor='w',justify='left',wraplength=400,)                
    lb.pack()
    another.mainloop()
#发送文件
def send():
    while snd_list.size()>0:
        f = snd_list.get(0)
        p=Popen(["./send",ipAddr.get(),portNum.get(),f],stdout=PIPE)
        output=p.stdout.read().decode()
        print(output)
        snd_list.delete(0)

def out():
    #print("记得删除seed.txt")
    os.chdir("..")
    os.system("rm seed.txt")
    root.destroy()


#添加文件按钮
addFileBtn=tk.Button(root,text='添加',command=addFile)
addFileBtn.place(x=600,y=150,width=100,height=40)
#删除文件按钮
delFileBtn=tk.Button(root,text='删除',command=lambda x=snd_list:x.delete(tk.ACTIVE))
delFileBtn.place(x=600,y=250,width=100,height=40)
#文件打开按钮
openFileBtn=tk.Button(root,text='打开',command=lambda x=snd_list:openFile(x.get(tk.ACTIVE)))
openFileBtn.place(x=600,y=350,width=100,height=40)
#发送文件按钮
sndBtn=tk.Button(root,text='发送',command=send)
sndBtn.place(x=120,y=500,width=100,height=40)
#退出按钮
quBtn=tk.Button(root,text='退出',command=out)
quBtn.place(x=400,y=500,width=100,height=40)
root.mainloop()