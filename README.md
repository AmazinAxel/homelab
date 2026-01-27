# Alec's homelab server

Runs on a Raspberry Pi Zero 2W:

- Accessible network-wide with an Avahi .local resolve shorthand
- Local webserver that accepts & stores AQI readings (on attached drive) and graphs it
- Network storage w/ Samba share which automounts attached USB drives
  - Integrates with my [desktop flake](https://github.com/amazinaxel/flake) for weekly reminders to sync data from this share
- Daily systemd tasks which make backups of private Github repos & downloads Spotify playlists

## Build on your own system

Add cross compiling support to your host Nix config: `boot.binfmt.emulatedSystems = [ "aarch64-linux" ];`
Rebuild your host and set `/home/alec/GithubToken` to a PAT to back up private Github repos.

```bash
nix run nixpkgs#nixos-generators -- -f sd-aarch64 --flake .#alechomelab --system aarch64-linux -o ./alechomelabsd
sudo cp alechomelabsd/sd-image/nixos-image-sd-card-<hash here>-aarch64-linux.img.zst ./alechomelabsd-compressed-sd-image.img.zst

unzstd -d alechomelabsd-compressed-sd-image.img.zst -o alechomelabsd-sd-image.img
sudo dd if=./alechomelabsd-sd-image.img of=/dev/sdX bs=1M status=progress
```

The image will only partition about 4GB of the card, so use udisks to resize the main partition to use the whole card. I recommend using an 8GB or 16GB card - larger cards can slow the system down and isn't used for file storage.

Default login is user `alec` password `nixos` (use `passwd alec` to change)

## How to use

Set `AIRNOW_TOKEN=` in `webserver/.env` for Airnow.gov data to work
Paste your Github auth token to `/home/alec/GithubToken`
Set the Samba user password: `sudo smbpasswd -a alec`
Update the client key and secret to use your own from the [Spotify developer dashboard.](https://developer.spotify.com/dashboard) in `/home/alec/.config/spotdl/config.json`. If you get a rate limit error, enable the `no_cache` option.

Use remote deployments to update: `nixos-rebuild switch --flake .#alechomelab --sudo --ask-sudo-password --target-host alec@alechomelab.local`

## RPi config

Replace the `config.txt` file in the Pi's FIRMWARE partition with:

```txt
# For proper boot
kernel=u-boot-rpi3.bin
arm_64bit=1
enable_uart=1

# Turn on spi & i2c and gpio buttons
dtparam=spi=on
dtparam=i2c_arm=on
gpio=6,19,5,26,13,21,20,16=pu

# Disable hdmi output
gpu_mem=16
disable_fw_kms_setup=1
disable_overscan=1
hdmi_force_hotplug=0
hdmi_blanking=2

# Faster boot
boot_delay=0
disable_splash=1
avoid_warnings=1
```

## 3D printable base case

I've made a simple model for the base of the Pi. I'm using [this acrylic kit](https://www.amazon.com/dp/B075FLGWJL) which I acquired from Daydream Seattle.

# Building the display program

Use the nix devshell to install gcc & the libraries for the program: `cd display && nix develop`
Then run `deploytohomelab` to build the program and send it directly over SFTP to the device.
You can run the `./homelabDisplay` binary in `/home/alec/homelab/display/` or reboot the Pi. 

Note that you will need GPIO header pins and this [1.44in Waveshare HAT](https://www.waveshare.com/1.44inch-lcd-hat.htm) to use the display program.
