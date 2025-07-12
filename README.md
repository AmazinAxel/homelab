# Alec's Homelab Server

Features:

- Avahi local web server which shows files at ~/public/
- Network storage which auto mounts to the attached USB drive
- ...

## Github & Onshape integration

Create a keys.nix file with the following contents:

```nix
{
  environment.sessionVariables = {
    GITHUB_TOKEN = "test";
    ONSHAPE_TOKEN = "test";
  };
}
```
