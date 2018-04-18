
include ../Makefile.head

hda: $(HDA_IMG).xz
ifneq ($(HDA_IMG), $(wildcard $(HDA_IMG)))
	$(Q)tar Jvxf $< >/dev/null 2>&1
endif

flp: $(FLP_IMG).xz
ifneq ($(FLP_IMG), $(wildcard $(FLP_IMG)))
	$(Q)tar Jvxf $< >/dev/null 2>&1
endif

ramfs: $(RAM_IMG).xz
ifneq ($(RAM_IMG), $(wildcard $(RAM_IMG)))
	$(Q)tar Jvxf $< >/dev/null 2>&1
endif

EXAMPLES ?= ../examples/

# Note: This not work on mac osx
ifeq ($(OS), Linux)
  HD_SECTSIZE=$(shell fdisk -l $(HDA_IMG) | grep Units | grep -oE "[0-9]*" | tail -1)
  HDP1_SECTOF=$(shell fdisk -l $(HDA_IMG) | grep img1 | tr -s ' ' | cut -d' ' -f2)
  HDP2_SECTOF=$(shell fdisk -l $(HDA_IMG) | grep img2 | tr -s ' ' | cut -d' ' -f2)
  HDP1_OFFSET=$(shell echo '$$(($(HD_SECTSIZE)*$(HDP1_SECTOF)))')
  HDP2_OFFSET=$(shell echo '$$(($(HD_SECTSIZE)*$(HDP2_SECTOF)))')
else
  HDP1_OFFSET=1024
  HDP2_OFFSET=63504384
endif

HDA_DIR = _hda
hda-mount: hda
	$(Q)mkdir -p $(HDA_DIR)
	$(Q)sudo mount -o offset=$(HDP1_OFFSET) $(HDA_IMG) $(HDA_DIR)
hda-umount:
ifeq ($(HDA_DIR), $(wildcard $(HDA_DIR)))
	$(Q)sudo umount $(HDA_DIR)
	$(Q)rm -rf $(HDA_DIR)
endif

hda-install: hda-mount
	$(Q)sudo cp -r $(EXAMPLES) $(HDA_DIR)/usr/root/
	$(Q)sync
	$(Q)make $(S) hda-umount

FLP_DIR = _flp
flp-mount: flp
	$(Q)mkdir -p $(FLP_DIR)
	$(Q)sudo mount -t minix $(FLP_IMG) $(FLP_DIR)
flp-umount:
ifeq ($(FLP_DIR), $(wildcard $(FLP_DIR)))
	$(Q)sudo umount $(FLP_DIR)
	$(Q)rm -rf $(FLP_DIR)
endif

flp-install: flp-mount
	$(Q)sudo cp -r $(EXAMPLES) $(FLP_DIR)/usr/root/
	$(Q)sync
	$(Q)make $(S) flp-umount

RAMFS_DIR = _ramfs
ramfs-mount: ramfs
	$(Q)mkdir -p $(RAMFS_DIR)
	$(Q)sudo mount $(RAM_IMG) $(RAMFS_DIR)
ramfs-umount:
ifeq ($(RAMFS_DIR), $(wildcard $(RAMFS_DIR)))
	$(Q)sudo umount $(RAMFS_DIR)
	$(Q)rm -rf $(RAMFS_DIR)
endif

ramfs-install: ramfs-mount
	$(Q)sudo cp -r $(EXAMPLES) $(RAMFS_DIR)/usr/root/
	$(Q)sync
	$(Q)make $(S) ramfs-umount

clean:
	$(Q)rm -rf $(HDA_IMG) $(HDA_DIR)
	$(Q)rm -rf $(FLP_IMG) $(FLP_DIR)
	$(Q)rm -rf $(RAM_IMG) $(RAMFS_DIR)
