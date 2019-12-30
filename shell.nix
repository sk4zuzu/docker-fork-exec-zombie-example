{ pkgs ? import <nixpkgs> {} }:

with pkgs;

stdenv.mkDerivation {
  name = "docker-fork-exec-zombie-example-env";
  buildInputs = [
    git
    pkgconfig gnumake
    gcc gdb
  ];
}

# vim:ts=2:sw=2:et:syn=nix:
