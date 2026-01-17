# Alec's homelab server

Runs on a Raspberry Pi Zero 2W:

- Local web server which hosts files at ~/public
  - Accessible network-wide with an Avahi .local resolve shorthand
  - Accepts AQI readings & saves them in a database on an attached drive
  - Shows readings & Airnow info on the site
- Network storage w/ Samba which automounts attached USB drives
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

The image will only partition about 4GB of the card, so use udisks to repartition to use the whole card. I recommend using an 8GB or 16GB card - larger cards can slow the system down to no benefit.

Default login is user `alec` password `nixos` (use `passwd alec` to change)

Use this to update: `nixos-rebuild switch --flake .#alechomelab --target-host alec@alechomelab.local --sudo --impure --ask-sudo-password`

## How to use

Set `AIRNOW_TOKEN=` in `webserver/.env` for Airnow.gov data to work
Set the Samba user password: `sudo smbpasswd -a alec`
Rebuild with key-protecting impurity: `sudo nixos-rebuild boot --flake path:/home/alec/homelab/ --impure`

## RPi config

```cs
start_x = 0;
gpu_mem = 16;
```

## 3D printable base case

I've made a simple model for the base of the Pi. I'm using [a kit similar to this](https://www.amazon.com/iUniker-Raspberry-Starter-Acrylic-Clear/dp/B075FLGWJL?crid=19R9M6Y2VPS1K&dib=eyJ2IjoiMSJ9.kndk7_nkqoPZ4vTxvDa9Zq7SdrswSm-4vgCWqNAxf9DGjHj071QN20LucGBJIEps.Q4Mlvhg1rg75f0sgXzmndpj8mxCsJ_E9HNwI-RCoc94&dib_tag=se&keywords=raspberry%2Bpi%2Bzero%2B2w%2Bkit&qid=1768593419&sprefix=raspberry%2Bpi%2Bzero%2B2w%2Bkit%2Caps%2C232&sr=8-21&xpid=omfYg_20kGhyg&th=1) which has a acrylic case w/ screws. 