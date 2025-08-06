# Alec's homelab server

Features:

- Local web server which hosts files at ~/public
  - accessible network-wide with an Avahi .local resolve shorthand
- Network storage w/ Samba which automounts attached USB drives
- Systemd tasks which:
  - make backups of private Github repos
  - downloads Spotify playlists for offline usage
  - Saves an image with an attached RPi (rev1.3) camera

## Some useful commands

`sudo smbpasswd -a alec` (set the user password for Samba)

`sudo nixos-rebuild boot --flake path:/home/alec/homelab/ --impure` (rebuild with key-protecting impurity)
