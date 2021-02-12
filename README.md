# Benchmarks in Evaluation of In-Fat Pointer

### Olden

The Olden benchmark is directly available in the [olden](olden) directory. It is forked from [compor/olden](https://github.com/compor/olden), which is in turn based on [Olden from LLVM test-suite subproject](https://github.com/llvm/llvm-test-suite/tree/main/MultiSource/Benchmarks/Olden). We applied minor fixes to suppress compilation warnings and improve standard compliance.

### PtrDist

The PtrDist source is retrieved from the official website with minimal changes. See the [README](ptrdist/README.md) for more details.

### wolfcrypt-dh

Wolfcrypt is embedded in WolfSSL. This repository includes the archive for wolfssl-4.4.0.zip. The official website is [here](https://www.wolfssl.com/products/wolfcrypt-2/).

Execute the following commands to compile the binary:
```bash
unzip SOME_DIRECTORY_YOU_WANT/wolfssl-4.4.0.zip -d .
mkdir -p wolfssl_build
cd wolfssl_build
../wolfssl-4.4.0/configure --enable-static=yes --enable-shared=no # additional arguments you need
# We configured wolfssl with the following arguments:
# ../wolfssl-4.4.0/configure --enable-static=yes --enable-shared=no --build=x86_64-linux-gnu --host=riscv64-unknown-linux-gnu CC=ccwrap.sh LD=ccwrap.sh build_alias=x86_64-linux-gnu host_alias=riscv64-unknown-linux-gnu LIBS=-lpthread
# where ccwrap.sh is our drop-in replacement for riscv64-unknown-linux-gnu-gcc
make wolfcrypt/benchmark/benchmark
```
The binary is `wolfssl_build/wolfcrypt/benchmark/benchmark`. Run with `-dh` to run the Diffie–Hellman benchmarks.

### sjeng

Please obtain sjeng from [SPEC CPU 2006](https://www.spec.org/cpu2006/).

### coremark

CoreMark can be obtained from `git clone https://github.com/eembc/coremark.git`

### bzip2

bzip2 can be obtained by:
```bash
wget https://sourceware.org/pub/bzip2/bzip2-1.0.8.tar.gz
gunzip -k bzip2-1.0.8.tar.gz # the result is the input file for bzip2 benchmark
tar -xzf bzip2-1.0.8.tar.gz
```


