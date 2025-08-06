{ pkgs, ... }: {
  systemd = {
    services = {
      "startWebserver" = {
        wantedBy = [ "default.target" ];
        serviceConfig.ExecStart = "${pkgs.bun}/bin/bunx ts-node /home/alec/homelab/webserver/webserver";
      };
      "devmon" = { # Automatic device mounting daemon
        wantedBy = [ "default.target" ];
        path = with pkgs; [ udevil procps udisks2 which ];
        # Mount all in client mode & continue mounting in daemon mode
        script = ''
          ${pkgs.udevil}/bin/devmon -a
          ${pkgs.udevil}/bin/devmon
        '';
      };
      "hourly".script = ''
        ${pkgs.fish}/bin/fish /home/alec/homelab/scripts/captureImg.fish
      '';
      "daily".script = ''
        ${pkgs.fish}/bin/fish /home/alec/homelab/scripts/githubBackup.fish
        ${pkgs.fish}/bin/fish /home/alec/homelab/scripts/spotifySync.fish
      '';
    };

    timers = {
      "hourly" = { # Every hour, every day
        wantedBy = [ "timers.target" ];
        partOf = [ "captureImg.service" ];
        timerConfig.OnCalendar = "*-*-* *:00:00";
      };
      "daily" = { # Every morning at 3AM PT
        wantedBy = [ "timers.target" ];
        partOf = [ "daily.service" ];
        timerConfig.OnCalendar = "*-*-* 03:00:00";
      };
    };
  };
}
