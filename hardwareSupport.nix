{ lib, pkgs, ...}: {
  users.groups.gpio = lib.mkDefault { };

  # "${pkgs.coreutils}/bin/chgrp gpio /dev/%k && chmod 660 /dev/%k"
  # sudo udevadm test --action=add /dev/gpiochip0 to test

  services.udev.extraRules = lib.mkBefore ''
    KERNEL=="gpiomem", GROUP="gpio", MODE="0660"
    SUBSYSTEM=="gpio", KERNEL=="gpiochip*", ACTION=="add", PROGRAM="${pkgs.bash}/bin/bash -c '${pkgs.coreutils}/bin/chgrp gpio /dev/%k && chmod 660 /dev/%k && ${pkgs.coreutils}/bin/chgrp -R gpio /sys/class/gpio && ${pkgs.coreutils}/bin/chmod -R g=u /sys/class/gpio'"
    SUBSYSTEM=="gpio", ACTION=="add", PROGRAM="${pkgs.bash}/bin/bash -c '${pkgs.coreutils}/bin/chgrp -R gpio /sys%p && ${pkgs.coreutils}/bin/chmod -R g=u /sys%p'"
  '';

  boot.kernelParams = [
    "iomem=relaxed"
  ];
}