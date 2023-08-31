import serial
import values
import database
from bluepy.btle import Scanner, DefaultDelegate
import os
import time


with open("grid.txt",'r') as file:
    last_line = file.readlines()[-1]
    
x=int(last_line[0])
y=int(last_line[2])
point=str(x)+","+str(y)
if(x==2 and y==6):
    x=0
    y=0
elif (x<=3 and y<6):
    y+=1
elif (y==6):
    x+=1
    y=0
with open("grid.txt",'a') as file:
    file.write("\n"+str(x)+","+str(y))

ser = serial.Serial('/dev/ttyACM0', 115200, timeout=1)
ser.reset_input_buffer()
while True:
    if ser.in_waiting > 0:
        line = ser.readline().decode('ascii').rstrip()
        print(line)
        if(line=="Start"):
            scanner = Scanner().withDelegate(values.ScanDelegate())
            devices = scanner.scan(2)
            for dev in devices:
                if(dev.addr=="a0:e6:f8:b7:03:04"):
                    print ("Device %s , RSSI=%d dB" % (dev.addr, dev.rssi))
                    rssi1=dev.rssi
                elif (dev.addr =="a0:e6:f8:af:68:00"):
                    print ("Device %s , RSSI=%d dB" % (dev.addr, dev.rssi))
                    rssi2=dev.rssi
                elif (dev.addr=="24:71:89:07:8e:81"):
                    print ("Device %s , RSSI=%d dB" % (dev.addr, dev.rssi))
                    rssi3=dev.rssi     
            host="A0:E6:F8:B7:03:04"
            print('Connecting to ' + host)

            output=os.system("rfkill unblock bluetooth")
            os.system("sudo hciconfig hci0 up")
            tag = values.SensorTag(host)
            # Enabling selected sensors
            tag.IRtemperature.enable()
            tag.humidity.enable()
            tag.barometer.enable()
            tag.accelerometer.enable()
            tag.magnetometer.enable()
            tag.gyroscope.enable()
            #tag.battery.enable()
            #tag.keypress.enable()
            #tag.setDelegate(values.KeypressDelegate())
            tag.lightmeter.enable()

            # Some sensors (e.g., temperature, accelerometer) need some time for initialization.
            # Not waiting here after enabling a sensor, the first read value might be empty or incorrect.
            time.sleep(1.0)
            temp = tag.IRtemperature.read()
            humidity = tag.humidity.read()
            barometer=tag.barometer.read()
            accelerometer = tag.accelerometer.read()
            magnetometer = tag.magnetometer.read()
            gyroscope = tag.gyroscope.read()
            light = tag.gyroscope.read()
            print('Temp: ', temp)
            print("Humidity: ", humidity)
            print("Barometer: ", barometer)
            print("Accelerometer: ", accelerometer)
            print("Magnetometer: ", magnetometer)
            print("Gyroscope: ", gyroscope)
            print("Light: ", light)
            #print("Battery: ", tag.battery.read())
            db=database.data()

            
            os.system("rfkill block bluetooth")
            time.sleep(2)
            output=os.system("rfkill unblock bluetooth")
            os.system("sudo hciconfig hci0 up")
            for i in range(30):
                time.sleep(20)
                print(i)
                scanner = Scanner().withDelegate(values.ScanDelegate())
                devices = scanner.scan(2)
                for dev in devices:
                    if(dev.addr=="address1"): #address1 is the MAC address of the first anchor node
                        print ("Device %s , RSSI=%d dB" % (dev.addr, dev.rssi))
                        rssi1=dev.rssi
                    elif (dev.addr =="address2"): #address1 is the MAC address of the second anchor node
                        print ("Device %s , RSSI=%d dB" % (dev.addr, dev.rssi))
                        rssi2=dev.rssi
                    elif (dev.addr=="address3"): #address1 is the MAC address of the third anchor node
                        print ("Device %s , RSSI=%d dB" % (dev.addr, dev.rssi))
                        rssi3=dev.rssi
                    
                db.upload(point,rssi1,rssi2,rssi3,temp,humidity,barometer,accelerometer,magnetometer,
                            gyroscope,light)
            
            db.write()
            ser.write(b"end\n")
            line = ser.readline().decode('utf-8').rstrip()
            print(line)
            break

                            
