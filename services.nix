{ pkgs, ... }: {
  systemd = {
    services = {
      webserver = {
        wantedBy = [ "multi-user.target" ];
        serviceConfig = {
          ExecStart = "${pkgs.bun}/bin/bun /home/alec/homelab/webserver/webserver";
          Restart = "always";
          RestartSec = 5;
          KillMode = "process";
          NoNewPrivileges = false;
          PrivateUsers = false;
        };
      };
      homelabDisplay = {
        wantedBy = [ "multi-user.target" ];
        serviceConfig = {
          ExecStart = "/home/alec/homelab/display/homelabDisplay";
          Restart = "always";
          RestartSec = 5;
          KillMode = "process";
          NoNewPrivileges = false;
          PrivateUsers = false;
        };
      };
      "daily".script = ''
        ${pkgs.fish}/bin/fish /home/alec/homelab/scripts/githubBackup.fish
        ${pkgs.fish}/bin/fish /home/alec/homelab/scripts/spotifySync.fish

        ${pkgs.toybox}/bin/time date +%s > /home/alec/lastSynced
      '';
    };

    timers."daily" = { # Every morning at 3AM PT
      wantedBy = [ "timers.target" ];
      partOf = [ "daily.service" ];
      timerConfig.OnCalendar = "*-*-* 03:00:00";
    };
  };
}
