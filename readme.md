This Arduino sketch is built for the Flexbot board. It utilizes the HM-11 Bluetooth Low Energy module and the onboard Temperature/Pressure sensors.

Currently you can connect to it using something like LightBlue for IOS.

Accepts write commands and responds with notifications - commands are also output to the Serial console if monitor connected through USB.

Commands:
* read - gives temp, pressure, and elevation data: 'temp/pressure/elevation'
* temp - gives temp data in C and F: 'XXc/XXf'
* pres - gives pressure data: 'XXXX hPa'
* elev - gives elevation data: 'XXXXm'
* set - sets local sea level pressure (find on a weather site). This is used to calculate elevation. If not set, a default of 1013.5 is used.
* get - gives the currently set sea level pressure.

Any other command will return 'err'

Note: Temperature module is physically close to the ATMega chipset on this board so temperature calculations will be off as the ATMega produces a fair amount of heat.
