{ pkgs, ... }: {
  systemd = {
    services = {
      startWebserver = {
        wantedBy = [ "multi-user.target" ];
        serviceConfig = {
          ExecStart = [ "${pkgs.bun}/bin/bun" "/home/alec/homelab/webserver/webserver" ];
          User = "alec";
          Restart = "always";
          RestartSec = 5;
          KillMode = "process";
          AmbientCapabilities = [ "CAP_NET_BIND_SERVICE" ];
          CapabilityBoundingSet = [ "CAP_NET_BIND_SERVICE" ];
          NoNewPrivileges = false;
          PrivateUsers = false;
        };
      };
      startDisplay = {
        wantedBy = [ "multi-user.target" ];
        serviceConfig = {
          ExecStart = [ "/home/alec/homelab/display/homelabDisplay" ];
          Restart = "always";
          RestartSec = 5;
          KillMode = "process";
          NoNewPrivileges = false;
          PrivateUsers = false;
        };
      };
      "daily".script = "${pkgs.fish}/bin/fish /home/alec/homelab/scripts/daily.fish";
    };

    timers."daily" = { # Every morning at 3AM PT
      wantedBy = [ "timers.target" ];
      partOf = [ "daily.service" ];
      timerConfig.OnCalendar = "*-*-* 03:00:00";
    };
  };
}
