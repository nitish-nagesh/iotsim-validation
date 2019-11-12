/***************************************************************
* MQTT Client running on ESP8266s. Publish power data to broker.
* Remember to change clientID for different ESP8266 device.
* Remember to set pt_interval for power sampling rate.
*
* Author: Xiaofan Yu
* Date: 11/11/2019
***************************************************************/
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_INA219.h>

Adafruit_INA219 ina219;

/*Connecting to WiFi network*/
const char* ssid = "SEELAB_IN_LAB";
const char* password = "seelab2148";

/*MQTT server credentials*/
const char* mqttBroker = "192.168.1.46";
const int mqttPort = 61613;
const char* clientID = "esp1"; // change this for different device
const int pt_interval = 200; // sampling power how many ms
bool sampling = false; // power sampling status flag

/*Declaring WiFi and mqtt client*/
WiFiClient wifiClient;
PubSubClient mqttClient(mqttBroker, mqttPort, mqttCallback, wifiClient);

void setup() 
{
    Serial.begin(115200);

    ina219.begin();
    ina219.setCalibration_32V_1A();

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print("Connecting to WiFi..");
    }
    Serial.println("WiFi connected.")

    if (mqttClient.connect(clientID)) {
        mqttClient.subscribe("cmd");
        Serial.println("Connect to broker and subscribe cmd topic.")
        char msg[20];
        ready_msg = sprintf(msg, "%s is ready!", clientID);
        mqttClient.publish("status", msg);
        Serial.println("Publish ready message.")
    } 
}

void mqttcallback(char* topic, uint8_t* payload, unsigned int length)
{
    String msg = String(payload);
    if (msg == "start") {
        sampling = true;
    }  
}

void loop() 
{
    if (sampling) {
        float power_mW = ina219.getPower_mW();
        Serial.println(power_mW);
        
        if (mqttClient.connected()) {
            char msg[20];
            sprintf(msg, "%f", power_mW);
            mqttClient.publish(clientID, msg);
        }
        else {
            if (mqttClient.connect(clientID))
                mqttClient.subscribe("cmd");
            Serial.println("Reconnect to broker and subscribe cmd topic.")
        }
    }

    client.loop();
    delay(pt_interval);
}
