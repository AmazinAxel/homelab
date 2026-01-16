# Alec's homelab server

Features:

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
mv alechomelabsd/sd-image/nixos-image-sd-card-<hash here>-aarch64-linux.img.zst ./alechomelabsd-compressed-sd-image.img.zst

unzstd -d alechomelabsd-compressed-sd-image.img.zst -o alechomelabsd-sd-image.img
sudo dd if=./alechomelabsd-sd-image.img of=/dev/sdb bs=1M status=progress
```

Use this to update: `nixos-rebuild switch --flake .#alechomelab --target-host alec@<ip here> --use-remote-sudo`

## How to use

Set `AIRNOW_TOKEN=` in `webserver/.env` for Airnow.gov data to work
Set the Samba user password: `sudo smbpasswd -a alec`
Rebuild with key-protecting impurity: `sudo nixos-rebuild boot --flake path:/home/alec/homelab/ --impure`

## RPi config

```cs
start_x = 0;
gpu_mem = 16;
```
