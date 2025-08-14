# Alec's homelab server

Features:

- Local web server which hosts files at ~/public
  - Accessible network-wide with an Avahi .local resolve shorthand
  - Accepts AQI readings & saves them in a database on an attached drive
  - Shows readings & Airnow info on the site
- Network storage w/ Samba which automounts attached USB drives
- Daily systemd tasks which make backups of private Github repos & downloads Spotify playlists

## How to use

Set `AIRNOW_TOKEN=` in the `/webserver/.env` for Airnow data to work
Set the Samba user password: `sudo smbpasswd -a alec`
Rebuild with key-protecting impurity: `sudo nixos-rebuild boot --flake path:/home/alec/homelab/ --impure`
