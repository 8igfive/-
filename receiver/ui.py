import tkinter as tk
import os
import threading
import time
from subprocess import Popen,PIPE
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
#文件地址
filePath=tk.StringVar()
#filePath.set('aaa')
#文件列表
snd_list=tk.Listbox(root)
sb=tk.Scrollbar(snd_list)
sb.pack(side=tk.RIGHT,fill=tk.Y)
snd_list['yscrollcommand']=sb.set
snd_list.place(x=120,y=240,height=200,width=400)
sb.config(command=snd_list.yview)
#rt不要和root重名了
def checkFile():
    while True:
        for rt, dirs, files in os.walk("./"):
            for file in files:
                if os.path.splitext(file)[1] == '.txt' and file not in ['sha256.txt','sha256_gen.txt']:
                    if file not in snd_list.get(0,snd_list.size()-1):
                        snd_list.insert(tk.END,file)
        time.sleep(1)

#打开文件
def openFile(filePath):
    another=tk.Toplevel()
    another.geometry('400x300')
    with open(filePath) as file_obj:
        content = file_obj.read()
    lb = tk.Label(another, text=content,width=400,height=10,anchor='w',justify='left',wraplength=400,)                
    lb.pack()
    another.mainloop()
def deletFile(obj):
    filePath=obj.get(tk.ACTIVE)
    if(os.path.splitext(filePath)[1] == '.txt' and filePath not in ['sha256.txt','sha256_gen.txt']):
        os.system("rm "+filePath)
        obj.delete(tk.ACTIVE)

#删除文件按钮
delFileBtn=tk.Button(root,text='删除',command=lambda x=snd_list:deletFile(x))
delFileBtn.place(x=600,y=250,width=100,height=40)
#文件打开按钮
openFileBtn=tk.Button(root,text='打开',command=lambda x=snd_list:openFile(x.get(tk.ACTIVE)))
openFileBtn.place(x=600,y=350,width=100,height=40)
#监听
sndBtn=tk.Button(root,text='开始监听')
th=None
def execute():
    global p
    cmd = "./rec "+ipAddr.get()+" "+portNum.get()
    p = Popen(cmd,shell=True)
    sndBtn['text']='监听中'
    sndBtn['state']=tk.DISABLED
def out():
    os.chdir("..")
    os.system("rm seed.txt")
    root.destroy()
sndBtn['command']=execute
sndBtn.place(x=120,y=500,width=100,height=40)

#退出按钮
quBtn=tk.Button(root,text='退出',command=out)
quBtn.place(x=400,y=500,width=100,height=40)

th2=threading.Thread(target=checkFile)  
th2.setDaemon(True)#守护线程  
th2.start()  
root.mainloop()
