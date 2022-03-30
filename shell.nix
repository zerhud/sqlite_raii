{ pkgs ? import <nixos-unstable> {} }:
  pkgs.mkShell {
    nativeBuildInputs = with pkgs; [ qtcreator cmake ninja ];
    buildInputs = with pkgs; [ boost17x ];
}
