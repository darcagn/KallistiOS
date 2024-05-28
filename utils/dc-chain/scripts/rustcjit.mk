# The rustcjit target builds libgccjit from the rust-lang/gcc fork.
# This fork contains improvements necessary to allow the Rust compiler to
# generate code for SH4 using libgccjit.
# Only the libgccjit headers and files are copied to the sh-elf toolchain
# path, while the rest of the compiler existing at that path is preserved.

rustcjit_name = gcc-rustc
rustcjit_git_repo = https://github.com/rust-lang/gcc.git

rustcjit: build-sh4-gcc-rustcjit

gcc-rustc/download.stamp: src_dir = $(rustcjit_name)
gcc-rustc/download.stamp:
	@echo "+++ Cloning rust-lang/gcc..."
	rm -rf $(src_dir)
	git clone --filter=tree:0 $(rustcjit_git_repo) $(src_dir)
	touch $@

# Patching for KOS is not strictly necessary here, but we do it
# anyway to pick up build fixes, cross-platform fixes, etc.
gcc-rustc/patch.stamp: gcc-rustc/download.stamp
gcc-rustc/patch.stamp: src_dir = $(rustcjit_name)
gcc-rustc/patch.stamp: diff_patches := $(wildcard $(patches)/$(src_dir)*.diff)
gcc-rustc/patch.stamp: diff_patches += $(wildcard $(patches)/$(host_triplet)/$(src_dir)*.diff)
ifeq ($(uname_s), Darwin)
ifeq ($(uname_p), arm)
gcc-rustc/patch.stamp: diff_patches += $(wildcard $(patches)/$(uname_p)-$(uname_s)/$(src_dir)*.diff)
endif
endif
gcc-rustc/patch.stamp:
	@echo "+++ Copying required KOS files into GCC directory..."
	cp $(kos_base)/kernel/arch/dreamcast/kernel/startup.s $(src_dir)/libgcc/config/sh/crt1.S
	cp $(patches)/gcc/gthr-kos.h $(src_dir)/libgcc/config/sh/gthr-kos.h
	cp $(patches)/gcc/fake-kos.S $(src_dir)/libgcc/config/sh/fake-kos.S
	@patches=$$(echo "$(diff_patches)" | xargs); \
	if ! test -z "$${patches}"; then \
		echo "+++ Patching $(src_dir)..."; \
		echo "$${patches}" | xargs -n 1 patch -N -d $(src_dir) -p1 -i; \
	fi;
	$(call update_config_guess_sub)
	touch $@

build-sh4-gcc-rustcjit: gcc-rustc/download.stamp gcc-rustc/patch.stamp
build-sh4-gcc-rustcjit: src_dir = $(rustcjit_name)
build-sh4-gcc-rustcjit: target = $(sh_target)
build-sh4-gcc-rustcjit: build = build-gcc-$(target)-rustc
build-sh4-gcc-rustcjit: prefix = $(sh_toolchain_path)
build-sh4-gcc-rustcjit: log = $(logdir)/$(build).log
build-sh4-gcc-rustcjit: logdir
	@echo "+++ Building $(src_dir) to $(build)..."
	-mkdir -p $(build)
	> $(log)
	cd $(build); \
	    ../$(src_dir)/configure \
	      --target=$(target) \
	      --prefix=$(prefix) \
	      --with-gnu-as \
	      --with-gnu-ld \
	      --without-headers \
	      --with-newlib \
	      --enable-languages=c,jit \
	      --disable-libssp \
	      --enable-checking=release \
	      --with-multilib-list=m4-single \
	      --with-endian=little \
	      --with-cpu=m4-single \
	      --enable-host-shared \
	      $(macos_gcc_configure_args) \
	      MAKEINFO=missing \
	      CC="$(CC)" \
	      CXX="$(CXX)" \
	      $(static_flag) \
	      $(to_log)
	$(MAKE) $(jobs_arg) -C $(build) DESTDIR=$(abspath $(build)) $(to_log)
	$(MAKE) -C $(build) $(install_mode) DESTDIR=$(abspath $(build)) $(to_log)
# Copy libgccjit files from DESTDIR to prefix dir
	mkdir -p $(prefix)/include
	cp -f $(build)$(prefix)/include/libgccjit.h $(prefix)/include/libgccjit.h
	cp -f $(build)$(prefix)/include/libgccjit++.h $(prefix)/include/libgccjit++.h
	mkdir -p $(prefix)/lib
	cp -f $(build)$(prefix)/lib/libgccjit.so.0.0.1 $(prefix)/lib/libgccjit.so.0.0.1
ifndef MINGW32
	ln -sf libgccjit.so.0.0.1 $(prefix)/lib/libgccjit.so.0
	ln -sf libgccjit.so.0 $(prefix)/lib/libgccjit.so
else
# Use copy instead of symlink for MinGW/MSYS or MinGW-w64/MSYS2.
	cp $(build)$(prefix)/lib/libgccjit.so.0 $(prefix)/lib/libgccjit.so.0
	cp $(build)$(prefix)/lib/libgccjit.so $(prefix)/lib/libgccjit.so
endif
	mkdir -p $(prefix)/share/info
	cp -f $(build)$(prefix)/share/info/libgccjit.info $(prefix)/share/info/libgccjit.info
# End copy libgccjit files
	$(clean_up)
