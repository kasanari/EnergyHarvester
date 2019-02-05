ifndef TARGET
TARGET=z1
endif

# Enable to pull-in Z1SP specific test/source files
ZOLERTIA_Z1SP=0

CONTIKI_PROJECT = test-light-ziglet

CONTIKI_SOURCEFILES += reed-sensor.c sht25.c tlc59116.c light-ziglet.c \
                       relay-phidget.c

MODULES += dev/sht11

ifeq ($(ZOLERTIA_Z1SP),1)
CONTIKI_PROJECT += test-potent
endif

all: $(CONTIKI_PROJECT)
CONTIKI = /home/jakob/contiki
CONTIKI_WITH_RIME = 1
include $(CONTIKI)/Makefile.include
