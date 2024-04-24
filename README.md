# CAB202 | Microprocessors and Digital Systems

> coursework for CAB202

**below are some "brief" notes on my environment setup (in no particular order)**:
- general platformio and atmelavr library/compiler setup,
- neovim language server setup,
- troubleshooting (including sourcing `avr-libc` headers for `clangd`)

## pio & avr-libc dependencies

- `platformio-core` & the `platformio` `udev` ruleset
- `avr-gcc`
- `avr-libc`
- `pyprogmcu`

```bash
sudo pacman -S platformio-core platformio-core-udev avr-gcc avr-libc
yay -S python-pymcuprog # some `pio` commands require this package
```

## `QUTy` platform pkg installation

```bash
pio pkg install --platform "https://github.com/cab202/quty" --global

# i believe this will be installed by `pio` when required, but for reference:
pio pkg install --tool toolchain-atmelavr --global
```

## `clangd` `avr-libc` & `avr-gcc` integration

generate a `compile_commands.json` compilation database:

```bash
# in the project root (same directory as `platformio.ini`):
pio run -t compiledb
```
## troubleshooting

### fixing the `multiple offset_encodings` warning

- i am using `mason`, `mason-lspconfig` and `nvim-lspconfig` in my neovim config

```lua
-- ... lsp setup ...

	["clangd"] = function()
		local lspconfig = require("lspconfig")
		lspconfig.clangd.setup({
			capabilities = {
				capabilities,

                -- explicitly specify 'utf-8' in the 'offsetEncoding'
                -- field of `capabilities`:
				offsetEncoding = "utf-8",
			},
		})
	end,

-- ... lsp setup ...
```

### manual `udev` ruleset installation

- udev rules can also be installed manually (though this was not in sync with the version of `platformio-core` on `pacman` or `pypi`):

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
