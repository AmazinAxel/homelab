{ pkgs, lib, ... }: {
  imports = [
    ./hardware-configuration.nix
    ./secrets.nix
    ./tasks.nix
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
    jre
    killall
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
    kernelPackages = pkgs.linuxPackages_latest; #pkgs.linuxKernel.kernels.linux_rpi4; # Latest Linux kernel version
    enableContainers = false;
  };

  # Networking
  networking = {
    wireless.iwd = {
      enable = true;
      settings = {
        IPv6.Enabled = true;
        Settings.AutoConnect = true;
      };
    };
    firewall = {
      allowedTCPPorts = [ 80 445 139 ];
      allowedUDPPorts = [ 137 138 ];
    };
  };

  services = {
    # SSH support
    openssh = {
      enable = true;
      startWhenNeeded = true;
    };

    # SSH IP resolve shorthand by publishing its address on the network
    avahi = {
      enable = true;
      openFirewall = true; # For NAS
      publish = {
        enable = true;
        addresses = true; # For http IP
        userServices = true; # For NAS
      };
    };
    
    journald.extraConfig = "SystemMaxUse=20M";

    # NAS
    udisks2.enable = true;
    gvfs.enable = true;
    samba = {
      enable = true;
      settings = {
        "alecnas" = {
          comment = "Alec's home NAS";
          path = "/run/media/alec/ALEC BACKUP";
          writable = true;
          "guest ok" = false;
          "valid users" = [ "alec" ];
        };
      };
    };
    samba-wsdd = {
      enable = true;
      openFirewall = true;
    };
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

  fileSystems."/".options = [ "noatime" ]; # Optimal SD card mount options
  
  # Some cleanup
  documentation.enable = false;
  environment.defaultPackages = lib.mkForce [];

  system.stateVersion = "24.05";
}


