{ lib, pkgs, ...}: {
  services.udev.extraRules = ''
    KERNEL=="spidev*", GROUP="spi", MODE="0660"
    KERNEL=="gpio*", GROUP="gpio", MODE="0660"
  '';

  hardware.deviceTree = {
    enable = true;
    filter = "*rpi-zero-2*.dtb";
    overlays = [
      {
         name = "spi0-custom";
         dtsFile = ./spi0.dts;
      }
    ];
  };

  users.groups.spi = { };
  users.groups.gpio = {};
}