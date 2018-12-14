#
# Makefile for the memctl application
#
export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig/

include Makefile.in

OBJDIR := objdir
PBPATH := src/proto

PROJECT_BASE    := /home/root/src
PROJECT_WEB_DIR := /home/root/src/web
PROJECT_INIT_DIR := /system/init

OUTPUT_DIR      := /home/root/src/system_rev
BIT_DIR         := $(OUTPUT_DIR)/bit
BIN_DIR         := $(OUTPUT_DIR)/bin
ETC_DIR         := $(OUTPUT_DIR)/etc
INIT_DIR        := $(OUTPUT_DIR)/init
LIB_DIR         := $(OUTPUT_DIR)/lib
CONFIG_DIR      := $(OUTPUT_DIR)/config
RAMDISK_DIR     := $(OUTPUT_DIR)/ramdisk
UPDATE_DIR      := $(OUTPUT_DIR)/update
WEB_DIR         := $(OUTPUT_DIR)/web

DATE			:= $(shell date '+%d/%m/%Y')

# common
# INCLUDEDIR := -Isrc -Iinclude -I./ -I$(PROJECT_BASE)/external/cppzmq
INCLUDEDIR := -Isrc -Iinclude -I./ -I$//system/lib/zeromq-4.1.6/include -I$//lib/poco/include -I$//system/lib/protobuf-3.4.1/src -I$//system/lib/protobuf-3.4.1/src/.libs
# INCLUDEDIR := -I$//system/lib/protobuf-3.4.1/src -I$//system/lib/protobuf-3.4.1/src/.libs


LIBDIR     :=
LIBRARIES  := -lPocoUtil -lPocoNet -lPocoFoundation -lzmq -lprotobuf


CFLAGS 	   := -Wall $(DEFINES) $(OPTIMIZATION) $(INCLUDEDIR) -D_DEBUG
LDFLAGS    := $(LIBDIR) $(LIBRARIES)

# src directory
SOURCES	   := src/MonitorApplication.cpp
INCLUDES   := src/MonitorApplication.hpp


SOURCES	   += src/Device/devmem.cpp
INCLUDES   += src/Device/devmem.h

SOURCES	   += src/Channel.cpp
INCLUDES   += src/Channel.hpp

SOURCES	   += src/DeviceConfigurator.cpp
INCLUDES   += src/DeviceConfigurator.hpp

SOURCES	   += src/AGCTunerManager.cpp
INCLUDES   += src/AGCTunerManager.hpp

SOURCES	   += src/AGCTunerControl.cpp
INCLUDES   += src/AGCTunerControl.hpp

SOURCES	   += src/SysMonitor.cpp
INCLUDES   += src/SysMonitor.hpp

INCLUDES   += src/TunerObserver.hpp
INCLUDES   += src/Notifications.hpp


# src/Common directory
INCLUDES   += src/Common/Helpers.hpp
SOURCES    += src/Common/Helpers.cpp

INCLUDES   += src/Common/BitFileParser.hpp
SOURCES    += src/Common/BitFileParser.cpp

INCLUDES   += src/Common/ByteOperations.hpp
INCLUDES   += src/Common/LoadedNotification.hpp
INCLUDES   += src/Common/Noncopyable.hpp
INCLUDES   += src/Common/Typedefs.hpp
INCLUDES   += src/Common/Defines.hpp
INCLUDES   += src/Common/Tracer.hpp
#INCLUDES   += src/Common/MemoryIOMixin.hpp

# src/NetworkSubsystem

SOURCES    += src/UART/tty.cpp
INCLUDES   += src/UART/tty.h

SOURCES    += src/FFT/BasicDataProvider.cpp
INCLUDES   += src/FFT/BasicDataProvider.hpp

SOURCES    += src/FFT/BasicSender.cpp
INCLUDES   += src/FFT/BasicSender.hpp

SOURCES    += src/FFT/Connection.cpp
INCLUDES   += src/FFT/Connection.hpp

SOURCES    += src/FFT/FFTDataProvider.cpp
INCLUDES   += src/FFT/FFTDataProvider.hpp

SOURCES    += src/FFT/NetworkSender.cpp
INCLUDES   += src/FFT/NetworkSender.hpp

INCLUDES   += src/FFT/ConnectionFactory.hpp
# src/ExchangeAgent directory

# src/Settings directory
SOURCES    += src/Settings/AGCSettings.cpp
INCLUDES   += src/Settings/AGCSettings.hpp

SOURCES    += src/Settings/SettingsGroup.cpp
INCLUDES   += src/Settings/SettingsGroup.hpp

