{
  description = "EInk frame code";
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixpkgs-unstable";
    flake-utils.url = "github:numtide/flake-utils/main";
  };
  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let pkgs = nixpkgs.legacyPackages.${system};
      in rec {
        packages = flake-utils.lib.flattenTree {
          gitAndTools = pkgs.gitAndTools;
          stdenv = pkgs.clangStdenv;
          cairo = pkgs.cairo.dev;
        };
        devShell = pkgs.mkShell {
          buildInputs = with pkgs; [
            (enableDebugging cairo)
            llvmPackages_9.clangUseLLVM
            clangStdenv
            gnumake
            bear
            pkg-config
          ];
        };
      });
}
