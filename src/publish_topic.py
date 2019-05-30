# Python standard libruary import
import logging
import time
import sys

# Python 3rd parties libruary import
import paho.mqtt.client as mqtt

def on_connect(client, userdata, flags, rc):
   """callback function, manages the connection acknowledge from broker"""
   if rc == 0:
      client.connected_flag=True #set flag
      logging.debug("Connected with result code "+str(rc))
   else:
      logging.error("Connection failed with code "+str(rc))

def on_message(client, userdata, msg):
   """callback function that manages message received from broker"""
   pass

def publish_msg(topic="dummy",msg="dummy"):
   """ publish a message over a given topic """
   mqtt.Client.connected_flag=False #create flag in class, 
   
   # open connection to mqtt broker   
   client = mqtt.Client("raspi3")
   client.username_pw_set(username="guest",password="guest") //here your MTQQ server user/pwf
   client.on_connect = on_connect
   client.on_message = on_message
   client.loop_start()
   logging.debug("Connecting to broker ")   
   client.connect("m24.cloudmqtt.com", 11436, 60) //here your MQTT port
   client.loop_start()

   for x in range(50):              #loop waiting connection for 5 seconds
      if not client.connected_flag: #wait in loop
         time.sleep(0.1)
      else:
         break

   if client.connected_flag:
      ret= client.publish(topic,msg) # publish an event
      client.loop_stop()  # Stop loop 
      client.disconnect() # disconnect
      return 0  # OK   
   else:
      return -1 # Connection failed


def main():
   """sintax: python3 publish_topic.py <message>"""
   logging.basicConfig(level=logging.DEBUG)
   if (len(sys.argv)>=2):
      logging.debug( "argv1: " + str(sys.argv[1]))
      
      # prepare the timestamp
      timenow = time.strftime('%Y-%m-%d %H:%M:%S', time.localtime())
      logging.debug( "timestamp: " + timenow)

      if (not publish_msg(topic="bell",msg="cuceglio, "+timenow)):
         logging.debug("Topic published")
         sys.exit(0)
      else:
         logging.debug("Connection failed")
         sys.exit(-1)
   logging.debug("argv[1] required as message")
   sys.exit(-2)

# main
if __name__ == "__main__":
   main()

