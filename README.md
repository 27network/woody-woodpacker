# [woody-woodpacker](https://projects.intra.42.fr/projects/42cursus-woody-woodpacker)

> A logical follow-up project on nm/otools that is designed to modify the headers of a ELF64 file. The goal here is to add a chunk of code and hide a non-stripped part of a file.

## Bonus features

Taking inspiration from the subject, here are some extra features we added:

- [ ] 32-bit ELF support
- [ ] Binary Compression
  - Using a custom-made algorithm based on [LZ77](https://en.wikipedia.org/wiki/LZ77_and_LZ78), `SMZL`.
- [ ] Customizable payload
  - [ ] Binary shellcode
  - [ ] ELF executable
  - [ ] Shell script / any interpretable file
- [ ] CLI arguments for every option/operation
  - [ ] Encryption algorithms
  - [ ] Compression algorithms
  - [ ] Parameterized key

## License

This project is licensed under the [ISC License](./LICENSE).
