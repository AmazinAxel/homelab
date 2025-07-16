# Alec's homelab server

Features:

- Avahi local web server which shows files at ~/public/
- Network storage which auto mounts to the attached USB drive
- Systemd tasks which auto-update Github flake locks and make backups of Onshape models & private Github repos

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

## Notes

All files under /home/alec/public are shared on the LAN through an HTTP server

## Some useful commands

`sudo smbpasswd -a alec` (set user password for NAS)

`sudo nixos-rebuild boot --flake path:/home/alec/homelab/ --impure` (rebuild with impurity)
