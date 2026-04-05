{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-25.11";
    planning = {
      url = "github:AmazinAxel/Planning";
      inputs.nixpkgs.follows = "nixpkgs";
    };
  };

  outputs = { nixpkgs, planning, ... }: {
    nixosConfigurations."alechomelab" = nixpkgs.lib.nixosSystem {
      system = "aarch64-linux";
      modules = [
        ./homelab.nix
        ./services.nix
        { _module.args.planning = planning.packages.aarch64-linux.planning; }
      ];
    };
  };
}
