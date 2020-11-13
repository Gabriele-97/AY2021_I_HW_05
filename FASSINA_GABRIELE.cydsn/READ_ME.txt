                                READ ME GABRIELE FASSINA

In this Assignment, I was asked to develop the firmware to acquire data from an accelerometer.

The development is based on the high resolution mode (set by proper configuration registers) 
in which a 12 bits signal is read with a FS of +-2g. Since the intended use of the accelerometer
was not declared, I assume it can be used to identify the orientation in space and so this FS is enough. 

The signal is later converted into m/s^2 signal. 3 decimal digits have been mantained by multiplying the value
times 1000 and casting procedure from floating to integers. A scaling factor 0.001 in the Bridge Control Panel 
allows to retrieve the correct value. 

The user can change the sampling frequency among 6 possible configurations from 1 to 200 Hz by pressing a button.
I had two possibilities to implement this aspect: either using an Interrupt which could prevent checking the 
status of the button at every cycle or doing it in the infinite loop. I have chosen this latter solution because
it allows the introduction of debug strings writing what is the sampling frequency that the user is choosing.
As a matter of fact, this functionality could not be introduced in the ISR function since UART_PUTSRING is a blocking
function and so such communication would, by the way, require a control in the main. 