{ pkgs, lib, ... }: {
  imports = [ ./services.nix ];

  hardware = {
    firmware = [ pkgs.raspberrypiWirelessFirmware ];
    i2c.enable = true;
  };

  users.users.alec = { # Default user
    isNormalUser = true;
    extraGroups = [ "wheel" ];
    initialPassword = "nixos";
  };

  environment = {
    systemPackages = with pkgs; [ git bun spotdl jq fish ];
    
    # GitHub token
    etc."GithubToken".source = "/home/alec/GithubToken";
    sessionVariables.GITHUB_TOKEN = "/etc/GithubToken";
  };

  # Raspi boot
  boot = {
    loader = {
      grub.enable = false;
      generic-extlinux-compatible.enable = true;
      timeout = 0; # Hold down space on boot to access menu
    };
    tmp.cleanOnBoot = true;
    kernelPackages = pkgs.linuxPackages_rpi02w;
    initrd.availableKernelModules = [ "xhci_pci" "usbhid" "usb_storage" ];
  };

  # Networking
  networking = {
    hostName = "alechomelab";
    firewall.allowedTCPPorts = [ 80 ];
    networkmanager.enable = true; # For nmtui
  };

  services = {
    openssh.enable = true; # SSH support

    # IP resolve shorthand for .local address
    avahi = {
      enable = true;
      openFirewall = true;
      publish = {
        enable = true;
        addresses = true; # For HTTP IP
        userServices = true; # For NAS
      };
    };

    # USB NAS
    samba = {
      enable = true;
      package = pkgs.samba4Full; # Autodiscovery support
      openFirewall = true;
      settings."USB" = {
        path = "/media";
        writable = true;
        "valid users" = [ "alec" ];
        "admin users" = [ "alec" ]; # Full read & write access
      };
    };
    samba-wsdd = { # Auto-disovery
      enable = true;
      openFirewall = true;
    };
    journald.extraConfig = "SystemMaxUse=20M";
  };

  time.timeZone = "America/Los_Angeles";
  nix.settings = {
    experimental-features = "nix-command flakes";
    auto-optimise-store = true;
    warn-dirty = false;
    trusted-users = [ "alec" ];
  };
  
  fileSystems = {
    "/" = { # Device SD card
      device = "/dev/disk/by-label/NIXOS_SD";
      fsType = "ext4";
      options = [ "noatime" ];
    };
    "/media" = { # Attached USB drive
      device = "/dev/disk/by-label/AlecHomelabDrive";
      fsType = "ext4";
      options = [ "nofail" ];
    };
  };

  system.stateVersion = "25.11";
  nixpkgs.hostPlatform = "aarch64-linux";

  # Some cleanup
  documentation.enable = false;
  environment.defaultPackages = lib.mkForce [];
  programs.command-not-found.enable = false;
}


