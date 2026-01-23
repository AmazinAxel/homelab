{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-25.11";
    nixos-raspberrypi.url = "github:nvmd/nixos-raspberrypi/main";
  };

  outputs = { nixpkgs, ... }@inputs: {
    nixosConfigurations."alechomelab" = nixpkgs.lib.nixosSystem {
      #specialArgs = { inherit inputs; };
      system = "aarch64-linux";
      modules = [ ./homelab.nix ];
    };
  };
}
