let
  pkgs = import (fetchTarball "https://github.com/nixOS/nixpkgs/archive/refs/tags/25.11.tar.gz") {};
in
pkgs.mkShell {
  nativeBuildInputs = with pkgs; [
    gcc
    gnumake
    pkg-config
  ];
  buildInputs = with pkgs; [
    SDL2
  ];
}
