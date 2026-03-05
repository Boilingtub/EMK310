Hint 1: Inaccurate ADC conversions

The input impedance to the ADC should not be higher than 3 kΩ and may
not be higher than 10 kΩ. Use an opamp buffer if necessary to lower
the impedance load to the pin and increase the acquisition time,
Tacq, to allow the capacitor of the internal sample-and-hold circuit
to fully load before a conversion starts.
