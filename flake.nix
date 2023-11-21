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
    ion-c-git = {
      url = "https://github.com/amazon-ion/ion-c?ref=v1.1.2";
      flake = false;
      type = "git";
      submodules = true;
    };
  };
  outputs = { self, nixpkgs, stable, flake-utils, crow-git, ion-c-git }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
        stablePkgs = stable.legacyPackages.${system};
      in rec {
        packages = flake-utils.lib.flattenTree {
          gitAndTools = pkgs.gitAndTools;
          cairo = pkgs.cairo.dev;
          ion-c = pkgs.stdenv.mkDerivation {
            name = "ion-c";
            src = ion-c-git;
            nativeBuildInputs = [
              pkgs.cmake
              pkgs.python3
              (pkgs.writeTextFile {
                name = "ion-c-git-spit-out-version";
                executable = true;
                destination = "/bin/git";
                text = ''
                  # see: https://github.com/amazon-ion/ion-c/blob/master/cmake/VersionHeader.cmake#L15
                  echo 'v1.1.2-0-gece2e8c23e9d017852dff67646652689ff9e8d2b'
                '';
              })
            ];
          };
        };
        devShell = pkgs.mkShell {
          nativeBuildInputs = [ pkgs.pkg-config ];
          buildInputs = with pkgs; [
            (enableDebugging cairo)
            gcc13
            gnumake
            bear
            pkg-config
            packages.ion-c

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
            export ION_C_NIX=${packages.ion-c}
          '';
        };
      });
}
