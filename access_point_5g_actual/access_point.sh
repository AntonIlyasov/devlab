#!/bin/bash
echo "y" | sudo airmon-ng check kill
sudo ip link set wlan0 up
sudo ip addr add 192.168.0.1/24 dev wlan0
sudo dnsmasq -C /home/orangepi/dnsmasq.conf
sudo hostapd /home/orangepi/hostapd.conf
