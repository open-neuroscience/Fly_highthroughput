s = serialport('/dev/ttyUSB0',9600);

s.readline();

s.writeline('1');

s.readline();

s.writeline('set opto 3 10');