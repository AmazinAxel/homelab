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

  environment.systemPackages = with pkgs; [ git ];

  # Raspi boot
  boot = {
    loader = {
      grub.enable = false;
      generic-extlinux-compatible.enable = true;
      timeout = 0; # Hold down space on boot to access menu
    };
    tmp.cleanOnBoot = true;
    kernelPackages = pkgs.linuxKernel.packages.linux_rpi4;
    initrd.availableKernelModules = [ "xhci_pci" "usbhid" "usb_storage" ];
  };

  # Networking
  networking = {
    firewall.allowedTCPPorts = [ 80 ];
    networkmanager.enable = true;
  };

  services = {
    openssh.enable = true; # SSH support

    # IP resolve shorthand - publish .local address on the network
    avahi = {
      enable = true;
      openFirewall = true;
      publish = {
        enable = true;
        addresses = true; # For http IP
        userServices = true; # For NAS
      };
    };

    # NAS
    samba = {
      enable = true;
      package = pkgs.samba4Full; # Use full package for better autodiscovery support
      openFirewall = true;
      settings."USB" = {
        path = "/media/";
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
  
  # Some cleanup
  documentation.enable = false;
  environment.defaultPackages = lib.mkForce [];
}


