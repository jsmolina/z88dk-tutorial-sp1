# AGENTS.md
# ZX Spectrum 128K / z88dk / SP1 / AY Development Protocol

Version: 1.1
Target Machine: ZX Spectrum 128K
Compiler Model: z88dk + SDCC (`-clib=sdcc_iy`)
Renderer: SP1
Audio: AY-3-8912

---

# Mission Statement

This repository exists to build a performant, stable, authentic ZX Spectrum game using modern z88dk tooling while respecting real 8-bit hardware constraints.

The codebase prioritizes:

1. Stable 50Hz gameplay
2. Minimal screen redraw cost
3. Small binaries
4. Fast iteration
5. Hardware-accurate behavior
6. Clean separation between engine and game logic

Visual ambition must never compromise frame pacing.

---

# Canonical Technical Stack

## Required Toolchain

Mandatory:

- z88dk
- SDCC
- SP1
- `-clib=sdcc_iy`

Never replace SP1 with another rendering engine unless explicitly approved.

---

# Memory Layout

## Address Map

| Range | Purpose |
|-------|---------|
| `0x0000–0x3FFF` | ROM |
| `0x4000–0x5AFF` | Screen (SP1 display file) |
| `0x61A8 (25000)` | Code origin (`CRT_ORG_CODE`) |
| `0xC000–0xFFFF` | Paged RAM (16K window, bank-switched via port `0x7FFD`) |
| `0xD000` | IM2 vector table + stack top (`REGISTER_SP = 0xD000`) |
| `0xD1D1` | ISR jump target |

## Bank Usage

| Bank | Contents | Section |
|------|----------|---------|
| 0 | Default (code/data) | — |
| 1 | Map 1 data (800 bytes) | `BANK_1` |
| 3 | Map 2 data (800 bytes) | `BANK_3` |
| 4 | Map 3 data (800 bytes) | `BANK_4` |
| 6 | Audio engine (TinyPlayer + music/sfx data) | `BANK_6` |

Banks 2, 5, 7 are currently unused.

## Key zpragma.inc Settings

| Pragma | Value |
|--------|-------|
| `CRT_ORG_CODE` | 25000 |
| `REGISTER_SP` | 0xD000 |
| `CRT_STACK_SIZE` | 128 |
| `CLIB_STDIO_HEAP_SIZE` | 0 |

---

# Bank Switching Protocol

Bank switching is implemented in `int.asm` (`SECTION code_crt_common`, placed low in memory to avoid top-16K conflicts).

## Rules

- **Always** move the stack pointer away from the top 16K before paging (`temp_sp` pattern).
- **Always** restore bank 0 after any bank operation.
- **Never** call bank switching routines from within the top 16K (0xC000–0xFFFF).

## Routines

| Routine | Purpose |
|---------|---------|
| `enable_bank_6` | Page in bank 6 (audio). Reads nothing; hardcoded `a=6`. |
| `enable_bank_n` | Page in bank N. Caller must load `a` = bank number before `call`. |
| `restore_bank_0` | Restore bank 0 and SP. |
| `load_map_from_bank(uint8_t bank)` | C-callable. Pages in `bank`, copies 800 bytes from `0xC000` into `currentmap`, restores bank 0. |

`load_map_from_bank` uses the SDCC `sdcc_iy` calling convention: first `uint8_t` argument is passed in the `L` register.

---

# Level / Map System

## Map Data

- 3 maps, each 25 rows × 32 columns = **800 bytes**.
- Map data is stored as raw `defb` bytes in banked RAM (`maps/map1.asm`, `maps/map2.asm`, `maps/map3.asm`).
- Tile values match the macros in `globals.h` (e.g. `A=1`, `B=2`, `I=9` pill, `K=11` energizer).
- Each bank's map data starts at offset 0 in that bank (i.e. at `0xC000` when paged in).

## Working Buffer

- `currentmap[25][32]` in main RAM (800 bytes) is the **only** live copy used during gameplay.
- Pills are eaten in-place by modifying `currentmap` (values 9→16, 11→18).
- On level load or reset, `load_map_from_bank()` re-copies clean data from the bank, overwriting all mutations.

## Level Loading

```c
// next_level() in logic.c
load_map_from_bank(1); // map1 from BANK_1
load_map_from_bank(3); // map2 from BANK_3
load_map_from_bank(4); // map3 from BANK_4
```

`map_num` cycles 1→2→3→1. `level` (0–4) controls difficulty/speed independently.

## Map-Specific Constants (globals.h)

| Constant | Map 1 | Map 2 | Map 3 |
|----------|-------|-------|-------|
| `MAPx_TOTAL_POINTS` | 238 | 251 | 254 |
| `MAPx_Y_SIDE_CHG` | 12 | 10 | 12 |
| `Y_GHOSTS_HOME_MAPx` | 15 | 10 | 12 |

## Adding New Maps

1. Create `maps/mapN.asm` with `SECTION BANK_X` and `PUBLIC _mapN_data` + 800 `defb` bytes.
2. Add the file to `maps/maps.lst`.
3. Add `z88dk-appmake` and loader lines to `build.sh` and `build-p3.sh`.
4. Extend the `map_num` cycle and add a `load_map_from_bank(X)` branch in `next_level()` and `reset_map()` in `logic.c`.
5. Add per-map constants to `globals.h`.

---

# Build Outputs

| Binary | Origin | Purpose |
|--------|--------|---------|
| `msnampac_CODE.bin` | 25000 | Main game code + static data |
| `msnampac_BANK_6.bin` | 0xC000 | Audio engine |
| `msnampac_BANK_1.bin` | 0xC000 | Map 1 data |
| `msnampac_BANK_3.bin` | 0xC000 | Map 2 data |
| `msnampac_BANK_4.bin` | 0xC000 | Map 3 data |

## Tape Loader (`loader.bas`)

Loads in order: `screen` → `code` → `bank6` → `bank1` → `bank3` → `bank4`, paging in each bank before its LOAD, then restores bank 0 and jumps to 25000.

## +3 Disk Loader (`loader-p3.bas` / `build-p3.sh`)

Uses a 12-byte machine code stager at 25000 (`LDIR` from 0x8000→0xC000) to copy each block loaded to 32768 into its target bank at 0xC000. Banks are loaded in order: 6, 1, 3, 4. Bank 0 is restored, then `code` is loaded at 25000, overwriting the stager. `PAUSE 0` then `USR 25000` starts the game.

---

# Sprite & Tile Architecture

- All sprite/tile graphics are in `SECTION rodata_user` (`.asm` files under `build/` and `font/`).
- SP1 tile entries are registered at startup via `sp1_TileEntry()` in `msnampac.c:all_lives_lost()`.
- Tile value → char: `get_map_char(v)` returns `v + 96` (lowercase letters), or `' '` for 0.
- Tile value → color: `get_map_color(v)` in `logic.c`; color varies by `map_num`.

---

# ISR / Timing

- IM2 interrupt fires at 50Hz.
- ISR (`int.asm:isr`) increments `tick`, pages in bank 6, runs `FxPlay` + `MusicPlay` + `UpdateAy`, restores bank 0.
- Game loop waits on `tick` via `wait()` (`WFRAMES = 2` → ~25 FPS target).
- Stack is 128 bytes (`CRT_STACK_SIZE`); keep call depth shallow.

