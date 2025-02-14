{
  pkgs ? import <nixpkgs> { },
}:

let
  stdenv = pkgs.llvmPackages_19.stdenv;
in
(pkgs.mkShell.override { inherit stdenv; }) {
  nativeBuildInputs = with pkgs; [
    nasm
    valgrind
  ];
}

# vim: ts=2 sw=2 et 
