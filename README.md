# CAB202 - Microprocessors and Digital Systems

general coursework and notes and stuff for CAB202

**contents**:

- [using the CAB202 avr emulator](#using-the-cab202-avr-emulator)
	- [events.txt](#formatting-events.txt)
 	- [peripheral events](#peripheral-event-strings)
- [environment](#environment-setup)
	- [pio on the command line](#using-the-pio-cli-tool)
 	- [neovim config for platformio](#neovim-and-clangd-with-platformio-projects)
        - [troubleshooting](#troubleshooting)

 > and then some other stuff i cant be bothered doing the table of contents entries for (die).

## USING THE CAB202 AVR EMULATOR

clone the [cab202 avremu repo](https://github.com/cab202/avremu) repo, and install [rustup](https://rustup.rs/) 
so `cargo` is available. `cd` into the repo and run `cargo install --path .` to build a release binary and copy it to
`/$HOME/.cargo/bin/avremu` (which should be on system `PATH` anyway).

```bash
git clone https://github.com/cab202/avremu
cd ./avremu
cargo install --path .

# [ ... ]

avremu --version
avremu 1.0.0
```

#### formatting events

i didnt find this easier than just using the canvas autograder/actual microcontroller, but it occasionally helped during bugfixing.

the emulator is fed a plaintext file (the canvas autograder uses `events.txt`) to specify the timing and action of an event:

```rust
// uses a regex selector to read events from an events file:
// ...
let re_events = Regex::new("@([0-9A-F-a-f]+)\\s+(.+):\\s+(.+)\\n+").unwrap();

// this is selecting for the following, which is one event:

@<TIME_IN_HEX><whitespace><PERIPHERAL>:<whitespace><EVENT>

// ... where multiple events are separated by a newline, eg:

@000000000001 S1: PRESS
@000000000002 S1: RELEASE
```

> see `misc/events-example.txt`

**assuming you have built the `firmware.hex` with `pio run`:**

write a hex timecode, followed by the device and a semicolon, and then the event into a plaintext file,
eg. `echo '@000000000000 S1: PRESS' > events.txt`,
and then pass that file to the `-e/--event `avremu -e events.txt .pio/build/QUTy/firmware.hex`.

#### peripheral event strings

> the source code names hardware and peripherals as 'devices'.

**PUSHBUTTONS:**
- devices:
    - `S1`
    - `S2`
    - `S3`
    - `S4`
- events:
    - `PRESS`
    - `RELEASE`

e.g:
```
@0000FFFFFF S1: PRESS
```

**POTENTIOMETER:**
- devices:
    - `R1`
- events:
    - set the rotation of the pot via a floating point number in the range `0.000..=1.000`

e.g:
```
@0000FFFFFF R1: 0.111
```

ultimately, you want to build your project with `pio run`, and then run the firmware with the emulator using
e.g `avremu -e path/to/events.txt .pio/build/QUTy/firmware.hex`:
```bash
# build project

pio run
Processing QUTy (platform: quty; board: QUTy)

Verbose mode can be enabled via `-v, --verbose` option
CONFIGURATION: https://docs.platformio.org/page/boards/quty/QUTy.html
PLATFORM: QUTy (1.0.2+sha.f87d4a8) > QUTy
HARDWARE: ATTINY1626 3MHz, 2KB RAM, 16KB Flash

# [...]

======================================= [SUCCESS] Took 0.46 seconds ============== # ...

# run emulator

avremu -e ../misc/events-example.txt  .pio/build/QUTy/firmware.hex
[FIRMWARE] .pio/build/QUTy/firmware.hex.
[EVENTS] ../misc/events-example.txt: Parsed 20 events.
[RUN] No emulation time limit specified.
[WARNING] PORTMUX is not implemented in this emulator. Writes to PORTMUX registers will have no effect.
[@0000001000E0] PB|S1: Pressed
[@0000001000E0] PB|S1: Released
# [...]
[@0000005590B0] DISP|DS1: Off (LHS)
[@000000EE2348] DISP|DS1:    (100 Hz, 50 %)
[@000000080AFC] BUZZER|P1: 0.0 Hz, 0.0 % duty cycle
^C
```

## environment setup

**below are some "brief" notes on my environment setup (in no particular order)**:
- general platformio and atmelavr library/compiler setup,
- neovim language server setup,
- troubleshooting (including sourcing `avr-libc` headers for `clangd`)

### using the `pio` cli tool  

on arch, using a virtual environment was easiest. `platformio` very much wants to do everything its own automatic way, 
so using something like [`pyenv`](https://github.com/pyenv/pyenv) means it can pull whatever `pypi` packages it wants on-demand:

```bash
# install pio with `pip` in a venv - e.g with pyenv
pyenv virtualenv 3.11 pio
pyenv activate pio
pip install --upgrade pip && pip install platformio
pio --help
```

### neovim and clangd with platformio projects

`clangd` can hook into the `pio` dependencies with a `compile_commands.json` compilation database, which pio can generate:

```bash
# in the project root with `platformio.ini`:
pio run -t compiledb
```


### troubleshooting

#### fixing the `multiple offset_encodings` warning in neovim

i am using `mason`, `mason-lspconfig` and `nvim-lspconfig` in my neovim config.
> see [my lsp.lua](https://github.com/plsuwu/dotfiles/blob/main/nvim/lua/plugins/lsp.lua) to contextualize the snippet below:

```lua
-- ... lsp setup ...
require("mason-lspconfig").setup({
			-- ...
			handlers = {
				-- ...
				["clangd"] = function()
					local lspconfig = require("lspconfig")
					lspconfig.clangd.setup({
						capabilities = {
							capabilities,

							-- we explicitly specify 'utf-8' in the 'offsetEncoding'
							-- field in the lsp capabilities table:
							offsetEncoding = "utf-8",
						},
					})
				end,
			},
-- ...
-- ... lsp setup ...
```

### `udev` ruleset

installing the `platformio-core-udev` package works fine:

```bash
sudo pacman -S platformio-core-udev
```

- udev rules can also be installed manually if the package on `pacman` is like, out of date or something:

```bash
# install udev rules (this will be different depending on OS - see https://docs.platformio.org/en/stable/core/installation/udev-rules.html)
curl -fsSL \
    https://raw.githubusercontent.com/platformio/platformio-core/develop/platformio/assets/system/99-platformio-udev.rules \
    | sudo tee /lib/udev/rules.d/99-platformio-udev.rules # this may be at `/etc/udev/...` rather than `/lib/udev` on non-arch systems, see above.

# restart udev manager
sudo udevadm control --reload-rules
sudo udevadm trigger

# add user to uucp group
sudo usermod -a -G uucp $USER
sudo usermod -a -G lock $USER
```

### pio & avr-libc dependencies

as a venv alternative, you could *try* to do this manually by installing the following packages:

- `platformio-core` & the `platformio` `udev` ruleset
- `avr-gcc`
- `avr-libc`
- `pyprogmcu`

```bash
sudo pacman -S platformio-core platformio-core-udev avr-gcc avr-libc

# this doesn't get sourced by pio how we would like it to, but it needs this library to flash the firmware:
yay -S python-pymcuprog # some `pio` commands require this package
```

this was more temperamental compared to just using a venv. just use the venv. 

### `QUTy` platform pkg installation

```bash
pio pkg install --platform "https://github.com/cab202/quty" --global

# this is installed by `pio` automatically/as required, but for reference:
pio pkg install --tool toolchain-atmelavr --global
```
