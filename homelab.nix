{ pkgs, lib, ... }: {
  imports = [
    ./hardware-configuration.nix
  ];

  networking.hostName = "alechomelab";

  users.users.alec = { # Default user
    isNormalUser = true;
    extraGroups = [ "wheel" "dialout" ];
  };

  # Packages
  environment.systemPackages = with pkgs; [
    git
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
    kernelPackages = pkgs.linuxPackages_latest; # Latest Linux kernel version
    enableContainers = false;
  };

  # Networking
  networking.wireless.iwd = {
    enable = true;
    settings = {
      IPv6.Enabled = true;
      Settings.AutoConnect = true;
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
      publish = {
        enable = true;
        addresses = true;
      };
    };
    journald.extraConfig = "SystemMaxUse=20M";
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
  programs.command-not-found.enable = false;

  system.stateVersion = "24.05";
}


