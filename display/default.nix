{ pkgs ? import <nixpkgs> {} }:

pkgs.stdenv.mkDerivation {
  pname = "homelabDisplay";
  version = "1.0";
  src = ./.;

  buildPhase = ''
    gcc main.c -o homelabDisplay
  '';

  installPhase = ''
    mkdir -p $out/bin
    cp homelabDisplay $out/bin/
  '';
}
