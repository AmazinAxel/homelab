{
  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixos-25.05";
  inputs.nixos-hardware.url = "github:NixOS/nixos-hardware";

  outputs = { nixpkgs, nixos-hardware, ... }: {
    nixosConfigurations."alechomelab" = nixpkgs.lib.nixosSystem {
      system = "aarch64-linux";
      modules = [ ./homelab.nix ];
    };
  };
}
