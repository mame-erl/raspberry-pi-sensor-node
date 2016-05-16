# 
#	TSL2591 Ansteuerung in Python
#	am Raspberry Pi über I2C
#	
#	2016-03-20 Erstellt von Markus Meier
#

import tsl2591

INTEGRATIONTIME_100MS = 0x00
INTEGRATIONTIME_200MS = 0x01
INTEGRATIONTIME_300MS = 0x02
INTEGRATIONTIME_400MS = 0x03
INTEGRATIONTIME_500MS = 0x04
INTEGRATIONTIME_600MS = 0x05

GAIN_LOW = 0x00  # low gain (1x)
GAIN_MED = 0x10  # medium gain (25x)
GAIN_HIGH = 0x20  # high gain (428x)
GAIN_MAX = 0x30  # max gain (9876x)

tsl = tsl2591.Tsl2591()  # initialize

tsl.set_gain(GAIN_MED)
tsl.set_timing(INTEGRATIONTIME_200MS)
none		= tsl.get_full_luminosity()
full, ir	= tsl.get_full_luminosity()

# Abfangen von ungültigen Infrarot-Messungen bei wenig Licht
if ir==0:
        tsl.set_gain(GAIN_MAX)
	tsl.set_timing(INTEGRATIONTIME_500MS)
        full, ir= tsl.get_full_luminosity()

# Abfangen von Überlauf im kombinierten Spektrum
if full==65535:
	tsl.set_timing(INTEGRATIONTIME_100MS)
	tsl.set_gain(GAIN_LOW)
	full, ir= tsl.get_full_luminosity()

# Abfangen von Überlauf im Infrarot-Spektrum bei viel Licht
if ir==65535:
	tsl.set_timing(INTEGRATIONTIME_100MS)
	tsl.set_gain(GAIN_LOW)
	full, ir= tsl.get_full_luminosity()

# Ermitteln des Lux-Wertes
lux		= tsl.calculate_lux(full, ir)

# Falls das errechnete Lux-Ergebnis negativ ist, wird
# die Messung mit höherer Empfindlichkeit und längerer
# Integrationszeit wiederholt
if lux<0:
	tsl.set_gain(GAIN_MAX)
        tsl.set_timing(INTEGRATIONTIME_500MS)
        full, ir= tsl.get_full_luminosity()
	lux     = tsl.calculate_lux(full, ir)

# Ausgabe des Messergebnisses als Lux-Wert sowie Volles Spektrum und Infrarot
print ('Lux=%0.2f full=%i ir=%i' % (lux, full, ir))
