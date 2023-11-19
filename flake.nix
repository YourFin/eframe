{
  description = "EInk frame code";
  inputs = {
    stable.url = "github:nixos/nixpkgs/nixos-23.05";
    nixpkgs.url = "github:nixos/nixpkgs/nixpkgs-unstable";
    flake-utils.url = "github:numtide/flake-utils/main";
    crow-git = {
      url = "github:CrowCpp/Crow/edf12f6";
      flake = false;
    };
  };
  outputs = { self, nixpkgs, stable, flake-utils, crow-git }:
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
          nativeBuildInputs = [ pkgs.pkg-config ];
          buildInputs = with pkgs; [
            (enableDebugging cairo)
            gcc13
            gnumake
            bear
            pkg-config

            # simulator
            asio
            zmqpp.dev
            cambalache
            curlpp
            curl.dev
            gtkmm4.dev
          ];
          shellHook = ''
            # Put extra i.e. environment variables here
            export CROW_NIX=${crow-git}
            export ZMQPP_NIX=${pkgs.zmqpp.dev}
          '';
        };
      });
}
