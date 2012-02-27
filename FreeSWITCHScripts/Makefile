FS_LOC = /usr/local/freeswitch
SCRIPT_FILES = VBTS_Wait_for_PA.py
SCRIPT_LOC = $(FS_LOC)/scripts/
DEF_FILES = dialplan/default/kurtis.xml
DEF_LOC = $(FS_LOC)/conf/dialplan/default

$(SCRIPT_FILES): 
$(CONF_FILES): 

install: $(SCRIPT_FILES) $(CONF_FILES)
	cp $(SCRIPT_FILES) $(SCRIPT_LOC)
	cp $(DEF_FILES) $(DEF_LOC)
