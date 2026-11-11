.SUFFIXES = .c .cpp

EJECUTABLE=libcatalogadorws.so
MIS_LINKS=-ldl -lpthread
MIS_DEFINE=-DSQLITE_THREADSAFE=1 -DSQLITE_ENABLE_COLUMN_METADATA -DSQLITE_DEFAULT_MEMSTATUS=0 -DSQLITE_OMIT_DEPRECATED -DSQLITE_OMIT_PROGRESS_CALLBACK -DSQLITE_USE_ALLOCA -DSQLITE_OMIT_SHARED_CACHE -DGURULOG_CPP -DGURUHTTP_VERSION=\"GuruHTTP/2.5\"
# MIS_DEFINE=-DSQLITE_THREADSAFE=1 -DSQLITE3_EXPORTS -DGURULOG_CPP -DGURUHTTP_VERSION=\"GuruHTTP/2.5\"
DIR_OBJETOS = ../build/linux/${EJECUTABLE}/
#CXXFLAGS=-march=pentium3m
CXXFLAGS=-fPIC -shared
LIBRERIAS=../../../Librerias
# CXX=/usr/bin/arm-linux-gnueabi-g++-4.6
SALIDA_EJECUTABLE=../../bin
# mismos parametros para compilar C/C++
CFLAGS=$(CXXFLAGS)

VERSION=1.6

# Para compilar en 64 sobre 32, instalar el paquete g++-multilib
ifeq ($(BITS),64)
    CXXFLAGS+=-m64
else ifeq ($(BITS),32)
        CXXFLAGS+=-m32
endif

ifeq (${DEBUG},1)
 CXXFLAGS+=-ggdb -g3
else
 CXXFLAGS+=-s -O2
endif

RAIZ=.
INCLUDED=${RAIZ}/ \
	${LIBRERIAS}/ \
	${LIBRERIAS}/GuruCompat/ \
	${LIBRERIAS}/sqlite/ \
	${LIBRERIAS}/jsoncpp/ \
	${LIBRERIAS}/polarssl/include \
	${LIBRERIAS}/polarssl/library \
	${RAIZ}/../libs

PARAM_INCLUDE:=$(addprefix "-I", ${INCLUDED})

VPATH=${INCLUDED}

MIS_LIBS := jsoncpp.o sqlite3.o GuruConfigV2.o GuruConfigLeer.o GuruLog.o 
OBJETOS += $(addprefix ${DIR_OBJETOS}, ${MIS_LIBS})

PROGRAMA := main.o
OBJETOS += $(addprefix ${DIR_OBJETOS}, ${PROGRAMA})

${EJECUTABLE}: crear_directorios Makefile ${OBJETOS}
	@$(CXX) ${CXXFLAGS} $(LDFLAGS) -o $(SALIDA_EJECUTABLE)/$@ $(OBJETOS) ${MIS_LINKS}
	@echo "Todo terminado????"

${DIR_OBJETOS}%.o: %.cpp
	@echo "Compilando C++ $? -> $@"
	@${CXX} -std=c++11 -fPIC -O2 -x c++ -pipe ${CXXFLAGS} ${MIS_DEFINE} ${PARAM_INCLUDE} -c $? -o $@

${DIR_OBJETOS}%.o:%.c
	@echo "Compilando C $? -> $@"
	@${CC} -fPIC -O2 -s -pipe ${CFLAGS} ${PARAM_INCLUDE} ${MIS_DEFINE} -c $? -o $@

clean:
	@rm -f ${OBJETOS}

crear_directorios:
	@mkdir -p ${DIR_OBJETOS}

compilar:
	@echo ${VPATH} ${OBJETOS}
