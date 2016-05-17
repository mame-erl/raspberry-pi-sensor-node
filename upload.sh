#!/bin/bash
# Created by Markus Meier
pfad="/home/pi/programme/"
program_temp="dht22_mm_csv"
program_lux="tsl2591_dyna.py"

# API Key for thingspeak is kept separate
source /home/pi/programme/apikeys.sh
urlthing="https://api.thingspeak.com/update?api_key=$apithing&"

# Read temperature and humidity from DHT22
ergebnis="$(sudo $pfad$program_temp)"
# Separate temperature and humidity into dedicated variables
wert1="$(echo $ergebnis | cut -d / -f 1)"
wert2="$(echo $ergebnis | cut -d / -f 2)"

# Read ambient light from TSL2591
ergebnis="$(python $pfad$program_lux)"
# Get only Lux value from TSL2591 output
wert4="$(echo $ergebnis | cut -d = -f 2 | cut -d " " -f 1)"

# Create URL parameters for ThingSpeak
feld1="field1=$wert1"
feld2="&field2=$wert2"
feld4="&field4=$wert4"

# Create combined URL
gesamturl="$urlthing$feld1$feld2$feld4"

# Use cURL to HTTP GET the results to ThingSpeak API
curl "$gesamturl"
