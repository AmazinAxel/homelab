# Alec's homelab server

Features:

- Local web server which hosts files at ~/public
  - accessible network-wide with an Avahi .local resolve shorthand
- Network storage w/ Samba which auto mounts attached USB drives
- Systemd tasks which:
  - auto-update Github flake locks
  - make backups of Onshape models & private Github repos
  - downloads Spotify playlists for offline usage

## Github & Onshape integration

Create a `secrets.nix` file with these contents (fill in your tokens)

```nix
{
  environment.sessionVariables = {
    GITHUB_TOKEN = "";
    ONSHAPE_TOKEN = "";
  };
}
```

## Some useful commands

`sudo smbpasswd -a alec` (set user password for NAS)

`sudo nixos-rebuild boot --flake path:/home/alec/homelab/ --impure` (rebuild with impurity)
