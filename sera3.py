import serial
import requests
from firebase import firebase
import time
import struct
import threading


istenilen_sicaklik = 0

def make_request():
    while True:
      get()
      time.sleep(5)

a = threading.Thread(target=make_request)


url = 'https://smart-greenhouse-eeeb1-default-rtdb.firebaseio.com'

firebase = firebase.FirebaseApplication(url, None)
# new_post = """{"name": "Sera1", "temperature":15.5}"""


# result = firebase.get('/greenhouseData', None)
# print(result)

serialPort = serial.Serial(port="COM5", baudrate=9600,
                           bytesize=8, timeout = 1, stopbits=serial.STOPBITS_ONE, parity=serial.PARITY_NONE)


time.sleep(5)

# class sera:
#     isim = "sera1"
#     sicaklik = ""

#     def __init__(self, sicaklik):
#         self.sicaklik = sicaklik

def get():
    x = requests.get('https://smart-greenhouse-eeeb1-default-rtdb.firebaseio.com/istenen_sicaklik/-MQbeuDh7a-1khe56FRP/temp.json') # endAt="3"
    #print(type(x))
    global istenilen_sicaklik
    istenilen_sicaklik = int(x.text)
    print('istenilen sicaklik:' + str(istenilen_sicaklik))

def post(serialString):
        new_sera =  {
          'sicaklik': serialString[:4]
          }


        print("Sera'nın su anki sicakligi:" + str(serialString[:4]))
        result = firebase.post('/sera3', new_sera, {'print': 'pretty'}, {'X_FANCY_HEADER': 'VERY FANCY'})
        print(result)

a.start()   

while True:


    time.sleep(1)
    serialPort.setDTR(1)
    time.sleep(1)
    serialPort.flush()

    data = serialPort.write(struct.pack('>B', istenilen_sicaklik))
            

    if serialPort.in_waiting > 0:
        serialString = serialPort.readline()
        
        serialString = serialString.decode('ascii')
        
        post(serialString)

