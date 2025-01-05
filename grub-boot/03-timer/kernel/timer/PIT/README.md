# PIT
The Intel 8254 is a counter/timer device designed to solve the common timing control problems in microcomputer system design. It provides three independent 16-bit counters, each capable of handling clock inputs up to 10 MHz. All modes are software programmable. The 8254 is a superset of the 8253.

The Programmable Interval Timer (PIT) chip (Intel 8253/8254) basically consists of an oscillator, a prescaler and 3 independent frequency dividers. Each frequency divider has an output(as the PIT chip has 3 pins labeled OUT), which is used to allow the timer to control external circuitry.

The input clock frequency of the PIT chip runs (roughly) at 1.193182 MHz(1193182 Hz), which is 1/12 the frequency of a 14.31818 MHz oscillator.

(<a href="https://www.scs.stanford.edu/23wi-cs212/pintos/specs/8254.pdf" target="_blank"><u>Must read this document, that is 8254 PROGRAMMABLE INTERVAL TIMER datasheet</u></a>)
<br><br>
(<a href="https://wiki.osdev.org/Programmable_Interval_Timer"><b>https://wiki.osdev.org/Programmable_Interval_Timer</b></a>)


## I/O PORT

The Control Word Register and the three Counters have separate addresses.

| I/O Ports | Usagea |
|-----------|--------|
| 0x40 | Channel 0 data port(read/write) |
| 0x41 | Channel 1 data port(read/write) |
| 0x42 | Channel 2 data port(read/write) |
| 0x43 | Control Word Regiser or Mode/Command register (write only, a read is ignored) |

Each data port is 8-bit.

The Mode/Command register at I/O address 0x43 contains the following: 

| Bits   | Usage                                                                                                                                       |
|--------|---------------------------------------------------------------------------------------------------------------------------------------------|
| 6 and 7| **Select channel:** <br> &nbsp;&nbsp;&nbsp;&nbsp; 0 0 = Channel 0 <br> &nbsp;&nbsp;&nbsp;&nbsp; 0 1 = Channel 1 <br> &nbsp;&nbsp;&nbsp;&nbsp; 1 0 = Channel 2 <br> &nbsp;&nbsp;&nbsp;&nbsp; 1 1 = Read-back command (8254 only)|
| 4 and 5| **Access mode:** <br> &nbsp;&nbsp;&nbsp;&nbsp; 0 0 = Latch count value command <br> &nbsp;&nbsp;&nbsp;&nbsp; 0 1 = Access mode: lobyte only <br> &nbsp;&nbsp;&nbsp;&nbsp; 1 0 = Access mode: hibyte only <br> &nbsp;&nbsp;&nbsp;&nbsp; 1 1 = Access mode: lobyte/hibyte|
| 1 to 3 | **Operating mode:** <br> &nbsp;&nbsp;&nbsp;&nbsp; 0 0 0 = Mode 0 (interrupt on terminal count) <br> &nbsp;&nbsp;&nbsp;&nbsp; 0 0 1 = Mode 1 (hardware re-triggerable one-shot) <br> &nbsp;&nbsp;&nbsp;&nbsp; 0 1 0 = Mode 2 (rate generator) <br> &nbsp;&nbsp;&nbsp;&nbsp; 0 1 1 = Mode 3 (square wave generator) <br> &nbsp;&nbsp;&nbsp;&nbsp; 1 0 0 = Mode 4 (software triggered strobe) <br> &nbsp;&nbsp;&nbsp;&nbsp; 1 0 1 = Mode 5 (hardware triggered strobe) <br> &nbsp;&nbsp;&nbsp;&nbsp; 1 1 0 = Mode 2 (rate generator, same as 010b) <br> &nbsp;&nbsp;&nbsp;&nbsp; 1 1 1 = Mode 3 (square wave generator, same as 011b)|
| 0      | **BCD/Binary mode:** 0 = 16-bit binary, 1 = four-digit BCD                                                                                  |


The **current counter** value is always either decremented or reset to the reload value on the falling edge of the (1.193182 MHz) input signal.


## Programming the 8254

Counters are programmed by writing a Control Word into the Control Word Register(Command Register) and then an initial count.

By contrast, initial counts are written into the Counters, not the Control Word Register. The A1, A0 inputs(bit 6 and 7 of Command Register) are used to select the Counter to be written into. The format of the initial count is determined by the Control Word used.


There are two basic operations in here:

- Write Operations : Configuring and set a initial count value
- Read Operations 

__If acess mode is lobyte/hibyte, then LSB is read/write first followed by MSB__


#### Write Operations

The programming procedure for the 8254 is very flexible. Only two conventions need to be remembered:

1. For each Counter, the Control Word must be written before the initial count is written.

2. The initial count must follow the count format specified in the Control Word (least significant byte only, most significant byte only, or least significant byte and then most significant byte)

A new initial count may be written to a Counter at any time without affecting the Counter’s programmed Mode in any way

#### Read Operations

There are three possible methods for reading the counters: a simple read operation, the Counter Latch Command, and the Read-Back Command.


##### Counter Latch Command

Like a Control Word, this command is written to the Control Word Register. The selected Counter’s output latch (OL) latches the count at the time the Counter Latch Command is
received. This __Count is held in the latch until it is read by the CPU (or until the Counter is reprogrammed)__. The count is then unlatched automatically and the OL returns to ‘following’ the counting element. This allows reading the contents of the Counters ‘on the fly’ without affecting counting in progress.

If a Counter is latched and then, some time later, latched again before the count is read, the second Counter Latch Command is ignored. With either method, the count must be read according to the programmed format; specifically, if the Counter is programmed for two byte counts, two bytes must be read.

Counter Latching Command Format is as below:

| D7 | D6 | D5 | D4 | D3 | D2 | D1|
|----|----|----|----|----|----|---|
| SC1 | SC0 | 0 | 0 | X | X | X | X |

__NOTE :__ Don’t care bits (X) should be 0.

SC1, SC0 – specify counter to be latched

| SC1 | SC2 | Counter Selected |
|-----|-----|------|
|  0  | 0 | Counter 0 |
| 0 | 1 | Counter 1 |
| 1 | 0 | Counter 2 |

##### READ-BACK Command

Read-Back Command allows the user to check the __count value__, __programmed Mode__, and __current states of the OUT pin__ and Null Count flag of the selected counter(s).

The command is written into the Control Word Register, with following format:

| Bits | Value | Usages |
|------|-------|--------|
|bit 7 | 1 | |
|bit 6 | 1 | |
|bit 5 | COUNT | 0 = Latch count of selected counters(s) |
|bit 4 | STATUS| 0 = Latch status of selected counters(s) |
|bit 3 | CNT2  | 1 = Select Counter 2 |
|bit 2 | CNT1  | 1 = Select Counter 1 |
|bit 1 | CNT0  | 1 = Select Counter 0 |
|bit 0 | 0     | Reserved for future expansion; Must be 0 |

The read-back command may be used to latch multiple counter output latches (OL) by setting the COUNT bit D5(or bit5) = 0 and selecting the desired counter(s). This single command is functionally equivalent to several counter latch commands, one for each counter latched. Then we can read latched count value by reading to counter ports directly. Each counter’s latched count is held until it is read (or the counter is reprogrammed.

The read-back command may also be used to latch status information of selected counter(s) by setting STATUS bit D4(or bit4) = 0. The counter status format is as below:

| D7 | D6 | D5 | D4 | D3 | D2 | D1|
|----|----|----|----|----|----|---|
| Output | Null Count | RW1 | RW0 | M2 | M1 | M0 | BCD |


D7 : 1 = OUT Pin is 1  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 0 = OUT Pin is 0  

D6 : 1 = Null Count  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 0 = Count available for reading  

D5–D0 : Counter programmed mode(RW = Access Bits, M = Operating Mode Bits)

After issuing the read-back command, you can read the latched data (e.g., the counter value) from the channel's I/O port(as the command word is write-only). Depending on the access mode (e.g., "Read/Write LSB then MSB"), you may need multiple reads to get the full counter value.

__Note__ : The PIT does not allow direct reading of the counter values unless a specific read-back command is issued. Without issuing a read-back command, the values returned may not be meaningful.

### MODE 0: INTERRUPT ON TERMINAL COUNT

Mode 0 is typically used for event counting. After the Control Word is written, OUT is initially low, and will remain low until the Counter reaches zero. OUT then goes high and remains high until a new count or a new Mode 0 Control Word is written into the Counter.


### MODE 1: HARDWARE RETRIGGERABLE ONE-SHOT

OUT will be initially high. OUT will go low on the CLK pulse following a trigger to begin the one-shot pulse, and will remain low until the Counter reaches zero. OUT will then go high and remain high until the CLK pulse after the next trigger.


### MODE 2: RATE GENERATOR

This Mode functions like a divide-by-N counter. It is typically used to generate a Real Time Clock interrupt. OUT will initially be high. When the initial count has decremented to 1, OUT goes low for one CLK pulse. OUT then goes high again, the Counter reloads the initial count and the process is repeated. Mode 2 is periodic; the same sequence is repeated indefinitely. For an initial count of N, the sequence repeats every N CLK cycles.
