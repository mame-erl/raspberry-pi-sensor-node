#!/bin/bash
mydate="$(date +%Y-%m-%d:%H-%M-%S)"
pfad="/home/pi/programme/"
program_temp="dht22_mm_csv"
program_lux="tsl2591_dyna.py"
logpfad="$pfad"
logdatei="2016.txt"
# API Key for thingspeak is kept separate
source /home/pi/programme/apikeys.sh
urlthing="https://api.thingspeak.com/update?api_key=$apithing&"

ergebnis="$(sudo $pfad$program_temp)"
wert1="$(echo $ergebnis | cut -d / -f 1)"
wert2="$(echo $ergebnis | cut -d / -f 2)"
wert3="$(echo $ergebnis | cut -d / -f 3)"

ergebnis="$(python $pfad$program_lux)"
wert4="$(echo $ergebnis | cut -d = -f 2 | cut -d " " -f 1)"

feld1="field1=$wert1"
feld2="&field2=$wert2"
feld3="&field3=$wert3"
feld4="&field4=$wert4"

gesamturl="$urlthing$feld1$feld2$feld3$feld4"

curl "$gesamturl"

echo "$mydate|$wert1:$wert2:$wert3:$wert4" >> $logpfad$logdatei