SOURCES    += src/Settings/SettingsManager.cpp
INCLUDES   += src/Settings/SettingsManager.hpp

SOURCES    += src/Settings/TunerSettings.cpp
INCLUDES   += src/Settings/TunerSettings.hpp

SOURCES    += src/Settings/CommonSettings.cpp
INCLUDES   += src/Settings/CommonSettings.hpp

SOURCES    += src/Settings/ChannelSettings.cpp
INCLUDES   += src/Settings/ChannelSettings.hpp

# SOURCES    += src/Settings/MiscSettings.cpp
# INCLUDES   += src/Settings/MiscSettings.hpp

INCLUDES   += src/Settings/Datum.hpp
INCLUDES   += src/Settings/SettingsHolderBase.hpp
INCLUDES   += src/Settings/SettingsParams.hpp

SOURCES    += src/Settings/DecimationSettings.cpp
INCLUDES   += src/Settings/DecimationSettings.hpp

# src/Statistics directory
    
SOURCES    += src/Statistics/AGCTunerStatistics.cpp
INCLUDES   += src/Statistics/AGCTunerStatistics.hpp

SOURCES    += src/Statistics/StatisticsManager.cpp
INCLUDES   += src/Statistics/StatisticsManager.hpp

INCLUDES   += src/Statistics/ChannelStatistics.hpp
INCLUDES   += src/Statistics/ConnectionStatistics.hpp
INCLUDES   += src/Statistics/CommonStatistics.hpp

# src/SNMP directory
SOURCES    += src/Connection/ConnectionHandler.cpp
INCLUDES   += src/Connection/ConnectionHandler.hpp

SOURCES    += src/Connection/ZMQHandler.cpp
INCLUDES   += src/Connection/ZMQHandler.hpp

SOURCES    += src/Connection/ZMQServer.cpp
INCLUDES   += src/Connection/ZMQServer.hpp

SOURCES    += src/Connection/ZMQPUBServer.cpp
INCLUDES   += src/Connection/ZMQPUBServer.hpp

SOURCES    += src/Connection/DataConversion.cpp
INCLUDES   += src/Connection/DataConversion.hpp

SOURCES    += src/Connection/SNMPSubsystem.cpp
INCLUDES   += src/Connection/SNMPSubsystem.hpp

SOURCES    += src/Connection/ProtoDataConversion.cpp
INCLUDES   += src/Connection/ProtoDataConversion.hpp

# include directory
INCLUDES   += include/monitord_io.h

OBJECTS := $(addprefix $(OBJDIR)/,$(patsubst %.cpp,%.o,$(SOURCES)))

$(shell mkdir -p $(PBPATH) 2> /dev/null)
$(shell mkdir -p $(OBJDIR)/src 2> /dev/null)
$(shell find src -type d -maxdepth 1 ! -path "src" -exec mkdir objdir/{} \; 2> /dev/null)

