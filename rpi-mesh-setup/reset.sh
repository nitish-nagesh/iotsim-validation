#!/bin/bash
# Reset mesh network to normal mode.
# Remember to edit /etc/rc.local, comment out the last second line.
# Run this script and reboot the system.
#
# Author: Xiaofan Yu
# Date: 11/6/2019
ip link set wlan0 down;
iw wlan0 set type managed;
iwconfig wlan0 mode managed;
ip link set down dev bat0;
ip link set wlan0 up;

update-rc.d dhcpcd enable;
sudo systemctl unmask wpa_supplicant.service;
systemctl restart wpa_supplicant.service;
systemctl restart networking.service;
