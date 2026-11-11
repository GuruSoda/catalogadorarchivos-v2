.SUFFIXES = .c .cpp

EJECUTABLE=catalogador
BOOST=/opt/boost
LIBRERIAS=../../../Librerias
SALIDA_EJECUTABLE=../../bin
DIR_OBJETOS = ../build/linux/${EJECUTABLE}/
MIS_DEFINE=-DSQLITE_THREADSAFE=0
MIS_LINKS=-ldl

# mismos parametros para compilar C/C++
CFLAGS=$(CXXFLAGS)

RAIZ=.
INCLUDED=${LIBRERIAS}/ \
	${LIBRERIAS}/sqlite \
	${LIBRERIAS}/argtable \
	${LIBRERIAS}/polarssl/include \
	${LIBRERIAS}/polarssl/library \
	${LIBRERIAS}/GuruCompat/ \
	${BOOST}/libs/regex/src/ \
	${RAIZ}/../libs

EXTERN := sqlite3.o
OBJETOS += $(addprefix ${DIR_OBJETOS}, ${EXTERN})

ARGTABLE := arg_rex.o arg_end.o arg_int.o arg_str.o getopt.o arg_file.o arg_lit.o argtable2.o getopt1.o
OBJETOS += $(addprefix ${DIR_OBJETOS}, ${ARGTABLE})

BOOST_REGEX:=regex.o cpp_regex_traits.o posix_api.o icu.o regex_traits_defaults.o c_regex_traits.o regex_raw_buffer.o fileiter.o usinstances.o instances.o static_mutex.o
OBJETOS += $(addprefix ${DIR_OBJETOS}, ${BOOST_REGEX})

# TRE := regcomp.o regexec.o tre-compile.o tre-match-backtrack.o tre-mem.o tre-stack.o regerror.o tre-ast.o tre-match-approx.o tre-match-parallel.o tre-parse.o xmalloc.o
# OBJETOS += $(addprefix ${DIR_OBJETOS}, ${TRE})

POLARSSL:=sha1.o sha2.o sha4.o md5.o
OBJETOS += $(addprefix ${DIR_OBJETOS}, ${POLARSSL})

ifeq (${ARM},1)
	BOOST_REGEX:=regex.o cpp_regex_traits.o posix_api.o icu.o regex_traits_defaults.o c_regex_traits.o regex_raw_buffer.o fileiter.o usinstances.o instances.o static_mutex.o
	OBJETOS += $(addprefix ${DIR_OBJETOS}, ${BOOST_REGEX})

	MIS_DEFINE+= -DBOOST_ALL_NO_LIB

	INCLUDED += ${BOOST}/ ${BOOST}/libs/regex/src/
else
	CXXFLAGS+= -std=c++11
endif

PROGRAMA := main.o
OBJETOS += $(addprefix ${DIR_OBJETOS}, ${PROGRAMA})

PARAM_INCLUDE:=$(addprefix "-I", ${INCLUDED})
VPATH=${INCLUDED}

${EJECUTABLE}: crear_directorios Makefile ${OBJETOS}
	@$(CXX) ${CXXFLAGS} $(LDFLAGS) -o $(SALIDA_EJECUTABLE)/$@ $(OBJETOS) ${MIS_LINKS}
	@echo "Todo terminado????"

${DIR_OBJETOS}%.o: %.cpp
	@echo "Compilando C++ $? -> $@"
	@${CXX} -O2 -x c++ -pipe ${CXXFLAGS} ${MIS_DEFINE} ${PARAM_INCLUDE} -c $? -o $@

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
	@$(eval DST:=/opt/catalogadorv2)

makedeb: listxml2sqlite 
	@$(eval DIR_TEMP:=$(shell mktemp -du --suffix=atlas))
	@$(eval RELEASE:=$(shell date +%Y%m%d.%H%M))
	@$(eval VERSION:=$(shell cat debian/control | grep Version | cut -d ':' -f 2 | tr -d '[:blank:]'))
	@mkdir -p ${DIR_TEMP}${DST}/bin
	@mkdir -p ${DIR_TEMP}${DST}/bases
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

