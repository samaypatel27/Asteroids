# Lab 4: Asteroids Simulation

A multi-ship asteroids game built in C with dynamic memory management and linked list implementation.

## What It Does

This project simulates ships flying around and shooting rocks. When shots hit rocks, the rocks break into smaller pieces. Ships can crash into rocks and rocks can bump into each other.

### Main Features
- Multiple ships that can fire shots
- Linked list to track all the shots in the air
- Rocks that split when hit
- File input/output for game configuration
- Collision detection between ships, rocks, and shots

### Bonus Features
- **Bonus 1**: Shots actually hit and break rocks (10 points)
- **Bonus 2**: Playable mode - control ships with keyboard (10 points)
- **Bonus 3**: Custom linked list code instead of library (10 points)

## How to Build and Run

```bash
cd files
make -r all
./lab4 [inputFile] [outputFile]
```

### Examples

Basic run:
```bash
./lab4 xZ1.rx output.txt
```

With bonus mode (graphics required):
```bash
./lab4 xZB.rx output.txt bonus 2>diagnostics.txt
```

## How It Works

The program reads ship and rock positions from an input file, simulates their movement, handles collisions, and writes results to an output file. 

**Linked List**: Shots are stored in a custom linked list that keeps them sorted by position. The list supports adding, removing, and looping through shots.

**Memory Management**: Uses dynamic memory allocation for shots and list nodes. Handles allocation failures gracefully.

**Rock Splitting**: When a shot hits a rock, the rock splits into two smaller rocks that fly apart in opposite directions.


## Technical Details

- Written in C
- Custom linked list with `insert`, `delete`, and `iterate` functions
- File I/O with command line arguments
- Collision physics and rock fission
- Unreliable memory allocation testing

## Test Files

- `xZ1.rx` - Basic shot and rock splitting
- `xZ0.rx` - Ships firing multiple times
- `xZB.rx` - Three ships, good for multiplayer
- `xZS.rx` - Single ship vs one huge rock
