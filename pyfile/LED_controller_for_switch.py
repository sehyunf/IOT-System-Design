from gpiozero import LED
from gpiozero import Button
from time import sleep

led = LED(21)
switch = Button(20)
pullup =LED(16)

while True:
    pullup.on()
    sleep(0.5)
    
    if switch.is_pressed:
        led.on()
        print("switch on")
    else:
        led.off()
        print("switch off")