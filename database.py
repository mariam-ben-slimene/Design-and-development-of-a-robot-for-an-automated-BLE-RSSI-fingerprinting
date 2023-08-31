import os
import pandas as pd
import matplotlib.pyplot as plt
import time
from datetime import datetime
import pytz
import csv


class data():
    dates=[]
    tz = pytz.timezone('Etc/UTC')
    list_points=[]
    list_RSSI1=[]
    list_RSSI2=[]
    list_RSSI3=[]
    list_temp=[]
    list_hum=[]
    list_bar=[]
    list_acc=[]
    list_magnetic=[]
    list_gero=[]
    list_light=[]
        

    def upload(self,points,rssi1,rssi2,rssi3,temp,hum,bar,acc,magnetic,gero,light):
        self.list_points.append(points)
        self.list_temp.append(temp)
        self.list_hum.append(hum)
        self.list_RSSI1.append(rssi1)
        self.list_RSSI2.append(rssi2)
        self.list_RSSI3.append(rssi3)
        self.list_bar.append(bar)
        self.list_acc.append(acc)
        self.list_magnetic.append(magnetic)
        self.list_gero.append(gero)
        self.list_light.append(light)
        self.t=datetime.now(self.tz)
        self.dt=self.t.strftime('%d/%m/%Y %H:%M:%S')
        self.dates.append(self.dt)
        
    def write(self):
        filename="sensortagData.csv"
        file_exists = os.path.isfile(filename)
    
        data = {'Point':self.list_points,'RSSI1' : self.list_RSSI1,'RSSI2' : self.list_RSSI2,'RSSI3' : self.list_RSSI3, 'Temperature' : self.list_temp, 'Humidity':self.list_hum,
                'Barometer': self.list_bar,'Accelerometer': self.list_acc,
                'Magnetometer' : self.list_magnetic,'Gyroscope' : self.list_gero,
                'Light' : self.list_light}
        if not file_exists:
            df = pd.DataFrame(data,index=self.dates)
            index=df.index
            index.name="date"
            csv = df.to_csv(filename,mode='a')
            cwd = os.getcwd()
        else:
            df = pd.DataFrame(data,index=self.dates)
            csv = df.to_csv(filename,mode='a',header=False)
            cwd = os.getcwd()