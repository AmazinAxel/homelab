{
  description = "Homelab display dev shell";

  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";

  outputs = { self, nixpkgs }:
    let
      pkgs = nixpkgs.legacyPackages.x86_64-linux;
    in {
      devShells.x86_64-linux.default = nixpkgs.legacyPackages.x86_64-linux.mkShell {
        buildInputs = with pkgs; [
          pkgsCross.aarch64-multiplatform.buildPackages.gcc
          pkgsCross.aarch64-multiplatform.libgpiod_1
          lftp
        ];

        shellHook = ''
          deploytohomelab() {
            make -j$(nproc)
            lftp -u alec -e "put homelabDisplay -o /home/alec/homelab/display/homelabDisplay; bye" sftp://alechomelab.local
          }
        '';
      };
    };
}
