# Client-Server Model

The Assignment: 

Machine ‘A’ will send a C struct comprising of an integer value, a character value and a float value to Machine ‘B’.

Machine ‘B’ changes each of the value in the following way – it doubles the integer value (a 2 becomes a 4), adds 1 to the float value (a 2.3 becomes a 3.3) and increases the character value to the next higher character with a roll over (i.e a ‘z’ becomes ‘a’, ‘b’ becomes a ‘c’). Machine ‘B’ then prints the original values that it received and the modified values as well (label these values as “Before” and “After” values). Machine ‘B’ then transmits these values to another machine ‘C’.

Machine ‘C’ prints the values that it received from the server and terminates the connection. All other processes are terminated as well.

Compile and run this exercise in the Unix domain.
