{
  fileSystems."/" = {
    device = "/dev/disk/by-label/NIXOS_SD";
    fsType = "ext4";
    options = [ "noatime" ];
  };
  hardware.enableRedistributableFirmware = true;
  system.stateVersion = "24.05";
}
