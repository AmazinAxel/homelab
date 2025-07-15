{ pkgs, lib, ... }: {
  imports = [
    ./hardware-configuration.nix
    ./scheduler.nix
    ./secrets.nix
  ];

  networking.hostName = "alechomelab";

  users.users.alec = { # Default user
    isNormalUser = true;
    extraGroups = [ "wheel" ];
  };

  # Packages
  environment.systemPackages = with pkgs; [
    git
    webfs # HTTP server
  ];

  # Raspi boot
  boot = {
    loader = {
      generic-extlinux-compatible.enable = true;
      grub.enable = false;
      efi.canTouchEfiVariables = true;
      timeout = 0; # Hold down space on boot to access menu
    };
    tmp.cleanOnBoot = true;
    kernelPackages = pkgs.linuxKernel.packages.linux_rpi4;
    initrd.availableKernelModules = [ "xhci_pci" "usbhid" "usb_storage" ];
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

  security.polkit = { # Required for automounting (but reduces security)
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


