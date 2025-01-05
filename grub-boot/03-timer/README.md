# Hardware Timers

The Most Common Hardware Timers are:

- Programmable Interval Timer (PIT) : A timer that generates periodic interrupts. Accessed via I/O Ports. [link](./kernel/timer/PIT/README.md)

- High Precision Event Timer (HPET) : A high-precision timer providing multiple timers and counters. Accessed via Memory Mapped I/O.

- Real-Time Clock (RTC) : A clock and timer maintaining real-time and generating periodic/alarm interrupts. Accessed via I/O Ports.

- Advanced Programmable Interrupt Controller (APIC) Timer : A high-precision timer integrated into the Advanced Programmable Interrupt Controller. Accessed via Memory Mapped I/O.


## Compile the Project

```bash
make
```

This will create a .iso disk image in `release/iso` folder.

## Run the Project

```bash
make run
```

## Clean the Project Directory

```bash
make clean
```
