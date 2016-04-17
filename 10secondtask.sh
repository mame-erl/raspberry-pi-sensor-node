#!/bin/bash
# Befehl wird alle 10 Sekunden ausgeführt
while (sleep 10 && /home/pi/programme/upload.sh) &
do
	wait $!
done
