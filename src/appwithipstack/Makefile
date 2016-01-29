
include $(USER_DEPTH)/make/makeinclude/wf_pre.mak

USER_BIN = ONCEAPP

USER_SRCS_CPP = \
CProcessAPP.cpp\
onceapp.cpp
	
USER_INCLDIRS = -I./ \
	-I$(ONCEMAIN_INCLUDE)\
	-I$(DIPC_INCLUDE)\
	-I$(OAM_INCLUDE)\
	-I$(SYSLOG_INCLUDE)\
	-I$(TRANSPORT_INCLUDE)\
	-I$(PACKETSERVICE_INCLUDE)\
	-I$(USER_DEPTH)/include\
	-I$(USER_DEPTH)/../include

USER_DEFFLAGS =
USER_LINK_LIBS =
USER_LINK_DLLS = -lipstack -ltputils -ltransport  -ldipc -lsysdb -loncesys -loncemain -loam -lsyslog -lpacketservice
USER_LINK_FLAGS= -L$(USER_DEPTH)/target -L$(SYSDB_LIB_PATH) -L$(TRANSPORT_LIB_PATH) -L$(PACKETSERVICE_LIB_PATH) -L$(DIPC_LIB_PATH) -L$(ONCEMAIN_LIB_PATH) -L$(OAM_LIB_PATH) -L$(SYSLOG_LIB_PATH) -L$(ONCESYS_LIB_PATH)
USER_OBJ_DIR=$(TARGET_PATH)/src/onceapp

include $(USER_DEPTH)/make/makeinclude/wf_post.mak



