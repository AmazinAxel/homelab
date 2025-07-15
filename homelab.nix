{ pkgs, lib, ... }: {
  imports = [
    ./hardware-configuration.nix
    ./scheduler.nix
    ./secrets.nix
  ];

  networking.hostName = "alechomelab";

  users.users.alec = { # Default user
    isNormalUser = true;
    extraGroups = [ "wheel" "dialout" ];
  };

  # Packages
  environment.systemPackages = with pkgs; [
    git
    webfs
  ];

  # Raspi boot
  boot = {
    loader = {
      generic-extlinux-compatible.enable = true;
      grub.enable = false;
      efi.canTouchEfiVariables = true;
      timeout = 0; # Hold down space on boot to access menu
    };
    kernelModules = [ "bcm2835-v4l2" ];
    tmp.cleanOnBoot = true;
    kernelPackages = pkgs.linuxPackages_latest;
  };

  # Networking
  networking = {
    firewall.allowedTCPPorts = [ 80 ];
    wireless.iwd = {
      enable = true;
      settings = {
        IPv6.Enabled = true;
        Settings.AutoConnect = true;
      };
    };
  };

  services = {
    # SSH support
    openssh.enable = true;

    # SSH IP resolve shorthand by publishing its address on the network
    avahi = {
      enable = true;
      openFirewall = true;
      publish = {
        enable = true;
        addresses = true; # For http IP
        userServices = true; # For NAS
      };
    };
    
    journald.extraConfig = "SystemMaxUse=20M";

    # NAS
    devmon.enable = true; # Auto-mount
    udisks2.mountOnMedia = true; # Fix mount on boot - always mount in the same directory regardless of user
    samba = {
      enable = true;
      package = pkgs.samba4Full; # Use full package for better autodiscovery support
      openFirewall = true;
      settings."USB" = {
        comment = "Primary & temporary backup storage devices";
        path = "/media/";
        writable = true;
        "guest ok" = false;
        "valid users" = [ "alec" ];
      };
    };
    samba-wsdd = { # Auto-disovery
      enable = true;
      openFirewall = true;
    };
  };

  security.polkit = {
    enable = true;
    extraConfig = "polkit.addRule(function(action, subject) { return polkit.Result.YES; });";
  };

  programs = {
    fish.enable = true;
    command-not-found.enable = false;
  };

  time.timeZone = "America/Los_Angeles";

  nix.settings = {
    experimental-features = "nix-command flakes";
    auto-optimise-store = true;
    warn-dirty = false;
  };

  fileSystems."/".options = [ "noatime" ];
  
  # Some cleanup
  documentation.enable = false;
  environment.defaultPackages = lib.mkForce [];

  system.stateVersion = "24.05";
}


