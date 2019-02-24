import serial
import sys
strCMD = sys.argv[1]
t = serial.Serial('COM5',9600)
intCMD = int(strCMD)
temp = '\0'+chr(intCMD)
t.write(temp.encode('utf-8'))
