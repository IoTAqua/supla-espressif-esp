#############################################################
# Required variables for each makefile
# Discard this section from all parent makefiles
# Expected variables (with automatic defaults):
#   CSRCS (all "C" files in the dir)
#   SUBDIRS (all subdirs with a Makefile)
#   GEN_LIBS - list of libs to be generated ()
#   GEN_IMAGES - list of object file images to be generated ()
#   GEN_BINS - list of binaries to be generated ()
#   COMPONENTS_xxx - a list of libs/objs in the form
#     subdir/lib to be extracted and rolled up into
#     a generated lib/image xxx.a ()
#
TARGET = eagle
FLAVOR = release
#FLAVOR = debug


EXTRA_CCFLAGS += -DESP8266 -D__SINGLE_THREAD -D__EH_DISABLED -D__BOARD_$(BOARD) -DCFG_SECTOR=$(CFG_SECTOR) $(__EXTRA_CCFLAGS)

ifndef PDIR # {
GEN_IMAGES= eagle.app.v6.out
GEN_BINS= eagle.app.v6.bin
SPECIAL_MKTARGETS=$(APP_MKTARGETS)
SUBDIRS=    \
	user    \
    driver \
	supla-dev

	ifeq ($(FOTA), 1)
	SUBDIRS += nettle
	endif #FOTA 
	
endif # } PDIR


ifeq ($(FOTA), 1)
	EXTRA_CCFLAGS += -D__FOTA=1
endif #FOTA 

CCFLAGS = -Os -std=gnu99 -Wpointer-arith -Wundef -Wall -Wl,-EL -fno-inline-functions \
		-nostdlib -mlongcalls -mtext-section-literals  -D__ets__ -DICACHE_FLASH \
		-Wno-address

TARGET_LDFLAGS =		\
	-nostdlib		\
	-Wl,-EL \
	--longcalls \
	--text-section-literals

ifeq ($(FLAVOR),debug)
    TARGET_LDFLAGS += -g -O2
endif

ifeq ($(FLAVOR),release)
    TARGET_LDFLAGS += -g -O0
endif

COMPONENTS_eagle.app.v6 = \
	user/libuser.a  \
	driver/libdriver.a \
	supla-dev/libsupladev.a \

ifeq ($(FOTA), 1)
COMPONENTS_eagle.app.v6 += nettle/libnettle.a
endif #FOTA 

LINKFLAGS_eagle.app.v6 = \
	-L$(SDK_PATH)/lib        \
	-Wl,--gc-sections   \
	-nostdlib	\
    -T$(LD_FILE)   \
	-Wl,--no-check-sections	\
    -u call_user_start1	\
	-Wl,-static						\
	-Wl,--start-group					\
	-lc \
	-lg \
	-lgcc	\
	-lpp	\
	-lphy	\
	-lnet80211	\
	-lwpa	\
	-lmain	\
	-llwip	\
	-ldriver \
        -lcrypto \
        $(SUPLA_DEP_LIBS) \
	$(DEP_LIBS_eagle.app.v6)					\
	-Wl,--end-group

#############################################################
# Configuration i.e. compile options etc.
# Target specific stuff (defines etc.) goes in here!
# Generally values applying to a tree are captured in the
#   makefile at its root level - these are then overridden
#   for a subtree within the makefile rooted therein
#

#UNIVERSAL_TARGET_DEFINES =		\

# Other potential configuration flags include:
#	-DTXRX_TXBUF_DEBUG
#	-DTXRX_RXBUF_DEBUG
#	-DWLAN_CONFIG_CCX
CONFIGURATION_DEFINES =	-DICACHE_FLASH

DEFINES +=				\
	$(UNIVERSAL_TARGET_DEFINES)	\
	$(CONFIGURATION_DEFINES)

DDEFINES +=				\
	$(UNIVERSAL_TARGET_DEFINES)	\
	$(CONFIGURATION_DEFINES)


#############################################################
# Recursion Magic - Don't touch this!!
#
# Each subtree potentially has an include directory
#   corresponding to the common APIs applicable to modules
#   rooted at that subtree. Accordingly, the INCLUDE PATH
#   of a module can only contain the include directories up
#   its parent path, and not its siblings
#
# Required for each makefile to inherit from the parent
#

INCLUDES := $(INCLUDES) -I $(PDIR)include 

ifeq ($(FOTA), 1)
INCLUDES := $(INCLUDES) -I $(PDIR)nettle/include
endif #FOTA 

sinclude $(SRC_ROOT)/Makefile.sdk


.PHONY: FORCE
FORCE:

