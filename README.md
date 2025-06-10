# K-2 Assembly (8-bit) Interpreter

A custom 8-bit assembly language interpreter with a 16-instruction set architecture.

## Overview

K-2 Assembly is a simple 8-bit assembly language designed for educational purposes and embedded system simulation. Each instruction is 16 bits (2 bytes) long with a 4-bit opcode and 12 bits of data/addressing space.

## Architecture Specifications

### Instruction Format
- **Instruction Length**: 2 bytes (16 bits)
- **Format**: `<opcode> <data>`
- **Storage**: MSB first (big-endian)
- **Alignment**: First byte of each instruction must be at even addresses
- **Opcode**: First 4 bits of instruction

### Registers

#### General Purpose Registers (12)
- **R1-R12**: 8-bit general purpose registers
- **Count**: 16 total registers (0x0 to 0xF)
- **Data Width**: 8 bits each

#### Special Registers
- **RM**: Memory address register (12-bit addressing)
- **RD**: Delay register (decrements at 60Hz when non-zero)
- **RS**: Sound delay register (similar to RD)
- **RF**: Temporary register
- **PC**: Program counter (16-bit address)
- **SP**: Stack pointer (8-bit, points to stack top)

#### Stack
- **Size**: 16 entries
- **Entry Width**: 16 bits each
- **Type**: Address stack for subroutine calls

## Instruction Set

| Opcode | Mnemonic | Description | Format |
|--------|----------|-------------|---------|
| 0x0 | LD | Load immediate | `LD Rx, xyz` - Sets Rx = xyz |
| 0x1 | MV | Move register | `MV Rx, Ry` - Sets Rx = Ry |
| 0x2 | ADD | Add registers | `ADD Rx, Ry` - Rx = Rx + Ry |
| 0x3 | SUB | Subtract registers | `SUB Rx, Ry` - Rx = Rx - Ry |
| 0x4 | MULT | Multiply registers | `MULT Rx, Ry` - Rx = Rx * Ry |
| 0x5 | DIV | Divide registers | `DIV Rx, Ry` - Rx = Rx / Ry |
| 0x6 | MOD | Modulo operation | `MOD Rx, Ry` - RF = Rx % Ry |
| 0x7 | SKP | Skip if equal | `SKP Rx, Ry` - Skip next instruction if Rx == Ry |
| 0x8 | SNE | Skip if not equal | `SNE Rx, Ry` - Skip next instruction if Rx != Ry |
| 0x9 | J | Jump | `J addr` - PC = addr |
| 0xA | CALL | Call subroutine | `CALL addr` - Push PC to stack, jump to addr |
| 0xB | RET | Return | `RET` - Pop PC from stack |
| 0xC | LA | Load address | `LA addr` - Set RM = addr |
| 0xD | SRA | Shift right arithmetic | `SRA Rx, Ry` - Rx = Rx >> Ry, set RF if MSB was 1 |
| 0xE | SLA | Shift left arithmetic | `SLA Rx, Ry` - Rx = Rx << Ry, set RF if MSB was 1 |
| 0xF | WA | Write to address | `WA Rx, addr` - Write Rx to memory address |

### Instruction Encoding Details

#### Register-Register Operations (MV, ADD, SUB, MULT, DIV, MOD, SKP, SNE, SRA, SLA)
```
Bits: 15-12 | 11-8 | 7-4 | 3-0
      Opcode|  Rx  | Ry  | 0000
```

#### Immediate Load (LD)
```
Bits: 15-12 | 11-8 | 7-0
      Opcode|  Rx  | xyz (8-bit immediate)
```

#### Jump/Call Instructions (J, CALL, LA)
```
Bits: 15-12 | 11-0
      Opcode| addr (12-bit address)
```

#### Write Address (WA)
```
Bits: 15-12 | 11-8 | 7-0
      Opcode|  Rx  | addr (8-bit address)
```

#### Return (RET)
```
Bits: 15-12 | 11-0
      Opcode| 000000000000
```

## Memory Layout

### Total Memory: 4096 bytes (0x000 - 0xFFF)

| Address Range | Purpose | Description |
|---------------|---------|-------------|
| 0x000 - 0x400 | Program Memory | Instruction storage (512 max instructions) |
| 0x401 - 0xFFF | Data Memory | Free-use data storage |

### Initial State
- All memory initialized to 0
- All registers initialized to 0
- Stack initialized to 0
- PC starts at 0x000
- SP starts at 0

## Data Types

```c
uint8_t registers[16];    // General purpose registers
uint16_t RM;             // Memory address register
uint8_t RS;              // Sound delay register
uint16_t RF;             // Temporary register
uint16_t PC;             // Program counter
uint8_t SP;              // Stack pointer
uint16_t stack[16];      // Call stack
uint8_t memory[4096];    // Main memory
```

## Usage Examples

### Basic Arithmetic
```assembly
LD R1, 0x05    ; Load 5 into R1
LD R2, 0x03    ; Load 3 into R2
ADD R1, R2     ; R1 = R1 + R2 (R1 now contains 8)
```

### Conditional Branching
```assembly
LD R1, 0x0A    ; Load 10 into R1
LD R2, 0x0A    ; Load 10 into R2
SKP R1, R2     ; Skip next instruction if R1 == R2
J 0x100        ; This instruction is skipped
```

### Subroutine Call
```assembly
CALL 0x200     ; Call subroutine at address 0x200
; ... main code continues here after return

; Subroutine at 0x200:
LD R1, 0xFF    ; Some subroutine code
RET            ; Return to caller
```

### Memory Operations
```assembly
LA 0x500       ; Load address 0x500 into RM
LD R1, 0x42    ; Load value 0x42 into R1
WA R1, 0x00    ; Write R1 to address 0x500 (using lower 8 bits)
```

## Implementation Notes

- Instructions must be aligned to even addresses
- The delay register (RD) decrements at 60Hz when non-zero
- Shift operations set RF flag when the original MSB was 1
- Division by zero behavior should be defined by implementation
- Stack overflow/underflow handling should be implemented
- Memory access bounds checking recommended

## Example Program
```asm
; K-2 Assembly - Simple "Hello World" Example
; This program demonstrates basic K-2 Assembly operations

; Load two numbers and add them
LD R1, 0x05        ; Load 5 into register R1
LD R2, 0x03        ; Load 3 into register R2
ADD R1, R2         ; R1 = R1 + R2 (R1 now contains 8)

; Store result in memory
LA 0x500           ; Load address 0x500 into memory register RM
WA R1, 0x00        ; Write R1 to memory address 0x500

; Simple conditional check
LD R3, 0x08        ; Load expected result (8)
SKP R1, R3         ; Skip next instruction if R1 equals R3
J 0x020            ; Jump to error if not equal

; Success path - store "OK" status
LD R4, 0x4F        ; Load 'O' (79)
WA R4, 0x01        ; Write to 0x501
LD R4, 0x4B        ; Load 'K' (75)  
WA R4, 0x02        ; Write to 0x502
J 0x030            ; Jump to end

; Error path - store "ERR" status
LD R4, 0x45        ; Load 'E' (69)
WA R4, 0x01        ; Write to 0x501
LD R4, 0x52        ; Load 'R' (82)
WA R4, 0x02        ; Write to 0x502
WA R4, 0x03        ; Write to 0x503

; Program end - infinite loop
J 0x030            ; Halt by jumping to self

; Memory Layout:
; 0x500: Result of addition (8)
; 0x501-0x503: Status message ("OK" or "ERR")
```
