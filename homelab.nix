{ pkgs, lib, ... }: {
  imports = [
    ./hardware-configuration.nix
    ./services.nix
  ];

  nixpkgs.overlays = [
    (final: super: {
      makeModulesClosure = x: super.makeModulesClosure (x // { allowMissing = true; });
    })
  ];
  nixpkgs.hostPlatform = "aarch64-linux";

  zramSwap = {
    enable = true;
    algorithm = "zstd";
  };

  hardware = {
    enableRedistributableFirmware = lib.mkForce false;
    firmware = [ pkgs.raspberrypiWirelessFirmware ]; # May not be necessary?
    i2c.enable = true;

    deviceTree = {
      enable = true;
      kernelPackage = pkgs.linuxKernel.packages.linux_rpi3.kernel;
      filter = "*2837*";

      overlays = [
        {
          name = "enable-i2c";
          dtsFile = ./dts/i2c.dts;
        }
        {
          name = "pwm-2chan";
          dtsFile = ./dts/pwm.dts;
        }
        {
          name = "spi1-2cs";
          dtsFile = ./dts/spi.dts;
        }
      ];
    };
  };

  users.users.alec = { # Default user
    isNormalUser = true;
    extraGroups = [ "wheel" ];
    initialPassword = "nixos";
  };

  environment = {
    systemPackages = with pkgs; [ git bun spotdl jq fish ];
    sessionVariables.GITHUB_TOKEN = builtins.readFile /home/alec/GithubToken;
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
    #swraid.enable = lib.mkForce false; # https://github.com/NixOS/nixpkgs/issues/254807
  };

  # Networking
  networking = {
    hostName = "alechomelab";
    firewall.allowedTCPPorts = [ 80 ];
    networkmanager.enable = true; # For nmtui
    #interfaces."wlan0".useDHCP = true;
    #wireless.enable = true;
    #wireless.interfaces = [ "wlan0" ];
  };

  services = {
    openssh.enable = true; # SSH support
    #timesyncd.enable = true; # NTP time sync

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
      package = pkgs.samba4Full; # Better autodiscovery support
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
  
  # Some cleanup
  documentation.enable = false;
  environment.defaultPackages = lib.mkForce [];
  programs.command-not-found.enable = false;
}


