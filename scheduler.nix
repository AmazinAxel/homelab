{ pkgs, ... }: {
  systemd = {
    services = {
      "captureImg".serviceConfig = {
        Type = "oneshot";
        ExecStart = "${pkgs.fish}/bin/fish /home/alec/homelab/scripts/captureImg.fish";
      };
      "dailyBackup".serviceConfig = {
        Type = "oneshot";
        ExecStart = "${pkgs.fish}/bin/fish /home/alec/homelab/scripts/backup.fish";
      };
      "flakeUpdate".serviceConfig = {
        Type = "oneshot";
        ExecStart = "${pkgs.fish}/bin/fish /home/alec/homelab/scripts/flakeUpdate.fish";
      };
      "mountAllOnBoot" = { # Mount all connected drives on boot
        #after = [ "network.target" ];
        wantedBy = [ "default.target" ];
        serviceConfig = {
          Type = "oneshot";
          ExecStart = "${pkgs.util-linux}/bin/mount -a";
        };
      };
      "startWebserver" = { # Start web server
        wantedBy = [ "default.target" ];
        serviceConfig.ExecStart = "${pkgs.webfs}/bin/webfsd -p 80 -r /home/alec/public -f index.html";
      };
    };

    timers = {
      "captureImg" = { # Every hour, every day
        wantedBy = [ "timers.target" ];
        partOf = [ "captureImg.service" ];
        timerConfig.OnCalendar = "*-*-* *:00:00";
      };
      "dailyBackup" = { # Every morning at 1AM PT
        wantedBy = [ "timers.target" ];
        partOf = [ "dailyBackup.service" ];
        timerConfig.OnCalendar = "*-*-* 01:00:00";
      };
      "flakeUpdate" = { # Every Friday at 2AM PT
        wantedBy = [ "timers.target" ];
        partOf = [ "flakeUpdate.service" ];
        timerConfig.OnCalendar = "Fri *-*-* 02:00:00";
      };
    };
  };
}