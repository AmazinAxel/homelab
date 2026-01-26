{ lib, pkgs, ...}: {
  services.udev.extraRules = ''
    KERNEL=="spidev*", GROUP="wheel", MODE="0660"
    KERNEL=="gpio*", GROUP="wheel", MODE="0660"
  '';

  hardware.deviceTree = {
    enable = true;
    filter = "*rpi-zero-2*.dtb";
    overlays = [
      {
         name = "spi0";
         dtsFile = ./spi0.dts;
      }
      {
         name = "waveshare-buttons";
         dtsFile = ./buttons.dts;
      }
    ];
  };
}