# Alec's Homelab Server

Features:

- Avahi local web server which shows files at ~/public/
- Network storage which auto mounts to the attached USB drive
- ...

## Github & Onshape integration

Create a `secrets.nix` file and paste & fill these contents

```nix
{
  environment.sessionVariables = {
    GITHUB_TOKEN = "";
    ONSHAPE_TOKEN = "";
  };
}
```

## Some useful commands

`sudo webfsd -p 80 -r ~/public -f index.html` (run web server)

`sudo nixos-rebuild switch --flake path:/home/alec/homelab/ --impure` (rebuild with impurity)
