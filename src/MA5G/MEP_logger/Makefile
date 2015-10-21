####################################################################################
# MORPHO EMBEDDED PLATFORM
# MEP Logger Makefile
#
####################################################################################

LIB_NAME = libmep-logger

# Library Version
LIB_MAJOR = 2
LIB_MINOR = 0
LIB_RELEASE = 0
INSTALL_PREFIX ?= /usr

####################################################################################

OUTDIR ?= .
OBJDIR ?= $(OUTDIR)/obj

VPATH += src/

SRC  = src/MEP_logger.c
SRC += src/rollingpolicy_type_complete.c 
SRC += src/appender_type_socket.c 
SRC += src/layout_type_encrypted.c 
SRC += src/MEP_log_rc.c 
SRC += src/MEP_logger_init.c


####################################################################################
OBJ = $(addprefix $(OBJDIR)/,$(notdir $(SRC:.c=.o)))

INC_DIR += include/
SRC_INC += $(patsubst %,-I%,$(INC_DIR))

CFLAGS += -fPIC
CFLAGS += -DLIB_MAJOR=$(LIB_MAJOR) -DLIB_MINOR=$(LIB_MINOR) -DLIB_RELEASE=$(LIB_RELEASE)

LDFLAGS += -llog4c ${SYSROOT}/lib/libcrypto.a -ldl

ifeq ($(VERBOSE),1)
Q =
else
Q = @
endif

####################################################################################
all: $(LIB_NAME)

$(LIB_NAME): $(OBJ)
	@ echo "[SHARED]     $@"
	$(Q)$(CC) -shared -Wl,-soname,$(LIB_NAME).so.$(LIB_MAJOR).$(LIB_MINOR).$(LIB_RELEASE) $(OBJ) \
		-o $(OUTDIR)/$(LIB_NAME).so.$(LIB_MAJOR).$(LIB_MINOR).$(LIB_RELEASE) $(LDFLAGS) 
	$(Q)cd $(OUTDIR) && ln -fs $(LIB_NAME).so.$(LIB_MAJOR).$(LIB_MINOR).$(LIB_RELEASE) $(LIB_NAME).so

$(OBJDIR)/%.o: %.c
	@ echo "[CC]        $<"
	$(Q)$(CC) $(CFLAGS) -c $(SRC_INC) $< -o $@

clean:
	rm -f $(OUTDIR)/$(LIB_NAME).so* $(OBJ)

install:
	install -p -m 755 $(OUTDIR)/$(LIB_NAME).so.$(LIB_MAJOR).$(LIB_MINOR).$(LIB_RELEASE) $(INSTALL_PREFIX)/lib
	$(Q)cd $(INSTALL_PREFIX)/lib && ln -fs $(LIB_NAME).so.$(LIB_MAJOR).$(LIB_MINOR).$(LIB_RELEASE) $(LIB_NAME).so
	install -p -m 644 include/MEP_logger.h $(INSTALL_PREFIX)/include/mep-logger
	install -p -m 644 include/rollingpolicy_type_complete.h $(INSTALL_PREFIX)/include/mep-logger
	install -p -m 644 include/appender_type_socket.h $(INSTALL_PREFIX)/include/mep-logger
	install -p -m 644 include/layout_type_encrypted.h $(INSTALL_PREFIX)/include/mep-logger
	install -p -m 644 include/MEP_log_rc.h $(INSTALL_PREFIX)/include/mep-logger
	install -p -m 644 include/MEP_logger_init.h $(INSTALL_PREFIX)/include/mep-logger

-include $(shell mkdir $(OUTDIR) 2>/dev/null)
-include $(shell mkdir $(OBJDIR) 2>/dev/null)
-include $(DEP)
