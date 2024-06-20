# CAB202 - Microprocessors and Digital Systems

> general coursework and notes and stuff for CAB202

this README contains some potentially handy notes on things like
- using the CAB202 QUTy emulator,
- neovim LSP setup,
- pio setup,
- arch linux-related stuff throughout.

## USING THE CAB202 AVR EMULATOR

assuming the [cab202 avremu repo](https://github.com/cab202/avremu) has been
cloned somewhere locally and cargo is available on the system path (see
[rustup](https://rustup.rs/) website if not), we can `cd` into the repo and install
the `avremu` binary into the system path with `cargo install --path .`.

```bash
$ avremu --version
avremu 1.0.0
```

#### Formatting `events.txt`

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

#### Peripheral strings

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
    - > set a floating point number anywhere from `0.000..=1.000`

e.g:
```
@0000FFFFFF R1: 0.111
```

## ENVIRONMENT SETUP - NEOVIM, ARCH LINUX, PIO

**below are some "brief" notes on my environment setup (in no particular order)**:
- general platformio and atmelavr library/compiler setup,
- neovim language server setup,
- troubleshooting (including sourcing `avr-libc` headers for `clangd`)

### pio & avr-libc dependencies

- `platformio-core` & the `platformio` `udev` ruleset
- `avr-gcc`
- `avr-libc`
- `pyprogmcu`

```bash
sudo pacman -S platformio-core platformio-core-udev avr-gcc avr-libc

# i am unsure if this works correctly so maybe a venv is required...
yay -S python-pymcuprog # some `pio` commands require this package
```

### `QUTy` platform pkg installation

```bash
pio pkg install --platform "https://github.com/cab202/quty" --global

# i believe this will be installed by `pio` when required, but for reference:
pio pkg install --tool toolchain-atmelavr --global
```

### `clangd`, `avr-libc` & `avr-gcc` integration

generate a `compile_commands.json` compilation database:

```bash
# in the project root (same directory as `platformio.ini`):
pio run -t compiledb
```
### troubleshooting

#### fixing the `multiple offset_encodings` warning in neovim

- i am using `mason`, `mason-lspconfig` and `nvim-lspconfig` in my neovim config

```lua
-- ... lsp setup ...

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

-- ... lsp setup ...
```

### manual `udev` ruleset installation

- udev rules can also be installed manually (though the package on pacman is fine):

```bash
# install udev rules (this will be different depending on OS - see https://docs.platformio.org/en/stable/core/installation/udev-rules.html)
curl -fsSL \
    https://raw.githubusercontent.com/platformio/platformio-core/develop/platformio/assets/system/99-platformio-udev.rules \
    | sudo tee /lib/udev/rules.d/99-platformio-udev.rules # this may be at `/etc/udev/...` on non-arch systems, see above.

# restart udev manager
sudo udevadm control --reload-rules
sudo udevadm trigger

# add user to uucp group
sudo usermod -a -G uucp $USER
sudo usermod -a -G lock $USER
```

### python virtual environment

alternatively, a venv lets `platformio` automatically install any required `pypi` packages on-demand:

```bash
# install pio with `pip` in a venv - e.g with pyenv
pyenv virtualenv 3.11 pio
pyenv activate pio
pip install --upgrade pip && pip install platformio
pio --help
```