# Protobuf section
PROTOS   := $(wildcard proto/*.proto)
PBOBJS   := $(addprefix $(OBJDIR)/$(PBPATH)/,$(notdir $(PROTOS:.proto=.pb.o)))
PBSRCS   := $(addprefix $(PBPATH)/,$(notdir $(PROTOS:.proto=.pb.cc)))
PBHDRS   := $(addprefix $(PBPATH)/,$(notdir $(PROTOS:.proto=.pb.h)))
PBGENS   := $(PBSRCS) $(PBHDRS) 
.PRECIOUS: $(PBGENS)	# Keep generated protobuf files


APPNAME    := MonitorX


.PHONY: all
all: $(APPNAME)

.PHONY: print
print:
	@echo -e "PBOBJS: $(PBOBJS)"
	@echo -e "PBSRCS: $(PBSRCS)"
	@echo -e "PBHDRS: $(PBHDRS)"
	@echo -e "PBGENS: $(PBGENS)"
	@echo -e "$(CXX) $(CFLAGS) -c -o $@ `pkg-config --cflags --libs protobuf` $>"
	@echo -e "OBJECTS: $(OBJECTS)"

#$(APPNAME): $(SOURCES) $(INCLUDES)
#	@echo -e "\E[32m\033[4m\033[1mBuilding $(APPNAME)\033[0m\033[0m"
#	@tput sgr0
#	$(CXX) $(CFLAGS) $(SOURCES) -o $(APPNAME)
#	$(STRIP) $(APPNAME)

$(APPNAME): $(PBOBJS) $(OBJECTS)
	@echo -e "\E[32m\033[4m\033[1mLinking $(APPNAME)\033[0m\033[0m"
	@tput sgr0
	@$(CXX) $(LDFLAGS) -o $(APPNAME) $^
	@$(STRIP) $(APPNAME)

$(OBJDIR)/%.o: %.cpp $(INCLUDES)
	@echo -e "\E[32m\033[4m\033[1mCompiling $<\033[0m\033[0m"
	@tput sgr0
	@$(CXX) $(CFLAGS) -c $< -o $@

$(PBPATH)/%.pb.cc: proto/%.proto
	@echo -e "\E[32m\033[4m\033[1mGenerating from $<\033[0m\033[0m"
	@tput sgr0
	@protoc --cpp_out=src $<

$(OBJDIR)/%.pb.o: %.pb.cc
	@echo -e "\E[32m\033[4m\033[1mCompiling (generated) $<\033[0m\033[0m"
	@tput sgr0	
	@$(CXX) $(CFLAGS) -c -o $@ $< `pkg-config --cflags --libs protobuf`

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(PBOBJS) $(PBGENS) *~ ~* *.bck core $(APPNAME)

.PHONY: collect
collect:
	@echo -e "\E[31m\033[4m\033[1mRunning $(MAKECMDGOALS) for RCV-20 project\033[0m\033[0m"
	@tput sgr0
	rm -rf $(OUTPUT_DIR)
	mkdir -p $(BIT_DIR)
	mkdir -p $(BIN_DIR)
	mkdir -p $(ETC_DIR)
#    mkdir -p $(INIT_DIR)
	mkdir -p $(LIB_DIR)
	mkdir -p $(CONFIG_DIR)
#    mkdir -p $(RAMDISK_DIR)
	mkdir -p $(UPDATE_DIR)
#	mkdir -p $(WEB_DIR)

#	cp $(PROJECT_BASE)/utils/deploy/*                       $(OUTPUT_DIR)
#	cp $(PROJECT_BASE)/external/poco/lib*                   $(LIB_DIR)
#	cp $(PROJECT_BASE)/external/zeromq/lib*                 $(LIB_DIR)
#	cp $(PROJECT_BASE)/external/protobuf/lib*               $(LIB_DIR)
#	cp $(PROJECT_BASE)/external/libevent/libevent-2.0.so*   $(LIB_DIR)

	cp $(PROJECT_BASE)/MonitorX/MonitorX                    $(BIN_DIR)
	cp $(PROJECT_BASE)/TCPServer/TCPServer                  $(BIN_DIR)
	cp $(PROJECT_BASE)/HTTPUpdateServer/HTTPServer          $(BIN_DIR)
	cp $(PROJECT_BASE)/testmem/testmem                      $(BIN_DIR)

#	cp $(PROJECT_BASE)/setip/setip                          $(SYSBIN_DIR)

	cp -r $(PROJECT_INIT_DIR)                               $(OUTPUT_DIR)
#	cp -r $(PROJECT_BASE)/img                               $(OUTPUT_DIR)
#	cp -r $(PROJECT_BASE)/script                            $(OUTPUT_DIR)
	cp -r $(PROJECT_WEB_DIR)                                $(OUTPUT_DIR)
#	cp -r $(PROJECT_BASE)/testbench                         $(OUTPUT_DIR)

#	cp $(PROJECT_BASE)/../../hardware/bit.gz/*                 $(BIT_DIR)
	cp $(PROJECT_BASE)/bit/interfaces_top.bit                $(BIT_DIR)
	# gzip                                                    $(BIT_DIR)/*

	cp $(PROJECT_BASE)/etc/*                                $(ETC_DIR)
#	cp $(PROJECT_BASE)/external/net-snmp/snmpd.conf         $(ETC_DIR)
	echo "$(DATE)" > 										$(ETC_DIR)/sw_version

	cp $(PROJECT_BASE)/etc/mutable.cfg                      $(CONFIG_DIR)
	cp $(PROJECT_BASE)/etc/mutable.cfg                      $(ETC_DIR)/mutable.cfg.default
#	cp $(PROJECT_BASE)/external/tmux/.tmux.conf             $(ADM_HOME_DIR)

#	cp $(PROJECT_BASE)/external/net-snmp/Rcv20Table.conf    $(ETC_DIR)

	cp $(PROJECT_BASE)/utils/make_update                    $(OUTPUT_DIR)

	# Remove all hidden directories, in our case .svn
	find $(OUTPUT_DIR) -depth -name ".[^.]*" -type d -exec rm -rf {} \;

.PHONY: update
update: 

ifdef REV
	echo "$(DATE) rev.$(REV)" > 							$(ETC_DIR)/sw_version
endif

	$(OUTPUT_DIR)/make_update sr_370_rev$(REV).ctci

