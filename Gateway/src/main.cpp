#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEAdvertisedDevice.h>
#include<BLEBeacon.h>
#include <WiFi.h>

#include <PubSubClient.h>

#include<string.h>


#include <Preferences.h>
#include <nvs_flash.h>
const char* ssid = "TMNL";
const char* password = "testingiy";
const char* deviceName = "BLEScanner";
BLEScan* pBLEScan;


//The broker and port are provided by http://www.mqtt-dashboard.com/
char *mqttServer = "192.168.43.1";
int mqttPort = 18883;

const char* mqtt_client_name = "E";
const char* mqtt_pub_topic = "/ys/testpub"; //The topic to which our client will publish
const char* mqtt_sub_topic = "/ys/testsub"; //The topic to which our client will subscribe

WiFiClient client;
PubSubClient mqttClient(client);

static volatile uint8_t msg_flag = 0;
BLEScanResults foundSensors;
Preferences preferences;

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received from: "); Serial.println(topic);
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  Serial.println();
}

void WiFitask (void *parameters){
  while(1){
     if (msg_flag == 1) {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
    vTaskDelay(500/portTICK_PERIOD_MS);
    Serial.print(".");
  }
  Serial.println("");  Serial.print("WiFi connected to: "); Serial.println(ssid);  Serial.println("IP address: ");  Serial.println(WiFi.localIP());
  vTaskDelay(1000/portTICK_PERIOD_MS);
  if (!mqttClient.connected()){
      while (!mqttClient.connected()){
        if(mqttClient.connect(mqtt_client_name)){
          Serial.println("MQTT Connected!");
          mqttClient.subscribe(mqtt_sub_topic);
          }
         else{
          Serial.print(".");
          }
      }
    }
  
      Serial.println("here goes nothing");
      int count = foundSensors.getCount();
      Serial.println("found " +String(count)+ " devices");
      for (int i = 0; i < count; i++) {
        BLEAdvertisedDevice bleSensor = foundSensors.getDevice(i);
        String address = bleSensor.getAddress().toString().c_str();
       //String ManufData = bleSensor.toString().c_str();
       String deviceName = bleSensor.getName().c_str();
      String Major = deviceName.substring(0,1); 
      String Minor = deviceName.substring(1,2);
      //Serial.println("address: "+address);
       
        //Serial.println("major : "+Major + " minor:"+Minor);
        
        
       
        mqttClient.publish(mqtt_pub_topic,"checking");
        
        
        if(Major=="e"){
          const char* pub_topic = "A5";
           Serial.println("address: "+address);
           Serial.println("name: "+deviceName);   
          mqttClient.publish(mqtt_pub_topic, "found");
          Serial.println("name: "+deviceName);   
          if(Minor=="0"){
            mqttClient.publish(pub_topic,"Empty");
          }
          else{
            mqttClient.publish(pub_topic,"Full");
          }
          Serial.println("Message published");
          mqttClient.loop();
        }
        if(Major=="f"){
          const char* pub_topic = "A6";
           Serial.println("address: "+address);
           Serial.println("name: "+deviceName);   
          mqttClient.publish(mqtt_pub_topic, "found");
          Serial.println("name: "+deviceName);   
          if(Minor=="0"){
            mqttClient.publish(pub_topic,"Empty");
          }
          else{
            mqttClient.publish(pub_topic,"Full");
          }
          Serial.println("Message published");
          mqttClient.loop();
        }
        if(Major=="g"){
          const char* pub_topic = "A7";
           Serial.println("address: "+address);
           Serial.println("name: "+deviceName);   
          mqttClient.publish(mqtt_pub_topic, "found");
          Serial.println("name: "+deviceName);   
          if(Minor=="0"){
            mqttClient.publish(pub_topic,"Empty");
          }
          else{
            mqttClient.publish(pub_topic,"Full");
          }
          Serial.println("Message published");
          mqttClient.loop();
        }
        if(Major=="h"){
          const char* pub_topic = "A8";
           Serial.println("address: "+address);
           Serial.println("name: "+deviceName);   
          mqttClient.publish(mqtt_pub_topic, "found");
          Serial.println("name: "+deviceName);   
          if(Minor=="0"){
            mqttClient.publish(pub_topic,"Empty");
          }
          else{
            mqttClient.publish(pub_topic,"Full");
          }
          Serial.println("Message published");
          mqttClient.loop();
        }

      }
  
    vTaskDelay(2000/portTICK_PERIOD_MS);
  Serial.println("Exiting task now");
  
  mqttClient.disconnect();
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
 // btStart();
  msg_flag=0;
  
  }
  }
}

void set_value(){

preferences.begin("devices",false);
if(preferences.getString("l1") ==0){
  preferences.putString("A1", "20:00:07:38:86:0a");
  preferences.putString("l1", "b4:e6:2d:87:69:c3");
  preferences.end();
}
else{
  Serial.println("A1 : "+ preferences.getString("A1"));
}

}

void BLEtask (void *parameters){
  while(1){
    BLEDevice::init(deviceName);
      pBLEScan = BLEDevice::getScan(); //create new scan
      pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
      pBLEScan->setInterval(100);
      pBLEScan->setWindow(99);  // less or equal setInterval value
    
    Serial.println("Listing BLE Sensors");
      foundSensors = pBLEScan->start(5, false);
      Serial.println("here goes nothing");
      int count = foundSensors.getCount();
      Serial.println("found " +String(count)+ " devices");
     
       msg_flag = 1;
        
    vTaskDelay(5000/portTICK_PERIOD_MS);
  
  }
  
}
//}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  set_value();
  WiFi.mode(WIFI_STA);    
  mqttClient.setServer(mqttServer, mqttPort);
  mqttClient.setCallback(callback);   
  
  xTaskCreate(WiFitask,"Wifi task",3000,NULL,1,NULL);
  xTaskCreatePinnedToCore(BLEtask, "Calling task", 2048,NULL,2,NULL,0);
  // Delete "setup and loop" task
  vTaskDelete(NULL); 
}

void loop() {
  // put your main code here, to run repeatedly:
  

}
