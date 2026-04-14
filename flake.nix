{
  description = "Homelab display dev shell";

  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";

  outputs = { self, nixpkgs }:
    let
      pkgs = nixpkgs.legacyPackages.aarch64-linux;
    in {
      packages.aarch64-linux.homelabDisplay = pkgs.stdenv.mkDerivation {
        name = "homelabDisplay";
        src = self;
        buildInputs = [ pkgs.libgpiod_1 ];
        buildPhase = "mkdir -p bin && make CC=$CC";
        installPhase = ''
          mkdir -p $out/bin
          cp homelabDisplay $out/bin/
        '';
      };

      devShells.x86_64-linux.default =
        let x86pkgs = nixpkgs.legacyPackages.x86_64-linux; in x86pkgs.mkShell {
          buildInputs = with x86pkgs; [
            pkgsCross.aarch64-multiplatform.buildPackages.gcc
            pkgsCross.aarch64-multiplatform.libgpiod_1
          ];
        };
    };
}
