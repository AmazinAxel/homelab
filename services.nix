{ pkgs, ... }: {
  systemd = {
    services = {
      "startWebserver" = {
        wantedBy = [ "default.target" ];
        after = [ "devmon.service" ];
        requires = [ "devmon.service" ];
        serviceConfig = {
          ExecStart = "${pkgs.bun}/bin/bun /home/alec/homelab/webserver/webserver";
          User = "alec";
          Restart = "always";
          RestartSec = 5;
          KillMode = "process";
          ExecStop = "${pkgs.toybox}/bin/pkill bun";
          AmbientCapabilities = [ "CAP_NET_BIND_SERVICE" ];
          CapabilityBoundingSet = [ "CAP_NET_BIND_SERVICE" ];
          NoNewPrivileges = false;
        };
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
      "daily".script = ''
        ${pkgs.fish}/bin/fish /home/alec/homelab/scripts/githubBackup.fish
        ${pkgs.fish}/bin/fish /home/alec/homelab/scripts/spotifySync.fish
      '';
    };

    timers."daily" = { # Every morning at 3AM PT
      wantedBy = [ "timers.target" ];
      partOf = [ "daily.service" ];
      timerConfig.OnCalendar = "*-*-* 03:00:00";
    };
  };
}
