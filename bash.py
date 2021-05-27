import os
from subprocess import Popen,PIPE
snd_ip=input("请输入发送端的IP地址:")
snd_port=input("发送端的端口号:")
filePath=input("待发送的文件地址:")
os.chdir("./sender")
#output=subprocess.check_output(["ls"],shell=True)
#output=subprocess.check_output(["./send", "192.168.31.128", "8888" "data.txt"],shell=True);
#out=output.decode()
#print(out)
#os.system("./send 192.168.31.128 8888 data.txt")
print("等待建立连接======>")
os.system("make")
p=Popen(["./send",snd_ip,snd_port,filePath],stdout=PIPE)
output=p.stdout.read()
out=output.decode()
print("发送完成!")
print(out)
