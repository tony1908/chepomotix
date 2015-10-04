import serial
import json
import requests
import time
from pytify import Spotify
spotify = Spotify()
 
id_ant=''
tempo=7


print PuertoSerie.readline()
while True:
     PuertoSerie = serial.Serial('/dev/tty.usbmodem1411', 9600)
     if PuertoSerie.isOpen == False:
       PuertoSerie.open()
       print 'abierto'
    req = requests.get('https://vast-mesa-3135.herokuapp.com/domotica')
    jsonData = json.loads(req.text)
    print jsonData
    dato = str(jsonData[0]['gesto'].strip())

    print dato
    a,b = dato.split(',')
    enviar = str(b)+str(a)+'*' 
    if enviar == '11': 
        PuertoSerie.write('0')
   elif enviar == '10': 
        PuertoSerie.write('1')
    elif enviar == '21': 
        PuertoSerie.write('2')
    elif enviar == '20': 
        PuertoSerie.write('3')
    elif enviar == '31': 
        PuertoSerie.write('4')
    elif enviar == '30': 
        PuertoSerie.write('5')
    elif enviar == '71' && tempo=='7': 
        PuertoSerie.write('6')
        tempo='6'
    elif enviar == '70' $$ tempo=='6': 
        PuertoSerie.write('7')
        tempo='7'
    elif enviar == '81': 
        PuertoSerie.write('8')
    elif enviar == '80': 
        PuertoSerie.write('9')
    if id_ant!=jsonData[0]['id']:
        if a=='9':
            spotify.playpause()
        elif a== '10':
            for i in range (15):spotify.volumeUp()
        elif a== '11':
            spotify.next()
        elif a== '12':
            for i in range (15):spotify.volumeDown()
        elif a== '13':
            spotify.previous()
        else:
            print enviar
    id_ant=jsonData[0]['id']

    time.sleep(1)
