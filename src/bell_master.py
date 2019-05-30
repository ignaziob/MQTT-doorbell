# Python standard libruary import
import logging
import time
import sys
import datetime

# Python 3rd parties libruary import
import RPi.GPIO as GPIO
import paho.mqtt.client as mqtt

# Python package module import
from publish_topic import publish_msg

def button_callback(channel):
   logging.debug( "Button was pushed!")
   time.sleep(0.1)
   if not GPIO.input(channel):
      return -3 #debouncing if IN value is HIGH it is a falling edge bouncing

   # in main loop
   # prepare the timestamp
   timenow = time.strftime('%Y-%m-%d %H:%M:%S', time.localtime())
   logging.debug( "timestamp: " + timenow)
   if (not publish_msg(topic="bell",msg="cuceglio, "+timenow)):
      logging.debug("Topic published")
      return 0
   else:
      logging.error("Connection failed")
      return -1
   return -2

def main():
   """sintax: python3 bell_master.py"""
   logging.basicConfig(level=logging.DEBUG) #  set debugging level

   GPIO.setwarnings(False) # Ignore warning for now
   GPIO.setmode(GPIO.BOARD) # Use physical pin numbering
   GPIO.setup(11, GPIO.IN, pull_up_down=GPIO.PUD_DOWN) # Set pin 11 to be an input pin and set initial value to be pulled low (off)
   GPIO.add_event_detect(11,GPIO.RISING,callback=button_callback, bouncetime=300) # Setup event on pin 11 rising edge

   # Wait indefinitely...
   #print  " Ready!"
   sys.stdout.flush()

   while True: # Run forever
      time.sleep(1)

# main
if __name__ == "__main__":
   main()

