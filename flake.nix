{
  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixos-25.05";

  outputs = { nixpkgs, ... }: {
    nixosConfigurations."alechomelab" = nixpkgs.lib.nixosSystem {
      system = "aarch64-linux";
      modules = [ ./homelab.nix ];
    };
  };
}
