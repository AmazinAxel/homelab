# Alec's homelab server

Features:

- Local web server which hosts files at ~/public
  - accessible network-wide with an Avahi .local resolve shorthand
- Network storage w/ Samba which auto mounts attached USB drives
- Systemd tasks which:
  - auto-update Github flake locks
  - make backups of Onshape models & private Github repos
  - downloads Spotify playlists for offline usage


## Some useful commands

`sudo smbpasswd -a alec` (set the user password for Samba)

`sudo nixos-rebuild boot --flake path:/home/alec/homelab/ --impure` (rebuild with key-protecting impurity)
