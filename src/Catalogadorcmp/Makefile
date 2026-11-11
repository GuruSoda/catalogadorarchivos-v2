.SUFFIXES = .c .cpp

LIBRERIAS=../../../Librerias
SALIDA_EJECUTABLE=../../bin
DIR_OBJETOS=../build/linux/
MIS_LINKS=-ldl -lpthread
MIS_DEFINE=-DSQLITE_THREADSAFE=1

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

# mismos parametros para compilar C/C++
CFLAGS=$(CXXFLAGS)

RAIZ=.
INCLUDED=${BOOST}/ \
	${LIBRERIAS}/ \
	${LIBRERIAS}/sqlite/ \
	${LIBRERIAS}/GuruCompat/ \
	${LIBRERIAS}/argtable/ \
	${LIBRERIAS}/GuruCompat/ \
	${RAIZ}/../libs

PARAM_INCLUDE:=$(addprefix "-I", ${INCLUDED})

VPATH = ${INCLUDED}

PROGRAMA := main.o
OBJETOS += $(addprefix ${DIR_OBJETOS}, ${PROGRAMA})

ARGTABLE := arg_end.o arg_int.o arg_str.o getopt.o arg_file.o arg_lit.o argtable2.o getopt1.o
OBJETOS += $(addprefix ${DIR_OBJETOS}, ${ARGTABLE})

EXTERN := sqlite3.o 
OBJETOS += $(addprefix ${DIR_OBJETOS}, ${EXTERN})

catalogadorcmp: crear_directorios Makefile ${OBJETOS}
	@$(CXX) ${CXXFLAGS} $(LDFLAGS) -o $(SALIDA_EJECUTABLE)/$@ $(OBJETOS) ${MIS_LINKS}
	@echo "Todo terminado????"

${DIR_OBJETOS}%.o: %.cpp
	@echo "Compilando C++ $? -> $@"
	@${CXX} -O2 -x c++ -std=c++11 -pipe ${CFLAGS} ${MIS_DEFINE} ${PARAM_INCLUDE} -c $? -o $@

${DIR_OBJETOS}%.o:%.c
	@echo "Compilando C $? -> $@"
	@${CC} -O2 -s -pipe ${PARAM_INCLUDE} ${MIS_DEFINE} -c $? -o $@

clean:
	@rm -f ${OBJETOS}

crear_directorios:
	@mkdir -p ${DIR_OBJETOS}

compilar:
	@echo ${VPATH} ${OBJETOS}

envdeb:
	@$(eval DST:=/usr/local)

envopt: 
	@$(eval DST:=/opt/atlas)

makedeb: listxml2sqlite 
	@$(eval DIR_TEMP:=$(shell mktemp -du --suffix=atlas))
	@$(eval RELEASE:=$(shell date +%Y%m%d.%H%M))
	@$(eval VERSION:=$(shell cat debian/control | grep Version | cut -d ':' -f 2 | tr -d '[:blank:]'))
	@mkdir -p ${DIR_TEMP}${DST}/bin
	@mkdir -p ${DIR_TEMP}${DST}/sitios
	@mkdir -p ${DIR_TEMP}${DST}/log
	@mkdir -p ${DIR_TEMP}/DEBIAN
	@mkdir -p ${DIR_TEMP}/etc/init.d
	@cp ../../bin/atlas ${DIR_TEMP}${DST}/bin
	@cp ../../bin/atlas.css ${DIR_TEMP}${DST}/bin
	@cp ../../bin/atlas.mime ${DIR_TEMP}${DST}/bin
	@cp ../../bin/atlas.init ${DIR_TEMP}/etc/init.d/atlas
	@sed 's/^Version:\ [0-9]\..*/Version: ${VERSION}-${RELEASE}/' debian/control > ${DIR_TEMP}/DEBIAN/control 
	@dpkg -b ${DIR_TEMP} ../../atlas-${VERSION}-${RELEASE}_i386.deb
	@rm -rf ${DIR_TEMP}

deb: envdeb makedeb
	@echo "Todo Listo."

debopt: envopt makedeb
	@echo "Todo Listo."

