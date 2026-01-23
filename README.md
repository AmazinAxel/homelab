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
Use remote deployments to update: `nixos-rebuild switch --flake .#alechomelab --sudo --ask-sudo-password --target-host alec@alechomelab.local`
Update the client key and secret to use your own from the [Spotify developer dashboard.](https://developer.spotify.com/dashboard) in `/home/alec/.config/spotdl/config.json`. If you get a rate limit error, enable the `no_cache` option.

## RPi config

Replace the `config.txt` file in the Pi's FIRMWARE partition with:

```txt
# For proper boot
kernel=u-boot-rpi3.bin
arm_64bit=1
enable_uart=1

# Disable display output
gpu_mem=16
disable_fw_kms_setup=1
disable_overscan=1
hdmi_force_hotplug=0
hdmi_blanking=2

# Turn on spi and i2c
dtparam=audio=off
dtoverlay=disable-bt
dtoverlay=sdtweak,poll_once
dtparam=spi=on
dtparam=i2c_arm=on

# Faster boot
boot_delay=0
disable_splash=1
avoid_warnings=1
```

## 3D printable base case

I've made a simple model for the base of the Pi. I'm using [a kit similar to this](https://www.amazon.com/iUniker-Raspberry-Starter-Acrylic-Clear/dp/B075FLGWJL?crid=19R9M6Y2VPS1K&dib=eyJ2IjoiMSJ9.kndk7_nkqoPZ4vTxvDa9Zq7SdrswSm-4vgCWqNAxf9DGjHj071QN20LucGBJIEps.Q4Mlvhg1rg75f0sgXzmndpj8mxCsJ_E9HNwI-RCoc94&dib_tag=se&keywords=raspberry%2Bpi%2Bzero%2B2w%2Bkit&qid=1768593419&sprefix=raspberry%2Bpi%2Bzero%2B2w%2Bkit%2Caps%2C232&sr=8-21&xpid=omfYg_20kGhyg&th=1) which has a acrylic case w/ screws. 