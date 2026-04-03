{ pkgs, ... }:

let
  service = { # basic service config
    wantedBy = [ "multi-user.target" ];
    serviceConfig = {
      Restart = "always";
      RestartSec = 5;
    };
  };
  privileges = { # Fix webserver stuff
    NoNewPrivileges = false;
    PrivateUsers = false;
  };
in {
  systemd = {
    services = {
      webserver = service // {
        path = [ pkgs.util-linux ];
        serviceConfig = service.serviceConfig // privileges // {
          EnvironmentFile = "/home/alec/homelab/webserver/.env";
          ExecStart = "${pkgs.bun}/bin/bun /home/alec/homelab/webserver/webserver";
        };
      };
      homelabDisplay = service // {
        serviceConfig = service.serviceConfig // privileges // {
          ExecStart = "/home/alec/homelab/display/homelabDisplay";
        };
      };
      lofi = service // {
        serviceConfig = service.serviceConfig // privileges // {
          ExecStart = "${pkgs.php82}/bin/php -S 0.0.0.0:9000 -t /media/lofi/";
        };
      };
      planning = service // {
        after = [ "broadwayd-planning.service" ];
        environment = {
          GDK_BACKEND = "broadway";
          BROADWAY_DISPLAY = ":5";
        };
        serviceConfig = service.serviceConfig // {
          User = "alec";
          WorkingDirectory = "/home/alec/planning";
          ExecStart = "/home/alec/planning/build/planning";
        };
      };
      planning-broadway = service // {
        serviceConfig = service.serviceConfig // {
          User = "alec";
          ExecStart = "${pkgs.gtk4}/bin/broadwayd --port 8000 :5";
        };
      };

      daily.script = ''
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
