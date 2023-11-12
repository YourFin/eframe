{
  description = "EInk frame code";
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixpkgs-unstable";
    flake-utils.url = "github:numtide/flake-utils/main";
  };
  outputs = { self, nixpkgs, stable, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
        stablePkgs = stable.legacyPackages.${system};
      in rec {
        packages = flake-utils.lib.flattenTree {
          gitAndTools = pkgs.gitAndTools;
          cairo = pkgs.cairo.dev;
        };
        devShell = pkgs.mkShell {
          buildInputs = with pkgs; [
            (enableDebugging cairo)
            gcc13
            imgui
            gnumake
            bear
            pkg-config
            pkgs.SDL2
          ];
          shellHook = ''
            export IMGUI_NIX="${pkgs.imgui.out}"
            export SDL2_NIX="${pkgs.SDL2.out}"
          '';
        };
      });
}
