{
  pkgs ? import <nixpkgs> { },
}:

let
  stdenv = pkgs.stdenvAdapters.useMoldLinker pkgs.llvmPackages_19.stdenv;
in
(pkgs.mkShell.override { inherit stdenv; }) {
  nativeBuildInputs = with pkgs; [
    nasm
    valgrind
    python312Packages.pyelftools
	bear
	clang-tools
    strace
    ltrace
  ];
  NIX_ENFORCE_NO_NATIVE=0;
}

# vim: ts=2 sw=2 et 
